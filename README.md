This Geant4 project simulates the Scintillator-SiPM tests with gamma source (Na22) with reference detectors BaF2 and CeBr3.

## Optical Simulation (Simple Flow)

- Involved files: `src/PhysicsList.cc`, `include/PhysicsList.hh`, `src/Materials.cc`, `include/Materials.hh`, `src/DetectorConstruction.cc`, `include/DetectorConstruction.hh`, `src/SensitiveDetector.cc`, `include/SensitiveDetector.hh`, `src/RunAction.cc`

This project now includes full optical-photon transport and SiPM detection.

### 1. Physics is enabled
- Involved files: `src/PhysicsList.cc`, `include/PhysicsList.hh`
- Optical processes are registered in the physics list.
- Without this, Geant4 does not transport scintillation photons.

### 2. Optical properties are set in materials
- Involved files: `src/Materials.cc`, `include/Materials.hh`
- Material tables define behavior inside each material.
- Main properties used:
	- `RINDEX`: Refractive index.
	- `ABSLENGTH`: attenuation inside material.
	- `RAYLEIGH`: scattering inside material.
	- `SCINTILLATIONCOMPONENT1`: emission spectrum shape.
	- `SCINTILLATIONYIELD`: mean photons per deposited energy.
	- `SCINTILLATIONTIMECONSTANT1`: emission timing.

### 3. Geometry uses optical-ready materials
- Involved files: `src/DetectorConstruction.cc`, `include/DetectorConstruction.hh`, `src/Materials.cc`
- Bar, Al film, grease, and SiPM use materials with optical tables.
- Grease is placed between bar and SiPM for coupling (`bar -> grease -> SiPM`).

### 4. Interface behavior is defined separately
- Involved files: `src/DetectorConstruction.cc`, `include/DetectorConstruction.hh`
- Material properties are bulk behavior.
- Boundary behavior is defined with optical surfaces.
- Al wrap boundaries are set as reflective using `G4LogicalBorderSurface`.

### 5. SiPM detection is modeled in sensitive detector logic
- Involved files: `src/SensitiveDetector.cc`, `include/SensitiveDetector.hh`
- Only optical photons are handled for SiPM ntuples.
- For each arriving photon:
	- Compute photon energy and wavelength.
	- Get PDE from wavelength-dependent SiPM curve.
	- Draw one random number in `[0, 1)`.
	- If random number is below PDE, photon is counted as detected.

### 6. Why photon track is killed at SiPM
- Involved files: `src/SensitiveDetector.cc`
- After one photon is logged, track is stopped with `fStopAndKill`.
- This avoids counting the same photon multiple times in the SiPM volume.

### 7. Output structure for optics
- Involved files: `src/RunAction.cc`, `src/SensitiveDetector.cc`
- Ntuple 4: `SiPMXPlusHits`.
- Ntuple 5: `SiPMXMinusHits`.
- Each row stores:
	- `eventID`, `detected`, `photonEnergyEV`, `wavelengthNm`, `time`
	- momentum, position, track and parent IDs, process and particle name.

### 8. Random detection interpretation
- Involved files: `src/SensitiveDetector.cc`
- Example: if PDE at one wavelength is 0.60, each photon has 60 percent chance to be detected.
- For 100 photons at that wavelength, result is typically around 60 detected and 40 missed.
- Small runs fluctuate more. Large runs converge to PDE expectation.

## Notes
- Involved files: `src/SensitiveDetector.cc`, `README.md`
- Current SiPM PDE is wavelength-dependent in code (interpolated from the provided PDE curve).
- For best realism, replace curve points with exact vendor datasheet points when available.

## Optical Edit Map (What to Change and Where)

Use this table when you want to tune optical behavior.

| Scope | Property / Setting | Material or Interface | Current location to edit |
|---------|--------------------------|-------------------|-------------------------|
| Physics | Register optical physics | Global simulation | `src/PhysicsList.cc:10` |
| Material (bulk) | `RINDEX` | Air | `src/Materials.cc:214` |
| Material (bulk) | `ABSLENGTH` | Air | `src/Materials.cc:215` |

| Material (bulk) | `RINDEX` | eljen230 | `src/Materials.cc:232` |
| Material (bulk) | `ABSLENGTH` | eljen230 | `src/Materials.cc:233` |
| Material (bulk) | `RAYLEIGH` | eljen230 | `src/Materials.cc:234` |
| Scintillation | `SCINTILLATIONCOMPONENT1` | eljen230 | `src/Materials.cc:235` |
| Scintillation | `SCINTILLATIONYIELD` | eljen230 | `src/Materials.cc:236` |
| Scintillation | `RESOLUTIONSCALE` | eljen230 | `src/Materials.cc:237` |
| Scintillation | `SCINTILLATIONTIMECONSTANT1` | eljen230 | `src/Materials.cc:238` |
| Scintillation | `SCINTILLATIONYIELD1` | eljen230 | `src/Materials.cc:239` |

| Material (bulk) | `RINDEX` | eljen232q | `src/Materials.cc:251` |
| Material (bulk) | `ABSLENGTH` | eljen232q | `src/Materials.cc:252` |

| Material (bulk) | `RINDEX` | Aluminium | `src/Materials.cc:264` |
| Material (bulk) | `ABSLENGTH` | Aluminium | `src/Materials.cc:265` |

| Material (bulk) | `RINDEX` | silicon (SiPM) | `src/Materials.cc:277` |
| Material (bulk) | `ABSLENGTH` | silicon (SiPM) | `src/Materials.cc:278` |

| Material (bulk) | `RINDEX` | OpticalGrease | `src/Materials.cc:289` |
| Material (bulk) | `ABSLENGTH` | OpticalGrease | `src/Materials.cc:290` |

| Surface (interface) | `SetType(dielectric_metal)` | Bar <-> Al film | `src/DetectorConstruction.cc:121` |
| Surface (interface) | `SetModel(glisur)` | Bar <-> Al film | `src/DetectorConstruction.cc:123` |
| Surface (interface) | `SetFinish(polished)` | Bar <-> Al film | `src/DetectorConstruction.cc:124` |
| Surface (interface) | `REFLECTIVITY` | Bar <-> Al film | `src/DetectorConstruction.cc:131` |
| Surface (interface) | `EFFICIENCY` | Bar <-> Al film | `src/DetectorConstruction.cc:132` |
| Surface (interface) | `G4LogicalBorderSurface` bindings | All 4 long Al faces (+/-Y, +/-Z) | `src/DetectorConstruction.cc:137-144` |

| SiPM detection | PDE curve wavelength points (`wlNm`) | SiPM readout model | `src/SensitiveDetector.cc:9` |
| SiPM detection | PDE values (`pdeFrac`) | SiPM readout model | `src/SensitiveDetector.cc:12` |
| SiPM detection | Energy to wavelength conversion | SiPM readout model | `src/SensitiveDetector.cc:68-71` |
| SiPM detection | PDE lookup call | SiPM readout model | `src/SensitiveDetector.cc:75` |
| SiPM detection | One-photon consume (`fStopAndKill`) | SiPM readout model | `src/SensitiveDetector.cc:108` |
| Output schema | Ntuple name and columns (+X SiPM) | optical output tree 4 | `src/RunAction.cc:102-118` |
| Output schema | Ntuple name and columns (-X SiPM) | optical output tree 5 | `src/RunAction.cc:121-137` |

Tip: if line numbers shift after edits, search by property name in the same file (for example `SCINTILLATIONYIELD`, `REFLECTIVITY`, or `pdeFrac`).