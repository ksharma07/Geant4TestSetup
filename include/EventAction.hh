#ifndef EventAction_hh
#define EventAction_hh 

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include "RunAction.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
};

#endif
