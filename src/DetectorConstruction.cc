#include "DetectorConstruction.hh"

MyDetectorConstruction::MyDetectorConstruction() : helmetSize(18)
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();
    
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

    G4Material *SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

    G4Material *H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);

    G4Element *C = nist->FindOrBuildElement("C");

    G4Material *Aerogel = new G4Material("Aerogel", 0.2*g/cm3, 3);
    Aerogel->AddMaterial(SiO2, 62.5*perCent);
    Aerogel->AddMaterial(H2O, 37.4*perCent);
    Aerogel->AddElement(C, 0.1*perCent);

    // G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    // G4Material* water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
    // G4Material* cobalt = G4NistManager::Instance()->FindOrBuildMaterial("G4_Co");
    // G4Material* Pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
    // G4Material* tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
    // G4Material* Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    // G4Material* Fe = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");

    
    

    const G4double worldX = 400.0 *cm;
    const G4double worldY = 400.0 *cm;
    const G4double worldZ = 400.0 *cm;

    G4Box* treatmentRoom = new G4Box("TreatmentRoom",worldX,worldY,worldZ);

    G4LogicalVolume* logicTreatmentRoom = new G4LogicalVolume(treatmentRoom, 
                                                              worldMat, 
                                                              "logicTreatmentRoom", 
							    0,0,0);
    G4VPhysicalVolume* physicalTreatmentRoom = new G4PVPlacement(0,
					    G4ThreeVector(),
    					    "physicalTreatmentRoom", 
    					    logicTreatmentRoom, 
    					    0,false,0);

    logicTreatmentRoom -> SetVisAttributes (G4VisAttributes::GetInvisible());

    G4ThreeVector pos = G4ThreeVector(0,0,-40.1*cm);
    G4RotationMatrix* rot = 0;
    G4LogicalVolume* mv = logicTreatmentRoom;


    GetSolid(pos, 0, logicTreatmentRoom);
    
    return physicalTreatmentRoom;
}

void MyDetectorConstruction::GetSolid(G4ThreeVector& pos, G4RotationMatrix* rot, G4LogicalVolume* mv)
{
    static G4VSolid* shape = nullptr;
    static G4LogicalVolume* logicTube = nullptr;
    static G4LogicalVolume* logicTube_source = nullptr;
    static G4LogicalVolume* logicTube_post = nullptr;
    static G4LogicalVolume* logicTube_coll = nullptr;
    static G4LogicalVolume* logicTube_Fe = nullptr;
    static G4LogicalVolume* logicTube_Al = nullptr;
    static G4LogicalVolume* logicTube_coll_Fe = nullptr;
    static G4LogicalVolume* logicColl_fixed = nullptr;
    static G4LogicalVolume* logicColl_fixed_Fe = nullptr;
    // static G4LogicalVolume* logicColl_helmet = nullptr;
    // static G4LogicalVolume* logicColl_helmet_Fe = nullptr;

    static G4int number = 0;

    if(shape == nullptr) {

        // List of Materials

        G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
        G4Material* water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
        G4Material* cobalt = G4NistManager::Instance()->FindOrBuildMaterial("G4_Co");
        G4Material* Pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
        G4Material* tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
        G4Material* Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
        G4Material* Fe = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");

        G4Element* elFe = G4NistManager::Instance()->FindOrBuildElement("Fe");
        G4Element* elNi = G4NistManager::Instance()->FindOrBuildElement("Ni");
        G4Element* elCr = G4NistManager::Instance()->FindOrBuildElement("Cr");
        G4Material* steel = new G4Material("StainlessSteel", 7.80 * g/cm3, 3 /* components */);
        steel -> AddElement(elFe, 70 * perCent);
        steel -> AddElement(elCr, 18 * perCent);
        steel -> AddElement(elNi, 12 * perCent);


        G4VisAttributes * grayFe = new G4VisAttributes(G4Colour(0.5 ,0.5 ,0.5));
        grayFe -> SetVisibility(true);
        grayFe -> SetForceSolid(true);

        G4VisAttributes * blueCobalt = new G4VisAttributes(G4Colour(0. ,0. ,0.7));
        blueCobalt -> SetVisibility(true);
        blueCobalt -> SetForceSolid(true);

        G4VisAttributes * graySS = new G4VisAttributes(G4Colour(0.9 ,0.9 ,0.9));
        graySS -> SetVisibility(true);
        graySS -> SetForceSolid(true);

        G4VisAttributes * grayAl = new G4VisAttributes(G4Colour(0.7 ,0.7 ,0.7));
        grayAl -> SetVisibility(true);
        grayAl -> SetForceSolid(true);

        G4VisAttributes * blackLead = new G4VisAttributes(G4Colour(0.2 ,0.2 ,0.2));
        blackLead -> SetVisibility(true);
        blackLead -> SetForceSolid(true);


        G4VisAttributes * colorTungsten = new G4VisAttributes(G4Colour(0.3 ,0.3 ,0.3));
        colorTungsten -> SetVisibility(true);
        colorTungsten -> SetForceSolid(true);
        
        // Main Tube 

        G4double innerRadiusOfTheTube = 15.*mm;
        G4double outerRadiusOfTheTube = 50.*mm;
        G4double hightOfTheTube_Fe = 1*cm;
        G4double startAngleOfTheTube = 0.*deg;
        G4double spanningAngleOfTheTube = 360.*deg;
    
        G4Tubs* solidTube = new G4Tubs("solidTube",    
				 innerRadiusOfTheTube,    
				 outerRadiusOfTheTube,   
				 hightOfTheTube_Fe,          
				 startAngleOfTheTube,     
				 spanningAngleOfTheTube); 
        logicTube = new G4LogicalVolume(solidTube, Fe, "logicTube_Fe",0,0,0);
        logicTube -> SetVisAttributes(graySS);

        // tube_source

        G4double innerRadiusOfTheTube_source = 0.;
        G4double outerRadiusOfTheTube_source = 0.5*mm;
        G4double hightOfTheTube_source = 1*cm;

        G4Tubs* solidTube_source = new G4Tubs("solidTube_source",    
        				 innerRadiusOfTheTube_source,    
        				 outerRadiusOfTheTube_source,   
        				 hightOfTheTube_source,       
        				 startAngleOfTheTube,             
        				 spanningAngleOfTheTube);  
        logicTube_source = new G4LogicalVolume(solidTube_source, Fe,"logicTube_source",0,0,0);
        logicTube_source -> SetVisAttributes(blueCobalt);

        // Tube_post

        G4double innerRadiusOfTheTube_post = 0;
        G4double outerRadiusOfTheTube_post = 50*mm;
        G4double hightOfTheTube_post = 1*cm;

        G4Tubs* solidTube_post = new G4Tubs("solidTube_post",    
        				 innerRadiusOfTheTube_post,   
        				 outerRadiusOfTheTube_post,   
        				 hightOfTheTube_post,          
        				 startAngleOfTheTube,     
        				 spanningAngleOfTheTube); 
        logicTube_post = new G4LogicalVolume(solidTube_post, Fe,"logicTube_post",0,0,0);
        logicTube_post -> SetVisAttributes(grayFe);

        // Tube_coll 

        G4double innerRadiusOfTheTube_coll = 2.5*mm;
        G4double outerRadiusOfTheTube_coll = 15.*mm;
        G4double hightOfTheTube_coll = 3.25*cm;
 
        G4Tubs* solidTube_coll = new G4Tubs("solidTube_coll",    
        				 innerRadiusOfTheTube_coll,    
        				 outerRadiusOfTheTube_coll,    
        				 hightOfTheTube_coll,         
        				 startAngleOfTheTube,     
        				 spanningAngleOfTheTube); 
        logicTube_coll = new G4LogicalVolume(solidTube_coll,tungsten,"logicTube_coll",0,0,0);
        logicTube_coll -> SetVisAttributes(colorTungsten);

        // Tube_Al

        G4double innerRadiusOfTheTube_Al = 4.*mm;
        G4double outerRadiusOfTheTube_Al = 15.*mm;
        G4double hightOfTheTube_Al = 1*cm;

        G4Tubs* solidTube_Al = new G4Tubs("solidTube_Al",    
        				 innerRadiusOfTheTube_Al,   
        				 outerRadiusOfTheTube_Al,    
        				 hightOfTheTube_Al,         
        				 startAngleOfTheTube,     
        				 spanningAngleOfTheTube); 
        logicTube_Al = new G4LogicalVolume(solidTube_Al,Al,"logicTube_Al",0,0,0);
        logicTube_Al -> SetVisAttributes(grayAl);

        // Tube_Fe

        G4double innerRadiusOfTheTube_Fe = 15.*mm;
        G4double outerRadiusOfTheTube_Fe = 50.*mm;

        G4Tubs* solidTube_Fe = new G4Tubs("solidTube_Fe",    
        				 innerRadiusOfTheTube_Fe,    
        				 outerRadiusOfTheTube_Fe,   
        				 hightOfTheTube_Fe,          
        				 startAngleOfTheTube,     
        				 spanningAngleOfTheTube); 
        logicTube_Fe = new G4LogicalVolume(solidTube_Fe,Fe,"logicTube_Fe",0,0,0);
        logicTube_Fe -> SetVisAttributes(grayFe);

        // Tube_Fe_coll

        G4double innerRadiusOfTheTube_coll_Fe = 15.*mm;
        G4double outerRadiusOfTheTube_coll_Fe = 50.*mm;
        G4double hightOfTheTube_coll_Fe = 3.25*cm;

        G4Tubs* solidTube_coll_Fe = new G4Tubs("solidTube_coll_Fe",    
        				 innerRadiusOfTheTube_coll_Fe,    
        				 outerRadiusOfTheTube_coll_Fe,    
        				 hightOfTheTube_coll_Fe,          
        				 startAngleOfTheTube,     
               				 spanningAngleOfTheTube); 
        logicTube_coll_Fe = new G4LogicalVolume(solidTube_coll_Fe,Fe,"logicTube_coll_Fe",0,0,0);
        logicTube_coll_Fe -> SetVisAttributes(grayFe);

        // Coll_Fixed

        G4double Rmin1Coll_fixed = 2.5*mm;
        G4double Rmax1Coll_fixed = 15.*mm;
        G4double Rmin2Coll_fixed = 4.25*mm;
        G4double Rmax2Coll_fixed = 15.*mm;
        G4double hightColl_fixed = 4.625*cm;

        G4Cons* solidColl_fixed = new G4Cons("solidColl_fixed",
        			       Rmin1Coll_fixed,  
        			       Rmax1Coll_fixed, 
        			       Rmin2Coll_fixed,  
        			       Rmax2Coll_fixed, 
        			       hightColl_fixed,  
        			       startAngleOfTheTube,    
        			       spanningAngleOfTheTube);
        logicColl_fixed = new G4LogicalVolume(solidColl_fixed,Pb,"logicColl_fixed",0,0,0);
        logicColl_fixed -> SetVisAttributes(blackLead);

        // Coll_Fixed_Fe

        G4double Rmin1Coll_fixed_Fe = 15.*mm;
        G4double Rmax1Coll_fixed_Fe = 50.*mm;
        G4double Rmin2Coll_fixed_Fe = 15.*mm;
        G4double Rmax2Coll_fixed_Fe = 40.*mm;
        G4double hightColl_fixed_Fe = 4.625*cm;


        G4ThreeVector positionColl_fixed_Fe = G4ThreeVector(0,0,-27.325*cm);

        G4Cons* solidColl_fixed_Fe = new G4Cons("solidColl_fixed_Fe",
        			       Rmin1Coll_fixed_Fe,  
        			       Rmax1Coll_fixed_Fe,  
        			       Rmin2Coll_fixed_Fe,  
        			       Rmax2Coll_fixed_Fe,  
        			       hightColl_fixed_Fe,  
        			       startAngleOfTheTube,    // 
        			       spanningAngleOfTheTube);
        logicColl_fixed_Fe = new G4LogicalVolume(solidColl_fixed_Fe,Fe,"logicColl_fixed_Fe",0,0,0);
        logicColl_fixed_Fe -> SetVisAttributes(grayFe);

        // Helmet

        G4double Rmax1Coll_helmet = 15.*mm;
        G4double Rmax2Coll_helmet = 15.*mm;
        G4double hightColl_helmet = 3.0*cm;

        // set function influence ON!
        G4Cons* solidColl_helmet = new G4Cons("solidColl_helmet",
                        4.15*mm,  // will be set later
                        Rmax1Coll_helmet, 
                        5.3*mm,  // will be set later
        			    Rmax2Coll_helmet,  
        			    hightColl_helmet, 
        			    startAngleOfTheTube,   
                        spanningAngleOfTheTube);

        // UpdateHelmet();
        logicColl_helmet = new G4LogicalVolume(solidColl_helmet,tungsten,"logicColl_helmet",0,0,0);
        logicColl_helmet -> SetVisAttributes(colorTungsten);

        // Helmet_Fe

        G4double Rmin1Coll_helmet_Fe = 15.*mm;
        G4double Rmax1Coll_helmet_Fe = 40.*mm;
        G4double Rmin2Coll_helmet_Fe = 15.*mm;
        G4double Rmax2Coll_helmet_Fe = 30.*mm;
        G4double hightColl_helmet_Fe = 3.0*cm;

        G4Cons* solidColl_helmet_Fe = new G4Cons("solidColl_helmet_Fe",
        			       Rmin1Coll_helmet_Fe, 
        			       Rmax1Coll_helmet_Fe,  
        			       Rmin2Coll_helmet_Fe,  
        			       Rmax2Coll_helmet_Fe,  
        			       hightColl_helmet_Fe, 
        			       startAngleOfTheTube,   
        			       spanningAngleOfTheTube);
        logicColl_helmet_Fe = new G4LogicalVolume(solidColl_helmet_Fe,Fe,"logicColl_helmet_Fe",0,0,0);
        logicColl_helmet_Fe -> SetVisAttributes(grayFe);

    };

    // add G4PVPlacement
    G4VPhysicalVolume* tube = new G4PVPlacement(rot, pos, logicTube, "tube", mv, true, number, true);   
    G4VPhysicalVolume* physicTube_source = new G4PVPlacement(rot, pos, logicTube_source, "Tube_source", mv, true, number, true);
    G4VPhysicalVolume* physicTube_post = new G4PVPlacement(rot, pos - G4ThreeVector(0., 0., 1.1*cm), 
    logicTube_post, "Tube_post", mv, true, number, true);

    G4VPhysicalVolume* physicTube_coll = new G4PVPlacement(0, pos + G4ThreeVector(0., 0., 4.9*cm),
    logicTube_coll, "Tube_coll", mv, true, number, true);

    G4VPhysicalVolume* physicTube_Al = new G4PVPlacement(0, pos + G4ThreeVector(0., 0., 1.*cm),
    logicTube_Al, "Tube_Al", mv, true, number, true);

    G4VPhysicalVolume* physicTube_Fe = new G4PVPlacement(0, pos + G4ThreeVector(0., 0., 1.*cm), 
    logicTube_Fe, "Tube_Fe", mv, true, number, true);

    G4PVPlacement* physiTube_coll_Fe = new G4PVPlacement(rot, pos + G4ThreeVector(0., 0., 4.9*cm),
    logicTube_coll_Fe, "Tube_coll_Fe", mv, true, number, true);

    G4PVPlacement* physiColl_fixed = new G4PVPlacement(rot , pos + G4ThreeVector(0., 0., 12.755*cm), 
    logicColl_fixed, "Coll_fixed", mv, true, number, true);

    G4PVPlacement* physiColl_fixed_Fe = new G4PVPlacement(rot, pos + G4ThreeVector(0., 0., 12.755*cm), 
    logicColl_fixed_Fe, "Coll_fixed_Fe", mv, true, number, true);

    G4VPhysicalVolume* physicColl_helmet = new G4PVPlacement(rot, pos + G4ThreeVector(0., 0., 20.6*cm),
    logicColl_helmet, "Coll_helmet", mv, true, number, true);

    G4PVPlacement* physiColl_helmet_Fe = new G4PVPlacement(rot, pos + G4ThreeVector(0., 0., 20.6*cm), 
    logicColl_helmet_Fe, "Coll_helmet_Fe", mv, true, number, true);

    number += 1;
}


// Why doesn't work?..
void MyDetectorConstruction::UpdateHelmet()
{
    if (solidColl_helmet)
    {
        switch( helmetSize )
        {
        case 18:
            solidColl_helmet->SetInnerRadiusMinusZ( 4.15 * mm );
            solidColl_helmet->SetInnerRadiusPlusZ( 5.3  * mm );
            break;

        case 14:
            solidColl_helmet->SetInnerRadiusMinusZ( 3.15 * mm );
            solidColl_helmet->SetInnerRadiusPlusZ( 4.25  * mm );
            break;

        case 8:
            solidColl_helmet->SetInnerRadiusMinusZ( 1.9 * mm );
            solidColl_helmet->SetInnerRadiusPlusZ( 2.5  * mm );
            break;

        case 4:
            solidColl_helmet->SetInnerRadiusMinusZ( 1. * mm );
            solidColl_helmet->SetInnerRadiusPlusZ( 1.25  * mm );
            break;
        }
    }
}


void MyDetectorConstruction::SetHelmetSize(G4int size)
{
    if (size != helmetSize) // Only if the size changes
    {
        // Allow only valid numbers
        switch( size )
        {
        case 18:
        case 14:
        case 8:
        case 4:
            helmetSize = size;
            G4cout << "Helmet size set to " << helmetSize << std::endl;
            UpdateHelmet();
            break;
        default:
      G4Exception("GammaKnifeDetectorConstruction::SetHelmetSize()", 
		  "GammaKnife001", FatalException, 
		  "Error: Invalid helmet size.");
            return;
        }
    }
}
