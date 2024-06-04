//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#ifndef TGFACTIONINITIALIZATION_HH
#define TGFACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "TGFGenerator.hh"
#include "TGFRunAction.hh"

namespace TGF
{
	class TGFActionInitialization : public G4VUserActionInitialization
	{
	public:
		TGFActionInitialization(G4String);
		~TGFActionInitialization();

		virtual void Build() const;
	private:
		G4String fFilename;
	};
}

#endif