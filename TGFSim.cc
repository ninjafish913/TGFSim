//
// Created by Lucas Peterson
// Date: 5/31/2024
//

#include <chrono>

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include "TGFConstruction.hh"
#include "TGFPhysicsList.hh"
#include "TGFActionInitialization.hh"
#include "TGFDetector.hh"

using namespace TGF;

double RandomDouble(double min, double max)
{
    return min + (max - min) * ((double)rand() / (RAND_MAX));
}

G4int RandomRun(G4RunManager* runManager, TGFConstruction* construct, G4UImanager* UImanager, int verbose = 0, int batch_size = 100, int min_collect = 50, int max_tries = 100)
{
    // Initial Parameter Space
    // 10-20 km initial altitude
    // 0-90 degrees initial zenith angle
    // 0.1 to 10 MeV initial photon energy

    G4double altitude = RandomDouble(10., 20.);
    G4double energy = RandomDouble(0.1, 10.);
    G4double phi = RandomDouble(0., 90.);

    UImanager->ApplyCommand("/source/altitude " + std::to_string(altitude));
    UImanager->ApplyCommand("/source/energy " + std::to_string(energy));
    UImanager->ApplyCommand("/source/phi " + std::to_string(phi));

    if (verbose > 0)
    {
        G4cout << "source > altitude " << altitude << " km; energy " << energy << " MeV; phi " << phi << " degrees" << G4endl;
        std::cout << "source > altitude " << altitude << " km; energy " << energy << " MeV; phi " << phi << " degrees" << std::endl;
    }

    G4int tries = 0;
    G4int collected = 0;
    while (collected < min_collect && tries < max_tries)
    {
        runManager->BeamOn(batch_size);
        collected += construct->GetDetector()->ResetCollected();
        tries++;
    }

    return collected;
}

int main(int argc, char** argv)
{
    G4RunManager* runManager = new G4RunManager();

    TGFConstruction* construct = new TGFConstruction();
    runManager->SetUserInitialization(construct);
    runManager->SetUserInitialization(new TGFPhysicsList());
    runManager->SetUserInitialization(new TGFActionInitialization("RunData"));

    runManager->Initialize();

    G4UIExecutive* ui = new G4UIExecutive(argc, argv);

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    /*
    UImanager->ApplyCommand("/control/verbose 0");
    UImanager->ApplyCommand("/run/verbose 0");
    
    UImanager->ApplyCommand("/vis/open");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/viewer/autoRefresh true");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
    UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
    */

    //G4cout << construct->GetDetector()->GetCollected() << G4endl;
    //construct->GetDetector()->ResetCollected();

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto startTime = high_resolution_clock::now();
    auto prevTime = startTime;
    
    G4int collected = 0;
    G4int prevCollected = collected;
    G4int run_until_collected = 1000;
    while (collected < run_until_collected)
    {
        prevCollected = collected;
        collected += RandomRun(runManager, construct, UImanager, 1, 50, 10);
        std::cout << "events collected: " + std::to_string(collected) + " (+" << std::to_string(collected - prevCollected) << ")" << std::endl;

        auto curTime = high_resolution_clock::now();
        duration<double, std::milli> total_ms = curTime - startTime;
        duration<double, std::milli> change_ms = curTime - prevTime;
        std::cout << "runtime: " << change_ms.count() << " ms (~" << (run_until_collected / collected) * total_ms.count() / 1000 << " seconds remaining)" << std::endl;
        std::cout << std::endl;

        prevTime = curTime;
    }

    duration<double, std::milli> total_ms = prevTime - startTime;
    std::cout << "total runtime: " << total_ms.count() << " seconds" << std::endl;

    //detector->ResetCollected();

    /*
    UImanager->ApplyCommand("/source/altitude 15");

    runManager->BeamOn(1000);

    G4cout << analysisManager->GetNofNtuples() << G4endl;

    UImanager->ApplyCommand("/source/altitude 10");
    UImanager->ApplyCommand("/source/energy 5");

    runManager->BeamOn(1000);

    G4cout << analysisManager->GetNofNtuples() << G4endl;
    */

    //detector->ResetCollected();
    do
    {
        std::cout << '\n' << "Press a key to exit...";
    } while (std::cin.get() != '\n');

    return 0;
}