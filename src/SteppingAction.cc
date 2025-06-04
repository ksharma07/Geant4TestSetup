#include "SteppingAction.hh"

SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction)
{}

SteppingAction::~SteppingAction() 
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{}