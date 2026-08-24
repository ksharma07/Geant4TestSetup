#include "EventAction.hh"

EventAction::EventAction()
    : G4UserEventAction(),
      fOpticalPhotonsCreated(0)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
    fOpticalPhotonsCreated = 0;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4int evtID = event->GetEventID();
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(6, 0, evtID);
    analysisManager->FillNtupleIColumn(6, 1, fOpticalPhotonsCreated);
    analysisManager->AddNtupleRow(6);
}

void EventAction::AddOpticalPhotonsCreated(G4int count)
{
    fOpticalPhotonsCreated += count;
}
