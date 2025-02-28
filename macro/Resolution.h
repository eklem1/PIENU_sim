
#include "TRandom.h"

using namespace std;

class Resolution {
 public:

  Resolution();
  ~Resolution();

  //Random Generator
  TRandom *rdm;
  
  Float_t BinaResolution(Float_t x,Float_t res);
  Float_t CsIResolution(Float_t x, Float_t res);
  Float_t T2Resolution(Float_t x, Float_t res);
};
