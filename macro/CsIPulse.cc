
#include "CsIPulse.h"
#include "math.h"

CsIPulse::CsIPulse()
{
  //Discrete pulse from averaged data.
  //Every point is spaced by 16.6ns
  
  pulse = new TGraph();

  pulse->SetPoint(1,-16.6*5,0.0011);
  pulse->SetPoint(2,-16.6*4,0.0317);
  pulse->SetPoint(3,-16.6*3,0.0753);
  pulse->SetPoint(4,-16.6*2,0.3835);
  pulse->SetPoint(5,-16.6,0.6792);
  pulse->SetPoint(6,    0,0.8150);//top
  pulse->SetPoint(7, 16.6,0.7164);
  pulse->SetPoint(8, 16.6*2,0.5056);
  pulse->SetPoint(9, 16.6*3,0.3897);
  pulse->SetPoint(10,16.6*4,0.2961);
  pulse->SetPoint(11,16.6*5,0.2214);
  pulse->SetPoint(12,16.6*6,0.1650);
  pulse->SetPoint(13,16.6*7,0.1164);
  pulse->SetPoint(14,16.6*8,0.0819);
  pulse->SetPoint(15,16.6*9,0.0616);
  pulse->SetPoint(16,16.6*10,0.0468);
  pulse->SetPoint(17,16.6*11,0.0326);
  pulse->SetPoint(18,16.6*12,0.0308);
  pulse->SetPoint(19,16.6*13,0.0290);
  pulse->SetPoint(20,16.6*14,0.0279);
}

CsIPulse::~CsIPulse()
{

}

Float_t CsIPulse::GetPulseHeight(Int_t t)
{
  double x,y;
  pulse->GetPoint(t,x,y);

  return y;
}

Float_t CsIPulse::GetInterpolatedPH(Float_t t)
{
  return pulse->Eval(t,0,"S");
}

Float_t  CsIPulse::GetCharge(Float_t E)
{
  
  double gate = 16.6*5;
  double Int = 0.0;

  for (int i=0 ; i<100 ; i++){
    
    double t = -gate + i/100.0*(gate * 2);
    double step = 2*gate/100.0;

    Int += E*this->GetInterpolatedPH(t)*step + E*fabs(this->GetInterpolatedPH(t)-this->GetInterpolatedPH(t+step))*step/2.0;

  }

  return Int;

}
