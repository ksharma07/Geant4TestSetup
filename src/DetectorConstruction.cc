#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{

}

DetectorConstruction::~DetectorConstruction()
{

}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // Get the NIST material manager
    G4NistManager* nist = G4NistManager::Instance();

    G4double worldSize = 1.0 * m;

    G4Box* solidWorld = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);

    G4Material* worldmat = nist->FindOrBuildMaterial("G4_AIR");
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldmat, "World");

    // Place the world physical volume at origin, no mother volume
    G4VPhysicalVolume* physWorld = new G4PVPlacement(
        0,                    // no rotation
        G4ThreeVector(),      // at (0,0,0)
        logicWorld,           // its logical volume
        "World",              // its name
        0,                    // no mother volume
        false,                // no boolean operation
        0,                    // copy number
        true);                // check overlaps

    return physWorld;
}