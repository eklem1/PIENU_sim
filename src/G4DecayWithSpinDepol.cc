//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History:
//      17 August 2004  P. Gumplinger, T. MacPhail
//      11 April  2008  Kamil Sedlak (PSI), Toni Shiroka (PSI)
// ------------------------------------------------------------
//
#include "G4DecayWithSpinDepol.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4DecayTable.hh"
#include "G4MuonDecayChannelWithSpin.hh"

#include "G4Vector3D.hh"

#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4FieldManager.hh"
#include "G4Field.hh"

#include "G4Transform3D.hh"


#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


G4DecayWithSpinDepol::G4DecayWithSpinDepol(const G4String& processName):G4Decay(processName)
{
  // set Process Sub Type   
  SetProcessSubType(static_cast<int>(DECAY_WithSpin));

}

G4DecayWithSpinDepol::~G4DecayWithSpinDepol(){}

G4VParticleChange* G4DecayWithSpinDepol::DecayIt(const G4Track& aTrack, const G4Step& aStep)
{

// get particle
  const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
  G4ParticleDefinition* aParticleDef = aParticle->GetDefinition();

  //G4cout << aParticleDef->GetParticleName() << G4endl;// --> "mu+"

  // get parent_polarization
  G4ThreeVector parent_polarization = aParticle->GetPolarization();
  //G4cout << parent_polarization << G4endl;

  if(parent_polarization == G4ThreeVector(0,0,0))
  {
    // Generate random polarization direction

    G4double cost = 1. - 2.*G4UniformRand();
    G4double sint = std::sqrt((1.-cost)*(1.+cost));

    G4double phi = twopi*G4UniformRand();
    G4double sinp = std::sin(phi);
    G4double cosp = std::cos(phi);

    G4double px = sint*cosp;
    G4double py = sint*sinp;
    G4double pz = cost;

    parent_polarization.setX(px);
    parent_polarization.setY(py);
    parent_polarization.setZ(pz);

  }else{


    //SPIN DEPOLARIZATION IN SCINTILLATORS:
    G4double depol_tau = 1000.0;
    G4double t_flip = -1000.0;
    G4double t_left = fRemainderLifeTime;
    G4Material *material = aTrack.GetMaterial();
    
    if (material->GetName()=="Scintillator" && aParticleDef->GetParticleName()=="mu+"){
      t_flip = -2.0*depol_tau*log(G4UniformRand());
      while (t_flip<t_left){
	parent_polarization *= -1;
	t_left -= t_flip;
	t_flip = -2.0*depol_tau*log(G4UniformRand());
      }
    }
    
    //SPIN PRECESSION IN A MAGNETIC FIELD
    G4FieldManager* fieldMgr = aStep.GetTrack()->GetVolume()->
                                     GetLogicalVolume()->GetFieldManager();

    if (!fieldMgr) {
       G4TransportationManager *transportMgr =
                         G4TransportationManager::GetTransportationManager();
       G4PropagatorInField* fFieldPropagator = 
                                        transportMgr->GetPropagatorInField();
       if (fFieldPropagator) fieldMgr = 
                                  fFieldPropagator->GetCurrentFieldManager();
    }

    const G4Field* field = NULL;
    if(fieldMgr)field = fieldMgr->GetDetectorField();

    if (field) {

       G4double point[4];
       point[0] = (aStep.GetPostStepPoint()->GetPosition())[0];
       point[1] = (aStep.GetPostStepPoint()->GetPosition())[1];
       point[2] = (aStep.GetPostStepPoint()->GetPosition())[2];
       point[3] = aTrack.GetGlobalTime();

       G4double fieldValue[6];
       field -> GetFieldValue(point,fieldValue);

       G4ThreeVector B(fieldValue[0],fieldValue[1],fieldValue[2]);

       // Call the spin precession only for non-zero mag. field
       if (B.mag2() > 0.) parent_polarization = 
                                 Spin_Precession(aStep,B,fRemainderLifeTime);

    }
  }

// decay table
  G4DecayTable *decaytable = aParticleDef->GetDecayTable();

  if (decaytable) {
     G4MuonDecayChannelWithSpin *decaychannel;
     decaychannel = (G4MuonDecayChannelWithSpin*)decaytable->SelectADecayChannel();
     if (decaychannel) decaychannel->SetPolarization(parent_polarization);
  }

  G4ParticleChangeForDecay* pParticleChangeForDecay;

  pParticleChangeForDecay = (G4ParticleChangeForDecay*)G4Decay::DecayIt(aTrack,aStep);

  pParticleChangeForDecay->ProposePolarization(parent_polarization);

  return pParticleChangeForDecay;

}


G4ThreeVector G4DecayWithSpinDepol::GetRandomSpin()
{
  G4double cost = 1. - 2.*G4UniformRand();
  G4double sint = std::sqrt((1.-cost)*(1.+cost));
  
  G4double phi = twopi*G4UniformRand();
  G4double sinp = std::sin(phi);
  G4double cosp = std::cos(phi);
  
  G4double px = sint*cosp;
  G4double py = sint*sinp;
  G4double pz = cost;
  
  G4ThreeVector Spin(px,py,pz);

  return Spin;
}

G4ThreeVector G4DecayWithSpinDepol::Spin_Precession( const G4Step& aStep,
                                        G4ThreeVector B, G4double deltatime )
{
  G4double Bnorm = std::sqrt(sqr(B[0])  + sqr(B[1]) +sqr(B[2]) );

  G4double q = aStep.GetTrack()->GetDefinition()->GetPDGCharge();
  G4double a = 1.165922e-3;
  G4double s_omega = 8.5062e+7*rad/(s*kilogauss);

  G4double omega = -(q*s_omega)*(1.+a) * Bnorm;

  G4double rotationangle = deltatime * omega;

  G4Transform3D SpinRotation = G4Rotate3D(rotationangle,B.unit());

  G4Vector3D Spin = aStep.GetTrack() -> GetPolarization();

  G4Vector3D newSpin = SpinRotation * Spin;

#ifdef G4VERBOSE
  if (GetVerboseLevel()>2) {
    G4double normspin = std::sqrt(Spin*Spin);
    G4double normnewspin = std::sqrt(newSpin*newSpin);
    //G4double cosalpha = Spin*newSpin/normspin/normnewspin;
    //G4double alpha = std::acos(cosalpha);

    G4cout << "AT REST::: PARAMETERS " << G4endl;
    G4cout << "Initial spin  : " << Spin  << G4endl;
    G4cout << "Delta time    : " << deltatime  << G4endl;
    G4cout << "Rotation angle: " << rotationangle/rad  << G4endl;
    G4cout << "New spin      : " << newSpin  << G4endl;
    G4cout << "Checked norms : " << normspin <<" " << normnewspin << G4endl;
  }
#endif

  return newSpin;

}
