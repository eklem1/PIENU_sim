#include "rndmcpp.hh"
#include "RandomGen.hh"

void rndmcpp_(double *number) {
	RandomGen & rnd = RandomGen::GetInstance();
	*number = rnd.GetRand();
}

void ranluxcpp_(int *n, double *numbers) {
	RandomGen & rnd = RandomGen::GetInstance();
	rnd.GetRandArray(*n,numbers);
}
