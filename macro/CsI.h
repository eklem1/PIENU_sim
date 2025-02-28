#include <Rtypes.h> //Contains the ClassDef macro definition

class CsI {

 public:
  CsI();
  virtual ~CsI();

  double USIn[21];
  double USOut[28];
  double DSIn[21];
  double DSOut[27];

  double USInT[21];
  double USOutT[28];
  double DSInT[21];
  double DSOutT[27];
  
  double Sum;

  ClassDef(CsI,1)
};
