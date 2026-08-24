#include "RunAction.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto* analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetVerboseLevel(0);
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("/Users/kartikeyasharma/Desktop/Neutron_Emission_Spectrometer/Tasks/Geant4/TestSetup/output/output");

}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    fRunStartTime = std::chrono::steady_clock::now();
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
    analysisManager->CreateNtuple("L2BarHits", "Hits in L2Bar");
    analysisManager->CreateNtupleIColumn("eventID"); // col 0
    analysisManager->CreateNtupleDColumn("edep");    // col 1
    analysisManager->CreateNtupleDColumn("energychange"); // col 2
    analysisManager->CreateNtupleDColumn("time");    // col 3
    analysisManager->CreateNtupleDColumn("momx");       // col 4
    analysisManager->CreateNtupleDColumn("momy");       // col 5
    analysisManager->CreateNtupleDColumn("momz");       // col 6
    analysisManager->CreateNtupleDColumn("posx");       // col 4
    analysisManager->CreateNtupleDColumn("posy");       // col 5
    analysisManager->CreateNtupleDColumn("posz");
    analysisManager->CreateNtupleIColumn("trackID"); // col 7
    analysisManager->CreateNtupleIColumn("parentID");// col 8
    analysisManager->CreateNtupleSColumn("processName");// col 9
    analysisManager->CreateNtupleSColumn("pname");   // col 9
    analysisManager->FinishNtuple();                 // ID = 0

    // ntuple for CeBr3 detector (ID=1)
    analysisManager->CreateNtuple("CebrHits", "Hits in CeBr3");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleDColumn("energychange");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("momx");       // col 4
    analysisManager->CreateNtupleDColumn("momy");       // col 5
    analysisManager->CreateNtupleDColumn("momz");       // col 6
    analysisManager->CreateNtupleDColumn("posx");       // col 4
    analysisManager->CreateNtupleDColumn("posy");       // col 5
    analysisManager->CreateNtupleDColumn("posz");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->CreateNtupleSColumn("pname");
    analysisManager->FinishNtuple();

    // ntuple for BaF2 detector (ID=2)
    analysisManager->CreateNtuple("BafHits", "Hits in BaF2");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleDColumn("energychange");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("momx");       // col 4
    analysisManager->CreateNtupleDColumn("momy");       // col 5
    analysisManager->CreateNtupleDColumn("momz");       // col 6
    analysisManager->CreateNtupleDColumn("posx");       // col 4
    analysisManager->CreateNtupleDColumn("posy");       // col 5
    analysisManager->CreateNtupleDColumn("posz");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->CreateNtupleSColumn("pname");
    analysisManager->FinishNtuple();

    // ntuple for L2Bar aluminum films (ID=3)
    analysisManager->CreateNtuple("L2barAlFilmHits", "Hits in L2Bar Al films");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleDColumn("energychange");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("momx");
    analysisManager->CreateNtupleDColumn("momy");
    analysisManager->CreateNtupleDColumn("momz");
    analysisManager->CreateNtupleDColumn("posx");
    analysisManager->CreateNtupleDColumn("posy");
    analysisManager->CreateNtupleDColumn("posz");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->CreateNtupleSColumn("pname");
    analysisManager->FinishNtuple();

    // this part defines a dedicated optical schema for +X SiPM arrivals.
    analysisManager->CreateNtuple("SiPMXPlusHits", "Optical arrivals at SiPM +X face");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("detected");
    analysisManager->CreateNtupleDColumn("photonEnergyEV");
    analysisManager->CreateNtupleDColumn("wavelengthNm");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("momx");
    analysisManager->CreateNtupleDColumn("momy");
    analysisManager->CreateNtupleDColumn("momz");
    analysisManager->CreateNtupleDColumn("posx");
    analysisManager->CreateNtupleDColumn("posy");
    analysisManager->CreateNtupleDColumn("posz");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->CreateNtupleSColumn("pname");
    analysisManager->FinishNtuple();

    // this part mirrors the same optical schema for -X SiPM arrivals.
    analysisManager->CreateNtuple("SiPMXMinusHits", "Optical arrivals at SiPM -X face");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("detected");
    analysisManager->CreateNtupleDColumn("photonEnergyEV");
    analysisManager->CreateNtupleDColumn("wavelengthNm");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("momx");
    analysisManager->CreateNtupleDColumn("momy");
    analysisManager->CreateNtupleDColumn("momz");
    analysisManager->CreateNtupleDColumn("posx");
    analysisManager->CreateNtupleDColumn("posy");
    analysisManager->CreateNtupleDColumn("posz");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->CreateNtupleSColumn("pname");
    analysisManager->FinishNtuple();

    // this part stores one event-level summary row including created optical photons.
    analysisManager->CreateNtuple("EventSummary", "Per-event optical photon summary");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("nOpticalPhotonsCreated");
    analysisManager->FinishNtuple();

}

void RunAction::EndOfRunAction(const G4Run*) {
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    const auto runEndTime = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(runEndTime - fRunStartTime);
    G4cout << "Simulation finished. Time taken: " << elapsed.count() / 1000.0 << " s" << G4endl;
}