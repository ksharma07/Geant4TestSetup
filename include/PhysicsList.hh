#ifndef PhysicsList_hh
#define PhysicsList_hh

#include "G4VModularPhysicsList.hh"   // base physics list class
#include "G4ProcessManager.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4OpticalPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4Gamma.hh"

//#include "G4NeutronTrackingCut.hh"
//#include "G4HadronElasticPhysics.hh"

class PhysicsList : public G4VModularPhysicsList
{
public:
    PhysicsList();                 
    ~PhysicsList() override;

    void SetCuts() override;

};
#endif