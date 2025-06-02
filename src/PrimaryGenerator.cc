#include "PrimaryGenerator.hh"

PrimaryGenerator::PrimaryGenerator()
{
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // Choose particle: gamma
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particle);

    // Set initial energy
    fParticleGun->SetParticleEnergy(200*keV);

    // Set initial position
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));

    // Set initial direction (along z-axis)
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}