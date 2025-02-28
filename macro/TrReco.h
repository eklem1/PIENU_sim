#ifndef TrReco_h
#define TrReco_h

#include <reco_manager.h>
#include <reco_classes.h>
//#include <TTree.h>
#include "MCtruthClass.h"
//class ConfigFile;
//class EventClass;
class MCtruthClass;
// AS inclusion of Ptracks into a tree
#include <TTree.h>
const int NTKRS=5;

class TrRecoClass {
 public:
  PTracker         tkrs[NTKRS]; // Trackers
  vector<PTracks*> trks; // Track collections
  PManager   man;
  TrRecoClass();
  void Init(ConfigFile &Conf);
  void Process(MCtruthClass &MC);
  void setBranches(TTree*);
};


#endif
