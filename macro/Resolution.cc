
#include "Resolution.h"
#include "math.h"

Resolution::Resolution()
{
  rdm = new TRandom();
}

Resolution::~Resolution()
{

}

Float_t Resolution::BinaResolution(Float_t x, Float_t res)
{
  if (x!=0.0){
    //Float_t sigma = (4.6e-5)*pow(x,2) - (1.6e-3)*x + 0.46;
    //Float_t r = rdm->Gaus(1.0,sigma);
    //Float_t r = rdm->Gaus(1.0,1/(sqrt(143*x) ));
    // Float_t r = rdm->Gaus(0.0,0.045 + res*sqrt(x));
    Float_t r = rdm->Gaus(0.0,res*sqrt(x));
    if ((x+r)>=0) return x+r;
    else return 0;

  } else { return x;}

}

Float_t Resolution::CsIResolution(Float_t x, Float_t res)
{

  if (x!=0.0){
    Float_t r = rdm->Gaus(0.0,res*sqrt(x));
    return x+r;

  } else { return x;}

}

Float_t Resolution::T2Resolution(Float_t x, Float_t res)
{

  if (x!=0.0){
    Float_t r = rdm->Gaus(0.0,res*sqrt(x));
    return x+r;

  } else { return x;}

}
