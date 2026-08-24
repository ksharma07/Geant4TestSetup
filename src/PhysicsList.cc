#include "PhysicsList.hh"

PhysicsList::PhysicsList()
{
    // this part keeps current EM model exactly as-is (option3).
    RegisterPhysics(new G4EmStandardPhysics_option3());     

    // this part adds optical processes (scintillation, absorption, boundary, etc.)
    // transport optical photons to the SiPM volumes.
    RegisterPhysics(new G4OpticalPhysics());
}

PhysicsList::~PhysicsList()
{

}

void PhysicsList::SetCuts()
{   //any secondaries with distance above 0mm is produced. so basically all. 
    SetCutValue(0, "gamma");
    SetCutValue(0, "e-");
    SetCutValue(0, "e+");
    SetCutValue(0, "proton");
}