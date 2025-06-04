#ifndef PrimaryGenerator_hh
#define PrimaryGenerator_hh

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "globals.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGenerator();
  virtual ~PrimaryGenerator();

  virtual void GeneratePrimaries(G4Event*);

private:
  G4ParticleGun *fParticleGun;
};

#endif