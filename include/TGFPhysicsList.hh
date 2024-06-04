//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#ifndef TGFPHYSICSLIST_HH
#define TGFPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

namespace TGF
{
	class TGFPhysicsList : public G4VModularPhysicsList
	{
	public:
		TGFPhysicsList();
		~TGFPhysicsList();
	};
}

#endif