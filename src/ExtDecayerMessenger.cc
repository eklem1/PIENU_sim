

#include "ExtDecayerMessenger.hh"
#include "ExtDecayer.hh"


#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"

ExtDecayerMessenger::ExtDecayerMessenger(ExtDecayer* ext) : fExtDecayer(ext)
{

  fDataradiative = new G4UIcmdWithADouble("/radiative/mu_or_e", this);
  G4cout<< "fDataradiative was accessed in extmessenger"<<G4endl;
  fDataradiative->SetGuidance("Set the radiative mode to be used");
  fDataradiative->SetDefaultValue(0.);
  fDataradiative->AvailableForStates(G4State_PreInit,G4State_Idle);

} 

ExtDecayerMessenger::~ExtDecayerMessenger()
{
}

void ExtDecayerMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

  if (command == fDataradiative){
	  G4cout<< "fDataradiative (command) was accessed in extmessenger"<<G4endl;
	G4cout<< " and the value is="<< newValue << G4endl;
    fExtDecayer->dataradiative = fDataradiative->GetNewDoubleValue(newValue);
  }

}
