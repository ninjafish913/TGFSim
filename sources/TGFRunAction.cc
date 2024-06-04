//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#include "TGFRunAction.hh"

namespace TGF
{
	TGFRunAction::TGFRunAction(G4String filename)
	{
		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

		G4String path = "C:/Users/lucas/Python/TGFMachineLearning/CSVDump/" + filename + ".csv";

		analysisManager->OpenFile(path);
		analysisManager->CreateNtuple("Events", "Events");
		analysisManager->CreateNtupleIColumn("fEvent");
		analysisManager->CreateNtupleDColumn("fX");
		analysisManager->CreateNtupleDColumn("fY");
		analysisManager->CreateNtupleDColumn("fDirX");
		analysisManager->CreateNtupleDColumn("fDirY");
		analysisManager->CreateNtupleDColumn("fDirZ");
		analysisManager->CreateNtupleDColumn("fTime");
		analysisManager->CreateNtupleDColumn("fEnergy");
		analysisManager->CreateNtupleDColumn("fInitAltitude");
		analysisManager->CreateNtupleDColumn("fInitEnergy");
		analysisManager->CreateNtupleDColumn("fInitPhi");
		analysisManager->FinishNtuple();
	}

	TGFRunAction::~TGFRunAction()
	{ 
		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

		analysisManager->Write();
		analysisManager->CloseFile();
	}

	void TGFRunAction::BeginOfRunAction(const G4Run* aRun)
	{
		
	}

	void TGFRunAction::EndOfRunAction(const G4Run* aRun)
	{
		//G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

		//G4int count = analysisManager->GetNofNtuples();

		//analysisManager->Write();
	}
}