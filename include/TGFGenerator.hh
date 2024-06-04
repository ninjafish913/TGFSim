//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#ifndef TGFGENERATOR_HH
#define TGFGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4GenericMessenger.hh"
#include "Randomize.hh"

namespace TGF
{
    class TGFGenerator : public G4VUserPrimaryGeneratorAction
    {
    public:
        TGFGenerator();
        ~TGFGenerator();

        G4double GetAltitude();
        G4double GetEnergy();
        G4double GetPhi();

        virtual void GeneratePrimaries(G4Event*);

    private:
        G4ParticleGun* fParticleGun;

        G4double altitude;
        G4double energy;
        G4double phi;

        G4GenericMessenger* fMessenger;
    };
}

#endif