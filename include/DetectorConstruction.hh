#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4UnionSolid.hh"
#include "G4Cons.hh"

// namespace Materials{
//     G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
//     G4Material* water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
//     G4Material* cobalt = G4NistManager::Instance()->FindOrBuildMaterial("G4_Co");
//     G4Material* Pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
//     G4Material* tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
//     G4Material* Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
//     G4Material* Fe = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");

//     G4Element* elFe = G4NistManager::Instance()->FindOrBuildElement("Fe");
//     G4Element* elNi = G4NistManager::Instance()->FindOrBuildElement("Ni");
//     G4Element* elCr = G4NistManager::Instance()->FindOrBuildElement("Cr");
// }


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();
    
    virtual G4VPhysicalVolume *Construct();
    void SetHelmetSize(G4int);

private:
    void ConstructCell(G4ThreeVector&, G4RotationMatrix*, G4LogicalVolume*, G4Material*);
    void GetSolid(G4ThreeVector&, G4RotationMatrix*, G4LogicalVolume*);
    void UpdateHelmet();

    G4Cons* solidColl_helmet;
    G4LogicalVolume* logicColl_helmet;  
    G4VPhysicalVolume* physiColl_helmet;

    G4Cons* solidColl_helmet_Fe;           
    G4LogicalVolume* logicColl_helmet_Fe;  
    G4VPhysicalVolume* physiColl_helmet_Fe;


    G4int helmetSize;
};

#endif
