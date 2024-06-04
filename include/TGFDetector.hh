//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#ifndef TGFDETECTOR_HH
#define TGFDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "TGFGenerator.hh"

namespace TGF
{
	class TGFDetector : public G4VSensitiveDetector
	{
	public:
		TGFDetector(G4String);
		~TGFDetector();

		G4int GetCollected();
		G4int ResetCollected();
	private:
		virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

		std::ofstream file;

		G4int collected;
	};
}

#endif