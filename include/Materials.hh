#ifndef Materials_hh
#define Materials_hh

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

class Materials
{
public:
  Materials();
  ~Materials();

  G4Material *FindMaterial(G4String);

private:
  // Elements
  G4Element *elementH;
  G4Element *elementD;
  G4Element *elementO;
  G4Element *elementN;
  G4Element *elementS;
  G4Element *elementFe;
  G4Element *elementCa;
  G4Element *elementSi;
  G4Element *elementLi;
  G4Element *elementAl;
  G4Element *elementPb;
  G4Element *elementCe;
  G4Element *elementBr;
  G4Element *elementBa;
  G4Element *elementF;
  G4Element *elementC;
  G4Element *elementB;
  G4Element *elB10;
  G4Element *elB11;
  G4Isotope *deuter;
  G4Isotope *B10;
  G4Isotope *B11;
  G4Element *elementW;

  // Materials
  G4Material *vacuum;
  G4Material *water;
  G4Material *lead;
  G4Material *iron;
  G4Material *silicon;
  G4Material *heavywater;
  G4Material *air;
  G4Material *concrete;
  G4Material *carbon;
  G4Material *boron;
  G4Material *aluminium;
  G4Material *glassFiber;
  G4Material *epoxyResin;
  G4Material *FR4;
  G4Material *polyEth;
  G4Material *boratedPE;
  G4Material *lithiumPE;
  G4Material *boratedW;
  G4Material *ABS;
  G4Material *CeBr3;
  G4Material *BaF2;
  G4Material *SiO2;
  G4Material *ABS_GF;
  G4Material *opticalGrease;

  G4Material *eljen230;
  G4Material *eljen232q;
  G4Material *boricAcid;

  G4double temperature, pressure, density;

  // this part does all optical/scintillation table setup in one place so Construct()
  // can simply request materials by name and already have optical behavior attached.
  void BuildOpticalProperties();
};

#endif
