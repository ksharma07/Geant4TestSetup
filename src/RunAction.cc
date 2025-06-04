#include "RunAction.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto* analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetVerboseLevel(1);
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("/Users/kartikeyasharma/Desktop/Neutron_Emission_Spectrometer/Tasks/Geant4/TestSetup/output/output");

}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    G4cout << "BeginOfRunAction called" << G4endl;
    const char* filename = "/Users/kartikeyasharma/Desktop/Neutron_Emission_Spectrometer/Tasks/Geant4/TestSetup/output/output.root";

    //delete the existing file
    if ( std::remove(filename) == 0 ) {
        G4cout << "Deleted old ROOT file: " << filename << G4endl;
    } else {
        G4cout << "No existing ROOT file to delete" << G4endl;
    }

    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();

    // ntuple for L2Bar (ID=0)
    analysisManager->CreateNtuple("L2BarHits", "Hits Energy Time and Position");
    analysisManager->CreateNtupleIColumn("eventID"); // col 0
    analysisManager->CreateNtupleDColumn("energychange"); // col 1
    analysisManager->CreateNtupleDColumn("time");    // col 2
    analysisManager->CreateNtupleDColumn("x");       // col 3
    analysisManager->CreateNtupleDColumn("y");       // col 4
    analysisManager->CreateNtupleDColumn("z");       // col 5
    analysisManager->CreateNtupleIColumn("trackID"); // col 6
    analysisManager->CreateNtupleIColumn("parentID");// col 7
    analysisManager->CreateNtupleSColumn("processName");// col 8
    analysisManager->FinishNtuple();                 // ID = 0

    // ntuple for CeBr3 detector (ID=1)
    analysisManager->CreateNtuple("CebrHits", "Hits in CeBr3");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleDColumn("energychange");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->FinishNtuple();

    // ntuple for BaF2 detector (ID=2)
    analysisManager->CreateNtuple("BafHits", "Hits in BaF2");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleDColumn("energychange");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}