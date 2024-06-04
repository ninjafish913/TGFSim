//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#ifndef TGFCONSTRUCTION_HH
#define TGFCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "TGFDetector.hh"
#include "G4GenericMessenger.hh"
#include "TGFGenerator.hh"

namespace TGF
{
    class TGFConstruction : public G4VUserDetectorConstruction
    {
    public:
        TGFConstruction();
        ~TGFConstruction();

        TGFDetector* GetDetector();

        virtual G4VPhysicalVolume* Construct();
    private:
        G4LogicalVolume* logicDetector;

        virtual void ConstructSDandField();

        TGFDetector* fDetector;

        G4int detail;
        G4double width;
    };
}

#endif