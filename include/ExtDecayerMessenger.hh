
#ifndef ExtDecayerMessenger_h
#define ExtDecayerMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class ExtDecayer;

class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;

class ExtDecayerMessenger : public G4UImessenger
{

public:
  ExtDecayerMessenger(ExtDecayer*);
  virtual ~ExtDecayerMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  ExtDecayer* fExtDecayer;

  G4UIcmdWithADouble* fDataradiative;

};
#endif
