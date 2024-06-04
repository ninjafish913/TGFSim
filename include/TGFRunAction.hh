//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#ifndef TGFRUNACTION_HH
#define TGFRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

namespace TGF
{
    class TGFRunAction : public G4UserRunAction
    {
    public:
        TGFRunAction(G4String filename);
        ~TGFRunAction();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
    };
}

#endif