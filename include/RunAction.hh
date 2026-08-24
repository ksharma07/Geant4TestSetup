#ifndef RunAction_hh
#define RunAction_hh

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "globals.hh"
#include <chrono>

class G4Run;

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    std::chrono::steady_clock::time_point fRunStartTime;
};

#endif