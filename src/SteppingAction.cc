#include "SteppingAction.hh"

SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction)
{}

SteppingAction::~SteppingAction() 
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
	if (!fEventAction) {
		return;
	}

	const auto* secondaries = step->GetSecondaryInCurrentStep();
	if (!secondaries || secondaries->empty()) {
		return;
	}

	G4int opticalCreatedThisStep = 0;
	for (const auto* secondary : *secondaries) {
		if (secondary->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
			opticalCreatedThisStep += 1;
		}
	}

	if (opticalCreatedThisStep > 0) {
		fEventAction->AddOpticalPhotonsCreated(opticalCreatedThisStep);
	}
}