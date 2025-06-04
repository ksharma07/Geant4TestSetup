#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(const G4String& name, G4int ntupleID)
: G4VSensitiveDetector(name), fNtupleID(ntupleID)
{}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4Track* track = step->GetTrack();
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    G4String pname = track->GetDefinition()->GetParticleName();
    G4String vname = track->GetVolume()->GetName();
    //if (edep < 0.) return false;

    if (track->GetCurrentStepNumber() == 1) {
        G4cout << "Track started: " << "ID = " << trackID << ", ParentID = " << parentID << ", Particle = " << pname << ", Volume = " << vname << G4endl;
    }

    G4double edep = step->GetTotalEnergyDeposit();
    
    G4StepPoint* prePoint = step->GetPreStepPoint();
    G4StepPoint* postPoint = step->GetPostStepPoint();
    G4double energychange = prePoint->GetKineticEnergy() - postPoint->GetKineticEnergy();

    G4ThreeVector pos = prePoint->GetPosition();
    G4double time = prePoint->GetGlobalTime();

    G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    auto* analysisManager = G4AnalysisManager::Instance();

    // Use stored ntuple ID, no need to check volume name
    analysisManager->FillNtupleIColumn(fNtupleID, 0, eventID);
    analysisManager->FillNtupleDColumn(fNtupleID, 1, energychange);
    analysisManager->FillNtupleDColumn(fNtupleID, 2, time);
    analysisManager->FillNtupleDColumn(fNtupleID, 3, pos.x());
    analysisManager->FillNtupleDColumn(fNtupleID, 4, pos.y());
    analysisManager->FillNtupleDColumn(fNtupleID, 5, pos.z());
    analysisManager->FillNtupleIColumn(fNtupleID, 6, trackID);
    analysisManager->FillNtupleIColumn(fNtupleID, 7, parentID);
    analysisManager->FillNtupleSColumn(fNtupleID, 8, processName);
    analysisManager->AddNtupleRow(fNtupleID);

    return true;
}