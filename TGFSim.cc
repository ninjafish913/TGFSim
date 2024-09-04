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
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

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

    //auto startTime = high_resolution_clock::now();

    while (collected < min_collect)
    {
        runManager->BeamOn(batch_size);
        collected += construct->GetDetector()->ResetCollected();
        if ((tries % 100) == 0)
        {
            std::cout << "run " << tries << " > collected: " << collected << " (+" << collected - prevCollected << ")" << std::endl;
            prevCollected = collected;
        }
        tries++;
    }

    //auto curTime = high_resolution_clock::now();
    //duration<double, std::milli> total_ms = curTime - startTime;

    //std::cout << "[final] run " << tries << " > collected: " << collected << std::endl;

    //std::ofstream csvfile(filename, std::ios_base::app);
    //csvfile << std::to_string((double)(collected) / (batch_size * tries)) << "," << total_ms.count() << "," << std::to_string(altitude) << "," << std::to_string(energy) << "," << std::to_string(phi) << "\n";
    //csvfile.close();

    return collected;
}

G4int RandomRun(std::string filename, G4RunManager* runManager, TGFConstruction* construct, G4UImanager* UImanager, int verbose = 0, int batch_size = 100, int min_collect = 50, int max_tries = 100, double start_energy = 0.1, double end_energy = 10.0, double start_angle = 0.0, double end_angle = 90.0, double start_altitude = 10, double end_altitude = 20)
{
    // Initial Parameter Space
    // 10-20 km initial altitude
    // 0-90 degrees initial zenith angle
    // 0.1 to 10 MeV initial photon energy

    G4double altitude = RandomDouble(start_altitude, end_altitude);
    double energy_power = RandomDouble(log10(start_energy), log10(end_energy));
    G4double energy = pow(10, energy_power);
    //G4double energy = RandomDouble(0.1, 10.0);
    G4double phi = RandomDouble(start_angle, end_angle);

    UImanager->ApplyCommand("/source/altitude " + std::to_string(altitude));
    UImanager->ApplyCommand("/source/energy " + std::to_string(energy));
    UImanager->ApplyCommand("/source/phi " + std::to_string(phi));

    if (verbose > 0)
    {
        std::cout << "source > altitude " << altitude << " km; energy " << energy << " MeV; phi " << phi << " degrees" << std::endl;
    }

    G4int tries = 0;
    G4int newCollected = 0;
    G4int collected = 0;
    while (collected + newCollected < min_collect)
    {
        runManager->BeamOn(batch_size);
        newCollected += construct->GetDetector()->ResetCollected();
        tries++;

        if (verbose > 0 && tries % 100 == 0 && tries != 0 && newCollected != 0) {
            std::cout << "run " << tries << " > collected: " << (collected + newCollected) << " (+" << newCollected << ")" << std::endl;

            collected += newCollected;
            newCollected = 0;
        }
    }

    //std::ofstream csvfile(filename, std::ios_base::app);
    //csvfile << std::to_string((double)(collected) / (batch_size * tries)) << "," << std::to_string(altitude) << "," << std::to_string(energy) << "," << std::to_string(phi) << "\n";
    //csvfile.close();

    std::cout << "run " << tries << " > collected: " << (collected + newCollected) << " (+" << newCollected << ")" << std::endl;

    collected += newCollected;

    return collected;
}

int main(int argc, char** argv)
{
    int run_type = 0;
    int batch_size = 100;
    int num_collect = 200000;
    int min_collect = 10;
    int max_tries = -1;
    double start_energy = 0.1;
    double start_angle = 0.0;
    double start_altitude = 14.0;
    double end_energy = 10.0;
    double end_angle = 90.0;
    double end_altitude = 20.0;
    std::string save_file = "altitude_vary";

    /*
    for (int i = 0; i < argc; i++) 
    {
        std::string command_name = argv[2 * i];
        std::string command = argv[2 * i + 1];

        if (command_name == "type")
            run_type = std::stoi(command);
        else if (command_name == "batch_size")
            batch_size = std::stoi(command);
        else if (command_name == "num_collect")
            num_collect = std::stoi(command);
        else if (command_name == "min_collect")
            min_collect = std::stoi(command);
        else if (command_name == "max_tries")
            max_tries = std::stoi(command);
        else if (command_name == "start_energy")
            start_energy = std::stod(command);
        else if (command_name == "start_angle")
            start_angle = std::stod(command);
        else if (command_name == "start_altitude")
            start_altitude = std::stod(command);
        else if (command_name == "end_energy")
            end_energy = std::stod(command);
        else if (command_name == "end_angle")
            end_angle = std::stod(command);
        else if (command_name == "end_altitude")
            end_altitude = std::stod(command);
        else if (command_name == "sfile")
            save_file = command;
    }
    */

    G4RunManager* runManager = new G4RunManager();

    TGFConstruction* construct = new TGFConstruction();
    runManager->SetUserInitialization(construct);
    runManager->SetUserInitialization(new TGFPhysicsList());
    runManager->SetUserInitialization(new TGFActionInitialization(save_file));

    runManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
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
    
    // Training Data Collection //

    auto startTime = high_resolution_clock::now();
    auto prevTime = startTime;
    
    G4int run_until_collected = num_collect;

    G4int collected = 0;
    G4int prevCollected = collected;
    while (collected < run_until_collected)
    {
        prevCollected = collected;
        collected += RandomRun("C:/Users/lucas/Python/TGFMachineLearning/ProbData10.csv", runManager, construct, UImanager, 1, batch_size, min_collect, max_tries, start_energy, end_energy, start_angle, end_angle, start_altitude, end_altitude);
        std::cout << "events collected: " << std::to_string(collected) << " / " << std::to_string(run_until_collected) << " (+" << std::to_string(collected - prevCollected) << ")" << std::endl;

        auto curTime = high_resolution_clock::now();
        duration<double, std::milli> total_ms = curTime - startTime;
        duration<double, std::milli> change_ms = curTime - prevTime;
        std::cout << "runtime: " << change_ms.count() / 60000 << " min (~" << (collected / (run_until_collected - collected)) * total_ms.count() / 3600000 << " hours remaining)" << std::endl;
        std::cout << std::endl;

        prevTime = curTime;
    }

    duration<double, std::milli> total_ms = prevTime - startTime;
    std::cout << "total runtime: " << total_ms.count() / 3600000 << " hours" << std::endl;

    
    /*
    // Testing Data Collection //

    //G4UImanager* UImanager = G4UImanager::GetUIpointer();

    auto startTime = high_resolution_clock::now();
    auto prevTime = startTime;

    //const int total = 61;
    double altitudes[1] = { 10 };
    int alt_count = sizeof(altitudes) / sizeof(double);
    double energies[1] = { 1.0 };
    int ene_count = sizeof(energies) / sizeof(double);
    double phies[1] = { 45.0 };
    int phi_count = sizeof(phies) / sizeof(double);

    //double start = 0.1;
    //double end = 10;
    //double inc = (log10(end) - log10(start)) / (total - 1);
    //double inc = (end - start) / (total - 1);
    //for (int i = 0; i < total; i++)
    //{
    //    energies[i] = pow(10, log10(start) + i * inc);
    //    //energies[i] = start + i * inc;
    //}

    G4int collected = 0;
    G4int prevCollected = collected;
    for (int i = 0; i < alt_count; i++)
    {
        for (int j = 0; j < ene_count; j++)
        {
            for (int k = 0; k < phi_count; k++)
            {
                prevCollected = collected;
                collected += CollectionRun("C:/Users/lucas/Python/TGFMachineLearning/none.csv", runManager, construct, UImanager, altitudes[i], energies[j], phies[k], 1, 10000, 10);
                std::cout << "events collected: " << std::to_string(collected) << " (+" << std::to_string(collected - prevCollected) << ")" << std::endl;

                auto curTime = high_resolution_clock::now();
                duration<double, std::milli> total_ms = curTime - startTime;
                duration<double, std::milli> change_ms = curTime - prevTime;
                std::cout << "runtime: " << change_ms.count() / 60000 << " minutes" << std::endl;
                std::cout << std::endl;

                prevTime = curTime;
            }
        }
    }

    duration<double, std::milli> total_ms = prevTime - startTime;
    std::cout << "total runtime: " << total_ms.count() / 60000 << " minutes" << std::endl;
    */

    return 0;
}