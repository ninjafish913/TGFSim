//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#include "TGFGenerator.hh"

namespace TGF
{
	TGFGenerator::TGFGenerator()
	{
		fParticleGun = new G4ParticleGun(1);

		fMessenger = new G4GenericMessenger(this, "/source/", "Source Properties");

		fMessenger->DeclareProperty("altitude", altitude, "Sets source altitude in km");
		fMessenger->DeclareProperty("energy", energy, "Sets source energy in MeV");
		fMessenger->DeclareProperty("phi", phi, "Sets the angle along the Y-axis");

		altitude = 20.;
		energy = 1.;
		phi = 0.;
	}

	TGFGenerator::~TGFGenerator()
	{
		delete fParticleGun;
		delete fMessenger;
	}

	G4double TGFGenerator::GetAltitude()
	{
		return altitude;
	}

	G4double TGFGenerator::GetEnergy()
	{
		return energy;
	}

	G4double TGFGenerator::GetPhi()
	{
		return phi;
	}

	void TGFGenerator::GeneratePrimaries(G4Event* anEvent)
	{
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4String particleName = "gamma";
		G4ParticleDefinition* particle = particleTable->FindParticle(particleName);

		G4ThreeVector pos(0., 0., (altitude - 25) * km);
		G4ThreeVector mom(0., 0., 1.);
		G4double ene = energy * MeV;

		fParticleGun->SetParticlePosition(pos);
		fParticleGun->SetParticleMomentumDirection(mom.rotateY(phi * degree));
		fParticleGun->SetParticleEnergy(ene);
		fParticleGun->SetParticleDefinition(particle);

		fParticleGun->GeneratePrimaryVertex(anEvent);
	}
}