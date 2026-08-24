#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "Materials.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4RotationMatrix.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

#include "SensitiveDetector.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

    DetectorConstruction();
    virtual ~DetectorConstruction();
  
    virtual G4VPhysicalVolume *Construct(); //Whole description of detector put in this function

private:
    virtual void ConstructSDandField();
};

#endif