#include "PhysicsList.hh"

PhysicsList::PhysicsList()
{
    RegisterPhysics(new G4EmStandardPhysics_option3());     
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