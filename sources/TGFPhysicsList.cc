//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#include "TGFPhysicsList.hh"

namespace TGF
{
	TGFPhysicsList::TGFPhysicsList()
	{
		RegisterPhysics(new G4EmStandardPhysics());
		RegisterPhysics(new G4OpticalPhysics());
	}

	TGFPhysicsList::~TGFPhysicsList()
	{ }
}