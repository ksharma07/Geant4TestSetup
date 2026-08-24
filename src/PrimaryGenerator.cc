#include "PrimaryGenerator.hh"
#include <cmath>

PrimaryGenerator::PrimaryGenerator()
{
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);
    
    /*
    // Choose particle: gamma
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particle);

    // Set initial energy
    fParticleGun->SetParticleEnergy(200*keV);

    // Set initial position
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));

    // Set initial direction (along z-axis)
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    */
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{   
    /*G4int Z = 11;    // Sodium
    G4int A = 22;    // Mass number
    G4double excitationEnergy = 0.*keV;

    G4ParticleDefinition* particle = G4IonTable::GetIonTable()->GetIon(Z, A, excitationEnergy);
    if (!particle) {
        G4Exception("PrimaryGenerator::GeneratePrimaries", "NoNa22", FatalException, "Na22 ion could not be created!");
    }
    if (particle->GetDecayTable()) {
        G4cout << "Decay table for " << particle->GetParticleName() << " exists." << G4endl;
        G4cout << "Lifetime: " << particle->GetPDGLifeTime()/s << " s" << G4endl;
    } else {
        G4cout << "NO decay table found for " << particle->GetParticleName() << G4endl;
    }

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleCharge(0. * eplus);
    fParticleGun->SetParticleEnergy(0.0 * keV);

    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));  // Source position
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
    fParticleGun->GeneratePrimaryVertex(anEvent);*/

    G4ParticleGun* particleGun = new G4ParticleGun(1);

    // Define gamma particle
    G4ParticleDefinition* gamma = G4Gamma::GammaDefinition();

    // Sample one common source point uniformly over a disk of radius 2 mm.
    const G4double sourceRadius = 2.0*mm;
    const G4double u = G4UniformRand();
    const G4double phi = CLHEP::twopi*G4UniformRand();
    const G4double r = sourceRadius*std::sqrt(u);
    const G4double x = r*std::cos(phi);
    const G4double y = r*std::sin(phi);
    const G4ThreeVector sourcePos(x, y, 0.0);

    // Set first gamma
    particleGun->SetParticleDefinition(gamma);
    particleGun->SetParticleEnergy(511*keV);
    particleGun->SetParticlePosition(sourcePos);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    particleGun->GeneratePrimaryVertex(anEvent);

    // Set second gamma opposite direction
    particleGun->SetParticleDefinition(gamma);
    particleGun->SetParticleEnergy(511*keV);
    particleGun->SetParticlePosition(sourcePos);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
    particleGun->GeneratePrimaryVertex(anEvent);

    delete particleGun;
}