#include "RunAction.hh"
#include "SteppingAction.hh"

#include "G4VProcess.hh"
#include "G4StepStatus.hh"



SteppingAction::SteppingAction(RunAction* run) {

    runAction = run;
    savedT1Once = false;
    savedT3Once = false;
    
}

SteppingAction::~SteppingAction() { }

// This function is called each time Geant takes a step
//
void SteppingAction::UserSteppingAction(const G4Step* theStep) {

    G4String thePreVolume          = " ";
    G4String thePostVolume         = " ";
    G4String theProcessName        = " ";
    G4String theParticleName       = " ";
    G4String theCreatorProcessName = " ";

    G4ThreeVector preMomentum(0,0,0);
    G4ThreeVector prePosition(0,0,0);

    G4double preTime = 0;
    G4double preEnergy = 0;

    G4ThreeVector postMomentum(0,0,0);
    G4ThreeVector postPosition(0,0,0);

    G4double postTime = 0;
    G4double postEnergy = 0;

    G4StepPoint* pPrePoint = theStep->GetPreStepPoint();
    if (pPrePoint) {
        G4VPhysicalVolume* prePhysVol = pPrePoint->GetPhysicalVolume();
        if (prePhysVol) thePreVolume = prePhysVol->GetName();
        prePosition = pPrePoint->GetPosition();
        preMomentum = pPrePoint->GetMomentum();
	preTime     = pPrePoint->GetGlobalTime();
	preEnergy   = pPrePoint->GetKineticEnergy();
    } else { return; }

    G4StepPoint* pPostPoint = theStep->GetPostStepPoint();
    if (pPostPoint) {
        G4VPhysicalVolume* postPhysVol = pPostPoint->GetPhysicalVolume();
        if (postPhysVol) thePostVolume = postPhysVol->GetName();
        postPosition = pPostPoint->GetPosition();
        postMomentum = pPostPoint->GetMomentum();
	postTime     = pPostPoint->GetGlobalTime();
	postEnergy   = pPostPoint->GetKineticEnergy();
        const G4VProcess* pProcess = pPostPoint->GetProcessDefinedStep();
        if (pProcess) theProcessName = pProcess->GetProcessName();
    } else { return; }

    G4Track* theTrack = theStep->GetTrack();
    if (theTrack) {
      theParticleName = theTrack->GetDefinition()->GetParticleName();
      const G4VProcess* theCreatorProcess = theTrack->GetCreatorProcess();
      if (theCreatorProcess) theCreatorProcessName = theCreatorProcess->GetProcessName();

    } else { return; }
    
    //Record the position of a photon-capture with neutron emission
    if (theParticleName == "neutron" && 
	theCreatorProcessName == "PhotonInelastic" &&
	theTrack->GetCurrentStepNumber() == 1 &&
	thePostVolume == "NaI" && thePreVolume == "NaI"
	){
      runAction->SPhotonuclear(postPosition);
    } 

    if (theParticleName == "pi+") {
      if (theTrack->GetCurrentStepNumber() == 1)
	runAction->SPionStart(prePosition,preTime,preMomentum,preEnergy);
      if (theProcessName == "Decay" | theProcessName == "DecayWithSpin")
	runAction->SPionDecay(postPosition,postTime,postMomentum,postEnergy);
    }
    if (theParticleName == "mu+") {
      if (theTrack->GetCurrentStepNumber() == 1)
	runAction->SMuonStart(prePosition,preTime,preMomentum,preEnergy);
      if (theProcessName == "Decay" | theProcessName == "DecayWithSpin"){
	runAction->SMuonDecay(postPosition,postTime,postMomentum,postEnergy);
    	runAction->SMuPolarization(theTrack->GetPolarization()); //Save the polarization of the muon
      }
    }

    //Chloe's addition
    if (theParticleName == "e+" 
	&& thePostVolume == "/pienu/Target" 
	&& theProcessName == "eIoni") {
      runAction->SBhabha(postEnergy);
      // G4cout << postEnergy/MeV << G4endl;
    }
    
    


    if (theParticleName == "e+" ) {
      if (theCreatorProcessName=="DecayWithSpin" |
	  theCreatorProcessName=="Decay" ) {
	if (theTrack->GetCurrentStepNumber() == 1){
	  runAction-> SPositronStart(prePosition,postTime,preMomentum,preEnergy);
	  runAction->SEmomentum(preMomentum);//Save the decay positron polarization
	}
	if (theTrack->GetTrackStatus() == fStopAndKill) 
	  runAction->SPositronStop(postPosition,postTime,postMomentum,postEnergy);
      }
    }
    
    

    // If the particle is a pion...
    if (theParticleName == "pi+") {
        // If the particle is the primary pion...
        if (theTrack->GetParentID() == 0) {
	  // If the pi+ is stopped...
          if (theTrack->GetTrackStatus() == fStopAndKill) {
             savedT1Once = false;
             savedT3Once = false;
          }
        }
    }
    // If the particle is a positron...
    if (theParticleName == "e+") {
        // If the positron is in T3 
        if (thePreVolume == "T2") {
            if (savedT3Once == false) {
               runAction->SaveT3(prePosition, preMomentum);
               savedT3Once = true;
            }
        }
        // If the positron is in T1...
        if (thePreVolume == "T1") {
            if (savedT1Once == false) {
               runAction->SaveT1(prePosition, preMomentum);
               savedT1Once = true;
            }
        }
    }
}
