#include "RandomGen.hh"
#include <iostream>

RandomGen & RandomGen::GetInstance() {
	static RandomGen instance;
	return instance;
}

RandomGen::RandomGen() {
	// Seed is automatically computed via a TUUID object 
	// (guaranteed to be unique according to the doc). 
	fRndEng = new TRandom3(0);
}

RandomGen::~RandomGen() {
	delete fRndEng;
}

double RandomGen::GetRand() {
	return fRndEng->Rndm();
}

void RandomGen::GetRandArray(int n, double * array) {
	fRndEng->RndmArray(n,array);
}
