//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#include "TGFDetector.hh"

namespace TGF
{
	TGFDetector::TGFDetector(G4String name) : G4VSensitiveDetector(name)
	{
		collected = 0;
	}

	TGFDetector::~TGFDetector()
	{ }

	G4int TGFDetector::GetCollected()
	{
		return collected;
	}

	G4int TGFDetector::ResetCollected()
	{
		G4int prev = collected;
		collected = 0;
		return prev;
	}

	G4bool TGFDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
	{
		G4Track* track = aStep->GetTrack();

		track->SetTrackStatus(fStopAndKill);

		G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
		//G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

        G4ThreeVector position = preStepPoint->GetPosition();
        G4ThreeVector direction = preStepPoint->GetMomentumDirection();
        G4double energy = preStepPoint->GetKineticEnergy();
        G4double time = preStepPoint->GetGlobalTime();
		G4double mass = preStepPoint->GetMass();
		G4double charge = preStepPoint->GetCharge();

        TGFGenerator* generator = (TGFGenerator*)(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

        G4double init_altitude = generator->GetAltitude();
        G4double init_energy = generator->GetEnergy();
        G4double init_phi = generator->GetPhi();
		
		if (mass == 0 && position.getY() != 0)
		{
			collected += 1;

			G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

			G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
			analysisManager->FillNtupleIColumn(0, evt);
			analysisManager->FillNtupleDColumn(1, position.getX());
			analysisManager->FillNtupleDColumn(2, position.getY());
			analysisManager->FillNtupleDColumn(3, direction.getX());
			analysisManager->FillNtupleDColumn(4, direction.getY());
			analysisManager->FillNtupleDColumn(5, direction.getZ());
			analysisManager->FillNtupleDColumn(6, time);
			analysisManager->FillNtupleDColumn(7, energy);
			analysisManager->FillNtupleDColumn(8, mass);
			analysisManager->FillNtupleDColumn(9, charge);
			analysisManager->FillNtupleDColumn(10, init_altitude);
			analysisManager->FillNtupleDColumn(11, init_energy);
			analysisManager->FillNtupleDColumn(12, init_phi);
			analysisManager->AddNtupleRow();
		}

		return true;
	}
}