#include "EventAction.hh"

EventAction::EventAction() : G4UserEventAction()
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
    //run at start of each event
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    //run at end of each event 
    G4int evtID = event->GetEventID();
    G4cout << "End of event: " << evtID << G4endl;
    //fill an empty file so it doesnt get deleted on its own
    /*auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, 0);         // Dummy evtID = 0
    analysisManager->FillNtupleDColumn(1, 1.23);      // Dummy energy
    analysisManager->AddNtupleRow();*/
}
