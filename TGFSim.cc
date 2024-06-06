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

G4int CollectionRun(std::string filename, G4RunManager* runManager, TGFConstruction* construct, G4UImanager* UImanager, G4double altitude, G4double energy, G4double phi, int verbose = 0, int batch_size = 100, int min_collect = 50)
{
    UImanager->ApplyCommand("/source/altitude " + std::to_string(altitude));
    UImanager->ApplyCommand("/source/energy " + std::to_string(energy));
    UImanager->ApplyCommand("/source/phi " + std::to_string(phi));

    if (verbose > 0)
    {
        std::cout << "source > altitude " << altitude << " km; energy " << energy << " MeV; phi " << phi << " degrees" << std::endl;
    }

    G4int tries = 0;
    G4int collected = 0;
    G4int prevCollected = 0;
    while (collected < min_collect)
    {
        runManager->BeamOn(batch_size);
        collected += construct->GetDetector()->ResetCollected();
        if (tries % 10 == 0)
        {
            std::cout << "run " << tries << " > collected: " << collected << " (+" << collected - prevCollected << ")" << std::endl;
            prevCollected = collected;
        }
        tries++;
    }
    std::cout << "[final] run " << tries << " > collected: " << collected << std::endl;

    std::ofstream csvfile(filename, std::ios_base::app);
    csvfile << std::to_string((double)(collected) / (batch_size * tries)) << "," << std::to_string(altitude) << "," << std::to_string(energy) << "," << std::to_string(phi) << "\n";
    csvfile.close();

    return collected;
}

G4int RandomRun(std::string filename, G4RunManager* runManager, TGFConstruction* construct, G4UImanager* UImanager, int verbose = 0, int batch_size = 100, int min_collect = 50, int max_tries = 100)
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

    std::ofstream csvfile(filename, std::ios_base::app);
    csvfile << std::to_string((double)(collected) / (batch_size * tries)) << "," << std::to_string(altitude) << "," << std::to_string(energy) << "," << std::to_string(phi) << "\n";
    csvfile.close();

    return collected;
}

int main(int argc, char** argv)
{
    G4RunManager* runManager = new G4RunManager();

    TGFConstruction* construct = new TGFConstruction();
    runManager->SetUserInitialization(construct);
    runManager->SetUserInitialization(new TGFPhysicsList());
    runManager->SetUserInitialization(new TGFActionInitialization("TestData2"));

    runManager->Initialize();

    /*
    // Visual Run //

    G4UIExecutive* ui = new G4UIExecutive(argc, argv);

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    UImanager->ApplyCommand("/control/verbose 0");
    UImanager->ApplyCommand("/run/verbose 0");
    
    UImanager->ApplyCommand("/vis/open");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/viewer/autoRefresh true");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
    UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");

    ui->SessionStart();
    */
    
    /*
    // Training Data Collection //

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto startTime = high_resolution_clock::now();
    auto prevTime = startTime;
    
    G4int run_until_collected = 10000000;

    G4int collected = 0;
    G4int prevCollected = collected;
    while (collected < run_until_collected)
    {
        prevCollected = collected;
        collected += RandomRun("C:/Users/lucas/Python/TGFMachineLearning/ProbData.csv", runManager, construct, UImanager, 1, 10000, 100, 3);
        std::cout << "events collected: " << std::to_string(collected) << " / " << std::to_string(run_until_collected) << " (+" << std::to_string(collected - prevCollected) << ")" << std::endl;

        auto curTime = high_resolution_clock::now();
        duration<double, std::milli> total_ms = curTime - startTime;
        duration<double, std::milli> change_ms = curTime - prevTime;
        std::cout << "runtime: " << change_ms.count() / 1000 << " sec (~" << (run_until_collected / collected) * total_ms.count() / 60000 << " minutes remaining)" << std::endl;
        std::cout << std::endl;

        prevTime = curTime;
    }

    duration<double, std::milli> total_ms = prevTime - startTime;
    std::cout << "total runtime: " << total_ms.count() / 60000 << " minutes" << std::endl;
    */

    // Testing Data Collection //

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto startTime = high_resolution_clock::now();
    auto prevTime = startTime;

    G4double altitudes[3] = { 10.5, 15., 19.5 };
    G4double energies[3] = { 1.5, 5., 9.5 };
    G4double phies[3] = { 20., 5., 0.5 };

    G4int collected = 0;
    G4int prevCollected = collected;
    for (int i = 2; i < 3; i++)
    {
        for (int j = 2; j < 3; j++) 
        {
            for (int k = 2; k < 3; k++)
            {
                prevCollected = collected;
                collected += CollectionRun("C:/Users/lucas/Python/TGFMachineLearning/ProbData.csv", runManager, construct, UImanager, altitudes[i], energies[j], phies[k], 1, 100000, 500);
                std::cout << "events collected: " << std::to_string(collected) << " / 36000 (+" << std::to_string(collected - prevCollected) << ")" << std::endl;

                auto curTime = high_resolution_clock::now();
                duration<double, std::milli> total_ms = curTime - startTime;
                duration<double, std::milli> change_ms = curTime - prevTime;
                std::cout << "runtime: " << change_ms.count() / 1000 << " sec (~" << (18000 / collected) * total_ms.count() / 60000 << " minutes remaining)" << std::endl;
                std::cout << std::endl;

                prevTime = curTime;
            }
        }
    }

    duration<double, std::milli> total_ms = prevTime - startTime;
    std::cout << "total runtime: " << total_ms.count() / 60000 << " minutes" << std::endl;
    
    do
    {
        std::cout << '\n' << "Press a key to exit...";
    } while (std::cin.get() != '\n');

    return 0;
}