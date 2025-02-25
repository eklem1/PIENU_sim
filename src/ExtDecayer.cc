#include "ExtDecayer.hh"
#include "ExtDecayerMessenger.hh"

#define kch2lnug kch2lnug_
#define kch2lnug_ib kch2lnug_ib_
#define pilnug pilnug_
#define pich2enug_gatti pich2enug_gatti_ 
#define cmcint cmcint_
#define cmcdouble cmcdouble_

extern "C" {
	void kch2lnug(int &, int &, int &, int &, int &);
	void kch2lnug_ib(int &, int &, int &, int &, int &);
	void pilnug(int &, int &, int &);
        void pich2enug_gatti(int &, int &);
	
	struct {
		int NPART, NPARTGEN, MainDecay, PID[50], PIDGEN[50], KEEP[50], FLAG[50], UNIFORM;
	} cmcint_;

	struct {
		double P4INI[50][4], P4INIGEN[50][4], POL3INI[50][3];
	} cmcdouble_;
}

ExtDecayer::ExtDecayer() : G4VExtDecayer("ExtDecayer" ) {
  fMessenger = new ExtDecayerMessenger(this);
  G4cout<< "Messenger is initiated*************"<< G4endl;

}

ExtDecayer::~ExtDecayer() {
}

// Declaration for use with physics list (G4DecayTable, etc...)
G4DecayProducts * ExtDecayer::ImportDecayProducts(const G4Track & aTrack) {
	return ImportDecayProducts(*aTrack.GetDynamicParticle());
}	

// Declaration for use with PrimaryActionGenerator (assigned daughter particle)
G4DecayProducts * ExtDecayer::ImportDecayProducts(const G4DynamicParticle & aParticle) {


	// Initialize the common block 
	cmcint.NPART = 0;
	cmcint.NPARTGEN = 0;
	cmcint.PID[0] = 211; // Pion
	cmcdouble.P4INI[0][0] = aParticle.GetMomentum().getX()/CLHEP::GeV; 
	cmcdouble.P4INI[0][1] = aParticle.GetMomentum().getY()/CLHEP::GeV; 
	cmcdouble.P4INI[0][2] = aParticle.GetMomentum().getZ()/CLHEP::GeV; 
	cmcdouble.P4INI[0][3] = aParticle.GetTotalEnergy()/CLHEP::GeV;
	//			G4cout << " P4INI  " << cmcdouble.P4INI[0][3] << G4endl;

	// pi+ --> mu+ nu g (IB) with a Egamma cutoff
	int fBeamIndex = 1; /* Index of the beam particle in cmcdouble (Fortran numbering): fixed */ 
	int fParentType = 2;
	int fParentCharge = 1;
	
	// Call the FORTRAN77 code
	if(dataradiative == 1){
	  int fDecayType = 2;
	  int fDecayMode = 1;
	  kch2lnug_ib(fBeamIndex,fParentType,fDecayType,fParentCharge,fDecayMode);
	} else {
	int fDecayType = 1;  // for positron
	int fDecayMode = 6;  //for pilnug
	pich2enug_gatti(fBeamIndex,fDecayMode);
	}
	//	kch2lnug(fBeamIndex,fParentType,fDecayType,fParentCharge,fDecayMode);
	//	pilnug(fBeamIndex,fDecayType,fDecayMode);
	  
	// NOTE Who is deleting this? Doc is unclear 
	// --> G4DynamicParticle::~G4DynamicParticle() <-- 
	auto decayProducts = new G4DecayProducts(aParticle);
	
	// Import the generated particles 
	//		G4cout << " NPART " << cmcint.NPART << G4endl;
		for(int i = 0 ; i < cmcint.NPART ; i++) {
		if(!cmcint.KEEP[i]) continue;
		auto particleTable = G4ParticleTable::GetParticleTable();
		auto partDef = particleTable->FindParticle(cmcint.PID[i]);
		//		G4cout << " PID " << cmcint.PID[i] << G4endl;
		auto fourMomentum = G4LorentzVector(cmcdouble.P4INI[i][0]*CLHEP::GeV,cmcdouble.P4INI[i][1]*CLHEP::GeV,cmcdouble.P4INI[i][2]*CLHEP::GeV,cmcdouble.P4INI[i][3]*CLHEP::GeV);
		//			if(cmcint.PID[i] == 22) G4cout << cmcdouble.P4INI[i][3]*CLHEP::GeV << G4endl;
		auto dynaParticle = new G4DynamicParticle(partDef,fourMomentum);
		decayProducts->PushProducts(dynaParticle);			
	}	

	return decayProducts;	
}
