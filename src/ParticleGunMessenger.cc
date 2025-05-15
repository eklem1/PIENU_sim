
//Author: L.Doria 2011

#include "ParticleGunMessenger.hh"
#include "PrimaryGeneratorAction.hh"


#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

ParticleGunMessenger::ParticleGunMessenger(PrimaryGeneratorAction* pga) : fPrimaryGeneratorAction(pga)
{
  fPileUpDirectory = new G4UIdirectory("/pileup/");
  fPileUpDirectory->SetGuidance("Controls for the PileUp simulation");

  fBeamDirectory = new G4UIdirectory("/beam/");
  fBeamDirectory->SetGuidance("Options for the beam generation");

  fActive = new G4UIcmdWithABool("/pileup/Active", this);
  fActive->SetGuidance("If 1, activates the simulation of the pileup");
  fActive->SetDefaultValue(0);
  fActive->AvailableForStates(G4State_PreInit,G4State_Idle);

  fRate = new G4UIcmdWithADouble("/pileup/Rate", this);
  fRate->SetGuidance("Sets the beam rate in 1/seconds, default is 80000");
  fRate->SetDefaultValue(80000);
  fRate->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTmin = new G4UIcmdWithADouble("/pileup/Tmin", this);
  fTmin->SetGuidance("Sets the lower bound of the time window in ns, default is -8000ns");
  fTmin->SetDefaultValue(-8000.0);
  fTmin->AvailableForStates(G4State_PreInit,G4State_Idle);

  fTmax = new G4UIcmdWithADouble("/pileup/Tmax", this);
  fTmax->SetGuidance("Sets the upper bound of the time window in ns, default is +500ns");
  fTmax->SetDefaultValue(500.0);
  fTmax->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDatabeam = new G4UIcmdWithADouble("/beam/FromData", this);
  G4cout<< "fDatabeam was accessed in gunmessenger"<<G4endl;
  fDatabeam->SetGuidance("Set the BVF to be used");
  fDatabeam->SetDefaultValue(0.);
  fDatabeam->AvailableForStates(G4State_PreInit,G4State_Idle);

  fEnergy = new G4UIcmdWithADouble("/gun/energy", this);
  fEnergy->SetGuidance("Set energy of primary particles.");
  fEnergy->SetDefaultValue(0.);
  fEnergy->AvailableForStates(G4State_PreInit, G4State_Idle);

  fAngle = new G4UIcmdWithADouble("/gun/angle", this);
  fAngle->SetGuidance("Set energy of primary particles.");
  fAngle->SetDefaultValue(0.);
  fAngle->AvailableForStates(G4State_PreInit, G4State_Idle);

} 

ParticleGunMessenger::~ParticleGunMessenger()
{
  delete fActive;
  //delete fEnergy;
}

void ParticleGunMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == fActive){
    fPrimaryGeneratorAction->pileup = fActive->GetNewBoolValue(newValue);
  }

  if (command == fRate){
    fPrimaryGeneratorAction->rate = fRate->GetNewDoubleValue(newValue);
  }

  if (command == fTmin){
    fPrimaryGeneratorAction->Tmin = fTmin->GetNewDoubleValue(newValue);
  }

  if (command == fTmax){
    fPrimaryGeneratorAction->Tmax = fTmax->GetNewDoubleValue(newValue);
  }

  if (command == fDatabeam){
	  G4cout<< "fDatabeam (command) was accessed in gunmessenger"<<G4endl;
	G4cout<< " and the value is="<< newValue << G4endl;
	//G4cout<< " DataBeam Before  = " << fPrimaryGeneratorAction->databeam << G4endl;
    fPrimaryGeneratorAction->databeam = fDatabeam->GetNewDoubleValue(newValue);
    //G4cout<< " DataBeam After  = " << fPrimaryGeneratorAction->databeam << G4endl;
  }

  if (command == fEnergy){
    //G4cout<< " GunEnergy Before  = " << fPrimaryGeneratorAction->eEnergy << G4endl;
    fPrimaryGeneratorAction->eEnergy = fEnergy->GetNewDoubleValue(newValue);
    //G4cout<< " GunEnergy After  = " << fPrimaryGeneratorAction->eEnergy << G4endl;
  }

  if (command == fAngle){
    //G4cout<< " GunAngle Before  = " << fPrimaryGeneratorAction->eAngle << G4endl;
    fPrimaryGeneratorAction->eAngle = fAngle->GetNewDoubleValue(newValue);
    //G4cout<< " GunAngle After  = " << fPrimaryGeneratorAction->eAngle << G4endl;
  }

}
