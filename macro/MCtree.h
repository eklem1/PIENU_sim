#include "TChain.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TString.h"
#include "TRandom.h"
#include "TH1F.h"

#include "CsI.h"
#include "Resolution.h"
//#include "EventClass.h"
#include "log4cpp/Category.hh"

#include "MCtruthClass.h"
#include <reco_classes.h>



using namespace std;





class MCtree {
 public:
	
  
  MCtree(const char* tname1, const char* tname2);
  ~MCtree();
  
  //Trees
  TChain* MCTree1;
  TChain* MCTree2;
  TTree* OutputTree;
  
  //  EventClass E;
  MCtruthClass MC;
    PTracks         trks[3];
    PTracker         tkrs[3]; // Trackers
  //  vector <PTracks*> trks;  //  PTracks        *ptrks[3];
  //    PTracks         mctrks[3];
  //  PTracks*        pmctrks[3];
  //Files
  TFile* InFile;
  TFile* OutFile;

  //Input Variables
  Float_t energyDeposit;
  Float_t Ebirk;
  Float_t volumeID0;
  Float_t volumeID1;
  Float_t volumeID2;
  Float_t eventID;
  Int_t eventID2;
  Float_t uso,usi,dsi,dso;
  Float_t usot,usit,dsit,dsot;
  Float_t usoR,usiR,dsiR,dsoR;
  Float_t usoB,usiB,dsiB,dsoB;
  Float_t StartX,StartY;
  Float_t StartZ,StopZ;
	
  Float_t StartT,StopT;
  Float_t PID;

  Double_t PiDecayP[4];
  Double_t PiDecayX[4];
  Double_t MuDecayP[4];
  Double_t MuDecayX[4];
  Double_t PoStopP[4];
  Double_t PoStopX[4];
  Double_t PiStartP[4];
  Double_t PiStartX[4];
  Double_t MuStartP[4];
  Double_t MuStartX[4];
  Double_t PoStartP[4];
  Double_t PoStartX[4];
  // Tristan Sept 14/2011
  Double_t GStartP[4];
  Double_t GStartX[4];
  Double_t GStopP[4];
  Double_t GStopX[4];
  Double_t PhotonuclearX[3];
  Double_t MuPolarization[3];
  Double_t EMomentum[3];
  Double_t EBh;
  Double_t EBhT1;
  Double_t EBhT2;

  // Added by Tristan, Jan. 28/15, Bhabha stuff
  Double_t PreBhabhaPpos[4];
  Double_t PostBhabhaPpos[4];
  Double_t PostBhabhaPelec[4];

  // Tristan Feb. 4/15, Bhabha stuff
  Double_t EIntoBina;

  // Tristan Oct. 12/17
  Double_t WC3positronX[4];

  ///// Additions based on Tristan's work /////
  Double_t PosBremPreX[4];
  Double_t PosBremPreP[4];
  Double_t PosBremPostX[4];
  Double_t PosBremPostP[4];
  Double_t ElecBremPreX[4];
  Double_t ElecBremPreP[4];
  Double_t ElecBremPostX[4];
  Double_t ElecBremPostP[4];

  Double_t PosBhabhaPreX[4];
  Double_t PosBhabhaPreP[4];
  Double_t PosBhabhaPostX[4];
  Double_t PosBhabhaPostP[4];

  Double_t PosAnnihilPreX[4];
  Double_t PosAnnihilPreP[4];
  Double_t PosAnnihilPostX[4];
  Double_t PosAnnihilPostP[4];

  Double_t PosScatterPreX[4];
  Double_t PosScatterPreP[4];
  Double_t PosScatterPostX[4];
  Double_t PosScatterPostP[4];
  Double_t ElecScatterPreX[4];
  Double_t ElecScatterPreP[4];
  Double_t ElecScatterPostX[4];
  Double_t ElecScatterPostP[4];

  // Double_t ElecinWC3PreX[4];
  // Double_t ElecinWC3PreP[4];
  // Double_t PrimPosinWC3PreX[4];
  // Double_t PrimPosinWC3PreP[4];
  // Double_t SecPosinWC3PreX[4];
  // Double_t SecPosinWC3PreP[4];

  Double_t PosTotalBremE;
  Double_t ElecTotalBremE;
  Double_t PosTotalBhabhaE;

  //Output Variables

  Int_t static const MAX_NUM_HITS = 50;

  Float_t mupolarization[3];
  Float_t emomentum[3];

  Float_t ebina[4];
  Float_t eBbina[4];
  Float_t ebinar[4];
  Float_t xbina[4];
  Float_t efront[4];
  Float_t csi;
  Float_t Bcsi;
  Float_t csiR;
  Float_t binacsi;
  Float_t binacsiR;
  Float_t ebinag;

  Double_t csihits[97];
  Double_t csitimes[1000];
  int csinumhits;

  Float_t eb1[4];
  Float_t eb2[4];
  Float_t etg[4];
  Float_t et1[4];
  Float_t et2[4];
  Float_t ev2[4];
  Float_t ev3[4];

  Float_t eBb1[4];
  Float_t eBb2[4];
  Float_t eBtg[4];
  Float_t eBt1[4];
  Float_t eBt2[4];
  Float_t eBv2[4];
  Float_t eBv3[4];

  Float_t eCsIUSIch[21];
  Float_t eCsIUSOch[28];
  Float_t eCsIDSIch[21];
  Float_t eCsIDSOch[27];

  Float_t eCsIUSIchR[21];
  Float_t eCsIUSOchR[28];
  Float_t eCsIDSIchR[21];
  Float_t eCsIDSOchR[27];

  Float_t eb1r[4];
  Float_t eb2r[4];
  Float_t etgr[4];
  Float_t et1r[4];
  Float_t et2r[4];
  Float_t ev2r[4];
  Float_t ev3r[4];

  Float_t et2_trig;

  Float_t ess1_1[4];
  Float_t ess1_2[4];
  Float_t ess2_1[4];
  Float_t ess2_2[4];
  Float_t ess3_1[4];
  Float_t ess3_2[4];
  
  Float_t ess1_1r[4];
  Float_t ess1_2r[4];
  Float_t ess2_1r[4];
  Float_t ess2_2r[4];
  Float_t ess3_1r[4];
  Float_t ess3_2r[4];

  // Tristan Sep. 22/17

  Float_t ewc3_1;
  Float_t ewc3_2;
  Float_t ewc3_3;

  Float_t xb1[4];
  Float_t xb2[4];
  Float_t xtg[4];
  Float_t xt1[4];
  Float_t xt2[4];
  Float_t xv2[4];
  Float_t xv3[4];


  Float_t xss1_1[4];
  Float_t xss1_2[4];
  Float_t xss2_1[4];
  Float_t xss2_2[4];
  Float_t xss3_1[4];
  Float_t xss3_2[4];


  Float_t xwc1_1[4];
  Float_t xwc1_2[4];
  Float_t xwc1_3[4];
  Float_t xwc2_1[4];
  Float_t xwc2_2[4];
  Float_t xwc2_3[4];
  Float_t xwc3_1[4];
  Float_t xwc3_2[4];
  Float_t xwc3_3[4];




  Int_t   wc1_1n;
  Int_t wc1_1w[MAX_NUM_HITS];
  Float_t wc1_1t[MAX_NUM_HITS];
  Float_t wc1_1pid[MAX_NUM_HITS];

  Int_t   wc1_2n;
  Int_t wc1_2w[MAX_NUM_HITS];
  Float_t wc1_2t[MAX_NUM_HITS];
  Float_t wc1_2pid[MAX_NUM_HITS];

  Int_t   wc1_3n;
  Int_t wc1_3w[MAX_NUM_HITS];
  Float_t wc1_3t[MAX_NUM_HITS];
  Float_t wc1_3pid[MAX_NUM_HITS];

  Int_t   wc2_1n;
  Int_t wc2_1w[MAX_NUM_HITS];
  Float_t wc2_1t[MAX_NUM_HITS];
  Float_t wc2_1pid[MAX_NUM_HITS];

  Int_t   wc2_2n;
  Int_t wc2_2w[MAX_NUM_HITS];
  Float_t wc2_2t[MAX_NUM_HITS];
  Float_t wc2_2pid[MAX_NUM_HITS];

  Int_t   wc2_3n;
  Int_t wc2_3w[MAX_NUM_HITS];
  Float_t wc2_3t[MAX_NUM_HITS];
  Float_t wc2_3pid[MAX_NUM_HITS];





  Int_t   wc3_1n;
  Int_t wc3_1w[MAX_NUM_HITS];
  Float_t wc3_1t[MAX_NUM_HITS];
  Float_t wc3_1pid[MAX_NUM_HITS];

  Int_t   wc3_2n;
  Int_t wc3_2w[MAX_NUM_HITS];
  Float_t wc3_2t[MAX_NUM_HITS];
  Float_t wc3_2pid[MAX_NUM_HITS];

  Int_t   wc3_3n;
  Int_t wc3_3w[MAX_NUM_HITS];
  Float_t wc3_3t[MAX_NUM_HITS];
  Float_t wc3_3pid[MAX_NUM_HITS];


  Int_t ss1_1n;
  Float_t ss1_1w[MAX_NUM_HITS];
  Float_t ss1_1x[MAX_NUM_HITS];
  Float_t ss1_1t[MAX_NUM_HITS];
  Float_t ss1_1pid[MAX_NUM_HITS];
  Float_t ss1_1e[MAX_NUM_HITS];
  Int_t ss1_2n;
  Float_t ss1_2w[MAX_NUM_HITS];
  Float_t ss1_2y[MAX_NUM_HITS];
  Float_t ss1_2t[MAX_NUM_HITS];
  Float_t ss1_2pid[MAX_NUM_HITS];
  Float_t ss1_2e[MAX_NUM_HITS];
  Int_t ss2_1n;
  Float_t ss2_1w[MAX_NUM_HITS];
  Float_t ss2_1x[MAX_NUM_HITS];
  Float_t ss2_1t[MAX_NUM_HITS];
  Float_t ss2_1pid[MAX_NUM_HITS];
  Float_t ss2_1e[MAX_NUM_HITS];
  Int_t ss2_2n;
  Float_t ss2_2w[MAX_NUM_HITS];
  Float_t ss2_2y[MAX_NUM_HITS];
  Float_t ss2_2t[MAX_NUM_HITS];
  Float_t ss2_2pid[MAX_NUM_HITS];
  Float_t ss2_2e[MAX_NUM_HITS];
  Int_t ss3_1n;
  Float_t ss3_1w[MAX_NUM_HITS];
  Float_t ss3_1x[MAX_NUM_HITS];
  Float_t ss3_1t[MAX_NUM_HITS];
  Float_t ss3_1pid[MAX_NUM_HITS];
  Float_t ss3_1e[MAX_NUM_HITS];
  Int_t ss3_2n;
  Float_t ss3_2w[MAX_NUM_HITS];
  Float_t ss3_2y[MAX_NUM_HITS];
  Float_t ss3_2t[MAX_NUM_HITS];
  Float_t ss3_2pid[MAX_NUM_HITS];
  Float_t ss3_2e[MAX_NUM_HITS];


  Int_t   b1_n;
  Float_t b1_e[MAX_NUM_HITS];
  Float_t b1_t[MAX_NUM_HITS];
  Float_t b1_pid[MAX_NUM_HITS];

  Int_t   b2_n;
  Float_t b2_e[MAX_NUM_HITS];
  Float_t b2_t[MAX_NUM_HITS];
  Float_t b2_pid[MAX_NUM_HITS];

  Int_t   tg_n;
  Float_t tg_e[MAX_NUM_HITS];
  Float_t tg_t[MAX_NUM_HITS];
  Float_t tg_pid[MAX_NUM_HITS];

  Int_t   t1_n;
  Float_t t1_e[MAX_NUM_HITS];
  Float_t t1_t[MAX_NUM_HITS];
  Float_t t1_pid[MAX_NUM_HITS];

  Int_t   t2_n;
  Float_t t2_e[MAX_NUM_HITS];
  Float_t t2_t[MAX_NUM_HITS];
  Float_t t2_pid[MAX_NUM_HITS];

  Int_t   v2_n;
  Float_t v2_e[MAX_NUM_HITS];
  Float_t v2_t[MAX_NUM_HITS];
  Float_t v2_pid[MAX_NUM_HITS];


  Int_t   v3_n;
  Float_t v3_e[MAX_NUM_HITS];
  Float_t v3_t[MAX_NUM_HITS];
  Float_t v3_pid[MAX_NUM_HITS];


  


  Float_t tgstartzp,tgstartze,tgstartzm,tgstopzp,tgstopze,tgstopzm;

  Float_t USI,USO,DSI,DSO;
  Float_t USIr,USOr,DSIr,DSOr;
  Float_t USIb,USOb,DSIb,DSOb;
  Float_t startX,startY;
  Float_t startT,stopT;
  Float_t R;
  Float_t Tpos;

  Double_t pidecayp[4];
  Double_t pidecayx[4];
  Double_t mudecayp[4];
  Double_t mudecayx[4];
  Double_t postopp[4];
  Double_t postopx[4];
  Double_t pistartp[4];
  Double_t pistartx[4];
  Double_t mustartp[4];
  Double_t mustartx[4];
  Double_t postartp[4];
  Double_t postartx[4];
  //Tristan Sept 14/2011
  Double_t gstartp[4];
  Double_t gstartx[4];
  Double_t gstopp[4];
  Double_t gstopx[4];
  Double_t photonuclearx[3];
  Double_t ebh;
  // Added by Tristan, Jan. 28/15, Bhabha stuff
  Double_t prebhabhappos[4];
  Double_t postbhabhappos[4];
  Double_t postbhabhapelec[4];  
  Double_t ebht1;
  Double_t ebht2;
  // Tristan Feb. 4/15 (Bhabha stuff)
  Double_t eintobina;

    Double_t binahits[100];
    Double_t birkshits[100];
  Double_t binatimes[100];
  Double_t binaph;
  Double_t binaq;
  Double_t birksph;
  Double_t birksq;
  Double_t birksphr;
  Double_t birksqr;
  int numhits;
  //  Double_t tnai, energy, energy2;

  ///// Additions based on Tristan's work /////
  Double_t posbremprex[4];
  Double_t posbremprep[4];
  Double_t posbrempostx[4];
  Double_t posbrempostp[4];
  Double_t elecbremprex[4];
  Double_t elecbremprep[4];
  Double_t elecbrempostx[4];
  Double_t elecbrempostp[4];

  Double_t posbhabhaprex[4];
  Double_t posbhabhaprep[4];
  Double_t posbhabhapostx[4];
  Double_t posbhabhapostp[4];

  Double_t posannihilprex[4];
  Double_t posannihilprep[4];
  Double_t posannihilpostx[4];
  Double_t posannihilpostp[4];

  Double_t posscatterprex[4];
  Double_t posscatterprep[4];
  Double_t posscatterpostx[4];
  Double_t posscatterpostp[4];
  Double_t elecscatterprex[4];
  Double_t elecscatterprep[4];
  Double_t elecscatterpostx[4];
  Double_t elecscatterpostp[4];

  // Double_t elecinwc3prex[4];
  // Double_t elecinwc3prep[4];
  // Double_t primposinwc3prex[4];
  // Double_t primposinwc3prep[4];
  // Double_t secposinwc3prex[4];
  // Double_t secposinwc3prep[4];

  Double_t postotalbreme;
  Double_t electotalbreme;
  Double_t postotalbhabhae;

  Float_t ampx[48];
  Float_t ampxt[48];
  int ampxn[48];
  int Nampx;
  int Nclustx;
  Float_t s3_x_posi[MAX_NUM_HITS];
  Float_t aqr_x[MAX_NUM_HITS];
  Float_t namp_x[MAX_NUM_HITS];
  Float_t tcl_x[MAX_NUM_HITS];
  int nstrips_x;
  Float_t ampn_x[MAX_NUM_HITS];
  Float_t relamp;

  Float_t ampy[48];
  Float_t ampyt[48];
  int ampyn[48];
  int Nampy;
  int Nclusty;
  Float_t s3_y_posi[MAX_NUM_HITS];
  Float_t aqr_y[MAX_NUM_HITS];
  Float_t ampn_y[MAX_NUM_HITS];
  Float_t tcl_y[MAX_NUM_HITS];
  int nstrips_y;
  Float_t namp_y[MAX_NUM_HITS];

  
  //Tristan Oct. 12/17
  Double_t wc3positronx[4];

  Float_t zv;
  Float_t kv;
  Float_t phie;
  Float_t thetae;
  Float_t csich;
  Float_t dphi;
  Float_t usiZ,usiZn;

  // Tristan, Sep. 29/17
  Float_t Xwc3_1;

  Float_t Xwc3_2;
  Float_t Ywc3_2;
  Float_t Zwc3_2;
  Float_t Rwc3_2;

  // Tristan, Oct. 24/17
  Double_t s3wc3x0;
  Double_t s3wc3y0;
  Double_t s3wc3tx;
  Double_t s3wc3ty;

  // Tristan, July 20/17
  Double_t s12x0;
  Double_t s12y0;
  Double_t s12tx;
  Double_t s12ty;
  Float_t Xwc12;
  Float_t Ywc12;
  Float_t txwc12;
  Float_t tywc12;
  // Tristan, June 26/17
  Float_t Rwc3_2_besttrack;
  Float_t Rwc3_2_maxrad;
  Float_t Old_Rwc3_2;
 
  Int_t eventnumber;

  Float_t postime,pitime,posmin;

  CsI* csiall;


  Int_t ntracks;
  

  //Resolution Class
  Resolution *resolution;

  void Clear();
  void SetInputFile(const char* fname);
  void SetOutputFile(const char* fname, const char* tname);
  
  void Loop();

  void SaveOutputTree();

  
 private :



};

