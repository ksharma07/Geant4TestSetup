#include "SensitiveDetector.hh"
#include "Randomize.hh"
#include <array>

namespace {
G4double GetSiPMPDEFromWavelengthNm(G4double wavelengthNm)
{
    // this part tabulates the SiPM PDE-vs-wavelength curve from the datasheet.
    // Key features: UV rise to local max ~51% at 350 nm, dip to ~42% at 370 nm,
    // main peak ~63% at 420 nm, then gradual fall to ~6% at 900 nm.
    static const std::array<G4double, 17> wlNm = {
        260.0, 280.0, 300.0, 320.0, 350.0, 370.0, 390.0, 420.0, 450.0,
        500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 900.0};
    static const std::array<G4double, 17> pdeFrac = {
        0.06, 0.08, 0.25, 0.40, 0.51, 0.42, 0.52, 0.63, 0.58,
        0.51, 0.44, 0.39, 0.31, 0.23, 0.16, 0.11, 0.06};

    if (wavelengthNm <= wlNm.front()) {
        return pdeFrac.front();
    }
    if (wavelengthNm >= wlNm.back()) {
        return pdeFrac.back();
    }

    for (std::size_t i = 0; i + 1 < wlNm.size(); ++i) {
        if (wavelengthNm >= wlNm[i] && wavelengthNm < wlNm[i + 1]) {
            G4double t = (wavelengthNm - wlNm[i]) / (wlNm[i + 1] - wlNm[i]);
            return pdeFrac[i] + t * (pdeFrac[i + 1] - pdeFrac[i]);
        }
    }

    return pdeFrac.back();
}
}

SensitiveDetector::SensitiveDetector(const G4String& name, G4int ntupleID)
: G4VSensitiveDetector(name), fNtupleID(ntupleID)
{}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    // this part grabs commonly used event/track handles once for all branches.
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4Track* track = step->GetTrack();
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    G4String pname = track->GetDefinition()->GetParticleName();
    G4String vname = track->GetVolume()->GetName();
    //if (edep < 0.) return false;

    //if (track->GetCurrentStepNumber() == 1) {
    //G4cout << "Track started: " << "ID = " << trackID << ", ParentID = " << parentID << ", Particle = " << pname << ", Volume = " << vname << G4endl;
    //}

    // this part gets step points early because both optical and non-optical
    // branches use time/position/momentum information from these points.
    G4StepPoint* prePoint = step->GetPreStepPoint();
    G4StepPoint* postPoint = step->GetPostStepPoint();

    // this part handles SiPM trees as optical-photon counters with fixed PDE.
    // IDs 4 and 5 are dedicated to +X and -X SiPM trees respectively.
    if (fNtupleID == 4 || fNtupleID == 5) {
        // this part ignores non-optical particles for SiPM optical analysis.
        if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
            return false;
        }

        // this part converts photon kinetic energy to eV and wavelength (nm).
        G4double photonEnergyEV = prePoint->GetKineticEnergy() / eV;
        G4double wavelengthNm = -1.0;
        if (photonEnergyEV > 0.0) {
            wavelengthNm = 1239.841984 / photonEnergyEV;
        }

        // this part does a yes/no detection check using wavelength-dependent PDE.
        G4double pde = GetSiPMPDEFromWavelengthNm(wavelengthNm);
        G4double randomU = G4UniformRand();
        G4int detected = 0;
        if (randomU < pde) {
            detected = 1;
        }

        const G4ThreeVector& momdir = prePoint->GetMomentumDirection();
        G4ThreeVector pos = prePoint->GetPosition();
        G4double time = prePoint->GetGlobalTime();
        G4String processName = "optical-arrival";

        auto* analysisManager = G4AnalysisManager::Instance();

        // this part fills the custom SiPM optical schema (not the generic 14-col schema).
        analysisManager->FillNtupleIColumn(fNtupleID, 0, eventID);
        analysisManager->FillNtupleIColumn(fNtupleID, 1, detected);
        analysisManager->FillNtupleDColumn(fNtupleID, 2, photonEnergyEV);
        analysisManager->FillNtupleDColumn(fNtupleID, 3, wavelengthNm);
        analysisManager->FillNtupleDColumn(fNtupleID, 4, time);
        analysisManager->FillNtupleDColumn(fNtupleID, 5, momdir.x());
        analysisManager->FillNtupleDColumn(fNtupleID, 6, momdir.y());
        analysisManager->FillNtupleDColumn(fNtupleID, 7, momdir.z());
        analysisManager->FillNtupleDColumn(fNtupleID, 8, pos.x());
        analysisManager->FillNtupleDColumn(fNtupleID, 9, pos.y());
        analysisManager->FillNtupleDColumn(fNtupleID, 10, pos.z());
        analysisManager->FillNtupleIColumn(fNtupleID, 11, trackID);
        analysisManager->FillNtupleIColumn(fNtupleID, 12, parentID);
        analysisManager->FillNtupleSColumn(fNtupleID, 13, processName);
        analysisManager->FillNtupleSColumn(fNtupleID, 14, pname);
        analysisManager->AddNtupleRow(fNtupleID);

        // this part kills photon at SiPM boundary right after counted so one arriving photon cannot create multiple entries across multiple steps inside SiPM volume.
        track->SetTrackStatus(fStopAndKill);
        return true;
    }

    G4double edep = step->GetTotalEnergyDeposit();
    
    G4double energychange = prePoint->GetKineticEnergy() - postPoint->GetKineticEnergy();

    G4ThreeVector pre = prePoint->GetPosition();
    G4ThreeVector pos = postPoint->GetPosition();
    G4double time = prePoint->GetGlobalTime();

    const G4ThreeVector& momdir = step->GetPostStepPoint()->GetMomentumDirection();

    // this part safely handles null process pointers that can occur for some steps.
    G4String processName = "none";
    if (postPoint->GetProcessDefinedStep()) {
        processName = postPoint->GetProcessDefinedStep()->GetProcessName();
    }

    auto* analysisManager = G4AnalysisManager::Instance();

    // Use stored ntuple ID, no need to check volume name
    analysisManager->FillNtupleIColumn(fNtupleID, 0, eventID);
    analysisManager->FillNtupleDColumn(fNtupleID, 1, edep);
    analysisManager->FillNtupleDColumn(fNtupleID, 2, energychange);
    analysisManager->FillNtupleDColumn(fNtupleID, 3, time);
    analysisManager->FillNtupleDColumn(fNtupleID, 4, momdir.x());
    analysisManager->FillNtupleDColumn(fNtupleID, 5, momdir.y());
    analysisManager->FillNtupleDColumn(fNtupleID, 6, momdir.z());
    analysisManager->FillNtupleDColumn(fNtupleID, 7, pos.x());
    analysisManager->FillNtupleDColumn(fNtupleID, 8, pos.y());
    analysisManager->FillNtupleDColumn(fNtupleID, 9, pos.z());
    analysisManager->FillNtupleIColumn(fNtupleID, 10, trackID);
    analysisManager->FillNtupleIColumn(fNtupleID, 11, parentID);
    analysisManager->FillNtupleSColumn(fNtupleID, 12, processName);
    analysisManager->FillNtupleSColumn(fNtupleID,13, pname);
    analysisManager->AddNtupleRow(fNtupleID);

    return true;
}