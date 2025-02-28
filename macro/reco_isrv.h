/** \file reco_isrv.h Interface Services. Services are object that 
    manipulate info stored in other objects. Interface Services, besides, know
    how to interact to outside world. They are the layer to be rewritten if/when 
    the representation of info in outside world changed.
*/


#ifndef reco_isrv_h
#define reco_isrv_h

#include <reco_classes.h>
#include <ConfigFile.h>

//class EventClass;
class MCtruthClass;
class PManager;

/** An nterface object which holds framework dependent functions and members */
class PISrv {
  PManager *fMan;
  map<string,double> t0;
  void load(PPlane* pln, int N, int* W, float* t, double t0);  
  void load(PPlane* pln, int N, float* x, float* t, double t0, double q);
  void cleanTrks1(PTracks& trks);
  void cleanTrks3(PTracks& trks);
  void cleanTrks3tg(PTracks& trks);
public:
  PISrv(): fMan(0) {};
  PManager& man() {return *fMan;}
  void SetManager(PManager *pMan) {fMan=pMan;}
  void GeomInit(PTracker&, ConfigFile&);
  void LoadInit(PTracker&, ConfigFile&);
  void Load(PTracker&, MCtruthClass&);
  void CleanTrks(PTracks& trks);

  static const uint kTR_1TRKREFIT   = 0x1;
  static const uint kTR_CHI2CLEANUP = 0x2;
  static const uint kTR_RMDOUBLES   = 0x4;
  static const uint kTR_BESTOFTWO   = 0x8;
  static const uint kTR_NOTIMINFO   = 0x10;
};


#endif
