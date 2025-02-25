
#include "PhysicsListMessenger.hh"
#include "PhysicsList.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "G4PhaseSpaceDecayChannel.hh"
#include "G4PionRadiativeDecayChannelMine.hh"

#include "G4ParticleTable.hh"

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
    : fPhysicsList(pPhys)
{
    fDirectory = new G4UIdirectory("/exp/phys/");
    fDirectory->SetGuidance("Control the physics lists");

    fGammaCutCMD = new G4UIcmdWithADoubleAndUnit("/exp/phys/gammaCut",this);
    fGammaCutCMD->SetGuidance("Set gamma cut.");
    fGammaCutCMD->SetParameterName("Gcut",false);
    fGammaCutCMD->SetUnitCategory("Length");
    fGammaCutCMD->SetRange("Gcut>0.0");
    fGammaCutCMD->AvailableForStates(G4State_PreInit,G4State_Idle);

    fElectCutCMD = new G4UIcmdWithADoubleAndUnit("/exp/phys/electronCut",
                                                 this);
    fElectCutCMD->SetGuidance("Set electron cut.");
    fElectCutCMD->SetParameterName("Ecut",false);
    fElectCutCMD->SetUnitCategory("Length");
    fElectCutCMD->SetRange("Ecut>0.0");
    fElectCutCMD->AvailableForStates(G4State_PreInit,G4State_Idle);

    fPosCutCMD = new G4UIcmdWithADoubleAndUnit("/exp/phys/positronCut",
                                               this);
    fPosCutCMD->SetGuidance("Set positron cut.");
    fPosCutCMD->SetParameterName("Pcut",false);
    fPosCutCMD->SetUnitCategory("Length");
    fPosCutCMD->SetRange("Pcut>0.0");
    fPosCutCMD->AvailableForStates(G4State_PreInit,G4State_Idle);

    fAllCutCMD = new G4UIcmdWithADoubleAndUnit("/exp/phys/allCuts",this);
    fAllCutCMD->SetGuidance("Set cut for all.");
    fAllCutCMD->SetParameterName("cut",false);
    fAllCutCMD->SetUnitCategory("Length");
    fAllCutCMD->SetRange("cut>0.0");
    fAllCutCMD->AvailableForStates(G4State_PreInit,G4State_Idle);

    fDecayDirectory = new G4UIdirectory("/decay/");
    fDecayDirectory->SetGuidance("Decay chain control commands.");

    fDecayExtCMD = new G4UIcmdWithoutParameter("/decay/external", this);
    fDecayExtCMD->SetGuidance("Pass all the pions to the external decay generator");
    fDecayExtCMD->AvailableForStates(G4State_PreInit,G4State_Idle);
 
}

PhysicsListMessenger::~PhysicsListMessenger()
{
    delete fGammaCutCMD;
    delete fElectCutCMD;
    delete fPosCutCMD;
    delete fAllCutCMD;
//    delete fPhysicsListCMD;
//    delete fListCMD;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
  if(command == fDecayExtCMD) {
    particleTable = G4ParticleTable::GetParticleTable();
    particleDef = particleTable->FindParticle("pi+");
    particleDef->SetDecayTable(NULL); // Use ExtDecayer
    
    
    auto pmanager = particleDef->GetProcessManager();
    auto extDecayer = new ExtDecayer();
    auto processVector = pmanager->GetProcessList();
    
    for (G4int i=0; i<processVector->length(); i++) {
      G4Decay* decay = dynamic_cast<G4Decay*>((*processVector)[i]);
      if ( decay ) {
	G4cout << "Setting ext decayer for: " <<  particleDef->GetParticleName() << G4endl;
	decay->SetExtDecayer(extDecayer);
	decay->DumpInfo();
      }
    }
    
  }

    if (command == fGammaCutCMD) {
        fPhysicsList->SetCutForGamma(fGammaCutCMD
                                     ->GetNewDoubleValue(newValue));
    }
    else if (command == fElectCutCMD) {
        fPhysicsList->SetCutForElectron(fElectCutCMD
                                     ->GetNewDoubleValue(newValue));
    }
    else if (command == fPosCutCMD) {
        fPhysicsList->SetCutForPositron(fPosCutCMD
                                     ->GetNewDoubleValue(newValue));
    }
    else if (command == fAllCutCMD) {
        G4double cut = fAllCutCMD->GetNewDoubleValue(newValue);
        fPhysicsList->SetCutForGamma(cut);
        fPhysicsList->SetCutForElectron(cut);
        fPhysicsList->SetCutForPositron(cut);
    }
}
