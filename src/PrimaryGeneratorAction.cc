#include "PrimaryGeneratorAction.hh"
#include "ParticleGunMessenger.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4PionPlus.hh"
#include "G4MuonPlus.hh"
#include "G4Positron.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "TRandom3.h"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "TMath.h"

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
// Note: revisions implemented for 2019 BVF format
//
// Primary events are generated from a tab delineated file of the
// following format:
// Momentum magnitude (GeV/c)
// X position (cm)
// theta (mr)
// Y position (cm)
// phi (mr)
//
// The values are given for particles at the location of the target.
// For this simulation, they are propagated backward 60cm.

// Default constructor
//
PrimaryGeneratorAction::PrimaryGeneratorAction()

{
  init=0;
  
  //Beam Histograms
  hE  = new TH1F("hE","",1000,0.06,0.08);
  hx  = new TH1F("hx","",200,-3,3);
  hy  = new TH1F("hy","",200,-3,3);
  htx = new TH1F("htx","",200,-0.2,0.2);
  hty = new TH1F("hty","",200,-0.2,0.2);
  
  //Messenger for the Particle Gun

  fMessenger = new ParticleGunMessenger(this);
  G4cout<< "Messenger is initiated*************"<< G4endl;

  
  // Create a particle gun to shoot 1 particle per event
  G4int n_particle = 1;
  particleGunPi = new G4ParticleGun(n_particle);
  particleGunMu = new G4ParticleGun(n_particle);
  particleGunE = new G4ParticleGun(n_particle);

   // Find the definition of the particles
  G4String particleName1 = "pi+";
  G4String particleName2 = "mu+";
  G4String particleName3 = "e+";

  // Get the Geant4 particle table
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  particleGunPi->SetParticleDefinition(particleTable->FindParticle(particleName1));
  particleGunMu->SetParticleDefinition(particleTable->FindParticle(particleName2));
  particleGunE->SetParticleDefinition(particleTable->FindParticle(particleName3));

}
 
// Default destructor
// 
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    
  delete particleGunPi;
  delete particleGunMu;
  delete particleGunE;
  delete fMessenger;

  infile1->close();
  infile2->close();
}

// This method is called at the beginning of each event in order
// to generate a new primary vertex for tracking
//
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{


if(init==0)
{

// defining the beam data file
// 1 - /home/mischke/PlotBeam/BVF2010N448b_20200510.dat
// 2 - /home/mischke/PlotBeam/BVF2010N448b_20200510.dat
// 3 - /home/mischke/PlotBeam/BVF2011_20200510.dat
// 4 - /home/mischke/PlotBeam/BVF2012_20200510.dat

  const char filename1[] = "/home/mischke/project/mischke/data/alex_26sep.rays";
 const char filename2[] = "/home/mischke/PlotBeam/BVF2010N448a_20200510.dat";

/*if(databeam==1){

G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" Beam file BVF2010preNovB1_011023.dat will be used"<< G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;

        // BLOCKS 1
    const char filename2[] = "/project/6000314/civan/mc/bvf/BVF2010preNovB1_011023.dat";
      P0 = 75.081; // after WC3 shift
      L[0][0] = 1.0;  L[1][0] = 0.251584;  L[2][0] = -0.0758193;  L[3][0] = 0.0565415;  L[4][0] = -0.0608926;
      L[0][1] = 0.0;  L[1][1] = 0.967835;  L[2][1] = 0.14589;     L[3][1] = 0.599698;   L[4][1] = 0.118576;
      L[0][2] = 0.0;  L[1][2] = 0.0;       L[2][2] = 0.986391;    L[3][2] = -0.0112254; L[4][2] = 0.78103;
      L[0][3] = 0.0;  L[1][3] = 0.0;       L[2][3] = 0.0;         L[3][3] = 0.798148;   L[4][3] = 0.0279001;
      L[0][4] = 0.0;  L[1][4] = 0.0;       L[2][4] = 0.0;         L[3][4] = 0.0;        L[4][4] = 0.609464;
      }

      if(databeam==2){
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" Beam file BVF2010preNovB2_301121.dat will be used"<< G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;

     // BLOCK 2
     const char filename2[] = "/project/6000314/civan/mc/bvf/BVF2010preNovB2_301121.dat"; // BVF v1
     P0 = 75.0341; //
     //  //P0 = 75.0341*(1-0.005); //
       L[0][0] = 1.0;  L[1][0] = 0.25982;   L[2][0] = -0.0698679;  L[3][0] = 0.0635087;  L[4][0] = -0.0524559;
       L[0][1] = 0.0;  L[1][1] = 0.965657;  L[2][1] = 0.189198;    L[3][1] = 0.648109;   L[4][1] = 0.195976;
       L[0][2] = 0.0;  L[1][2] = 0.0;       L[2][2] = 0.97945;     L[3][2] = 0.0496893;  L[4][2] = 0.785995;
       L[0][3] = 0.0;  L[1][3] = 0.0;       L[2][3] = 0.0;         L[3][3] = 0.757266;   L[4][3] = 0.0882973;
       L[0][4] = 0.0;  L[1][4] = 0.0;       L[2][4] = 0.0;         L[3][4] = 0.0;        L[4][4] = 0.577284;
       }

       if(databeam==3){
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" Beam file BVF2010preNovB3_011121.dat will be used"<< G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;
     G4cout <<" ***************************"<<G4endl;

      // BLOCK 3
      const char filename2[] = "/project/6000314/civan/mc/bvf/BVF2010preNovB3_011121.dat";
      P0 = 74.517543;
        L[0][0] = 1.0;  L[1][0] = 0.307537;  L[2][0] = -0.067653;   L[3][0] = 0.0366907;  L[4][0] = -0.0714846;
        L[0][1] = 0.0;  L[1][1] = 0.951536;  L[2][1] = 0.172037;    L[3][1] = 0.530649;   L[4][1] = 0.119482;
        L[0][2] = 0.0;  L[1][2] = 0.0;       L[2][2] = 0.982765;    L[3][2] = -0.0142829; L[4][2] = 0.782448;
        L[0][3] = 0.0;  L[1][3] = 0.0;       L[2][3] = 0.0;         L[3][3] = 0.846676;   L[4][3] = 0.0832994;
        L[0][4] = 0.0;  L[1][4] = 0.0;       L[2][4] = 0.0;         L[3][4] = 0.0;        L[4][4] = 0.601207;
        }*/


if(databeam==1){
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" Beam file BVF2010N448a_20200510.dat will be used"<< G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
 const char filename2[] = "/home/mischke/PlotBeam/BVF2010N448a_20200510.dat";
      //P0 = 74.4222;
      P0 = 74.7596; //AFter SSC
  L[0][0] = 1.0;  L[1][0] = 0.312093; L[2][0] = -0.071054;  L[3][0] = 0.0588566;  L[4][0] = -0.0778541;
  L[0][1] = 0.0;  L[1][1] = 0.950051; L[2][1] = 0.224755;   L[3][1] = 0.561289;   L[4][1] = 0.17289;
  L[0][2] = 0.0;  L[1][2] = 0.0;      L[2][2] = 0.971821;   L[3][2] = -0.0100369; L[4][2] = 0.780972;
  L[0][3] = 0.0;  L[1][3] = 0.0;      L[2][3] = 0.0;        L[3][3] = 0.825463;   L[4][3] = 0.11143;
  L[0][4] = 0.0;  L[1][4] = 0.0;      L[2][4] = 0.0;        L[3][4] = 0.0;        L[4][4] = 0.584562;
}

 if(databeam==2){
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" Beam file BVF2010N448b_20200510.dat will be used"<< G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
    const char filename2[] = "/home/mischke/PlotBeam/BVF2010N448b_20200510.dat";
    //P0 = 73.8663;
    //P0 = 74.2037;
    P0 = 74.5052;
  L[0][0] = 1.0;  L[1][0] = 0.3391;   L[2][0] = -0.0627885; L[3][0] = 0.055023;   L[4][0] = -0.0614339;
  L[0][1] = 0.0;  L[1][1] = 0.94075;  L[2][1] = 0.0934388;  L[3][1] =  0.612371;  L[4][1] = 0.00260394;
  L[0][2] = 0.0;  L[1][2] = 0.0;      L[2][2] = 0.993643;   L[3][2] = -0.100237;  L[4][2] = 0.807246;
  L[0][3] = 0.0;  L[1][3] = 0.0;      L[2][3] = 0.0;        L[3][3] = 0.782258;   L[4][3] = -0.00358878;
  L[0][4] = 0.0;  L[1][4] = 0.0;      L[2][4] = 0.0;        L[3][4] = 0.0;        L[4][4] = 0.586992;
  }
  
if(databeam==3){
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" Beam file BVF2011_20200510.dat will be used"<< G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  const char filename2[] = "/home/mischke/PlotBeam/BVF2011_20200510.dat";
  //const char filename2[] = "/project/6000314/civan/mc/bvf/BeamData_G4_SI.dat";
    //P0 = 73.1953;
     P0 = 73.5348; // After SSC 1
    L[0][0] = 1.0;  L[1][0] = 0.30079;  L[2][0] = -0.0820981; L[3][0] = 0.0860666;   L[4][0] = -0.0656662;
  L[0][1] = 0.0;  L[1][1] = 0.95369;  L[2][1] = 0.151865;   L[3][1] = 0.596359;    L[4][1] = 0.123908;
  L[0][2] = 0.0;  L[1][2] = 0.0;      L[2][2] = 0.984986;   L[3][2] = -0.00948887; L[4][2] = 0.787578;
  L[0][3] = 0.0;  L[1][3] = 0.0;      L[2][3] = 0.0;        L[3][3] = 0.798034;    L[4][3] = 0.0257789;
  L[0][4] = 0.0;  L[1][4] = 0.0;      L[2][4] = 0.0;        L[3][4] = 0.0;         L[4][4] = 0.599492;

    //Definition of the Choleski Matrix Shintaro
  
  /*L[0][0] = 1.0;  L[1][0] = 0.501133; L[2][0] = -0.130539;  L[3][0] = 0.0794952;  L[4][0] = -0.128491;
  L[0][1] = 0.0;  L[1][1] = 0.86537;  L[2][1] = 0.259344;   L[3][1] = 0.387393;   L[4][1] = 0.102995;
  L[0][2] = 0.0;  L[1][2] = 0.0;      L[2][2] = 0.956922;   L[3][2] = -0.0534969; L[4][2] = 0.594524;
  L[0][3] = 0.0;  L[1][3] = 0.0;      L[2][3] = 0.0;        L[3][3] = 0.916922;   L[4][3] = 0.0307296;
  L[0][4] = 0.0;  L[1][4] = 0.0;      L[2][4] = 0.0;        L[3][4] = 0.0;        L[4][4] = 0.786434;*/
  
  
}
if(databeam==4){  
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" Beam file BVF2012_20200510.dat will be used"<< G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
  G4cout <<" ***************************"<<G4endl;
       const char filename2[] = "/home/mischke/PlotBeam/BVF2012_20200510.dat";
    //P0 = 73.9821; v0
    //P0 = 74.4222; SSC it1
    P0 = 74.5833;
  L[0][0] = 1.0;  L[1][0] = 0.286178; L[2][0] = -0.0747011; L[3][0] = 0.0798684;   L[4][0] = -0.0633374;
  L[0][1] = 0.0;  L[1][1] = 0.958177; L[2][1] = 0.171845;   L[3][1] = 0.58738;   L[4][1] = 0.121565;
  L[0][2] = 0.0;  L[1][2] = 0.0;      L[2][2] = 0.982288;   L[3][2] = -0.00513715; L[4][2] = 0.794892;
  L[0][3] = 0.0;  L[1][3] = 0.0;      L[2][3] = 0.0;        L[3][3] = 0.805344;   L[4][3] = 0.021046;
  L[0][4] = 0.0;  L[1][4] = 0.0;      L[2][4] = 0.0;        L[3][4] = 0.0;        L[4][4] = 0.59069;
 }
  	  
  // Momentum scaled (Feb. 27/19)
  //const char filename2[] = "/home/mischke/project/mischke/data/PositronBeam_74p0_20190226.dat";

  infile1 = new std::ifstream(filename1);
  infile2 = new std::ifstream(filename2);

  if(!(*infile1)) {
     G4cerr << "Error opening input file " << filename1 << G4endl;
     G4Exception("error","error",FatalException,"error");
  }

  if(!(*infile2)) {
     G4cerr << "Error opening input file " << filename2 << G4endl;
     G4Exception("error","error",FatalException,"error");
  }

 
  // An extra check - probably not needed
  std::filebuf* inbuff = infile1->rdbuf();
  if(!inbuff->is_open()) {
    G4cerr << "Input file could not be opened " << filename1 << G4endl;
    G4Exception("error","error",FatalException,"error");
  }

  //Fill the histogram to sample
  FillBeamHistograms();



init=1;
}

  // Variables to store the values read in from the file
  G4double X,dX,Y,dY,P;

  // Variables to store the final vertex position
  G4double xv,yv,zv,mdX,mdY,mdZ,kinEnergyPi,kinEnergyMu,kinEnergyE;

  // If we're at the end of the file, rewind.
  if (infile1->eof()) {
    infile1->clear();
    infile1->seekg(0);
  }

  if (infile2->eof()) {
    infile2->clear();
    infile2->seekg(0);
  }

  // Read in a line from the file
  if (databeam==0){
    //Revmoc file
    *infile1 >> X >> dX >> Y >> dY >> P;
    // Convert the angles to radians and find the x and y length of a
    // vector if the total length is 1.
    mdX = std::sin(dX/1000.0);
    mdY = std::sin(dY/1000.0);
    mdZ = std::sqrt(1-mdX*mdX-mdY*mdY);
  } else {
    //Data parameterization file
 
    //Sample the histograms and construct the correlated variables
    DoCorrelatedVariables();

    //Conversion not needed for the data beam
    P = r[0];
    X = r[1];
    Y = r[2];
    mdX = r[3];
    mdY = r[4];
    if (mdX*mdX+mdY*mdY<=1) mdZ = std::sqrt(1-mdX*mdX-mdY*mdY);
    else mdZ = 1.0;
  }

  // Scale and shift the vector.  Everything is in centimeters.
  xv = -60*cm*(mdX/mdZ) + X*cm;
  yv = -60*cm*(mdY/mdZ) + Y*cm;
  zv = -60*cm;

  //G4cout << xv << " " << yv << " " << mdX << " " << mdY << " " << mdZ << G4endl;
  
  G4double momentumFactor = 1.00;// P0 for Gaussian hard wired
  //G4double momentumFactor = 1.03588;//2011 Shintaro


  G4double PionMass = G4PionPlus::PionPlusDefinition()->GetPDGMass();
  G4double EMass    = G4Positron::PositronDefinition()->GetPDGMass();
  G4double MuMass   = G4MuonPlus::MuonPlusDefinition()->GetPDGMass();

  G4double Momentum = momentumFactor * P * GeV;

  kinEnergyPi = std::sqrt(Momentum*Momentum + PionMass*PionMass) - PionMass;
  kinEnergyMu = std::sqrt(Momentum*Momentum + MuMass*MuMass) - MuMass;
  //kinEnergyE  = std::sqrt(Momentum*Momentum + EMass*EMass) - EMass;
  //
  kinEnergyE = eEnergy*MeV;
  G4double theta = eAngle*(TMath::Pi()/180.0);
  G4double ndZ = std::cos(theta);

  G4double cc = std::sqrt(1-ndZ*ndZ);

  TRandom3 rng(0); // Seed = 0 uses time-based seed

    // Sample random angle theta in [0, 2π)
    G4double phi = rng.Uniform(0, 2 * TMath::Pi());

    // Compute x and y
    G4double ndX = cc*std::cos(phi);
    G4double ndY = cc*std::sin(phi);

    //G4cout << " GunEnergy " << kinEnergyE << " Angle " << theta << " (nx,ny,nz) " << ndX << " " << ndY << " " << ndZ << G4endl;


  // Set the particle position, energy, and momentum direction
  particleGunPi->SetParticlePosition(G4ThreeVector(xv,yv,zv));
  particleGunMu->SetParticlePosition(G4ThreeVector(xv,yv,zv));
  //particleGunE->SetParticlePosition(G4ThreeVector(xv,yv,zv));
  particleGunE->SetParticlePosition(G4ThreeVector(0.0,0.0,0.0));
  
  particleGunPi->SetParticleMomentumDirection(G4ThreeVector(mdX,mdY,mdZ));
  particleGunMu->SetParticleMomentumDirection(G4ThreeVector(mdX,mdY,mdZ));
  //particleGunE->SetParticleMomentumDirection(G4ThreeVector(mdX,mdY,mdZ));
  particleGunE->SetParticleMomentumDirection(G4ThreeVector(ndX,ndY,ndZ));
    
  particleGunPi->SetParticleEnergy(kinEnergyPi);
  particleGunMu->SetParticleEnergy(kinEnergyMu);
  particleGunE->SetParticleEnergy(kinEnergyE);
  //particleGunE->SetParticleEnergy(70*MeV);

  //POLARIZATION
  //particleGunMu->SetParticlePolarization(G4ThreeVector(mdX,mdY,mdZ));

  //NO-POLARIZATION
  //double rdmpol = G4UniformRand();
  //if (rdmpol>0.5) particleGunMu->SetParticlePolarization(G4ThreeVector(-mdX,-mdY,-mdZ));
  //else particleGunMu->SetParticlePolarization(G4ThreeVector(mdX,mdY,mdZ));

  //ANTI-POLARIZATION
  //particleGunMu->SetParticlePolarization(G4ThreeVector(-mdX,-mdY,-mdZ));

  //TRANSVERSE-POLARIZATION
  //particleGunMu->SetParticlePolarization(G4ThreeVector(0,0,1));

  //Begin Pileup code 
  if (pileup){

    G4double Rate = rate /s; //Beam rate (Hertz)
    
    G4int N = 0;
    G4int MAX=5;
    G4double tdecay;
    G4double tau = 2197.03*ns;//26.033 * ns;
    G4double t0[5]; //time
    G4double td[5]; //decay time
    
    //Time window
    G4double tmin = Tmin*ns;
    G4double tmax = Tmax*ns;
    
    for (int i=0;i<5;i++){t0[i]=0;td[i]=0;}

    //Generate the trigger particle
    t0[0] = 0;
    td[0] = -tau*log(G4UniformRand());

    G4double t=0;
    
    //For now, removed from the original algorithm 
    /*
    while (t>=-ttrig) {
      ttrig = -(1/Rate)*log(G4UniformRand());
      t     =  (1/Rate)*log(G4UniformRand());
    }
    

    //Try to place the first pileup particle before the trigger (->?)
    t = (1/Rate)*log(G4UniformRand()); 
    tdecay = (t - tau*log(G4UniformRand()))*ns;
    if ((t>=tmin)||(tdecay>=tmin && tdecay<=tmax)){    
      N=1;
      t0[N] = t*ns;
      td[N] = (-tau*log(G4UniformRand()))*ns;
    }
    */
    
    //Construct the other pileup particles before the trigger
    while ((t>=tmin)||(tdecay>=tmin && tdecay<=tmax)) {
      t = t - (-(1/Rate)*log(G4UniformRand()));
      tdecay = t - tau*log(G4UniformRand());
      
      if ((t>=tmin)||(tdecay>=tmin && tdecay<=tmax)){
	N++;
	N = min(N,MAX);
	t0[N] = t*ns;
	td[N] = (-tau*log(G4UniformRand()))*ns;
      }
    }
    
    //Construct the other pileup particles after the trigger
    t = 0.0 * ns;
    while (t<=tmax) {
      t = (t + (-(1/Rate)*log(G4UniformRand())))*ns;
      
      if (t<tmax){
	N++;
	N = min(N,MAX);      
	t0[N] = t*ns;
	td[N] = (-tau*log(G4UniformRand()))*ns;
      }
    }
 
    //G4cout << "---" << G4endl;
    //for (int i=0;i<5;i++) G4cout << "N = " << i << " " << t0[i] << "  " << td[i] << G4endl;
    
    G4double part;
    for (int i=0;i<=N;i++) {
      part = G4UniformRand();
      if (part<0.85)                 {particleGunPi->SetParticleTime(t0[i]);  particleGunPi->GeneratePrimaryVertex(anEvent);} //pion+
      if (part>=0.85 && part<0.99)   {particleGunMu->SetParticleTime(t0[i]);  particleGunMu->GeneratePrimaryVertex(anEvent);} //muon+
      if (part>=0.99)                {particleGunE->SetParticleTime(t0[i]);   particleGunE->GeneratePrimaryVertex(anEvent);}  //positron
    }
    

  } else { // No pileup

    //particleGunPi->GeneratePrimaryVertex(anEvent); //pion+
    //G4cout << particleGunMu->GetParticlePolarization() << G4endl;
    //particleGunMu->GeneratePrimaryVertex(anEvent); 
    particleGunE->GeneratePrimaryVertex(anEvent); 

  }
  
}

void PrimaryGeneratorAction::FillBeamHistograms()
{
 

  G4double P, X, Y, dX, dY;
   TRandom3  *r4 = new TRandom3(0);
   gRandom = r4;

  while (1)
  {
    *infile2 >> P >> X >> Y >> dX >> dY;
    //*infile2 >> X >> dX >> Y >> dY >> P;

    if (!infile2->good()) break;

    // start temporary code
    P = r4->Gaus(P0,0.6369);
    //P = r4->Gaus(P0,0.0637);
    //P = r4->Gaus(P0,0.9553); // 150 %

    //  end
    hE->Fill(P / 1000);
    hx->Fill(X / 10);
    hy->Fill(Y / 10);
    htx->Fill(dX);
    hty->Fill(dY);

    /*hE->Fill(P);
    hx->Fill(X);
    hy->Fill(Y);
    htx->Fill(dX);
    hty->Fill(dY)*/;

  }

  //Smooth the X and Y distributions
  hx->Smooth(200);
  hy->Smooth(200);

  
  //Fit the tx and ty distributions
  htx->Fit("gaus");
  hty->Fit("gaus");
  /*
  //Write out histograms
  file = new TFile("/home/mischke/mc/BeamData_G4_AS.root","RECREATE");
  hE->Write();
  hx->Write();
  hy->Write();
  htx->Write();
  hty->Write();
  file->Close();
  */
  tx_mean=htx->GetFunction("gaus")->GetParameter(1);
  tx_sigma=htx->GetFunction("gaus")->GetParameter(2);
  
  ty_mean=hty->GetFunction("gaus")->GetParameter(1);
  ty_sigma=hty->GetFunction("gaus")->GetParameter(2);
  
}

void PrimaryGeneratorAction::DoCorrelatedVariables()
{

  TRandom3  *r3 = new TRandom3(0);
  gRandom = r3;

  //Sample the histograms
  r[0] = hE->GetRandom();
  r[1] = hx->GetRandom();
  r[2] = hy->GetRandom();

  //Generate tx and ty disributions using gaussian
  r[3] = r3->Gaus(tx_mean,tx_sigma);
  r[4] = r3->Gaus(ty_mean,ty_sigma);

  delete r3;

  //G4cout << hE->GetMean() << "   " << hE->GetRMS() << G4endl;

  //Normalize the variables
  r[0] = (r[0] - hE->GetMean()) / hE->GetRMS();
  r[1] = (r[1] - hx->GetMean()) / hx->GetRMS();
  r[2] = (r[2] - hy->GetMean()) / hy->GetRMS();
  r[3] = (r[3] - htx->GetMean()) / htx->GetRMS();
  r[4] = (r[4] - hty->GetMean()) / hty->GetRMS();

  //Apply the Choleski Matrix
  G4double rt[5];
  for (int i = 0; i < 5; i++)
  {
    rt[i] = 0;
    for (int j = 0; j < 5; j++)
    {
      rt[i] += L[i][j] * r[j];
    } 
  }

  //Transform back the (correlated) variables
  r[0] = rt[0] * hE->GetRMS()  + hE->GetMean();
  r[1] = rt[1] * hx->GetRMS()  + hx->GetMean();
  r[2] = rt[2] * hy->GetRMS()  + hy->GetMean();
  r[3] = rt[3] * htx->GetRMS() + htx->GetMean();
  r[4] = rt[4] * hty->GetRMS() + hty->GetMean();
}









