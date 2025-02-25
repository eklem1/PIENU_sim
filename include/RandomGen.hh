// B. Velghe (Aug. 2021): This is a placeholder, the Fortran decay generators need a 
// source of random number. We call the ROOT TRandom3 code.
// Ideally, one should rely on the Geant4 infrastructure.
// The seed is set randomly (that is, the runs are not reproducible), this has to be fixed for instance by
// implementing a messenger class.
#ifndef _RandomGen_hh_
#define _RandomGen_hh_

#include "TRandom3.h"

class RandomGen {
	public:
		static RandomGen & GetInstance();	

		// Make the object non-copyable
		RandomGen(const RandomGen &) = delete;
		void operator=(const RandomGen &) = delete;

		double GetRand();
		void GetRandArray(int n, double *);
	private:
		RandomGen();	
		~RandomGen();
		TRandom3 * fRndEng;
};

#endif
