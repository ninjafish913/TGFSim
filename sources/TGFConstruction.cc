//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#include "TGFConstruction.hh"

namespace TGF
{
    TGFConstruction::TGFConstruction()
    {
        detail = 1;
        width = 200.;
    }

    TGFConstruction::~TGFConstruction()
    { }

    TGFDetector* TGFConstruction::GetDetector()
    {
        return fDetector;
    }

    G4VPhysicalVolume* TGFConstruction::Construct()
    {  
        /* https://www.digitaldutch.com/atmoscalc/table.htm */
        std::ifstream file("StandardAtmosphere.csv");

        std::string data[1001][2];
        std::string line;
        int row = 0;
        while (getline(file, line) && row < 1001) 
        {
            std::stringstream ss(line);
            std::string cell;
            int col = 0;
            while (getline(ss, cell, ',') && col < 2) \
            {
                data[row][col] = cell;
                col++;
            }
            row++;
        }
        file.close();

        double altitudes[1001];
        double densities[1001];
        for (int i = 0; i < 1001; i++) 
        {
            altitudes[i] = atof(data[i][0].c_str());
            densities[i] = atof(data[i][1].c_str());
        }

        // // TODO: Replace with NRLMSIS atmospheric model
        // double densities[] = { 1.225,1.112,1.007,0.9093,0.8194,0.7364,0.6601,0.59,0.5258,0.4671,0.4135,0.1948,0.08891,0.04008,0.01841,0.003996,0.001027,0.0003097,0.00008283,0.00001846 };
        // double altitudes[] = { 0,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,15000,20000,25000,30000,40000,50000,60000,70000,80000 };
        int layers = int(sizeof(densities) / sizeof(double));

        G4double worldLength = width*km;
        G4double worldHeight = (altitudes[detail * (layers / detail) - 1] - altitudes[0])*m;

        double originAlt = altitudes[0] + 0.5 * (altitudes[layers - 1] - altitudes[0]);

        // Constructing World
        G4NistManager* nist = G4NistManager::Instance();
        G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

        G4Box* solidWorld = new G4Box("solidWorld", 0.5*worldLength, 0.5*worldLength, 0.5*worldHeight);

        G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

        G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

        // Constructing Atmospheric Geometry
        for (int i = 1; i < layers / detail; i++)
        {
            G4double density = densities[detail * i - detail] * kg / m3;
            G4Material* layerMat = nist->BuildMaterialWithNewDensity("Air" + std::to_string(i), "G4_AIR", density);

            double startAlt = altitudes[detail * i - detail], endAlt = altitudes[detail * i];
            double centerAlt = startAlt + 0.5 * (endAlt - startAlt);

            if (i < layers / detail - 1)
            {
                std::string name = "Layer" + std::to_string(i);

                G4Box* solidLayer = new G4Box("solid" + name, 0.5 * worldLength, 0.5 * worldLength, 0.5 * (endAlt - startAlt) * m);

                G4LogicalVolume* logicLayer = new G4LogicalVolume(solidLayer, layerMat, "logic" + name);

                G4VPhysicalVolume* physLayer = new G4PVPlacement(0, G4ThreeVector(0., 0., (centerAlt - originAlt) * m), logicLayer, "phys" + name, logicWorld, false, 0, true);
            }
            else
            {
                G4Box* solidDetector = new G4Box("solidDetector", 0.5 * worldLength, 0.5 * worldLength, 0.5 * (endAlt - startAlt) * m);

                logicDetector = new G4LogicalVolume(solidDetector, layerMat, "logicDetector");

                G4VPhysicalVolume* physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., (centerAlt - originAlt) * m), logicDetector, "physDetector", logicWorld, false, 0, true);
            }
        }

        return physWorld;
    }

    void TGFConstruction::ConstructSDandField() 
    {
        fDetector = new TGFDetector("SensitiveDetector");

        logicDetector->SetSensitiveDetector(fDetector);
    }
}