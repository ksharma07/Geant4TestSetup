#ifndef SensitiveDetector_hh
#define SensitiveDetector_hh

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4ThreeVector.hh"
#include "G4HCofThisEvent.hh"

#include "G4StepPoint.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

class SensitiveDetector : public G4VSensitiveDetector {
public:
    SensitiveDetector(const G4String& name, G4int ntupleID);  // modified
    virtual ~SensitiveDetector();

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

private:
    G4int fNtupleID;  // NEW: to store assigned ntuple ID
};

#endif
