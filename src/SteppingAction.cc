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
        mutracknumber = theTrack->GetTrackID();
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
    
    

    //this is at the end of this function in Tristan's code - does order matter for this?
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

          // Start of event - reset all the flags
          if (theTrack->GetCurrentStepNumber() == 1)
          {
              mutracknumber = -1;
            //   WC3_1eflag = 0;
            //   WC3_2eflag = 0;
            //   WC3_3eflag = 0;
            //   WC3_1primposflag = 0;
            //   WC3_2primposflag = 0;
            //   WC3_3primposflag = 0;
            //   WC3_1secposflag = 0;
            //   WC3_2secposflag = 0;
            //   WC3_3secposflag = 0;
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

    /// Additions based on Tristan's work /////

    // bremsstrahlung with positrons
    if (theParticleName == "e+" && theProcessName == "eBrem" && thePostVolume != "NaI" && thePreVolume != "NaI")
    {
        // Check if primary positron
        if (theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber)
        {
            // This excludes CsI
            if (thePostVolume.find("Crystal") == std::string::npos && thePreVolume.find("Crystal") == std::string::npos) 
            {
                runAction->SPosBrem(preTime, postTime, prePosition, postPosition, preMomentum, postMomentum, preEnergy, postEnergy);
            }
        }
    }

    // bhabha with positrons
    if (theParticleName == "e+" && theProcessName == "eIoni" && thePostVolume != "NaI" && thePreVolume != "NaI")
    {
        // Check if primary positron
        if (theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber)
        {
            // This excludes CsI
            if (thePostVolume.find("Crystal") == std::string::npos && thePreVolume.find("Crystal") == std::string::npos) 
            {
                runAction->SPosBhabha(preTime, postTime, prePosition, postPosition, preMomentum, postMomentum, preEnergy, postEnergy);
            }
        }
    }

    //checking for annihililation positrons
    //why not in the NaI?
    if (theParticleName == "e+" && theProcessName == "annihil" && thePostVolume != "NaI" && thePreVolume != "NaI") 
    {
        // Check if primary positron
        if (theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber)
        {
            // This excludes CsI, here checking that Crystal is not in the pre/post vol name
            if (thePostVolume.find("Crystal") == std::string::npos && thePreVolume.find("Crystal") == std::string::npos) 
            {
                runAction->SPosAnnihil(preTime, postTime, prePosition, postPosition, preMomentum, postMomentum, preEnergy, postEnergy);
            }
        }
    }

    // bremsstrahlung with electrons
    if (theParticleName == "e-" && theProcessName == "eBrem" && thePostVolume != "NaI" && thePreVolume != "NaI")
    {
        // This excludes CsI
        if (thePostVolume.find("Crystal") == std::string::npos && thePreVolume.find("Crystal") == std::string::npos) 
        {
            runAction->SElecBrem(preTime, postTime, prePosition, postPosition, preMomentum, postMomentum, preEnergy, postEnergy);
        }
    }

    // scattering with positrons
    if (theParticleName == "e+" && PointingIntoBina(prePosition, preMomentum) && !PointingIntoBina(postPosition, postMomentum))
    {
        // Check if primary positron
        if (theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber)
        {
            runAction->SPosScatter(preTime, postTime, prePosition, postPosition, preMomentum, postMomentum, preEnergy, postEnergy);
        }
    }

    if (theParticleName == "e-" && PointingIntoBina(prePosition, preMomentum) && !PointingIntoBina(postPosition, postMomentum))
    {
        runAction->SElecScatter(preTime, postTime, prePosition, postPosition, preMomentum, postMomentum, preEnergy, postEnergy);
    }


    /*
    // Seems to tag e/e- in three regions along z of WC3, I don't think I need these for now
    // for electrons
    if (theParticleName == "e-" && insideWC3_1(prePosition)) WC3_1eflag = 1;
    if (theParticleName == "e-" && insideWC3_2(prePosition)) WC3_2eflag = 1;
    if (theParticleName == "e-" && insideWC3_3(prePosition)) WC3_3eflag = 1;

    // for positrons that are the primary particle (or primary after the muon)
    if (theParticleName == "e+" && (theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber) && insideWC3_1(prePosition)) WC3_1primposflag = 1;
    if (theParticleName == "e+" && (theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber) && insideWC3_2(prePosition)) WC3_2primposflag = 1;
    if (theParticleName == "e+" && (theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber) && insideWC3_3(prePosition)) WC3_3primposflag = 1;
    // and for positrons that are not the primary particle (or are still the primary after the muon)
    if (theParticleName == "e+" && !(theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber) && insideWC3_1(prePosition)) WC3_1secposflag = 1;
    if (theParticleName == "e+" && !(theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber) && insideWC3_2(prePosition)) WC3_2secposflag = 1;
    if (theParticleName == "e+" && !(theTrack->GetParentID() == 1 || theTrack->GetParentID() == mutracknumber) && insideWC3_3(prePosition)) WC3_3secposflag = 1;

    if (WC3_1eflag && WC3_2eflag && WC3_3eflag) 
    {
        runAction->SeinWC3(prePosition, preTime, preMomentum, preEnergy);
        WC3_1eflag = 0;
        WC3_2eflag = 0;
        WC3_3eflag = 0;
    }

    if (WC3_1primposflag && WC3_2primposflag && WC3_3primposflag) 
    {
        runAction->SprimposinWC3(prePosition, preTime, preMomentum, preEnergy);
        WC3_1primposflag = 0;
        WC3_2primposflag = 0;
        WC3_3primposflag = 0;
    }

    if (WC3_1secposflag && WC3_2secposflag && WC3_3secposflag) 
    {
        runAction->SsecposinWC3(prePosition, preTime, preMomentum, preEnergy);
        WC3_1secposflag = 0;
        WC3_2secposflag = 0;
        WC3_3secposflag = 0;
    }
    */


    //Emma's addition - Feb 2025
    // For tagging processes: Scattering, bremsstrahlung, annihilation
    /*
    if (theParticleName == "e+"){ //do I want this just for positrons?
      // just try printing out all the processes
      // G4cout << "A positron is doing " << theProcessName << G4endl;
      if (theProcessName == "annihil") { //annihilation process
        // AnnihilSeen
        runAction->AnnihilSeen();
      } 
      if (theProcessName == "eBrem") { //bremsstrahlung process
          // G4cout << "A positron is doing " << theProcessName << G4endl;
          runAction->BremsSeen();
        } 
      if (theProcessName == "msc") { //Scattering process
          // G4cout << "A positron is doing " << theProcessName << G4endl;
          runAction->MSCSeen();
        }
    } 
    */
    
}


bool SteppingAction::PointingIntoBina(G4ThreeVector Position, G4ThreeVector Momentum)
{
    //should double check these values
    // Z of front face in mm
    double binaz = 84.4;
    // Radius of BINA in mm 
    double binar = 240;
    
    double x = Position.x();
    double y = Position.y();
    double z = Position.z();

    double px = Momentum.x();
    double py = Momentum.y();
    double pz = Momentum.z();

    //unit vectors
    px /= pz;
    py /= pz;
    pz /= pz;

    //if particle z position is downstream of the front face of BINA -> false
    if (z > binaz) return false;
    //if particle has negative momentum -> false
    else if (pz < 0) return false;
    else
    {
        //I think this checks if the particle is on a colison course with BINA based on it's current momentum
        double zdiff = binaz - z;
        double xend = x + px * zdiff;
        double yend = y + py * zdiff;

        if (sqrt(xend * xend + yend * yend) < binar) 
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
}

/*
bool SteppingAction::insideWC3_1(G4ThreeVector Position)
{
    //G4cout << sqrt(Position.x() * Position.x() + Position.y() * Position.y()) << G4endl;
    //G4cout << Position.z() << G4endl;
    if (sqrt(Position.x() * Position.x() + Position.y() * Position.y()) < 124 && Position.z() > 50.5 && Position.z() < 54.6) return true;
    else return false;
}

bool SteppingAction::insideWC3_2(G4ThreeVector Position)
{
    if (sqrt(Position.x() * Position.x() + Position.y() * Position.y()) < 124 && Position.z() > 54.6 && Position.z() < 58.8) return true;
    else return false;
}

bool SteppingAction::insideWC3_3(G4ThreeVector Position)
{
    if (sqrt(Position.x() * Position.x() + Position.y() * Position.y()) < 124 && Position.z() > 58.8 && Position.z() < 62.9) return true;
    else return false;
}
*/
