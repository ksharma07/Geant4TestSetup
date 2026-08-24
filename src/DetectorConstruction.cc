#include "DetectorConstruction.hh"
#include <vector>

DetectorConstruction::DetectorConstruction()
{

}

DetectorConstruction::~DetectorConstruction()
{

}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // this part gets NIST for optional fallback use, while the active detector
    // materials are pulled from our custom Materials class (with MPT attached).
    G4NistManager* nist = G4NistManager::Instance();
    Materials *materials = new Materials();

    G4double worldSize = 1.0 * m;

    // this part deliberately uses custom material names so optical properties
    // from Materials::BuildOpticalProperties() are guaranteed to be present.
    G4Material* air = materials->FindMaterial("Air");
    G4Material* alMat = materials->FindMaterial("Aluminium");
    G4Material* sipmMat = materials->FindMaterial("silicon");
    G4Material* greaseMat = materials->FindMaterial("OpticalGrease");
    G4Box* solidWorld = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);

    auto* worldVis = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.1)); // light grey, mostly transparent
    worldVis->SetVisibility(true);
    worldVis->SetForceSolid(true);
    logicWorld->SetVisAttributes(worldVis);
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Detector: Eljen230 bar
    //G4Material* barMat = nist->FindOrBuildMaterial("G4_Al");
    G4Material *barMat = materials->FindMaterial("eljen230");//L1-eljen232q L2-eljen232

    G4double barHalfX = 27.0/2*cm;
    G4double barHalfY = 0.75/2*cm;
    G4double barHalfZ = 0.75/2*cm;
    G4double alFilmThick = 0.1*mm;
    G4double gap = 0.10*mm;
    // this part sets coupling-layer thickness between bar and each SiPM end.
    G4double greaseThick = 0.1*mm;
    G4double sipmThick = 1.23*mm;

    G4Box* barDetector = new G4Box("barDetector", barHalfX, barHalfY, barHalfZ);
    //G4Box* barDetector = new G4Box("barDetector", 0.75/2*cm, 0.67/2*cm, 0.50/2*cm);
    G4LogicalVolume* logicL2bar = new G4LogicalVolume(barDetector, barMat, "logicL2bar");

    // this part sizes the foil as a 4-wall sleeve around the bar cross-section.
    // Y walls are made longer in Z so changing gap does not open corner slits.
    G4double wrapInnerHalfY = barHalfY + gap;
    G4double wrapInnerHalfZ = barHalfZ + gap;

    G4Box* alFilmY = new G4Box("AlFilmY", barHalfX, alFilmThick/2.0, wrapInnerHalfZ + alFilmThick);
    G4LogicalVolume* logicAlFilmY = new G4LogicalVolume(alFilmY, alMat, "logicAlFilmY");

    G4Box* alFilmZ = new G4Box("AlFilmZ", barHalfX, wrapInnerHalfY, alFilmThick/2.0);
    G4LogicalVolume* logicAlFilmZ = new G4LogicalVolume(alFilmZ, alMat, "logicAlFilmZ");

    G4Box* sipm = new G4Box("SiPM", sipmThick/2.0, barHalfY*0.8, barHalfZ*0.8);
    G4LogicalVolume* logicSiPMXPlus = new G4LogicalVolume(sipm, sipmMat, "logicSiPMXPlus");
    G4LogicalVolume* logicSiPMXMinus = new G4LogicalVolume(sipm, sipmMat, "logicSiPMXMinus");

    // this part creates grease slabs matching the 7.5x7.5 mm bar end faces.
    G4Box* grease = new G4Box("OpticalGrease", greaseThick/2.0, barHalfY, barHalfZ);
    G4LogicalVolume* logicGreaseXPlus = new G4LogicalVolume(grease, greaseMat, "logicGreaseXPlus");
    G4LogicalVolume* logicGreaseXMinus = new G4LogicalVolume(grease, greaseMat, "logicGreaseXMinus");

    // Visual attributes: Yellow, solid, visible
    G4VisAttributes* visBar = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5)); 
    visBar->SetVisibility(true);
    visBar->SetForceSolid(true);
    logicL2bar->SetVisAttributes(visBar);

    G4VisAttributes* visAlFilm = new G4VisAttributes(G4Color(0.7, 0.7, 0.7, 0.6));
    visAlFilm->SetVisibility(true);
    visAlFilm->SetForceSolid(false);
    logicAlFilmY->SetVisAttributes(visAlFilm);
    logicAlFilmZ->SetVisAttributes(visAlFilm);

    G4VisAttributes* visSiPM = new G4VisAttributes(G4Color(1.0, 0.6, 0.0, 0.8));
    visSiPM->SetVisibility(true);
    visSiPM->SetForceSolid(false);
    logicSiPMXPlus->SetVisAttributes(visSiPM);
    logicSiPMXMinus->SetVisAttributes(visSiPM);

    // this part makes grease visible for debugging optical coupling alignment.
    G4VisAttributes* visGrease = new G4VisAttributes(G4Color(0.1, 0.9, 0.9, 0.3));
    visGrease->SetVisibility(true);
    visGrease->SetForceSolid(false);
    logicGreaseXPlus->SetVisAttributes(visGrease);
    logicGreaseXMinus->SetVisAttributes(visGrease);

    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateY(0 * deg);
    // this part is a single scan knob to shift the full L2 assembly in x.
    // keep 0.00*mm for baseline; set +/- values to scan gamma entry position.
    G4double centerShift = barHalfX-135.00* mm;//-constant mm from one end (270-constsnt)mm from other.
    // Place detector 20 cm in front of world origin on z-axis
    G4ThreeVector barCenter(centerShift, 0, 16.5*cm);
    // this part stores physical volume pointers because we need them to create
    // explicit optical border surfaces at interfaces below.
    G4VPhysicalVolume *L2bar = new G4PVPlacement(rot, barCenter, logicL2bar, "L2bar", logicWorld, false, 0, true);

    // this part places a tunable air gap between scintillator and foil while
    // keeping the four foil walls closed at the Y-Z corner lines.
    G4VPhysicalVolume* L2barAlFilmYPlus = new G4PVPlacement(rot, barCenter + G4ThreeVector(0, wrapInnerHalfY + alFilmThick/2.0, 0), logicAlFilmY, "L2barAlFilmYPlus", logicWorld, false, 0, true);
    G4VPhysicalVolume* L2barAlFilmYMinus = new G4PVPlacement(rot, barCenter + G4ThreeVector(0, -(wrapInnerHalfY + alFilmThick/2.0), 0), logicAlFilmY, "L2barAlFilmYMinus", logicWorld, false, 1, true);
    G4VPhysicalVolume* L2barAlFilmZPlus = new G4PVPlacement(rot, barCenter + G4ThreeVector(0, 0, wrapInnerHalfZ + alFilmThick/2.0), logicAlFilmZ, "L2barAlFilmZPlus", logicWorld, false, 0, true);
    G4VPhysicalVolume* L2barAlFilmZMinus = new G4PVPlacement(rot, barCenter + G4ThreeVector(0, 0, -(wrapInnerHalfZ + alFilmThick/2.0)), logicAlFilmZ, "L2barAlFilmZMinus", logicWorld, false, 1, true);

    // this part inserts grease directly after each bar end so photons see bar->grease->SiPM rather than bar->air->SiPM.
    G4VPhysicalVolume* L2barGreaseXPlus = new G4PVPlacement(rot, barCenter + G4ThreeVector(barHalfX + greaseThick/2.0, 0, 0), logicGreaseXPlus, "L2barGreaseXPlus", logicWorld, false, 0, true);
    G4VPhysicalVolume* L2barGreaseXMinus = new G4PVPlacement(rot, barCenter + G4ThreeVector(-(barHalfX + greaseThick/2.0), 0, 0), logicGreaseXMinus, "L2barGreaseXMinus", logicWorld, false, 1, true);

    // Two SiPMs on 7.5x7.5 mm bar end faces, extending outward along +/-X
    G4VPhysicalVolume* L2barSiPMXPlus = new G4PVPlacement(rot, barCenter + G4ThreeVector(barHalfX + greaseThick + sipmThick/2.0, 0, 0), logicSiPMXPlus, "L2barSiPMXPlus", logicWorld, false, 0, true);
    G4VPhysicalVolume* L2barSiPMXMinus = new G4PVPlacement(rot, barCenter + G4ThreeVector(-(barHalfX + greaseThick + sipmThick/2.0), 0, 0), logicSiPMXMinus, "L2barSiPMXMinus", logicWorld, false, 1, true);
    
    /////////////////////////////

    // This part handles effects at interface of 2 layers. Properties defined in materials are only effects in bulk of same material.
    G4OpticalSurface* alReflectiveSurface = new G4OpticalSurface("AlReflectiveSurface");
    alReflectiveSurface->SetType(dielectric_metal);//Al act like metal boundry where reflection absorption happens  
    //all options in G4SurfaceProperty.hh
    alReflectiveSurface->SetModel(glisur);//Some kind of reflectivity surface G4 model
    alReflectiveSurface->SetFinish(polished);//Smooth to rough the surface is.
    //all options at G4OpticalSurface.hh

    G4MaterialPropertiesTable* alSurfaceMPT = new G4MaterialPropertiesTable();
    // Wavelength-dependent Al reflectivity approximated from the provided
    // total-reflectivity plot over the scintillation band (350-500 nm).
    // Energies must be strictly ascending (= wavelength descending).
    std::vector<G4double> surfEnergy = {
        2.480*eV,  // 500 nm
        2.611*eV,  // 475 nm
        2.756*eV,  // 450 nm
        2.918*eV,  // 425 nm
        3.100*eV,  // 400 nm
        3.179*eV,  // 390 nm
        3.307*eV,  // 375 nm
        3.444*eV,  // 360 nm
        3.543*eV}; // 350 nm
    std::vector<G4double> surfReflectivity = {
        0.87, 0.872, 0.875, 0.878, 0.882, 0.884, 0.886, 0.887, 0.888};
    std::vector<G4double> surfEfficiency(surfEnergy.size(), 0.0); // surface does not detect photons
    alSurfaceMPT->AddProperty("REFLECTIVITY", surfEnergy, surfReflectivity);
    alSurfaceMPT->AddProperty("EFFICIENCY",   surfEnergy, surfEfficiency);
    alReflectiveSurface->SetMaterialPropertiesTable(alSurfaceMPT);

    // this part leaves scintillator-air behavior to Fresnel/TIR and binds the
    // reflective metal model on the air<->foil interfaces in both directions.
    new G4LogicalBorderSurface("Air_to_AlYPlus", physWorld, L2barAlFilmYPlus, alReflectiveSurface);
    new G4LogicalBorderSurface("AlYPlus_to_Air", L2barAlFilmYPlus, physWorld, alReflectiveSurface);
    new G4LogicalBorderSurface("Air_to_AlYMinus", physWorld, L2barAlFilmYMinus, alReflectiveSurface);
    new G4LogicalBorderSurface("AlYMinus_to_Air", L2barAlFilmYMinus, physWorld, alReflectiveSurface);
    new G4LogicalBorderSurface("Air_to_AlZPlus", physWorld, L2barAlFilmZPlus, alReflectiveSurface);
    new G4LogicalBorderSurface("AlZPlus_to_Air", L2barAlFilmZPlus, physWorld, alReflectiveSurface);
    new G4LogicalBorderSurface("Air_to_AlZMinus", physWorld, L2barAlFilmZMinus, alReflectiveSurface);
    new G4LogicalBorderSurface("AlZMinus_to_Air", L2barAlFilmZMinus, physWorld, alReflectiveSurface);

    // this part intentionally leaves bar-grease and grease-SiPM interfaces as
    // default dielectric-dielectric boundaries governed by material RINDEX.
    //////////////////////////////////////////////////////////////////////////////////////////////

    // Detector: CeBr3-Scionix
    G4double cebrD = 25.0*mm;
    G4double cebrH = 25.0*mm;

    G4double yshift = 7.5*mm;//if zshift <30.5mm then yshift>=7.5mm 
    G4double zshift = 7.5*mm;

    G4Material *cebr3Mat = materials->FindMaterial("CeBr3");
    G4Tubs* cebrCrystal = new G4Tubs("CeBr3", 0, cebrD/2, cebrH/2, 0, 360*deg);
    G4LogicalVolume* logicCebr = new G4LogicalVolume(cebrCrystal, cebr3Mat, "logicCeBr3");

    G4VisAttributes* visCebr = new G4VisAttributes(G4Color(0.0, 1.0, 0.0, 0.5)); 
    visCebr->SetVisibility(true);
    visCebr->SetForceSolid(true);
    logicCebr->SetVisAttributes(visCebr);

    G4VPhysicalVolume *CeBr3 = new G4PVPlacement(0, G4ThreeVector(0, (12.5 + 3.75)*mm + yshift, 16.5*cm + (12.5 + 3.75)*mm + zshift), logicCebr, "CeBr3", logicWorld, false, 0, true);
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

    //////////////////////////////////////////////////////////////////////////////////////////////

    // Detector: Lead brick
    /*
    G4Material *shldMat = materials->FindMaterial("Lead");
    G4Box* brickPb = new G4Box("LeadBrick", 5*cm, 5*cm, 1*cm);
    G4LogicalVolume* logicPb = new G4LogicalVolume(brickPb, shldMat, "logicPb");

    G4VisAttributes* visPb = new G4VisAttributes(G4Color(0.0, 1.0, 1.0, 0.5)); 
    visPb->SetVisibility(true);
    visPb->SetForceSolid(true);
    logicPb->SetVisAttributes(visPb);

    G4VPhysicalVolume *LeadBrick = new G4PVPlacement(0, G4ThreeVector(0, 0, 5*cm), logicPb, "LeadBrick", logicWorld, false, 0, true);
    */
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

    SensitiveDetector* l2barAlFilmSD = new SensitiveDetector("L2barAlFilmSD", 3);
    sdManager->AddNewDetector(l2barAlFilmSD);
    G4LogicalVolume* logicAlFilmY = G4LogicalVolumeStore::GetInstance()->GetVolume("logicAlFilmY");
    G4LogicalVolume* logicAlFilmZ = G4LogicalVolumeStore::GetInstance()->GetVolume("logicAlFilmZ");
    logicAlFilmY->SetSensitiveDetector(l2barAlFilmSD);
    logicAlFilmZ->SetSensitiveDetector(l2barAlFilmSD);

    SensitiveDetector* sipmXPlusSD = new SensitiveDetector("SiPMXPlusSD", 4);
    sdManager->AddNewDetector(sipmXPlusSD);
    G4LogicalVolume* logicSiPMXPlus = G4LogicalVolumeStore::GetInstance()->GetVolume("logicSiPMXPlus");
    logicSiPMXPlus->SetSensitiveDetector(sipmXPlusSD);

    SensitiveDetector* sipmXMinusSD = new SensitiveDetector("SiPMXMinusSD", 5);
    sdManager->AddNewDetector(sipmXMinusSD);
    G4LogicalVolume* logicSiPMXMinus = G4LogicalVolumeStore::GetInstance()->GetVolume("logicSiPMXMinus");
    logicSiPMXMinus->SetSensitiveDetector(sipmXMinusSD);
}