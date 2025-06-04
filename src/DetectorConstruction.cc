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
    Materials *materials = new Materials();

    G4double worldSize = 1.0 * m;

    G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
    G4Box* solidWorld = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);

    auto* worldVis = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.1)); // light grey, mostly transparent
    worldVis->SetVisibility(true);
    worldVis->SetForceSolid(true);
    logicWorld->SetVisAttributes(worldVis);
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Detector: Eljen230 box
    //G4Material* barMat = nist->FindOrBuildMaterial("G4_Al");
    G4Material *barMat = materials->FindMaterial("eljen230");

    G4Box* barDetector = new G4Box("barDetector", 13.5/2*cm, 0.75/2*cm, 0.75/2*cm);
    G4LogicalVolume* logicL2bar = new G4LogicalVolume(barDetector, barMat, "logicL2bar");

    // Visual attributes: Yellow, solid, visible
    G4VisAttributes* visBar = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5)); 
    visBar->SetVisibility(true);
    visBar->SetForceSolid(true);
    logicL2bar->SetVisAttributes(visBar);

    // Place detector 20 cm in front of world origin on z-axis
    G4VPhysicalVolume *L2bar = new G4PVPlacement(0, G4ThreeVector(0, 0, 20*cm), logicL2bar, "L2bar", logicWorld, false, 0, true);
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Detector: CeBr3-Scionix
    G4double cebrD = 25.0*mm;
    G4double cebrH = 25.0*mm;

    G4Material *cebr3Mat = materials->FindMaterial("CeBr3");
    G4Tubs* cebrCrystal = new G4Tubs("CeBr3", 0, cebrD/2, cebrH/2, 0, 360*deg);
    G4LogicalVolume* logicCebr = new G4LogicalVolume(cebrCrystal, cebr3Mat, "logicCeBr3");

    G4VisAttributes* visCebr = new G4VisAttributes(G4Color(0.0, 1.0, 0.0, 0.5)); 
    visCebr->SetVisibility(true);
    visCebr->SetForceSolid(true);
    logicCebr->SetVisAttributes(visCebr);

    G4VPhysicalVolume *CeBr3 = new G4PVPlacement(0, G4ThreeVector(0, 0, 25*cm), logicCebr, "CeBr3", logicWorld, false, 0, true);
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Detector: BaF2 (assumed same dimensions)
    G4double bafD = 25.0*mm;
    G4double bafH = 25.0*mm;

    G4Material *baf2Mat = materials->FindMaterial("BaF2");
    G4Tubs* bafCrystal = new G4Tubs("BaF2", 0, bafD/2, bafH/2, 0, 360*deg);
    G4LogicalVolume* logicBaf = new G4LogicalVolume(bafCrystal, baf2Mat, "logicBaF2");

    G4VisAttributes* visBaf = new G4VisAttributes(G4Color(0.0, 0.0, 1.0, 0.5)); 
    visBaf->SetVisibility(true);
    visBaf->SetForceSolid(true);
    logicBaf->SetVisAttributes(visBaf);

    G4VPhysicalVolume *BaF2 = new G4PVPlacement(0, G4ThreeVector(0, 0, -20*cm), logicBaf, "BaF2", logicWorld, false, 0, true);

    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    // Get the Sensitive Detector Manager
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();

    // Create your sensitive detector
    SensitiveDetector* L2barSD = new SensitiveDetector("L2barSD", 0);
    sdManager->AddNewDetector(L2barSD);
    G4LogicalVolume* logicL2Bar = G4LogicalVolumeStore::GetInstance()->GetVolume("logicL2bar");
    logicL2Bar->SetSensitiveDetector(L2barSD);

    SensitiveDetector* cebrSD = new SensitiveDetector("CebrSD", 1);
    sdManager->AddNewDetector(cebrSD);
    G4LogicalVolume* logicCebr = G4LogicalVolumeStore::GetInstance()->GetVolume("logicCeBr3");
    logicCebr->SetSensitiveDetector(cebrSD);

    SensitiveDetector* bafSD = new SensitiveDetector("BafSD", 2);
    sdManager->AddNewDetector(bafSD);
    G4LogicalVolume* logicBaf = G4LogicalVolumeStore::GetInstance()->GetVolume("logicBaF2");
    logicBaf->SetSensitiveDetector(bafSD);
}