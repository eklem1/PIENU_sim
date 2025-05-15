#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "globals.hh"

#include <fstream>
#include <iostream>

#include "G4VUserPrimaryGeneratorAction.hh"

#include <TH1.h>
#include <TF1.h>


class G4ParticleGun;
class G4Event;
class ParticleGunMessenger;


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

public:

  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();
  
  void GeneratePrimaries(G4Event* anEvent);
  void FillBeamHistograms();
  void DoCorrelatedVariables();
  
  G4bool   pileup;
  G4double rate;
  G4double Tmin,Tmax;

  G4double databeam;
  G4double r[5];
  G4double L[5][5];
  G4double P0;
  G4double eEnergy;
  G4double eAngle;

  
  G4double tx_mean,ty_mean,tx_sigma,ty_sigma;

  

  private:



  G4ParticleGun* particleGunPi;
  G4ParticleGun* particleGunMu;
  G4ParticleGun* particleGunE;

  G4double init;

  TH1F *hE, *hx, *hy, *htx, *hty;
  // TRandom Generator;  
  ParticleGunMessenger* fMessenger;

  std::ifstream* infile1;
  std::ifstream* infile2;
};

#endif
