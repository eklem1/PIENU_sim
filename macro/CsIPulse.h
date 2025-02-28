
/////////////////////////////////////////////////////
//
//This class implements the CsI pulse digitization
//
//LD, Dec11
//
/////////////////////////////////////////////////////


#include "TGraph.h"

using namespace std;

class CsIPulse {
 public:

  //Constructor
  CsIPulse();

  //Destructor
  ~CsIPulse();

  //Discrete pulse from averaged data.
  //Every point is spaced by 16.6ns (60MHz ADC sampling rate)
  TGraph *pulse;

  //Returns the discrete pulse points
  Float_t  GetPulseHeight(Int_t t);

  //Returns the interpolated pulse height at time t in nanoseconds.
  Float_t  GetInterpolatedPH(Float_t t);

  //Returns the pulse integral in the +/-5*16.6ns gate (+/-5 ADC samples = 183.3ns around the pulse maximum);
  Float_t  GetCharge(Float_t E);

};
