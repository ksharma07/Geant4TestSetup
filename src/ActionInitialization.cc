#include "ActionInitialization.hh"
#include "PrimaryGenerator.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization() 
{}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGenerator());

    RunAction* runAction = new RunAction();
    SetUserAction(runAction);

    EventAction* eventAction = new EventAction();
    SetUserAction(eventAction);

    SetUserAction(new SteppingAction(eventAction));
}

void ActionInitialization::BuildForMaster() const
{
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);

    SetUserAction(new RunAction());
}