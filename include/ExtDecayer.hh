// B. Velghe (Aug. 2021): Minimal implementation, not very configurable. 
// TODO: - Add an option to select the decay generator
//       - Remove hard-coded constants (PID codes, etc)
//       - CHECK the structure memory alignment (Fortran / C++ interface), this is prone to bugs.
#ifndef __EXTDECAYER_H__
#define __EXTDECAYER_H__

#include <G4VExtDecayer.hh>
#include <G4ParticleTable.hh>
#include <G4Track.hh>
#include <G4DynamicParticle.hh>

class ExtDecayerMessenger;

class ExtDecayer : public G4VExtDecayer
{
	public:
		ExtDecayer();
		~ExtDecayer();
		G4DecayProducts * ImportDecayProducts(const G4Track &);	
		G4DecayProducts * ImportDecayProducts(const G4DynamicParticle &);
  G4double dataradiative;
	private:
  ExtDecayerMessenger* fMessenger;
};


#endif
