//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#include "TGFActionInitialization.hh"

namespace TGF
{
	TGFActionInitialization::TGFActionInitialization(G4String filename)
	{
		fFilename = filename;
	}

	TGFActionInitialization::~TGFActionInitialization()
	{ }

	void TGFActionInitialization::Build() const
	{
		TGFGenerator* generator = new TGFGenerator();
		SetUserAction(generator);

		TGFRunAction* runAction = new TGFRunAction(fFilename);
		SetUserAction(runAction);
	}
}