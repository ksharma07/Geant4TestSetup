#ifndef PhysicsList_hh
#define PhysicsList_hh

#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"
#include "G4EmStandardPhysics.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4DecayPhysics.hh"
#include "NeutronHPphysics.hh"//
#include "G4OpticalPhysics.hh"
//#include "G4NeutronTrackingCut.hh"
//#include "G4HadronElasticPhysics.hh"

class PhysicsList : public G4VModularPhysicsList
{
public:
    PhysicsList();                 
    virtual ~PhysicsList();
};
#endif