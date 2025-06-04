#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
    // Run Manager
    auto* runManager = new G4RunManager();
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());
    runManager->Initialize();

    // Visualization Manager
    auto* visManager = new G4VisExecutive();
    visManager->Initialize();

    // UI Manager
    auto* UI = G4UImanager::GetUIpointer();

    // Check if batch mode or interactive
    if (argc == 1) {
        // Interactive
        G4UIExecutive* session = new G4UIExecutive(argc, argv);
        UI->ApplyCommand("/control/execute ../vis.mac");
        session->SessionStart();
        delete session;
    } else {
        // Batch mode
        G4String macroFile = argv[1];
        G4String command = "/control/execute " + macroFile;
        UI->ApplyCommand(command);
    }

    delete visManager;
    delete runManager;
    return 0;
}