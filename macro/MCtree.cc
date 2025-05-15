
////////////////////////////////////////////////////////////
//
// MCtree.cc
//
// Implements the main loop on the MC data and the name
// of the variables in the input and output trees
//
//
////////////////////////////////////////////////////////////


#include "MCtree.h"
#include "TrReco.h"
//#include "ConfigFile.h"
//#include"EventClass.h"
#include"MCtruthClass.h"

#include <vector>
#include "TRandom3.h"

TRandom3 *r=new TRandom3();

//Standard Constructor
 TrRecoClass TrReco;
// EventClass E;

TH1F *SumWF, *SumWF2;
TH1F *WF, *WF2;
TH1F *hh1,*hh2;

MCtree::MCtree(const char* tname1, const char* tname2)
{
  MCTree1 = new TChain(tname1);
  MCTree2 = new TChain(tname2);

  csiall = new CsI();

  this->Clear();
  resolution = new Resolution();
  string ConfFilename="recotest.conf";
  ConfigFile Conf(ConfFilename.c_str());
  TrReco.Init(Conf);

    TFile *f1=new TFile("/project/6000314/mischke/omb/NewBinaCsI2.root");
  hh1=(TH1F*)gDirectory->Get("bina");
  hh1->SetDirectory(0);
  hh2=(TH1F*)gDirectory->Get("csi");
  hh2->SetDirectory(0);
  f1->Close();

}
  
//Destructor
MCtree::~MCtree()
{
  cout << "Default Destructor" << endl;
}

//Initialize the variables
void MCtree::Clear()
{
  csi      = 0;
  Bcsi      = 0;
  csiR     = 0;
  binacsi  = 0;
  binacsiR = 0;
  ebinag = 0.;
  usiZ = 0;
  usiZn = 0;

  for(int i=0; i<3; i++){
    mupolarization[i] = -999;
    emomentum[i] = -999;
  }


  for (int i=0;i<4;i++)
    {
      eb1[i]=eb2[i]=etg[i]=et1[i]=et2[i]=ev2[i]=ev3[i]=0;
      eBb1[i]=eBb2[i]=eBtg[i]=eBt1[i]=eBt2[i]=eBv2[i]=eBv3[i]=0;
      eb1r[i]=eb2r[i]=etgr[i]=et1r[i]=et2r[i]=ev2r[i]=ev3r[i]=0;
      xb1[i]=xb2[i]=xtg[i]=xt1[i]=xt2[i]=xv2[i]=xv3[i]=-999;

      ess1_1[i]=ess1_2[i]=ess2_1[i]=ess2_2[i]=ess1_1r[i]=ess1_2r[i]=ess2_1r[i]=ess2_2r[i]=0;
      ess3_1[i]=ess3_2[i]=ess3_1r[i]=ess3_2r[i]=0;      

      // Tristan Sep. 22/17
      ewc3_1 = 0;
      ewc3_2 = 0;
      ewc3_3 = 0;

      
      xss1_1[i]=xss1_2[i]=xss2_1[i]=xss2_2[i]=-999;
      xss3_1[i]=xss3_2[i]=-999;
      xwc1_1[i]=xwc1_2[i]=xwc1_3[i]=-999;
      xwc2_1[i]=xwc2_2[i]=xwc2_3[i]=-999;
      xwc3_1[i]=xwc3_2[i]=xwc3_3[i]=-999;
      ebina[i]=eBbina[i]=ebinar[i]=0;
      efront[i]=0;
      xbina[i]=-999;
    }
   et2_trig = 0.;
  
  tgstartzp =-999;
  tgstartze =-999;
  tgstartzm =-999;
  tgstopzp =-999;
  tgstopze =-999;
  tgstopzm =-999;

  usi = uso = dsi = dso = 0.0;
  usiR = usoR = dsiR = dsoR = 0.0;
  usit = usot = dsit = dsot = 0.0; //REM
  usiB = usoB = dsiB = dsoB = 0.0;
  startX=startY=0;
  R = 0;
  startT=stopT = 0;

  zv = -999;
  kv = -999;
  phie = -999;
  thetae = -999;
  dphi = -999;
  Rwc3_2=Xwc3_2=Ywc3_2=Zwc3_2=999;
  Old_Rwc3_2=999;//shintaro
  // Tristan, Sep. 29/17
  Xwc3_1 = 999;

  // Tristan, June 26/17
  Rwc3_2_besttrack = 999;
  Rwc3_2_maxrad = 999;

  // Added by Tristan, July 20/17
  Xwc12=Ywc12=txwc12=tywc12=999;
  s12x0 = 999;
  s12y0 = 999;
  s12tx = 999;
  s12ty = 999;

  // Added by Tristan, Oct. 24/17
  s3wc3x0 = 999;
  s3wc3y0 = 999;
  s3wc3tx = 999;
  s3wc3ty = 999;

  //CsI Variables initialization
  for (int i=0;i<21;i++) {csiall->USIn[i]=0; eCsIUSIch[i] = 0;}
  for (int i=0;i<28;i++) {csiall->USOut[i]=0; eCsIUSOch[i] = 0;}
  // AAGH!
  //for (int i=0;i<28;i++) csiall->USIn[i]=0;
  //for (int i=0;i<21;i++) csiall->DSOut[i]=0;
  for (int i=0;i<21;i++) {csiall->DSIn[i]=0; eCsIDSIch[i] = 0;}
  for (int i=0;i<27;i++) {csiall->DSOut[i]=0; eCsIDSOch[i] = 0;}
  csiall->Sum = 0.0;  



  for (int i=0;i<100;i++) binahits[i]=-10000;
  for (int i=0;i<100;i++) birkshits[i]=-10000;
  for (int i=0;i<100;i++) binatimes[i]=-10000;
  for (int i=0;i<97;i++) csihits[i]=-10000;
  // Changed by Tristan, Mar. 6/17, for late CsI hits study
  for (int i=0;i<1000;i++) csitimes[i]=-10000;
 // Added by Tristan, Mar. 6/17, for late CsI hits study
  csinumhits = 0;
  binaph = -10000;
  binaq = -10000;
  birksph = -10000;
  birksq = -10000;
  birksphr = -10000;
  birksqr = -10000;

  for (int i=0;i<48;i++) ampx[i]=0;
  for (int i=0;i<48;i++) ampy[i]=0;
  for (int i=0;i<48;i++) ampxn[i]=-999;
  for (int i=0;i<48;i++) ampyn[i]=-999;
  for (int i=0;i<48;i++) ampn_x[i]=-999;
  for (int i=0;i<48;i++) ampn_y[i]=-999;
  for (int i=0;i<48;i++) ampxt[i]=-999;
  for (int i=0;i<48;i++) ampyt[i]=-999;
  Nclustx = 0;
  Nclusty = 0;
  nstrips_x = 0;
  nstrips_y = 0;

 
    for (int i=0;i<MAX_NUM_HITS;i++) {
      ss3_1w[i] = 0;
      ss3_2w[i] = 0;
      s3_x_posi[i] = 0;
      aqr_x[i] = -99;
      namp_x[i] = 0;
      s3_y_posi[i] = 0;
      aqr_y[i] = -99;
      namp_y[i] = 0;
      tcl_x[i] = -999;
      tcl_y[i] = -999;
    }
    
}

void MCtree::SetInputFile(const char* fname) {
  cout << "Opening MC file: " << fname << endl;
  InFile->Open(fname);
  
  cout << "This is a dummy line" << endl;
  
  MCTree1->Add(fname);
  MCTree2->Add(fname);
  
  MCTree1->SetBranchAddress("eventID",&eventID);
  MCTree1->SetBranchAddress("energyDeposit",&energyDeposit);
  MCTree1->SetBranchAddress("Ebirk",&Ebirk);
  MCTree1->SetBranchAddress("volumeID0",&volumeID0);
  MCTree1->SetBranchAddress("volumeID1",&volumeID1);
  MCTree1->SetBranchAddress("volumeID2",&volumeID2);
  MCTree1->SetBranchAddress("startX",&StartX);
  MCTree1->SetBranchAddress("startY",&StartY);
  MCTree1->SetBranchAddress("startZ",&StartZ);
  MCTree1->SetBranchAddress("stopZ",&StopZ);
  MCTree1->SetBranchAddress("startT",&StartT);
  MCTree1->SetBranchAddress("stopT",&StopT);
  MCTree1->SetBranchAddress("PID",&PID);
  MCTree2->SetBranchAddress("eventID",&eventID2);

  MCTree2->SetBranchAddress("PiStartX",&PiStartX); 
  MCTree2->SetBranchAddress("PiStartP",&PiStartP);
  MCTree2->SetBranchAddress("PiDecayX",&PiDecayX);
  MCTree2->SetBranchAddress("PiDecayP",&PiDecayP);
  MCTree2->SetBranchAddress("MuStartX",&MuStartX);
  MCTree2->SetBranchAddress("MuStartP",&MuStartP);
  MCTree2->SetBranchAddress("MuDecayX",&MuDecayX);
  MCTree2->SetBranchAddress("MuDecayP",&MuDecayP);
  MCTree2->SetBranchAddress("PoStartX",&PoStartX);
  MCTree2->SetBranchAddress("PoStartP",&PoStartP);
  MCTree2->SetBranchAddress("PoStopX",&PoStopX);
  MCTree2->SetBranchAddress("PoStopP",&PoStopP);
  /*
  MCTree2->SetBranchAddress("GStartX",&GStartX);
  MCTree2->SetBranchAddress("GStartP",&GStartP);
  MCTree2->SetBranchAddress("GStopX",&GStopX);
  MCTree2->SetBranchAddress("GStopP",&GStopP);
  */
  MCTree2->SetBranchAddress("PhotonuclearX",&PhotonuclearX);
  MCTree2->SetBranchAddress("BhabhaE",&EBh);
  MCTree2->SetBranchAddress("MuPolarization",&MuPolarization);
  MCTree2->SetBranchAddress("EMomentum",&EMomentum);
  /*
  // Added by Tristan, Jan. 28/15, Bhabha stuff
  MCTree2->SetBranchAddress("PreBhabhaPpos",&PreBhabhaPpos);
  MCTree2->SetBranchAddress("PostBhabhaPpos",&PostBhabhaPpos);
  MCTree2->SetBranchAddress("PostBhabhaPelec",&PostBhabhaPelec);
  MCTree2->SetBranchAddress("BhabhaET1",&EBhT1);
  MCTree2->SetBranchAddress("BhabhaET2",&EBhT2);
  MCTree2->SetBranchAddress("BinaE",&EIntoBina);
  // Added by Tristan, Oct. 12/17
  MCTree2->SetBranchAddress("WC3positronX",&WC3positronX);
  */


  /// Additions based on Tristan's work /////
  MCTree2->SetBranchAddress("PosBremPreX",&PosBremPreX);
  MCTree2->SetBranchAddress("PosBremPreP",&PosBremPreP);
  MCTree2->SetBranchAddress("PosBremPostX",&PosBremPostX);
  MCTree2->SetBranchAddress("PosBremPostP",&PosBremPostP);
  MCTree2->SetBranchAddress("ElecBremPreX",&ElecBremPreX);
  MCTree2->SetBranchAddress("ElecBremPreP",&ElecBremPreP);
  MCTree2->SetBranchAddress("ElecBremPostX",&ElecBremPostX);
  MCTree2->SetBranchAddress("ElecBremPostP",&ElecBremPostP);

  MCTree2->SetBranchAddress("PosBhabhaPreX",&PosBhabhaPreX);
  MCTree2->SetBranchAddress("PosBhabhaPreP",&PosBhabhaPreP);
  MCTree2->SetBranchAddress("PosBhabhaPostX",&PosBhabhaPostX);
  MCTree2->SetBranchAddress("PosBhabhaPostP",&PosBhabhaPostP);

  MCTree2->SetBranchAddress("PosAnnihilPreX",&PosAnnihilPreX);
  MCTree2->SetBranchAddress("PosAnnihilPreP",&PosAnnihilPreP);
  MCTree2->SetBranchAddress("PosAnnihilPostX",&PosAnnihilPostX);
  MCTree2->SetBranchAddress("PosAnnihilPostP",&PosAnnihilPostP);

  MCTree2->SetBranchAddress("PosScatterPreX",&PosScatterPreX);
  MCTree2->SetBranchAddress("PosScatterPreP",&PosScatterPreP);
  MCTree2->SetBranchAddress("PosScatterPostX",&PosScatterPostX);
  MCTree2->SetBranchAddress("PosScatterPostP",&PosScatterPostP);
  MCTree2->SetBranchAddress("ElecScatterPreX",&ElecScatterPreX);
  MCTree2->SetBranchAddress("ElecScatterPreP",&ElecScatterPreP);
  MCTree2->SetBranchAddress("ElecScatterPostX",&ElecScatterPostX);
  MCTree2->SetBranchAddress("ElecScatterPostP",&ElecScatterPostP);

  // MCTree2->SetBranchAddress("ElecinWC3PreX",&ElecinWC3PreX);
  // MCTree2->SetBranchAddress("ElecinWC3PreP",&ElecinWC3PreP);
  // MCTree2->SetBranchAddress("PrimPosinWC3PreX",&PrimPosinWC3PreX);
  // MCTree2->SetBranchAddress("PrimPosinWC3PreP",&PrimPosinWC3PreP);
  // MCTree2->SetBranchAddress("SecPosinWC3PreX",&SecPosinWC3PreX);
  // MCTree2->SetBranchAddress("SecPosinWC3PreP",&SecPosinWC3PreP);

  MCTree2->SetBranchAddress("PosTotalBremE", &PosTotalBremE);
  MCTree2->SetBranchAddress("ElecTotalBremE", &ElecTotalBremE);
  MCTree2->SetBranchAddress("PosTotalBhabhaE", &PosTotalBhabhaE);
  ////////////
  
  //Report
  cout << endl;
  cout << "Hits    = " << MCTree1->GetEntries() << endl;
  cout << "Events  = " << MCTree2->GetEntries() << endl;
  cout << endl;

}

void MCtree::SetOutputFile(const char* fname, const char* tname){

  //File
  cout << "Setting Output file: " << fname << endl;
  OutFile->Open(fname,"RECREATE");

  //Tree
  cout << "Created output tree: " << tname << endl;
  OutputTree = new TTree(tname,"");

  OutputTree->Branch("eCsIUSIch",&eCsIUSIch,"eCsIUSIch[21]/F");
  OutputTree->Branch("eCsIUSOch",&eCsIUSOch,"eCsIUSOch[28]/F");
  OutputTree->Branch("eCsIDSIch",&eCsIDSIch,"eCsIDSIch[21]/F");
  OutputTree->Branch("eCsIDSOch",&eCsIDSOch,"eCsIDSOch[27]/F");

  OutputTree->Branch("eCsIUSIchR",&eCsIUSIchR,"eCsIUSIchR[21]/F");
  OutputTree->Branch("eCsIUSOchR",&eCsIUSOchR,"eCsIUSOchR[28]/F");
  OutputTree->Branch("eCsIDSIchR",&eCsIDSIchR,"eCsIDSIchR[21]/F");
  OutputTree->Branch("eCsIDSOchR",&eCsIDSOchR,"eCsIDSOchR[27]/F");

  OutputTree->Branch("CsISum",&csi,"CsI/F");
  OutputTree->Branch("BCsISum",&Bcsi,"BCsI/F");
  OutputTree->Branch("CsIR",&csiR,"CsIR/F");

  OutputTree->Branch("BinaCsI",&binacsi,"BinaCsI/F");
  OutputTree->Branch("BinaCsIR",&binacsiR,"BinaCsIR/F");

  OutputTree->Branch("eBina",&ebina,"eBina[4]/F");
  OutputTree->Branch("eBBina",&eBbina,"eBBina[4]/F");
  OutputTree->Branch("eBinaR",&ebinar,"eBinaR[4]/F");
  OutputTree->Branch("eBinaG",&ebinag,"eBinaG/F");

  OutputTree->Branch("eFront",&efront,"eFront[4]/F");

  OutputTree->Branch("B1.N",&b1_n,"B1.N/I");
  OutputTree->Branch("B1.E",&b1_e,"B1.E[B1.N]/F");
  OutputTree->Branch("B1.t",&b1_t,"B1.t[B1.N]/F");
  OutputTree->Branch("B1.PID",&b1_pid,"B1.PID[B1.N]/F");


  OutputTree->Branch("B2.N",&b2_n,"B2.N/I");
  OutputTree->Branch("B2.E",&b2_e,"B2.E[B2.N]/F");
  OutputTree->Branch("B2.t",&b2_t,"B2.t[B2.N]/F");
  OutputTree->Branch("B2.PID",&b2_pid,"B2.PID[B2.N]/F");


  OutputTree->Branch("TG.N",&tg_n,"TG.N/I");
  OutputTree->Branch("TG.E",&tg_e,"TG.E[TG.N]/F");
  OutputTree->Branch("TG.t",&tg_t,"TG.t[TG.N]/F");
  OutputTree->Branch("TG.PID",&tg_pid,"TG.PID[TG.N]/F");

  OutputTree->Branch("T1.N",&t1_n,"T1.N/I");
  OutputTree->Branch("T1.E",&t1_e,"T1.E[T1.N]/F");
  OutputTree->Branch("T1.t",&t1_t,"T1.t[T1.N]/F");
  OutputTree->Branch("T1.PID",&t1_pid,"T1.PID[T1.N]/F");

  OutputTree->Branch("T2.N",&t2_n,"T2.N/I");
  OutputTree->Branch("T2.E",&t2_e,"T2.E[T2.N]/F");
  OutputTree->Branch("T2.t",&t2_t,"T2.t[T2.N]/F");
  OutputTree->Branch("T2.PID",&t2_pid,"T2.PID[T2.N]/F");

  OutputTree->Branch("V2.N",&v2_n,"V2.N/I");
  OutputTree->Branch("V2.E",&v2_e,"V2.E[V2.N]/F");
  OutputTree->Branch("V2.t",&v2_t,"V2.t[V2.N]/F");
  OutputTree->Branch("V2.PID",&v2_pid,"V2.PID[V2.N]/F");

  OutputTree->Branch("V3.N",&v3_n,"V3.N/I");
  OutputTree->Branch("V3.E",&v3_e,"V3.E[V3.N]/F");
  OutputTree->Branch("V3.t",&v3_t,"V3.t[V3.N]/F");
  OutputTree->Branch("V3.PID",&v3_pid,"V3.PID[V3.N]/F");

  OutputTree->Branch("WC1_1.N",&wc1_1n,"WC1_1.N/I");
  OutputTree->Branch("WC1_1.W",&wc1_1w,"WC1_1.W[WC1_1.N]/I");
  OutputTree->Branch("WC1_1.t",&wc1_1t,"WC1_1.t[WC1_1.N]/F");
  OutputTree->Branch("WC1_1.PID",&wc1_1pid,"WC1_1.PID[WC1_1.N]/F");

  OutputTree->Branch("WC1_2.N",&wc1_2n,"WC1_2.N/I");
  OutputTree->Branch("WC1_2.W",&wc1_2w,"WC1_2.W[WC1_2.N]/I");
  OutputTree->Branch("WC1_2.t",&wc1_2t,"WC1_2.t[WC1_2.N]/F");
  OutputTree->Branch("WC1_2.PID",&wc1_2pid,"WC1_2.PID[WC1_2.N]/F");

  OutputTree->Branch("WC1_3.N",&wc1_3n,"WC1_3.N/I");
  OutputTree->Branch("WC1_3.W",&wc1_3w,"WC1_3.W[WC1_3.N]/I");
  OutputTree->Branch("WC1_3.t",&wc1_3t,"WC1_3.t[WC1_3.N]/F");
  OutputTree->Branch("WC1_3.PID",&wc1_3pid,"WC1_3.PID[WC1_3.N]/F");

  OutputTree->Branch("WC2_1.N",&wc2_1n,"WC2_1.N/I");
  OutputTree->Branch("WC2_1.W",&wc2_1w,"WC2_1.W[WC2_1.N]/I");
  OutputTree->Branch("WC2_1.t",&wc2_1t,"WC2_1.t[WC2_1.N]/F");
  OutputTree->Branch("WC2_1.PID",&wc2_1pid,"WC2_1.PID[WC2_1.N]/F");

  OutputTree->Branch("WC2_2.N",&wc2_2n,"WC2_2.N/I");
  OutputTree->Branch("WC2_2.W",&wc2_2w,"WC2_2.W[WC2_2.N]/I");
  OutputTree->Branch("WC2_2.t",&wc2_2t,"WC2_2.t[WC2_2.N]/F");
  OutputTree->Branch("WC2_2.PID",&wc2_2pid,"WC2_2.PID[WC2_2.N]/F");

  OutputTree->Branch("WC2_3.N",&wc2_3n,"WC2_3.N/I");
  OutputTree->Branch("WC2_3.W",&wc2_3w,"WC2_3.W[WC2_3.N]/I");
  OutputTree->Branch("WC2_3.t",&wc2_3t,"WC2_3.t[WC2_3.N]/F");
  OutputTree->Branch("WC2_3.PID",&wc2_3pid,"WC2_3.PID[WC2_3.N]/F");

  OutputTree->Branch("WC3_1.N",&wc3_1n,"WC3_1.N/I");
  OutputTree->Branch("WC3_1.W",&wc3_1w,"WC3_1.W[WC3_1.N]/I");
  OutputTree->Branch("WC3_1.t",&wc3_1t,"WC3_1.t[WC3_1.N]/F");
  OutputTree->Branch("WC3_1.PID",&wc3_1pid,"WC3_1.PID[WC3_1.N]/F");

  OutputTree->Branch("WC3_2.N",&wc3_2n,"WC3_2.N/I");
  OutputTree->Branch("WC3_2.W",&wc3_2w,"WC3_2.W[WC3_2.N]/I");
  OutputTree->Branch("WC3_2.t",&wc3_2t,"WC3_2.t[WC3_2.N]/F");
  OutputTree->Branch("WC3_2.PID",&wc3_2pid,"WC3_2.PID[WC3_2.N]/F");

  OutputTree->Branch("WC3_3.N",&wc3_3n,"WC3_3.N/I");
  OutputTree->Branch("WC3_3.W",&wc3_3w,"WC3_3.W[WC3_3.N]/I");
  OutputTree->Branch("WC3_3.t",&wc3_3t,"WC3_3.t[WC3_3.N]/F");
  OutputTree->Branch("WC3_3.PID",&wc3_3pid,"WC3_3.PID[WC3_3.N]/F");


  OutputTree->Branch("S1_X.N",&ss1_1n,"S1_X.N/I");
  OutputTree->Branch("S1_X.ch",&ss1_1w,"S1_X.ch[S1_X.N]/F");
  OutputTree->Branch("S1_X.x",&ss1_1x,"S1_X.x[S1_X.N]/F");
  OutputTree->Branch("S1_X.t",&ss1_1t,"S1_X.t[S1_X.N]/F");
  OutputTree->Branch("S1_X.PID",&ss1_1pid,"S1_X.PID[S1_X.N]/F");
  OutputTree->Branch("S1_X.E",&ss1_1e,"S1_X.E[S1_X.N]/F");

  OutputTree->Branch("S1_Y.N",&ss1_2n,"S1_Y.N/I");
  OutputTree->Branch("S1_Y.ch",&ss1_2w,"S1_Y.ch[S1_Y.N]/F");
  OutputTree->Branch("S1_Y.y",&ss1_2y,"S1_Y.y[S1_Y.N]/F");
  OutputTree->Branch("S1_Y.t",&ss1_2t,"S1_Y.t[S1_Y.N]/F");
  OutputTree->Branch("S1_Y.PID",&ss1_2pid,"S1_Y.PID[S1_Y.N]/F");
  OutputTree->Branch("S1_Y.E",&ss1_2e,"S1_Y.E[S1_Y.N]/F");

  OutputTree->Branch("S2_X.N",&ss2_1n,"S2_X.N/I");
  OutputTree->Branch("S2_X.ch",&ss2_1w,"S2_X.ch[S2_X.N]/F");
  OutputTree->Branch("S2_X.x",&ss2_1x,"S2_X.x[S2_X.N]/F");
  OutputTree->Branch("S2_X.t",&ss2_1t,"S2_X.t[S2_X.N]/F");
  OutputTree->Branch("S2_X.PID",&ss2_1pid,"S2_X.PID[S2_X.N]/F");
  OutputTree->Branch("S2_X.E",&ss2_1e,"S2_X.E[S2_X.N]/F");

  OutputTree->Branch("S2_Y.N",&ss2_2n,"S2_Y.N/I");
  OutputTree->Branch("S2_Y.ch",&ss2_2w,"S2_Y.ch[S2_Y.N]/F");
  OutputTree->Branch("S2_Y.y",&ss2_2y,"S2_Y.y[S2_Y.N]/F");
  OutputTree->Branch("S2_Y.t",&ss2_2t,"S2_Y.t[S2_Y.N]/F");
  OutputTree->Branch("S2_Y.PID",&ss2_2pid,"S2_Y.PID[S2_Y.N]/F");
  OutputTree->Branch("S2_Y.E",&ss2_2e,"S2_Y.E[S2_Y.N]/F");

  OutputTree->Branch("S3_X.N",&ss3_1n,"S3_X.N/I");
  OutputTree->Branch("S3_X.ch",&ss3_1w,"S3_X.ch[S3_X.N]/F");
  OutputTree->Branch("S3_X.x",&ss3_1x,"S3_X.x[S3_X.N]/F");
  OutputTree->Branch("S3_X.t",&ss3_1t,"S3_X.t[S3_X.N]/F");
  OutputTree->Branch("S3_X.PID",&ss3_1pid,"S3_X.PID[S3_X.N]/F");
  OutputTree->Branch("S3_X.E",&ss3_1e,"S3_X.E[S3_X.N]/F");

  OutputTree->Branch("S3_X_Nst",&nstrips_x,"S3_X_Nst/I");
  OutputTree->Branch("S3_X_amp.N",&Nampx,"S3_X_amp.N/I");
  OutputTree->Branch("S3_X.amp",&ampx,"S3_X.amp[S3_X_amp.N]/F");
  OutputTree->Branch("S3_X.tamp",&ampxt,"S3_X.tamp[S3_X_amp.N]/F");
  OutputTree->Branch("S3_X.tcl",&tcl_x,"S3_X.tcl[S3_X_amp.N]/F");
  OutputTree->Branch("S3_X.ampn",&ampn_x,"S3_X.ampn[S3_X_amp.N]/I");
  OutputTree->Branch("S3_X_Nclustx",&Nclustx,"S3_X_Nclustx/I");
  OutputTree->Branch("S3_X.posi",&s3_x_posi,"S3_X.posi[S3_X_Nclustx]/F");
  OutputTree->Branch("aQR.x",&aqr_x,"aQR.x[S3_X_Nclustx]/F");
  OutputTree->Branch("Namp.x",&namp_x,"Namp.x[S3_X_Nclustx]/F");
 
  OutputTree->Branch("S3_Y.N",&ss3_2n,"S3_Y.N/I");
  OutputTree->Branch("S3_Y.ch",&ss3_2w,"S3_Y.ch[S3_Y.N]/F");
  OutputTree->Branch("S3_Y.y",&ss3_2y,"S3_Y.y[S3_Y.N]/F");
  OutputTree->Branch("S3_Y.t",&ss3_2t,"S3_Y.t[S3_Y.N]/F");
  OutputTree->Branch("S3_Y.PID",&ss3_2pid,"S3_Y.PID[S3_Y.N]/F");
  OutputTree->Branch("S3_Y.E",&ss3_2e,"S3_Y.E[S3_Y.N]/F");

  OutputTree->Branch("S3_Y_Nst",&nstrips_y,"S3_Y_Nst/I");
  OutputTree->Branch("S3_Y_amp.N",&Nampy,"S3_Y_amp.N/I");
  OutputTree->Branch("S3_Y.amp",&ampy,"S3_Y.amp[S3_Y_amp.N]/F");
  OutputTree->Branch("S3_Y.tamp",&ampyt,"S3_Y.tamp[S3_Y_amp.N]/F");
  OutputTree->Branch("S3_Y.tcl",&tcl_y,"S3_Y.tcl[S3_Y_amp.N]/F");
  OutputTree->Branch("S3_Y.ampn",&ampn_y,"S3_Y.ampn[S3_Y_amp.N]/I");
  OutputTree->Branch("S3_Y_Nclustx",&Nclusty,"S3_Y_Nclusty/I");
  OutputTree->Branch("S3_Y.posi",&s3_y_posi,"S3_Y.posi[S3_Y_Nclusty]/F");
  OutputTree->Branch("aQR.y",&aqr_y,"aQR.y[S3_Y_Nclusty]/F");
  OutputTree->Branch("Namp.y",&namp_y,"Namp.y[S3_Y_Nclusty]/F");

  OutputTree->Branch("eB1",&eb1,"eB1[4]/F");
  OutputTree->Branch("eBB1",&eBb1,"eBB1[4]/F");
  OutputTree->Branch("eB2",&eb2,"eB2[4]/F");
  OutputTree->Branch("eBB2",&eBb2,"eBB2[4]/F");
  OutputTree->Branch("eTG",&etg,"eTG[4]/F");
  OutputTree->Branch("eBTG",&eBtg,"eBTG[4]/F");
  OutputTree->Branch("eT1",&et1,"eT1[4]/F");
  OutputTree->Branch("eBT1",&eBt1,"eBT1[4]/F");
  OutputTree->Branch("eT2",&et2,"eT2[4]/F");
  OutputTree->Branch("eBT2",&eBt2,"eBT2[4]/F");

  OutputTree->Branch("eV2",&ev2,"eV2[4]/F");
  OutputTree->Branch("eBV2",&eBv2,"eBV2[4]/F");
  OutputTree->Branch("eV3",&ev3,"eV3[4]/F");
  OutputTree->Branch("eBV3",&eBv3,"eBV3[4]/F");

  OutputTree->Branch("eB1r",&eb1r,"eB1r[4]/F");
  OutputTree->Branch("eB2r",&eb2r,"eB2r[4]/F");
  OutputTree->Branch("eTGr",&etgr,"eTGr[4]/F");
  OutputTree->Branch("eT1r",&et1r,"eT1r[4]/F");
  OutputTree->Branch("eT2r",&et2r,"eT2r[4]/F");
  OutputTree->Branch("eT2_trig",&et2_trig,"eT2_trig/F");
  OutputTree->Branch("eV2r",&ev2r,"eV2r[4]/F");
  OutputTree->Branch("eV3r",&ev3r,"eV3r[4]/F");


  OutputTree->Branch("eSs1_1",&ess1_1,"eSs1_1[4]/F");
  OutputTree->Branch("eSs1_2",&ess1_2,"eSs1_2[4]/F");
  OutputTree->Branch("eSs2_1",&ess2_1,"eSs2_1[4]/F");
  OutputTree->Branch("eSs2_2",&ess2_2,"eSs2_2[4]/F");
  OutputTree->Branch("eSs3_1",&ess3_1,"eSs3_1[4]/F");
  OutputTree->Branch("eSs3_2",&ess3_2,"eSs3_2[4]/F");

  OutputTree->Branch("eSs1_1r",&ess1_1r,"eSs1_1r[4]/F");
  OutputTree->Branch("eSs1_2r",&ess1_2r,"eSs1_2r[4]/F");
  OutputTree->Branch("eSs2_1r",&ess2_1r,"eSs2_1r[4]/F");
  OutputTree->Branch("eSs2_2r",&ess2_2r,"eSs2_2r[4]/F");
  OutputTree->Branch("eSs3_1r",&ess3_1r,"eSs3_1r[4]/F");
  OutputTree->Branch("eSs3_2r",&ess3_2r,"eSs3_2r[4]/F");

  // Tristan Sep. 22/17
  OutputTree->Branch("eWC3_1",&ewc3_1,"eWC3_1/F");
  OutputTree->Branch("eWC3_2",&ewc3_2,"eWC3_2/F");
  OutputTree->Branch("eWC3_3",&ewc3_3,"eWC3_3/F");

  OutputTree->Branch("xBina",&xbina,"xBina[4]/F");
  OutputTree->Branch("xB1",&xb1,"xB1[4]/F");
  OutputTree->Branch("xB2",&xb2,"xB2[4]/F");
  OutputTree->Branch("xTG",&xtg,"xTG[4]/F");
  OutputTree->Branch("xT1",&xt1,"xT1[4]/F");
  OutputTree->Branch("xT2",&xt2,"xT2[4]/F");
  OutputTree->Branch("xV2",&xv2,"xV2[4]/F");
  OutputTree->Branch("xV3",&xv3,"xV3[4]/F");

  OutputTree->Branch("xSs1_1",&xss1_1,"xSs1_1[4]/F");
  OutputTree->Branch("xSs1_2",&xss1_2,"xSs1_2[4]/F");
  OutputTree->Branch("xSs2_1",&xss2_1,"xSs2_1[4]/F");
  OutputTree->Branch("xSs2_2",&xss2_2,"xSs2_2[4]/F");
  OutputTree->Branch("xSs3_1",&xss3_1,"xSs3_1[4]/F");
  OutputTree->Branch("xSs3_2",&xss3_2,"xSs3_2[4]/F");

  OutputTree->Branch("xWC1_1",&xwc1_1,"xWC1_1[4]/F");
  OutputTree->Branch("xWC1_2",&xwc1_2,"xWC1_2[4]/F");
  OutputTree->Branch("xWC1_3",&xwc1_3,"xWC1_3[4]/F");

  OutputTree->Branch("xWC2_1",&xwc2_1,"xWC2_1[4]/F");
  OutputTree->Branch("xWC2_2",&xwc2_2,"xWC2_2[4]/F");
  OutputTree->Branch("xWC2_3",&xwc2_3,"xWC2_3[4]/F");

  OutputTree->Branch("xWC3_1",&xwc3_1,"xWC3_1[4]/F");
  OutputTree->Branch("xWC3_2",&xwc3_2,"xWC3_2[4]/F");
  OutputTree->Branch("xWC3_3",&xwc3_3,"xWC3_3[4]/F");

  OutputTree->Branch("TGStartZp",&tgstartzp,"TGStartZp/F");
  OutputTree->Branch("TGStartZe",&tgstartze,"TGStartZe/F");
  OutputTree->Branch("TGStartZm",&tgstartzm,"TGStartZm/F");

  OutputTree->Branch("TGStopZp",&tgstopzp,"TGStopZp/F");
  OutputTree->Branch("TGStopZe",&tgstopze,"TGStopZe/F");
  OutputTree->Branch("TGStopZm",&tgstopzm,"TGStopZm/F");
 
  //CsI Partial Sums
  OutputTree->Branch("CsIUSI",&usi,"CsIUSI/F");
  OutputTree->Branch("CsIUSO",&uso,"CsIUSO/F");
  OutputTree->Branch("CsIDSI",&dsi,"CsIDSI/F");
  OutputTree->Branch("CsIDSO",&dso,"CsIDSO/F");

  OutputTree->Branch("CsIUSIr",&usiR,"CsIUSIr/F");
  OutputTree->Branch("CsIUSOr",&usoR,"CsIUSOr/F");
  OutputTree->Branch("CsIDSIr",&dsiR,"CsIDSIr/F");
  OutputTree->Branch("CsIDSOr",&dsoR,"CsIDSOr/F");

  OutputTree->Branch("CsIUSIt",&usit,"CsIUSIt/F"); //REM
  OutputTree->Branch("CsIUSOt",&usot,"CsIUSOt/F"); //REM
  OutputTree->Branch("CsIDSIt",&dsit,"CsIDSIt/F"); //REM
  OutputTree->Branch("CsIDSOt",&dsot,"CsIDSOt/F"); //REM

  OutputTree->Branch("CsIUSIb",&usiB,"CsIUSIb/F");
  OutputTree->Branch("CsIUSOb",&usoB,"CsIUSOb/F");
  OutputTree->Branch("CsIDSIb",&dsiB,"CsIDSIb/F");
  OutputTree->Branch("CsIDSOb",&dsoB,"CsIDSOb/F");

  //Hit XY positions
  OutputTree->Branch("startX",&startX,"startX/F");
  OutputTree->Branch("startY",&startY,"startY/F");
  OutputTree->Branch("R",&R,"R/F");

  //Hit Times
  OutputTree->Branch("startT",&startT,"startT/F");
  OutputTree->Branch("stopT",&stopT,"stopT/F");


  OutputTree->Branch("PosTime",&postime,"PosTime/F");
  OutputTree->Branch("PiTime",&pitime,"PiTime/F");

  //Decay Time
  OutputTree->Branch("Tpos",&Tpos,"Tpos/F");
  OutputTree->Branch("PosMin",&posmin,"PosMin/F");//shintaro
  
  //Pion momentum-energy
  OutputTree->Branch("PiDecayP",&pidecayp,"PiDecayP[4]/D");
  OutputTree->Branch("PiStartP",&pistartp,"PiStartP[4]/D");  
  OutputTree->Branch("Kink",&kv,"Kink/F");
  OutputTree->Branch("Zv",&zv,"Zv/F");
  
  //Pion position-time
  OutputTree->Branch("PiStartX",&pistartx,"PiStartX[4]/D");
  OutputTree->Branch("PiDecayX",&pidecayx,"PiDecayX[4]/D");
  
  //Muon momentum-energy
  OutputTree->Branch("MuStartP",&mustartp,"MuStartP[4]/D");   
  OutputTree->Branch("MuDecayP",&mudecayp,"MuDecayP[4]/D");
  
  //Muon position-time
  OutputTree->Branch("MuStartX",&mustartx,"MuStartX[4]/D");
  OutputTree->Branch("MuDecayX",&mudecayx,"MuDecayX[4]/D");
  
  //Positron momentum-energy
  OutputTree->Branch("PoStartP",&postartp,"PoStartP[4]/D");
  OutputTree->Branch("PoStopP",&postopp,"PoStopP[4]/D");  
  OutputTree->Branch("GStartP",&gstartp,"GStartP[4]/D");
  OutputTree->Branch("GStopP",&gstopp,"GStopP[4]/D");
  
  //Positron position-time
  OutputTree->Branch("PoStartX",&postartx,"PoStartX[4]/D");
  OutputTree->Branch("PoStopX",&postopx,"PoStopX[4]/D");
  OutputTree->Branch("GStartX",&gstartx,"GStartX[4]/D");
  OutputTree->Branch("GStopX",&gstopx,"GStopX[4]/D");
  
  // Tristan, Oct. 12/17
  OutputTree->Branch("WC3positronX",&wc3positronx,"WC3positronX[4]/D");

  // Tristan, Sep. 29/17
  OutputTree->Branch("Xwc3_1",&Xwc3_1,"Xwc3_2/F");

  OutputTree->Branch("Xwc3_2",&Xwc3_2,"Xwc3_2/F");
  OutputTree->Branch("Ywc3_2",&Ywc3_2,"Ywc3_2/F");
  OutputTree->Branch("Rwc3_2",&Rwc3_2,"Rwc3_2/F");
  OutputTree->Branch("Old_Rwc3_2",&Old_Rwc3_2,"Old_Rwc3_2/F");//shintaro
  OutputTree->Branch("Zwc3_2",&Zwc3_2,"Zwc3_2/F");
  
  // Tristan, June 26/17
  OutputTree->Branch("Rwc3_2_besttrack",&Rwc3_2_besttrack,"Rwc3_2_besttrack/F");
  OutputTree->Branch("Rwc3_2_maxrad",&Rwc3_2_maxrad,"Rwc3_2_maxrad/F");

  //Location of the photonuclear reactions with neutron emission
  OutputTree->Branch("PhotonuclearX",&photonuclearx,"PhotonuclearX[3]/D");
  
  //Energy of the positrons that make Bhabha
  OutputTree->Branch("EBh",&ebh,"EBh/D");

  // Added by Tristan, Jan. 28/15, Bhabha stuff
  OutputTree->Branch("PreBhabhaPpos",&prebhabhappos,"PreBhabhaPpos[4]/D");
  OutputTree->Branch("PostBhabhaPpos",&postbhabhappos,"PostBhabhaPpos[4]/D");  
  OutputTree->Branch("PostBhabhaPelec",&postbhabhapelec,"PostBhabhaPelec[4]/D");
  OutputTree->Branch("EBhT1",&ebht1,"EBhT1/D");
  OutputTree->Branch("EBhT2",&ebht2,"EBhT2/D");

  // Tristan Feb. 4/15 (Bhabha stuff)  
  OutputTree->Branch("EBina",&eintobina,"EBina/D");


  /// Additions based on Tristan's work /////
  // Bremsstrahlung and scattering diagnostics
  OutputTree->Branch("PosBremPreX",&posbremprex,"PosBremPreX[4]/D");
  OutputTree->Branch("PosBremPreP",&posbremprep,"PosBremPreP[4]/D");
  OutputTree->Branch("PosBremPostX",&posbrempostx,"PosBremPostX[4]/D");
  OutputTree->Branch("PosBremPostP",&posbrempostp,"PosBremPostP[4]/D");
  OutputTree->Branch("ElecBremPreX",&elecbremprex,"ElecBremPreX[4]/D");
  OutputTree->Branch("ElecBremPreP",&elecbremprep,"ElecBremPreP[4]/D");
  OutputTree->Branch("ElecBremPostX",&elecbrempostx,"ElecBremPostX[4]/D");
  OutputTree->Branch("ElecBremPostP",&elecbrempostp,"ElecBremPostP[4]/D");

  OutputTree->Branch("PosBhabhaPreX",&posbhabhaprex,"PosBhabhaPreX[4]/D");
  OutputTree->Branch("PosBhabhaPreP",&posbhabhaprep,"PosBhabhaPreP[4]/D");
  OutputTree->Branch("PosBhabhaPostX",&posbhabhapostx,"PosBhabhaPostX[4]/D");
  OutputTree->Branch("PosBhabhaPostP",&posbhabhapostp,"PosBhabhaPostP[4]/D");

  OutputTree->Branch("PosAnnihilPreX",&posannihilprex,"PosAnnihilPreX[4]/D");
  OutputTree->Branch("PosAnnihilPreP",&posannihilprep,"PosAnnihilPreP[4]/D");
  OutputTree->Branch("PosAnnihilPostX",&posannihilpostx,"PosAnnihilPostX[4]/D");
  OutputTree->Branch("PosAnnihilPostP",&posannihilpostp,"PosAnnihilPostP[4]/D");

  OutputTree->Branch("PosScatterPreX",&posscatterprex,"PosScatterPreX[4]/D");
  OutputTree->Branch("PosScatterPreP",&posscatterprep,"PosScatterPreP[4]/D");
  OutputTree->Branch("PosScatterPostX",&posscatterpostx,"PosScatterPostX[4]/D");
  OutputTree->Branch("PosScatterPostP",&posscatterpostp,"PosScatterPostP[4]/D");
  OutputTree->Branch("ElecScatterPreX",&elecscatterprex,"ElecScatterPreX[4]/D");
  OutputTree->Branch("ElecScatterPreP",&elecscatterprep,"ElecScatterPreP[4]/D");
  OutputTree->Branch("ElecScatterPostX",&elecscatterpostx,"ElecScatterPostX[4]/D");
  OutputTree->Branch("ElecScatterPostP",&elecscatterpostp,"ElecScatterPostP[4]/D");

  // OutputTree->Branch("ElecinWC3PreX",&elecinwc3prex,"ElecinWC3PreX[4]/D");
  // OutputTree->Branch("ElecinWC3PreP",&elecinwc3prep,"ElecinWC3PreP[4]/D");
  // OutputTree->Branch("PrimPosinWC3PreX",&primposinwc3prex,"PrimPosinWC3PreX[4]/D");
  // OutputTree->Branch("PrimPosinWC3PreP",&primposinwc3prep,"PrimPosinWC3PreP[4]/D");
  // OutputTree->Branch("SecPosinWC3PreX",&secposinwc3prex,"SecPosinWC3PreX[4]/D");
  // OutputTree->Branch("SecPosinWC3PreP",&secposinwc3prep,"SecPosinWC3PreP[4]/D");

  OutputTree->Branch("PosTotalBremE",&postotalbreme,"PosTotalBremE/D");
  OutputTree->Branch("ElecTotalBremE",&electotalbreme,"ElecTotalBremE/D");
  OutputTree->Branch("PosTotalBhabhaE",&postotalbhabhae,"PosTotalBhabhaE/D");
  //////////

  
  OutputTree->Branch("MuPolarization",&mupolarization,"MuPolarization[3]/D");
  OutputTree->Branch("EMomentum",&emomentum,"EMomentum[3]/D");

  //For massive neutrino search
  OutputTree->Branch("csich",&csich,"csich/F");
  OutputTree->Branch("phie",&phie,"phie/F");
  OutputTree->Branch("thetae",&thetae,"thetae/F");
  OutputTree->Branch("dphi",&dphi,"dphi/F");
  OutputTree->Branch("usiZ",&usiZ,"usiZ/F");

  OutputTree->Branch("EventID",&eventnumber,"eventID/I");

  // Tristan, June 15/17
  OutputTree->Branch("Ntrackswc3",&ntracks,"Ntrackswc3/I");

    //Bina hits 
  OutputTree->Branch("BinaHits",&binahits,"BinaHits[100]/D");
  OutputTree->Branch("BirksHits",&birkshits,"BirksHits[100]/D");
  OutputTree->Branch("BinaTimes",&binatimes,"BinaTimes[100]/D");
  OutputTree->Branch("BinaPH",&binaph,"BinaPH/D");
  OutputTree->Branch("BinaQ",&binaq,"BinaQ/D");
  OutputTree->Branch("BirksPH",&birksph,"BirksPH/D");
  OutputTree->Branch("BirksQ",&birksq,"BirksQ/D");
  OutputTree->Branch("BirksPHR",&birksphr,"BirksPHR/D");
  OutputTree->Branch("BirksQR",&birksqr,"BirksQR/D");
  OutputTree->Branch("Numhits",&numhits,"Numhits/I");


 //CsI hits 
  OutputTree->Branch("CsIHits",&csihits,"CsIHits[97]/D");

  // Changed by Tristan, Mar. 6/17, for CsI late hits study
  OutputTree->Branch("CsITimes",&csitimes,"CsITimes[1000]/D");

  // Tristan, July 20/17
  OutputTree->Branch("S12X0",&s12x0,"S12X0/D");
  OutputTree->Branch("S12Y0",&s12y0,"S12Y0/D");
  OutputTree->Branch("S12TX",&s12tx,"S12TX/D");
  OutputTree->Branch("S12TY",&s12ty,"S12TY/D");

  OutputTree->Branch("XWC12",&Xwc12,"XWC12/F");
  OutputTree->Branch("YWC12",&Ywc12,"YWC12/F");
  OutputTree->Branch("TXWC12",&txwc12,"TXWC12/F");
  OutputTree->Branch("TYWC12",&tywc12,"TYWC12/F");

  OutputTree->Branch("S3WC3X0",&s3wc3x0,"S3WC3/D");
  OutputTree->Branch("S3WC3Y0",&s3wc3y0,"S3WC3/D");
  OutputTree->Branch("S3WC3TX",&s3wc3tx,"S3WC3/D");
  OutputTree->Branch("S3WC3TY",&s3wc3ty,"S3WC3/D");

  // Tracker OBJECTS
  TrReco.setBranches(OutputTree);

}


void MCtree::SaveOutputTree()
{
  OutputTree->Write();
}


//Main loop over the MC events
void MCtree::Loop()
{
  int hitwc1_1 = 0;
  int hitwc1_2 = 0;
  int hitwc1_3 = 0;
  int hitwc2_1 = 0;
  int hitwc2_2 = 0;
  int hitwc2_3 = 0;

  int hitwc3_1 = 0;
  int hitwc3_2 = 0;
  int hitwc3_3 = 0;


  int hitss1_1 = 0;
  int hitss1_2 = 0;
  int hitss2_1 = 0;
  int hitss2_2 = 0;
  int hitss3_1 = 0;
  int hitss3_2 = 0;

  int hitb1 = 0;
  int hitb2 = 0;
  int hittg = 0;
  int hitt1 = 0;
  int hitt2 = 0;
  int hitv2 = 0;
  int hitv3 = 0;
  
  Tpos = -10000;

  postime=10000,pitime=10000;
  posmin=10000;//shintaro
  
  
  Long64_t hit = 0;
   for (Long64_t entry=0; entry<MCTree2->GetEntries();entry++) {
     //      for (Long64_t entry=0; entry<100000;entry++) {
    
    MCTree1->GetEntry(hit);
    MCTree2->GetEntry(entry);
    
    //Clear the variables
    this->Clear();
    
  int HIT = 0;

    double v[4]={0,0,0,0};
    while (eventID2==eventID){

      if (energyDeposit>0){
	
	
	if (volumeID0==5 && volumeID1==175 && volumeID2==175)
	  {
	    efront[0]+=energyDeposit;
	    if(PID == 211) efront[1] += energyDeposit;
	    if(PID == -13) efront[2] += energyDeposit;
	    if(PID == -11) efront[3] += energyDeposit;	 
	  }
	
	//Bina
	if (volumeID0==5 && volumeID2==100) 
	  { 
	    ebina[0] += energyDeposit;
	    eBbina[0] += Ebirk;

	                HIT++;

            //Try to sum hits only if they are close in time and build an array of bina summed hits
            if (HIT==1) {
              binahits[0] = energyDeposit;
              birkshits[0] = Ebirk;
              binatimes[0] = StartT;
            }
            if (HIT>1){

              for (int i=0;i<100;i++){

                if (fabs(StartT-binatimes[i])<2 && binatimes[i]!=-10000) break;
                if (fabs(StartT-binatimes[i])>2 && binatimes[i]==-10000)  {binatimes[i]=StartT;break;}

              }
                

              for (int i=0;i<100;i++){
                if (fabs(StartT-binatimes[i])<2 && binahits[i]!=-10000)   {binahits[i]+=energyDeposit; birkshits[i]+=Ebirk; break;}
                if (fabs(StartT-binatimes[i])<2 && binahits[i]==-10000)  { binahits[i]=energyDeposit; birkshits[i]=Ebirk;}

              }
	    }

	    xbina[0] = StartX;
	    xbina[1] = StartY;
	    xbina[2] = StartZ;
	    xbina[3] = StartT;
	    
	    if(PID == 211){ 
	      ebina[1] += energyDeposit;
	      eBbina[1] += Ebirk;
	    }
	    
	    if(PID == -13){
	      ebina[2] += energyDeposit;
	      eBbina[2] += Ebirk;
	    }		
	    
	    if(PID == -11) {
	      ebina[3] += energyDeposit;	    
	      eBbina[3] += Ebirk;
	    }

	    	    if(PID == 22) {
	      ebinag += energyDeposit;	    
	    }

	  }
	
	
	
	//CsI
	if (volumeID0==5 && volumeID2<100 /*&& stopT<100*/) 
	  {
	    // Added by Tristan (Oct. 29/14) to test equivalence between this and sum of quadrants
	    //csi  += energyDeposit;
	   csi  += energyDeposit;
	   // Bcsi  += Ebirk;
	  }
	  
	if (volumeID0==5 && volumeID2<100){
	  // Timing cut
	  //USIn
	  for (int i=0;i<21;i++)
	    if (volumeID2==0 && volumeID1==i) { if(StartT - PoStartX[3] < 50.0) {csiall->USIn[i] += energyDeposit;v[0]++; usiZ+=StartZ; usiZn++;usit = StartT;csitimes[csinumhits] = StopT; csinumhits++;}} //REM
	  //USOut
	  for (int i=0;i<28;i++)
	    if (volumeID2==1 && volumeID1==i) {if(StartT - PoStartX[3] < 50.0) {csiall->USOut[i] += energyDeposit;v[1]++; usot = StartT;csitimes[csinumhits] = StopT; csinumhits++;}} //REM
	  //DSIn
	  for (int i=0;i<21;i++)
	    if (volumeID2==2 && volumeID1==i) {if(StartT - PoStartX[3] < 50.0) {csiall->DSIn[i] += energyDeposit;v[2]++; dsit = StartT;csitimes[csinumhits] = StopT; csinumhits++;}} //REM
	  //DSOut
	  for (int i=0;i<27;i++)
	    if (volumeID2==3 && volumeID1==i) {if(StartT - PoStartX[3] < 50.0) {csiall->DSOut[i] += energyDeposit;v[3]++; dsot = StartT;csitimes[csinumhits] = StopT; csinumhits++;}} //REM

	  // No timing cut
	  //USIn
	  /*for (int i=0;i<21;i++)
	    if (volumeID2==0 && volumeID1==i) { if(energyDeposit>2.0) {csiall->USIn[i] = energyDeposit;v[0]++;  usi += energyDeposit; usiB += Ebirk; usiZ+=StartZ; usiZn++;}}
	  //USOut
	  for (int i=0;i<28;i++)
	    if (volumeID2==1 && volumeID1==i) {if(energyDeposit>2.0) {csiall->USOut[i] = energyDeposit;v[1]++; uso += energyDeposit; usoB += Ebirk;}}
	  //DSIn
	  for (int i=0;i<21;i++)
	    if (volumeID2==2 && volumeID1==i) {if(energyDeposit>2.0) {csiall->DSIn[i] = energyDeposit;v[2]++;  dsi += energyDeposit; dsiB += Ebirk;}}
	  //DSOut
	  for (int i=0;i<27;i++)
	    if (volumeID2==3 && volumeID1==i) {if(energyDeposit>2.0) {csiall->DSOut[i] = energyDeposit;v[3]++; dso += energyDeposit; dsoB += Ebirk;}}

	  for (int i = 0; i < 21; i++) eCsIUSIch[i] = csiall->USIn[i];
	  for (int i = 0; i < 28; i++) eCsIUSOch[i] = csiall->USOut[i];
	  for (int i = 0; i < 21; i++) eCsIDSIch[i] = csiall->DSIn[i];
	  for (int i = 0; i < 27; i++) eCsIDSOch[i] = csiall->DSOut[i];*/
	}
	
	
	//B1
	if (volumeID0==1 && volumeID1==1 && StartT>-8000) 
	  {
	    if (StartT < 35)
	      {
	    	eb1[0] += energyDeposit;
	    	eBb1[0] += Ebirk;
	      }
	    
	    xb1[0] = StartX;
	    xb1[1] = StartY;
	    xb1[2] = StartZ;
	    xb1[3] = StartT;
	    
	    
	    b1_e[hitb1] = energyDeposit;
	    //Bb1_e[hitb1]=Ebirk;
	    
            b1_t[hitb1] = StartT;
	    if(PID>1000) b1_pid[hitb1] = 0; else   b1_pid[hitb1] = (PID);
	    
	    //Approximate time variable (for quick studies)
	      if ((StartT-3.965)<0.2 && (StartT-3.965)>-0.2) pitime = StartT;
	      //  pitime = StartT; // remove cut for mue version
	    if (PID == 211)
	      {
		eb1[1] +=energyDeposit;
		eBb1[1] +=Ebirk;
	      }
	    else if (PID == -13)
	      {
		eb1[2] +=energyDeposit;
		eBb1[2] +=Ebirk;
	      }
	    else if (PID == -11)
	      {		
		eb1[3] +=energyDeposit;
		eBb1[3] +=Ebirk;
	      }	    
	    hitb1++;
	  }
	
	//B2
	if (volumeID0==1 && volumeID1==2 && StartT>-8000)
	  {
	    if (StartT < 35)
	      {
	    	eb2[0]  += energyDeposit;
	    	eBb2[0] += Ebirk;
	      }
	    
	    xb2[0] = StartX;
	    xb2[1] = StartY;
	    xb2[2] = StartZ;
	    xb2[3] = StartT;
	    
	    b2_e[hitb2] = energyDeposit;
            b2_t[hitb2] = StartT;
	    if(PID>1000) b2_pid[hitb2] = 0; else b2_pid[hitb2] = PID;
	    hitb2++;
	    
	    
	    if (PID == 211)
	      {
		eb2[1]  += energyDeposit;
		eBb2[1] += Ebirk;
	      }
	    else if (PID == -13)
	      {
		eb2[2]  += energyDeposit;
		eBb2[2]  += Ebirk;
	      }
	    else if (PID == -11)
	      {
		eb2[3]  += energyDeposit;
		eBb2[3]  += Ebirk;
	      }
	  }
	
	//TG
	if (volumeID0==1 && volumeID1==3 && StartT>-8000) 
	  {
	    etg[0] += energyDeposit;
	    eBtg[0] += Ebirk;
	    
	    xtg[0] = StartX;
	    xtg[1] = StartY;
	    xtg[2] = StartZ;
	    xtg[3] = StartT;

	    tg_e[hittg] = energyDeposit;
            tg_t[hittg] = StartT;

	    if(PID>1000) tg_pid[hittg] = 0; else tg_pid[hittg] = PID;
	    hittg++;
	    if (PID == 211) 
	      {
		etg[1] += energyDeposit;
		eBtg[1] += Ebirk;
		
		tgstartzp = StartZ;
		tgstopzp =  StopZ;
	      }
	    else if (PID == -13) 
	      {
		etg[2] += energyDeposit;
		eBtg[2] += Ebirk;
		
		tgstartzm = StartZ;
		tgstopzm =  StopZ;
	      }
	    else if (PID == -11)
	      {
		etg[3] += energyDeposit;
		eBtg[3] += Ebirk;
	
		tgstartze = StartZ;
		tgstopze =  StopZ;	
	      }
	  }
	
	//T1
	if (volumeID0==1 && volumeID1==4 && StartT>-8000){
	  
	  et1[0]  +=energyDeposit;
	  eBt1[0] += Ebirk;
	  
	  xt1[0] = StartX;
	  xt1[1] = StartY;
	  xt1[2] = StartZ;
	  xt1[3] = StartT;
	  
	  // Approximate time variable (for quick studies)
	   if(StartT>0 && StartT<postime) postime = StartT;
	   // postime = StartT; // fix for mue files
 	  //shintaro
 	  if(posmin>StartT){
 	    posmin=StartT;
 	  }
	  if (PID==-11)
	    //4.29 is the prompt in T1
	    t1_e[hitt1] = energyDeposit;
	  
	  t1_t[hitt1] = StartT;
	  
	  if(PID>1000) t1_pid[hitt1] = 0; else t1_pid[hitt1] = PID;
	  hitt1++;
	  if (PID == 211)
	    {
	      et1[1] += energyDeposit;
	      eBt1[1] += Ebirk;
	    }
	  else if (PID == -13)
	    {
	      et1[2] += energyDeposit;
	      eBt1[2] += Ebirk;
	    }
	  else if (PID == -11)
	    {
	      et1[3] += energyDeposit;
	      eBt1[3] += Ebirk;	      
	    }
	  
	}
	
	//T2
	if (volumeID0==1 && volumeID1==5) 
	  {
	    et2[0] +=energyDeposit;
	    eBt2[0] += Ebirk;
	  
	    xt2[0] = StartX;
	    xt2[1] = StartY;
	    xt2[2] = StartZ;
	    xt2[3] = StartT;

	    startX = StartX;
	    startY = StartY;
	    R = sqrt(startX*startX + startY*startY);
	    startT = StartT;
	    stopT  = StopT;

	    t2_e[hitt2] = energyDeposit;
            t2_t[hitt2] = StartT;

	    if(PID>1000) t2_pid[hitt2] = 0; else t2_pid[hitt2] = PID;
	    hitt2++;
	    
	    if (PID == 211)
	      {
		et2[1] +=energyDeposit;
		eBt2[1] += Ebirk;
	      }
	    else if (PID == -13)
	      {
		et2[2] +=energyDeposit;
		eBt2[2] += Ebirk;
		 }
	    else if (PID == -11) 
	      {
		et2[3] +=energyDeposit;
		eBt2[3] += Ebirk;
	      }
	  }



	//V2
	if (volumeID0==1 && volumeID1==6) 
	  {
	    ev2[0] +=energyDeposit;
	    eBv2[0] += Ebirk;
	  
	    xv2[0] = StartX;
	    xv2[1] = StartY;
	    xv2[2] = StartZ;
	    xv2[3] = StartT;

	    startT = StartT;
	    stopT  = StopT;

	    v2_e[hitv2] = energyDeposit;
            v2_t[hitv2] = StartT;

	    if(PID>1000) v2_pid[hitv2] = 0; else v2_pid[hitv2] = PID;
	    hitv2++;
	    
	    if (PID == 211)
	      {
		ev2[1] +=energyDeposit;
		eBv2[1] += Ebirk;
	      }
	    else if (PID == -13)
	      {
		ev2[2] +=energyDeposit;
		eBv2[2] += Ebirk;
		 }
	    else if (PID == -11) 
	      {
		ev2[3] +=energyDeposit;
		eBv2[3] += Ebirk;
	      }
	  }


	//V3
	if (volumeID0==1 && volumeID1==7) 
	  {
	    ev3[0] +=energyDeposit;
	    eBv3[0] += Ebirk;
	  
	    xv3[0] = StartX;
	    xv3[1] = StartY;
	    xv3[2] = StartZ;
	    xv3[3] = StartT;

	    startT = StartT;
	    stopT  = StopT;

	    v3_e[hitv3] = energyDeposit;
            v3_t[hitv3] = StartT;

	    if(PID>1000) v3_pid[hitv3] = 0; else v3_pid[hitv3] = PID;
	    hitv3++;
	    
	    if (PID == 211)
	      {
		ev3[1] +=energyDeposit;
		eBv3[1] += Ebirk;
	      }
	    else if (PID == -13)
	      {
		ev3[2] +=energyDeposit;
		eBv3[2] += Ebirk;
		 }
	    else if (PID == -11) 
	      {
		ev3[3] +=energyDeposit;
		eBv3[3] += Ebirk;
	      }
	  }


	//Beam Wire Chamber 1
	if (volumeID0 ==2 && volumeID2>300 && volumeID2<400)
	  {
	    if(volumeID2==301)
	      {
		xwc1_1[0] = StartX;
		xwc1_1[1] = StartY;
		xwc1_1[2] = StartZ;
		xwc1_1[3] = StartT;

		wc1_1w[hitwc1_1] = (Int_t)volumeID1;
		wc1_1t[hitwc1_1] = StartT;
		if(PID < 10000) wc1_1pid[hitwc1_1] = PID; else wc1_1pid[hitwc1_1] =0;
		hitwc1_1++;
	      }
	    else if (volumeID2 == 302)
	      {
		xwc1_2[0] = StartX;
		xwc1_2[1] = StartY;
		xwc1_2[2] = StartZ;
		xwc1_2[3] = StartT;

		wc1_2w[hitwc1_2] = (Int_t)volumeID1;
		wc1_2t[hitwc1_2] = StartT;
		if(PID < 10000) wc1_2pid[hitwc1_2] = PID; else wc1_2pid[hitwc1_2] =0;
		hitwc1_2++;
	      }
	    else if (volumeID2 == 303)
	      {
		xwc1_3[0] = StartX;
		xwc1_3[1] = StartY;
		xwc1_3[2] = StartZ;
		xwc1_3[3] = StartT;

		wc1_3w[hitwc1_3] = (Int_t)volumeID1;
		wc1_3t[hitwc1_3] = StartT;
		if(PID < 10000) wc1_3pid[hitwc1_3] = PID; else wc1_3pid[hitwc1_3] =0;
		hitwc1_3++;
	      }
            }

        //Beam Wire Chamber 2
	if (volumeID0 ==2 && volumeID2<400 && volumeID2>300)
	  {
	    if(volumeID2==304)
	      {
		xwc2_1[0] = StartX;
		xwc2_1[1] = StartY;
		xwc2_1[2] = StartZ;
		xwc2_1[3] = StartT;

		wc2_1w[hitwc2_1] = (Int_t)volumeID1;
		wc2_1t[hitwc2_1] = StartT;
		if(PID < 10000) wc2_1pid[hitwc2_1] = PID; else wc2_1pid[hitwc2_1] =0;
		hitwc2_1++;
	      }
	    else if (volumeID2 == 305)
	      {
		xwc2_2[0] = StartX;
		xwc2_2[1] = StartY;
		xwc2_2[2] = StartZ;
		xwc2_2[3] = StartT;

		wc2_2w[hitwc2_2] = (Int_t)volumeID1;
		wc2_2t[hitwc2_2] = StartT;
		if(PID < 10000) wc2_2pid[hitwc2_2] = PID; else wc2_2pid[hitwc2_2] =0;
		hitwc2_2++;
	      }
	    else if (volumeID2 == 306)
	      {
		xwc2_3[0] = StartX;
		xwc2_3[1] = StartY;
		xwc2_3[2] = StartZ;
		xwc2_3[3] = StartT;

		wc2_3w[hitwc2_3] = (Int_t)volumeID1;
		wc2_3t[hitwc2_3] = StartT;
		if(PID < 10000) wc2_3pid[hitwc2_3] = PID; else wc2_3pid[hitwc2_3] =0;
		hitwc2_3++;
	      }
          }
	
	
	//Wire Chamber 3
	//if (volumeID0 ==2 && volumeID2>400)
	// Tristan Sep. 22/17
	if (volumeID0 ==2 && volumeID2>400 && energyDeposit > 0.0005)
	  {
	    if(volumeID2==401)
	      {
		// Tristan Sep. 22/17
		ewc3_1 += energyDeposit;

		bool channelhit = false;
		for (int i = 0; i < hitwc3_1; i++) if (int(volumeID1) == wc3_1w[i]) channelhit = true;
		if (!channelhit)
		{
			xwc3_1[0] = StartX;
			xwc3_1[1] = StartY;
			xwc3_1[2] = StartZ;
			xwc3_1[3] = StartT;
			wc3_1w[hitwc3_1]=int(volumeID1);
			wc3_1t[hitwc3_1]=StartT;
			if(PID < 10000) wc3_1pid[hitwc3_1] = PID; else wc3_1pid[hitwc3_1] =0;
			hitwc3_1++;
		}
	      }
	    else if (volumeID2 == 402)
	      {
		// Tristan Sep. 22/17
		ewc3_2 += energyDeposit;

		bool channelhit = false;
		for (int i = 0; i < hitwc3_2; i++) if (int(volumeID1) == wc3_2w[i]) channelhit = true;
		if (!channelhit)
		{
			xwc3_2[0] = StartX;
			xwc3_2[1] = StartY;
			xwc3_2[2] = StartZ;
			xwc3_2[3] = StartT;
			wc3_2w[hitwc3_2]=int(volumeID1);
			wc3_2t[hitwc3_2]=StartT;
			if(PID < 10000) wc3_2pid[hitwc3_2] = PID; else wc3_2pid[hitwc3_2] =0;
			hitwc3_2++;
		}

	      }
	    else if (volumeID2 == 403)
	      {
		// Tristan Sep. 22/17
		ewc3_3 += energyDeposit;

		bool channelhit = false;
		for (int i = 0; i < hitwc3_3; i++) if (int(volumeID1) == wc3_3w[i]) channelhit = true;
		if (!channelhit)
		{
			xwc3_3[0] = StartX;
			xwc3_3[1] = StartY;
			xwc3_3[2] = StartZ;
			xwc3_3[3] = StartT;
			wc3_3w[hitwc3_3]=int(volumeID1);
			wc3_3t[hitwc3_3]=StartT;
			if(PID < 10000) wc3_3pid[hitwc3_3] = PID; else wc3_3pid[hitwc3_3] =0;
			hitwc3_3++;
		}
	      }
	  }


	// Silicon Detectors

	// Silicon Detector 1
	if (volumeID0 == 2 && (volumeID2 == 11 || volumeID2 == 12))
	  {
	    if(volumeID2 == 11) 
	      {
		ess1_1[0] +=energyDeposit;
		xss1_1[0] = StartX;
		xss1_1[1] = StartY;
		xss1_1[2] = StartZ;
		xss1_1[3] = StartT;
	        ss1_1e[hitss1_1] = energyDeposit;
		ss1_1w[hitss1_1] = volumeID1;
		ss1_1t[hitss1_1] = StartT;
		ss1_1x[hitss1_1] = StartX;
		if (PID <10000) ss1_1pid[hitss1_1] = PID; else ss1_1pid[hitss1_1] = 0;
		hitss1_1++;

		if(PID == 211)
		  {
		    ess1_1[1] +=energyDeposit;
		  }
		else if (PID == -13) 
		  {
		    ess1_1[2] +=energyDeposit;
		  }
		else if (PID == -11)
		  {
		    ess1_1[3] +=energyDeposit;
		  }
	      } else {
	      ess1_2[0] +=energyDeposit;
	      xss1_2[0] = StartX;
	      xss1_2[1] = StartY;
	      xss1_2[2] = StartZ;
	      xss1_2[3] = StartT;
	      
	      ss1_2e[hitss1_2] = energyDeposit;
	      ss1_2w[hitss1_2] = volumeID1;
	      ss1_2t[hitss1_2] = StartT;
	      ss1_2y[hitss1_2] = StartY;
	      if (PID <10000) ss1_2pid[hitss1_2] = PID;
	      else ss1_2pid[hitss1_2] = 0;
	      hitss1_2++;
	      
	      if(PID == 211)
		{
		  ess1_2[1] +=energyDeposit;
		}
	      else if (PID == -13) 
		{
		  ess1_2[2] +=energyDeposit;
		}
	      else if (PID == -11)
		{
		  ess1_2[3] +=energyDeposit;
		}
	    }
	  }
	
	// Silicon Detector 2
	if (volumeID0 == 2 && (volumeID2 == 21 || volumeID2 ==22))
	  {
	    if(volumeID2 == 21)
	      {
		ess2_1[0] +=energyDeposit;
		xss2_1[0] = StartX;
		xss2_1[1] = StartY;
		xss2_1[2] = StartZ;
		xss2_1[3] = StartT;

	        ss2_1e[hitss2_1] = energyDeposit;
		ss2_1w[hitss2_1] = volumeID1;
		ss2_1t[hitss2_1] = StartT;
		ss2_1x[hitss2_1] = StartX;
		if (PID <10000) ss2_1pid[hitss2_1] = PID; else ss2_1pid[hitss2_1] = 0;
		hitss2_1++;
		
		if(PID == 211)
		  {
		    ess2_1[1] +=energyDeposit;
		  }
		else if (PID == -13) 
		  {
		    ess2_1[2] +=energyDeposit;
		  }
		else if (PID == -11)
		  {
		    ess2_1[3] +=energyDeposit;
		  }
	      } else {
	      ess2_2[0] +=energyDeposit;
	      xss2_2[0] = StartX;
	      xss2_2[1] = StartY;
	      xss2_2[2] = StartZ;
	      xss2_2[3] = StartT;
	      
	      
	      ss2_2e[hitss2_2] = energyDeposit;
	      ss2_2w[hitss2_2] = volumeID1;
	      ss2_2t[hitss2_2] = StartT;
	      ss2_2y[hitss2_2] = StartY;
	      if (PID <10000) ss2_2pid[hitss2_2] = PID; else ss2_2pid[hitss2_2] = 0;
	      hitss2_2++;
	      
	      if(PID == 211)
		{
		  ess2_2[1] +=energyDeposit;
		}
	      else if (PID == -13) 
		{
		  ess2_2[2] +=energyDeposit;
		}
	      else if (PID == -11)
		{
		  ess2_2[3] +=energyDeposit;
		}
	    }
	  }
	
	// Silicon Detector 3
	//if (volumeID0 == 2 && (volumeID2 == 31 || volumeID2 == 32))
	//Tracking threshold study, Tristan, August 16/17
	if (volumeID0 == 2 && (volumeID2 == 31 || volumeID2 == 32) && energyDeposit > 0.02)
	  {
	    if(volumeID2 == 31 && hitss3_1 < MAX_NUM_HITS)
	      {
		bool channelhit = false;
		for (int i = 0; i < hitss3_1; i++) if (volumeID1 == ss3_1w[i]) channelhit = true;
		if (!channelhit)
		{
			ess3_1[0] +=energyDeposit;
			xss3_1[0] = StartX;
			xss3_1[1] = StartY;
			xss3_1[2] = StartZ;
			xss3_1[3] = StartT;
			//		cout << " ID1  " << volumeID1 << "  hits3_1 " << hitss3_1 << " X  " << StartX << "  T  " << StartT << "  E  " << energyDeposit << endl;
			ss3_1e[hitss3_1] = energyDeposit;
			ss3_1w[hitss3_1] = volumeID1;
			ss3_1t[hitss3_1] = StartT;
			ss3_1x[hitss3_1] = StartX;
			if (PID <10000) ss3_1pid[hitss3_1] = PID; else ss3_1pid[hitss3_1] = 0;
			hitss3_1++;
			if(PID == 211)
			  {
			    ess3_1[1] +=energyDeposit;
			  }
			else if (PID == -13)
			  {
			    ess3_1[2] +=energyDeposit;
			  }
			else if(PID == -11)
			  {
			    ess3_1[3] +=energyDeposit;
			  }
		}
	      } else if (hitss3_2 < MAX_NUM_HITS){

	      bool channelhit = false;
	      for (int i = 0; i < hitss3_2; i++) if (volumeID1 == ss3_2w[i]) channelhit = true;
	      if (!channelhit)
	      {
		      ess3_2[0] +=energyDeposit;
		      xss3_2[0] = StartX;
		      xss3_2[1] = StartY;
		      xss3_2[2] = StartZ;
		      xss3_2[3] = StartT;
		      //			cout << " ID1  " << volumeID1 << "  hits3_2 " << hitss3_2 << " Y  " << StartY << "  T  " << StartT << "  E  " << energyDeposit << endl;
		      
		      ss3_2e[hitss3_2] = energyDeposit;
		      ss3_2w[hitss3_2] = volumeID1;
		      ss3_2t[hitss3_2] = StartT;
		      ss3_2y[hitss3_2] = StartY;
		      if (PID <10000) ss3_2pid[hitss3_2] = PID; else ss3_2pid[hitss3_2] = 0;
		      hitss3_2++;
		      if(PID == 211)
			{
			  ess3_2[1] +=energyDeposit;
			}
		      else if (PID == -13) 
			{
			  ess3_2[2] +=energyDeposit;
			}
		      else if (PID == -11)
			{
			  ess3_2[3] +=energyDeposit;
			}
	      }
	    }
	  }
      }
      
      hit++;
      MCTree1->GetEntry(hit);
      if (hit == MCTree1->GetEntries()) break;
    }
    
    
  //the number of hits in each detector for the current event

    ss1_1n = hitss1_1;
    ss1_2n = hitss1_2;
    ss2_1n = hitss2_1;
    ss2_2n = hitss2_2;
    ss3_1n = hitss3_1;
    ss3_2n = hitss3_2;

    wc1_1n = hitwc1_1;
    wc1_2n = hitwc1_2;
    wc1_3n = hitwc1_3;

    wc2_1n = hitwc2_1;
    wc2_2n = hitwc2_2;
    wc2_3n = hitwc2_3;


    wc3_1n = hitwc3_1;
    wc3_2n = hitwc3_2;
    wc3_3n = hitwc3_3;
    

    b1_n = hitb1;
    b2_n = hitb2;
    tg_n = hittg;
    t1_n = hitt1;
    t2_n = hitt2;
    v2_n = hitv2;
    v3_n = hitv3;

    // apply CsI energy threshold
    if(csi <= 2.0) csi = 0.;
    
    for (int i=0;i<21;i++) {
      if(csiall->USIn[i]<=2.0) csiall->USIn[i]=0;
      else usi += csiall->USIn[i];
    }
    for (int i=0;i<28;i++) {
      if(csiall->USOut[i]<=2.0) csiall->USOut[i]=0;
      else uso += csiall->USOut[i];
    }
      for (int i=0;i<21;i++) {
	if(csiall->DSIn[i]<=2.0) csiall->DSIn[i]=0;
	else dsi += csiall->DSIn[i];
      }
      for (int i=0;i<27;i++) {
	if(csiall->DSOut[i]<=2.0) csiall->DSOut[i]=0;
	else dso += csiall->DSOut[i];
      }

    double minn;
    int P;
    double temp;
    for (int i = 0; i < 99; i++)
    {
	minn = fabs(binatimes[i]);
	P = i;
	for (int j = i + 1; j < 100; j++)
	{
	    if (fabs(binatimes[j]) < minn)
	    {
		minn = fabs(binatimes[j]);
		P = j;
	    }
	}

        temp = binatimes[i];
        binatimes[i] = binatimes[P];
        binatimes[P] = temp;

        temp = binahits[i];
        binahits[i] = binahits[P];
        binahits[P] = temp;
	
        temp = birkshits[i];
        birkshits[i] = birkshits[P];
        birkshits[P] = temp;
	
    }
        float tnai, energy, energy2;

    if(posmin<10000 && pitime<10000) Tpos = posmin-pitime;//shintaro
    if(Tpos != -10000){
      char nname[100];
      sprintf(nname,"Sum%d",entry);
      SumWF=new TH1F(nname,nname,42,0,1400);
      sprintf(nname,"Sums%d",entry);
     SumWF2=new TH1F(nname,nname,42,0,1400);

      numhits = 0;
        for(int i=0;i<100;i++){
	if(binahits[i] == -10000) break;
	char name[100];
	sprintf(name,"Entry%dWF",i);
	WF=new TH1F(name,name,42,0,1400);
	sprintf(name,"Entrys%dWF",i);
	WF2=new TH1F(name,name,42,0,1400);
	
	tnai=binatimes[i] - binatimes[0];
	energy=binahits[i];
	energy2=birkshits[i];
	for(int iii=3;iii<=40;iii++){
	  if((tnai+1./30.*1000.*iii)<0. || hh1->GetBinContent(iii)<0)
	    continue;
	  if((tnai+1./30.*1000.*iii)>1400.)
	    break;
	  WF->SetBinContent(WF->FindBin(tnai+1./30.*1000.*iii),hh1->GetBinContent(iii)*energy);
	  WF2->SetBinContent(WF2->FindBin(tnai+1./30.*1000.*iii),hh1->GetBinContent(iii)*energy2);
	}
	SumWF->Add(WF,1);
	delete WF;
	SumWF2->Add(WF2,1);
	delete WF2;
	numhits++;
	}

	if(numhits > 0) {

	  for(int pp = 6; pp <= 10; pp++){
	    if(binaph < SumWF->GetBinContent(pp)) binaph = SumWF->GetBinContent(pp);
	  }
	  binaq = (SumWF->Integral(0,34))/(hh1->Integral(0,34));
	  delete SumWF;
      
	  for(int pp = 6; pp <= 10; pp++){
	        if(birksph < SumWF2->GetBinContent(pp)) birksph = SumWF2->GetBinContent(pp);
	  }
	  birksq = (SumWF2->Integral(0,34))/(hh1->Integral(0,34));
	  delete SumWF2;

          birksphr = resolution->BinaResolution(birksph,0.033);
	  birksqr = resolution->BinaResolution(birksq,0.033);

	}  
    
	//	if(birksph > 0 && birksph < 1 && PoStopX[2] > 84 && PoStopX[2] < 150 && PiDecayP[3] == 0 && et1[0] > 0.1 && et2[0] >0.4) cout << " birksph entry  " << entry << "  numhits  " << numhits << " Z " << PoStopX[2] << endl;
	//	if(birksph > 0 && birksph < 1 && PoStopX[2] > 84 && PoStopX[2] < 150 && PiDecayP[3] == 0 && et1[0] > 0.1 && et2[0] >0.4) cout << " tnai  " << tnai << "  energy2  " << energy2 << "  birksphr  " << birksphr << endl;
	//    if(birksph > 0 && birksph < 1 && PoStopX[2] > 84 && PoStopX[2] < 150 && PiDecayP[3] == 0 && et1[0] > 0.1 && et2[0] >0.4) {
	/*	 for(int i = 0; i <= numhits; i++){
	   cout << binahits[i] << "  " << binatimes[i] << endl;
	   }
	   } */
    }
    //Add resolutions
    csiR  = resolution->CsIResolution(csi,0.25);
       
    for(int l=0;l<4;l++) 
      {  
	// Tristan, Aug. 23/16 (caltest)
	//ebinar[l] = resolution->BinaResolution(eBbina[l],0.05);
	//ebinar[l] = resolution->BinaResolution(ebina[l],0.05);
	// Tristan, Aug. 23/16 (caltest)
	//ebinar[l] = resolution->BinaResolution(eBbina[l],0.075);
	// Tristan Nov. 23/17
	//ebinar[l] = resolution->BinaResolution(eBbina[l],0.075);
	ebinar[l] = resolution->BinaResolution(eBbina[l],0.033);
	// Tristan Oct. 20/17
	//eb1r[l]   = resolution->T2Resolution(eBb1[l],0.072);
	eb1r[l]   = resolution->T2Resolution(eBb1[l],0.036);
	// Tristan Oct. 20/17
	//eb2r[l]   = resolution->T2Resolution(eBb2[l],0.077);
	eb2r[l]   = resolution->T2Resolution(eBb2[l],0.039);
	//etgr[l]   = resolution->T2Resolution(eBtg[l],0.03);
	// Tristan August 21/17
	//etgr[l]   = resolution->T2Resolution(eBtg[l],0.01);
	// Tristan Oct. 20/17
	etgr[l]   = resolution->T2Resolution(eBtg[l],0.07);
	et1r[l]   = resolution->T2Resolution(eBt1[l],0.018);
		et2r[l]   = resolution->T2Resolution(eBt2[l],0.30);
	//et2r[l]   = resolution->T2Resolution(eBt2[l],0.226);
	ev2r[l]   = resolution->T2Resolution(eBv2[l],0.19);
	ev3r[l]   = resolution->T2Resolution(eBv3[l],0.19);
	// Tristan, Oct. 23/17. Happy 97th birthday!
	//ess1_1r[l] = resolution->T2Resolution(ess1_1[l],0.15);
	//ess1_2r[l] = resolution->T2Resolution(ess1_2[l],0.15);
	ess1_1r[l] = resolution->T2Resolution(ess1_1[l],0.11);
	ess1_2r[l] = resolution->T2Resolution(ess1_2[l],0.11);
	ess2_1r[l] = resolution->T2Resolution(ess2_1[l],0.11);
	ess2_2r[l] = resolution->T2Resolution(ess2_2[l],0.11);
	ess3_1r[l] = resolution->T2Resolution(ess3_1[l],0.085);
	ess3_2r[l] = resolution->T2Resolution(ess3_2[l],0.085);
      }

    // Crude T2 threshold efficiency
    double rr = r->Rndm();
    et2_trig = et2r[0];
    if((et2_trig<0.1) && (rr > 0.0574)) et2_trig = 0.;
    if(et2_trig<0.2 && rr > 0.398) et2_trig = 0.;
    if(et2_trig<0.3 && rr > 0.780) et2_trig = 0.;
    if(et2_trig<0.4 && rr > 0.917) et2_trig = 0.;
    //    if(et2r[0] >0 && et2r[0] <1) cout << et2r[0] << "  rr  " << rr << "  et2_trig " << et2_trig << endl;
     
    //usiR  = resolution->CsIResolution(usi,0.25);
    //usoR  = resolution->CsIResolution(uso,0.25);
    //dsiR  = resolution->CsIResolution(dsi,0.25);
    // Tristan Aug. 9/16
    //dsiR  = resolution->CsIResolution(dsi,0.425);
    //dsoR  = resolution->CsIResolution(dso,0.25);
    
    usiR  = resolution->CsIResolution(usi,0.5);
    usoR  = resolution->CsIResolution(uso,0.5);
    dsiR  = resolution->CsIResolution(dsi,0.5);
    dsoR  = resolution->CsIResolution(dso,0.5);
    
    usiB = resolution->CsIResolution(usiB,0.25);
    usoB = resolution->CsIResolution(usoB,0.25);
    dsiB = resolution->CsIResolution(dsiB,0.25);
    dsoB = resolution->CsIResolution(dsoB,0.25);

    for (int i = 0; i < 21; i++) eCsIUSIchR[i] = resolution->CsIResolution(eCsIUSIch[i],0.25);
    for (int i = 0; i < 28; i++) eCsIUSOchR[i] = resolution->CsIResolution(eCsIUSOch[i],0.25);
    for (int i = 0; i < 21; i++) eCsIDSIchR[i] = resolution->CsIResolution(eCsIDSIch[i],0.25);
    for (int i = 0; i < 27; i++) eCsIDSOchR[i] = resolution->CsIResolution(eCsIDSOch[i],0.25);

    csiall->Sum = csi;
    
    //Bina+CsI
    binacsi  = ebina[0]+csi;
    binacsiR = ebinar[0]+csiR;
        
    //Approximate Decay Time (for quick studies)
    //if(postime<10000 && pitime<10000) Tpos = postime-pitime;
 
    //Pion/Muon/Positron momentum-energy/position-time
    for (int i=0;i<4;i++){
      pidecayp[i] = PiDecayP[i];
      mudecayp[i] = MuDecayP[i];
      postopp[i] = PoStopP[i];
      gstopp[i] = GStopP[i];
      
      pidecayx[i] = PiDecayX[i];
      mudecayx[i] = MuDecayX[i];
      postopx[i] = PoStopX[i];
      gstopx[i] = GStopX[i];
      
      pistartp[i] = PiStartP[i];
      mustartp[i] = MuStartP[i];
      postartp[i] = PoStartP[i];
      gstartp[i] = GStartP[i];
      
      pistartx[i] = PiStartX[i];
      mustartx[i] = MuStartX[i];
      postartx[i] = PoStartX[i];
      gstartx[i] = GStartX[i];

      // Added by Tristan, Jan. 28/15, Bhabha stuff
      prebhabhappos[i] = PreBhabhaPpos[i];
      postbhabhappos[i] = PostBhabhaPpos[i];
      postbhabhapelec[i] = PostBhabhaPelec[i];
      ebht1=EBhT1;
      ebht2=EBhT2;

      // Added by Tristan, Oct. 12/17
      wc3positronx[i] = WC3positronX[i];


      ///// Additions based on Tristan's work /////
      posbremprex[i] = PosBremPreX[i];
      posbremprep[i] = PosBremPreP[i];
      posbrempostx[i] = PosBremPostX[i];
      posbrempostp[i] = PosBremPostP[i];
      elecbremprex[i] = ElecBremPreX[i];
      elecbremprep[i] = ElecBremPreP[i];
      elecbrempostx[i] = ElecBremPostX[i];
      elecbrempostp[i] = ElecBremPostP[i];

      posbhabhaprex[i] = PosBhabhaPreX[i];
      posbhabhaprep[i] = PosBhabhaPreP[i];
      posbhabhapostx[i] = PosBhabhaPostX[i];
      posbhabhapostp[i] = PosBhabhaPostP[i];

      posannihilprex[i] = PosAnnihilPreX[i];
      posannihilprep[i] = PosAnnihilPreP[i];
      posannihilpostx[i] = PosAnnihilPostX[i];
      posannihilpostp[i] = PosAnnihilPostP[i];

      posscatterprex[i] = PosScatterPreX[i];
      posscatterprep[i] = PosScatterPreP[i];
      posscatterpostx[i] = PosScatterPostX[i];
      posscatterpostp[i] = PosScatterPostP[i];
      elecscatterprex[i] = ElecScatterPreX[i];
      elecscatterprep[i] = ElecScatterPreP[i];
      elecscatterpostx[i] = ElecScatterPostX[i];
      elecscatterpostp[i] = ElecScatterPostP[i];

      // elecinwc3prex[i] = ElecinWC3PreX[i];
      // elecinwc3prep[i] = ElecinWC3PreP[i];
      // primposinwc3prex[i] = PrimPosinWC3PreX[i];
      // primposinwc3prep[i] = PrimPosinWC3PreP[i];
      // secposinwc3prex[i] = SecPosinWC3PreX[i];
      // secposinwc3prep[i] = SecPosinWC3PreP[i];

      ///////////////

      
      if (i<3){ 
	photonuclearx[i] = PhotonuclearX[i];
	mupolarization[i] = MuPolarization[i];
	emomentum[i] = EMomentum[i];
      }
    }
    
    ebh=EBh;
    eintobina = EIntoBina;

    ///// Additions based on Tristan's work /////
    postotalbreme = PosTotalBremE;
    electotalbreme = ElecTotalBremE;
    postotalbhabhae = PosTotalBhabhaE;
    ////////////

    MC.Tpos = Tpos;
    MC.PiTime = pitime;
    MC.PosTime = postime;
    MC.PosMin = posmin;//shintaro
    
    //Silicon clustering

    double thresh = 0.0235;
    
    // let's sort ss3_1
    int size = ss3_1n;
    nstrips_x = ss3_1n;

    if(size>=1){
      /*
      //debug before Sorting
      cout << "Before X: "<<size<<endl;
      for (int i=0; i<size; i++)
        {
   	  cout << "strip: " << ss3_1w[i]<< "   time: " << ss3_1t[i] << "     energy: " <<  ss3_1e[i] << endl;
        }
      */

    for(int i=0;i<size;i++)
      {
	for(int j=i+1;j<size;j++)
	  {
	    if(ss3_1w[j] < ss3_1w[i])
	      {
		temp=ss3_1w[i];
		ss3_1w[i] = ss3_1w[j];
		ss3_1w[j] = temp;
		
		temp=ss3_1t[i];
		ss3_1t[i] = ss3_1t[j];
		ss3_1t[j] = temp;

		temp=ss3_1e[i];
		ss3_1e[i] = ss3_1e[j];
		ss3_1e[j] = temp;

		temp=ss3_1x[i];
		ss3_1x[i] = ss3_1x[j];
		ss3_1x[j] = temp;
	      }
	  }
      }
    /*
      //debug after Sorting
      cout << "After X: "<<size<<endl;
      for (int i=0; i<size; i++)
	{
	  cout << "strip: " << ss3_1w[i]<< "   time: " << ss3_1t[i] << "     energy: " <<  ss3_1e[i] << endl;
	}
    */


    // fill ampx array
    //  cout << " size " << size << endl;
    double r1, r2, r3;
    //r1 = 0.75; r2 = 0.58; r3 = 0.4;
     r1 = 0.75; r2 = 0.5; r3 = 0.25;
    for(int i=0;i<size;i++) {
      int j = ss3_1w[i]/4;
      int k = ss3_1w[i] - j*4;
      ampxt[j] = ss3_1t[i];
      if(k == 0) {ampx[j] += ss3_1e[i];}
      else if(k == 1) {ampx[j] += ss3_1e[i]*r1; ampx[j+1] += ss3_1e[i]*r3;}
      else if(k == 2) {ampx[j] += ss3_1e[i]*r2;  ampx[j+1] += ss3_1e[i]*r2;}
      else if(k == 3) {ampx[j] += ss3_1e[i]*r3;  ampx[j+1] += ss3_1e[i]*r1;}
      else{
	ampx[j+1] = +ss3_1e[i];
	cout << "  ampx["<<j+1<<"] " << ampx[j+1] << endl;
      }

      //    cout << " i, j, k " << i << "  " << j << "  " << k << endl;
      //cout << " i,  k " << i << " "<< k << " ampx["<<j<<"] " << ampx[j] << "  amp["<<j+1<<"]  " << ampx[j+1] << endl;
    }
    Nampx = 0;
    for(int i=0; i<48;i++) {
      if(ampx[i]>thresh) {ampxn[Nampx] = i; ampx[Nampx] = ampx[i]; ampxt[Nampx] = ampxt[i]; Nampx++;}
    }


    
    //   cout << " Nampx " << Nampx << endl;
    //   for(int i=0;i<Nampx;i++) cout << ampxn[i] << "   " << ampx[i] << endl;
      
    //define the vector to hold the indeces of the cluster hits
    vector<int> ix[MAX_NUM_HITS];
      
    ix->clear();
    int  N = -1;
    int  last = -1;
    int  fl   =  1; // 1 begin new cluster, 0 - continue old; 2 - bad state
    //      cout << " size " << ss3_1n << endl;
    for (int l=0; l<Nampx;l++)
      {
	int iw = ampxn[l];
	//	cout << "  l  "  << l << "  iw "  << iw  << "  last  " <<  last << endl;
	if(last!= -1) {
	  //	  cout << "  last " << last << endl;
	  if (iw ==last +1) fl=0;
	  else if(iw>last+1) fl =1;
	  else if(iw==last) {fl=0;}
	  else fl =2;
	}
	//		    cout << " Flag is: "<< fl << endl;
	if (fl==2)  {
	  cerr << "ERROR: channels not sorted!\n";
	  //   exit(1);
	}

	if (fl==1) { // beginning new cluster
	  if (N<MAX_NUM_HITS-1){
	    N++;
	    namp_x[N] = Nampx;
	  }
	}
	//		cout << "  l  " << l  << "  N "  << N << endl;
	ix[N].push_back(l);
	last = iw;
      }
    N++;

    
    //   cout << "Number of clusters found:"<< N <<" number of hits was " <<Nampx<<" difference is "<< Nampx-N<< endl;

    /*if(N>1){

      for(int hh=0; hh<Nampx; hh++)
	cout << "ampxn["<<hh<<"]  " << ampxn[hh] << endl;
      
    for (int k=0; k<N ;k++)
      {
	cout << "Cluster="<<k<<" size of cluster: " << ix[k].size() << endl;
	for(int j=0; j<ix[k].size(); j++)
	  cout << "hit: "<<ix[k].at(j)<<" amp: " << ampxn[ix[k].at(j)] << endl;
      }
      }*/
                      // changing the number of hits to clusters!

      Nclustx = N;   //  ############## fix MC.S3 code below
      
                // now let's find the highest strip in each clusters
      //           cout <<" ****Starting the cycle of clusters with a Number of clusters : "  << N<< endl;
                      for (int k=0; k<N; k++) {

                        int ie[2]={-1,-1}; // highest/next strip
			int ii;

                // Search for highest strip in each cluster

                        int ch;
                        double BIG = -1;
                        for (uint j=0; j<ix[k].size(); j++)
                          {
                          int l = ix[k].at(j);
                           if (ampx[l]>BIG)
                            {
                            BIG = ampx[l];
                            ie[0] = j;
			    //ie[0] = l;
                            }
                          }
			
                        // Search for next adjucent highest strip

		        //if(k>0) cout << "ix[k].size(),  ie[0]  " << ix[k].size() << "  " << ie[0] << endl;

			//if(k>0) cout << "ampxn[" << ie[0] << "] " << ampxn[ie[0]] << endl;
			
			//ie[0] = 0;
                        ie[1] = ie[0];
                        BIG = -1;
                        for (uint j=0; j<ix[k].size();j++)
                          {
                            int l = ix[k].at(j);
			    //cout << " l, ampxn[l], ampxn[ie[0]], ampx[l]  " << l << "  " << ampxn[l] << "  " <<  ampxn[ie[0]] << "  " << ampx[l] << endl;
                            if(abs(ampxn[l]-ampxn[ix[k].at(ie[0])])==1 && ampx[l]>BIG)   // ##### fixed error in array index
			    
			      //if(abs(ampxn[l]-ampxn[ie[0]])==1 && ampx[l]>BIG)
                              {
				BIG = ampx[l]; ie[1]=j;
				//ie[1]=l;
			      //	     cout << " BIG, ie[1]  "  << BIG << "   " << ie[1]  << endl;
                              }
                          }

			//cout << "OK" << endl;

			// if(ix[k].size()>1)
			//   cout << "ie[0],  ie[1]  " << ie[0] << "  " << ie[1] << endl;

			
			//           cout << "found the hits in the cluster, the highest w="<< ampxn[ix[k].at(ie[0])] << " and the next highest is" << ampxn[ix[k].at(ie[1])] << endl;  //  ##### fixed error in array index

                        int left = min(ie[0],ie[1]);
                        double gc0 = 1, gc1 = 1, gcleft = 1;
                        double aQ, aQ2;
			double at;
			//         cout <<" debug 1 (left):"<<left<< " and k="<<k << " and ix(k).size="<<ix[k].size() <<endl;
			//            cout <<" debug 1.5 ie[0]="<<ie[0]<<" ie[1]="<<ie[1]<<endl;
                        aQ = ampx[ix[k].at(left)];
			at = ampxt[ix[k].at(ie[0])];
			//aQ = ampx[left];

			//            cout <<" debug 2" <<endl;

		        aQ2 = ampx[ix[k].at(ie[0])];
                        //aQ2 = ampx[ie[0]];
			//          cout << "Energy is << "<< aQ << endl;  // ############ left energy only
                        if( ie[1]!=ie[0])
                          {
			    aQ2 += ampx[ix[k].at(ie[1])];
			    at = (ampxt[ix[k].at(ie[0])]*ampx[ix[k].at(ie[0])] + ampxt[ix[k].at(ie[1])]*ampx[ix[k].at(ie[1])])/aQ2;
                            //aQ2 += ampx[ie[1]];
			  /*
                          cout<< " debug3 cluster with more then one hit****************************"<<endl;
                          cout<< " ix[k].at(left)  "<< ix[k].at(left)<< endl;
                          cout<< " ix[k].at(ie[1])  "<< ix[k].at(ie[1])<<endl;
                          cout<< " aQw="<<ampxn[ix[k].at(left)]<<" aQ2w="<<ampxn[ix[k].at(ie[1])]<< " aQR="<<aQ/aQ2<< endl;
                          cout<< " aQ="<<ampx[ix[k].at(left)]<<" aQ2="<<ampx[ix[k].at(ie[1])]<< " aQR="<<aQ/aQ2<< endl;
			  */
                          }
			  
                        double aPosi=ampxn[ix[k].at(left)]*1.28-30.08;
			//double aPosi=ampxn[left]*1.28-30.08;
                        double aQR=aQ/aQ2;
			//   cout<< "Position before is " << aPosi << " and aQR="<<aQR<<endl;

                        double fPosi=0;
                        if (aQR<=0.14)       {fPosi=1;} // 0.25
                        else if (aQR<=0.34)  {fPosi=0.75;} // 0.40
                        else if (aQR<=0.51) {fPosi=0.50;} // 0.60
                        else if (aQR<=0.75) {fPosi=0.25;} // 0.80
                        else if (aQR<=1)    {fPosi=0;} // 1.0
                        aPosi=(ampxn[ix[k].at(left)]+fPosi)*1.28-30.08;
			//aPosi=(ampxn[left]+fPosi)*1.28-30.08;

			//    cout<< "Position after is " << aPosi << endl;


                // changing the position information
                	      MC.S3_X_ch[k] = aPosi;
			      s3_x_posi[k] = aPosi;
			      aqr_x[k] = aQR;
			      MC.S3_X_t[k] = at;
			      ampn_x[k] = ampxn[ix[k].at(left)];
			      tcl_x[k] = at;
			      

		      }

		      MC.S3_X_N = Nclustx;
		      
      
    }

    // else if(size == 1){
    // 	MC.S3_X_N = 1;
    // 	MC.S3_X_ch[0] = (ss3_1w[0]-94)*0.32;
    // 	MC.S3_X_t[0] = ss3_1t[0];

    // 	s3_x_posi[0] = (ss3_1w[0]-94)*0.32;
    // 	Nclustx = 1;
    //   }

    else MC.S3_X_N = 0;

      
    
      //**********************************

    // let's sort ss3_2
    size = ss3_2n;
    nstrips_y = ss3_2n;
    if(size>=1){
      
      /*
      //debug before Sorting
      cout << "Before X: "<<size<<endl;
      for (int i=0; i<size; i++)
        {
   	  cout << "strip: " << ss3_2w[i]<< "   time: " << ss3_2t[i] << "     energy: " <<  ss3_2e[i] << endl;
        }
      */

    for(int i=0;i<size;i++)
      {
	for(int j=i+1;j<size;j++)
	  {
	    if(ss3_2w[j] < ss3_2w[i])
	      {
		temp=ss3_2w[i];
		ss3_2w[i] = ss3_2w[j];
		ss3_2w[j] = temp;
		
		temp=ss3_2t[i];
		ss3_2t[i] = ss3_2t[j];
		ss3_2t[j] = temp;

		temp=ss3_2e[i];
		ss3_2e[i] = ss3_2e[j];
		ss3_2e[j] = temp;

		temp=ss3_2y[i];
		ss3_2y[i] = ss3_2y[j];
		ss3_2y[j] = temp;
	      }
	  }
      }
  
    /*
      //debug after Sorting
      cout << "After X: "<<size<<endl;
      for (int i=0; i<size; i++)
	{
	  cout << "strip: " << ss3_2w[i]<< "   time: " << ss3_2t[i] << "     energy: " <<  ss3_2e[i] << endl;
	}
    */


    // fill ampx array
    //  cout << " size " << size << endl;
    double r1, r2, r3;
    //r1 = 0.75; r2 = 0.58; r3 = 0.4;
     r1 = 0.75; r2 = 0.5; r3 = 0.25;
    for(int i=0;i<size;i++) {
      int j = ss3_2w[i]/4;
      int k = ss3_2w[i] - j*4;
      ampyt[j] = ss3_2t[i];
      if(k == 0) {ampy[j] += ss3_2e[i];}
      else if(k == 1) {ampy[j] += ss3_2e[i]*r1; ampy[j+1] += ss3_2e[i]*r3;}
      else if(k == 2) {ampy[j] += ss3_2e[i]*r2;  ampy[j+1] += ss3_2e[i]*r2;}
      else if(k == 3) {ampy[j] += ss3_2e[i]*r3;  ampy[j+1] += ss3_2e[i]*r1;}
      else{
	ampy[j+1] = +ss3_2e[i];
	cout << "  ampy["<<j+1<<"] " << ampy[j+1] << endl;
      }

      //    cout << " i, j, k " << i << "  " << j << "  " << k << endl;
      //cout << " i,  k " << i << " "<< k << " ampx["<<j<<"] " << ampx[j] << "  amp["<<j+1<<"]  " << ampx[j+1] << endl;
    }
    Nampy = 0;
    for(int i=0; i<48;i++) {
      if(ampy[i] >thresh) {ampyn[Nampy] = i; ampy[Nampy] = ampy[i]; ampyt[Nampy] = ampyt[i]; Nampy++;}
    }

    
    //   cout << " Nampx " << Nampx << endl;
    //   for(int i=0;i<Nampx;i++) cout << ampxn[i] << "   " << ampx[i] << endl;
      
    //define the vector to hold the indeces of the cluster hits
    vector<int> ix[MAX_NUM_HITS];
      
    // ix->clear();
    int  N = -1;
    int  last = -1;
    int  fl   =  1; // 1 begin new cluster, 0 - continue old; 2 - bad state
    //      cout << " size " << ss3_1n << endl;
    for (int l=0; l<Nampy;l++)
      {
	int iw = ampyn[l];
	//	cout << "  l  "  << l << "  iw "  << iw  << "  last  " <<  last << endl;
	if(last!= -1) {
	  //	  cout << "  last " << last << endl;
	  if (iw ==last +1) fl=0;
	  else if(iw>last+1) fl =1;
	  else if(iw==last) {fl=0;}
	  else fl =2;
	}
	//		    cout << " Flag is: "<< fl << endl;
	if (fl==2)  {
	  cerr << "ERROR: channels not sorted!\n";
	  //   exit(1);
	}

	if (fl==1) { // beginning new cluster
	  if (N<MAX_NUM_HITS-1){
	    N++;
	    namp_y[N] = Nampx;
	  }
	}
	//		cout << "  l  " << l  << "  N "  << N << endl;
	ix[N].push_back(l);
	last = iw;
      }
    N++;
    
    //   cout << "Number of clusters found:"<< N <<" number of hits was " <<Nampx<<" difference is "<< Nampx-N<< endl;

    /*if(N>1){

      for(int hh=0; hh<Nampx; hh++)
	cout << "ampxn["<<hh<<"]  " << ampxn[hh] << endl;
      
    for (int k=0; k<N ;k++)
      {
	cout << "Cluster="<<k<<" size of cluster: " << ix[k].size() << endl;
	for(int j=0; j<ix[k].size(); j++)
	  cout << "hit: "<<ix[k].at(j)<<" amp: " << ampxn[ix[k].at(j)] << endl;
      }
      }*/

                      // changing the number of hits to clusters!

      Nclusty = N;   //  ############## fix MC.S3 code below
      
                // now let's find the highest strip in each clusters
      //           cout <<" ****Starting the cycle of clusters with a Number of clusters : "  << N<< endl;
                      for (int k=0; k<N; k++) {

                        int ie[2]={-1,-1}; // highest/next strip
			int ii;

                // Search for highest strip in each cluster

                        int ch;
                        double BIG = -1;
                        for (uint j=0; j<ix[k].size(); j++)
                          {
                          int l = ix[k].at(j);
                           if (ampy[l]>BIG)
                            {
                            BIG = ampy[l];
                            ie[0] = j;
			    //ie[0] = l;
                            }
                          }
			
                        // Search for next adjucent highest strip

		        //if(k>0) cout << "ix[k].size(),  ie[0]  " << ix[k].size() << "  " << ie[0] << endl;

			//if(k>0) cout << "ampxn[" << ie[0] << "] " << ampxn[ie[0]] << endl;
			
			//ie[0] = 0;
                        ie[1] = ie[0];
                        BIG = -1;
                        for (uint j=0; j<ix[k].size();j++)
                          {
                            int l = ix[k].at(j);
			    //cout << " l, ampxn[l], ampxn[ie[0]], ampx[l]  " << l << "  " << ampxn[l] << "  " <<  ampxn[ie[0]] << "  " << ampx[l] << endl;
                            if(abs(ampyn[l]-ampyn[ix[k].at(ie[0])])==1 && ampy[l]>BIG)   // ##### fixed error in array index
			    
			    //if(abs(ampxn[l]-ampxn[ie[0]])==1 && ampx[l]>BIG)
                              {
				BIG = ampy[l]; ie[1]=j;
				//ie[1]=l;
			      //	     cout << " BIG, ie[1]  "  << BIG << "   " << ie[1]  << endl;
                              }
                          }

			//cout << "OK" << endl;

			// if(ix[k].size()>1)
			//   cout << "ie[0],  ie[1]  " << ie[0] << "  " << ie[1] << endl;

			
			//           cout << "found the hits in the cluster, the highest w="<< ampxn[ix[k].at(ie[0])] << " and the next highest is" << ampxn[ix[k].at(ie[1])] << endl;  //  ##### fixed error in array index

                        int left = min(ie[0],ie[1]);
                        double gc0 = 1, gc1 = 1, gcleft = 1;
                        double aQ, aQ2, at;
			//         cout <<" debug 1 (left):"<<left<< " and k="<<k << " and ix(k).size="<<ix[k].size() <<endl;
			//            cout <<" debug 1.5 ie[0]="<<ie[0]<<" ie[1]="<<ie[1]<<endl;
                        aQ = ampy[ix[k].at(left)];
			at = ampyt[ix[k].at(ie[0])];
			// aQ = ampx[left];

			//            cout <<" debug 2" <<endl;

		        aQ2 = ampy[ix[k].at(ie[0])];
                        // aQ2 = ampx[ie[0]];
			//          cout << "Energy is << "<< aQ << endl;  // ############ left energy only
                        if( ie[1]!=ie[0])
                          {
			    aQ2 += ampy[ix[k].at(ie[1])];
			    at = (ampyt[ix[k].at(ie[0])]*ampy[ix[k].at(ie[0])] + ampyt[ix[k].at(ie[1])]*ampy[ix[k].at(ie[1])])/aQ2;
                            // aQ2 += ampx[ie[1]];
			  /*
                          cout<< " debug3 cluster with more then one hit****************************"<<endl;
                          cout<< " ix[k].at(left)  "<< ix[k].at(left)<< endl;
                          cout<< " ix[k].at(ie[1])  "<< ix[k].at(ie[1])<<endl;
                          cout<< " aQw="<<ampxn[ix[k].at(left)]<<" aQ2w="<<ampxn[ix[k].at(ie[1])]<< " aQR="<<aQ/aQ2<< endl;
                          cout<< " aQ="<<ampx[ix[k].at(left)]<<" aQ2="<<ampx[ix[k].at(ie[1])]<< " aQR="<<aQ/aQ2<< endl;
			  */
                          }
			  
                        double aPosi=ampyn[ix[k].at(left)]*1.28-30.08;
			// double aPosi=ampxn[left]*1.28-30.08;
                        double aQR=aQ/aQ2;
			//   cout<< "Position before is " << aPosi << " and aQR="<<aQR<<endl;

                        double fPosi=0;
                        if (aQR<=0.14)       {fPosi=1;} // 0.25
                        else if (aQR<=0.34)  {fPosi=0.75;} // 0.40
                        else if (aQR<=0.51) {fPosi=0.50;} // 0.60
                        else if (aQR<=0.75) {fPosi=0.25;} // 0.80
                        else if (aQR<=1)    {fPosi=0;} // 1.0
                        aPosi=(ampyn[ix[k].at(left)]+fPosi)*1.28-30.08;
			// aPosi=(ampxn[left]+fPosi)*1.28-30.08;
 

			//    cout<< "Position after is " << aPosi << endl;


                // changing the position information
                	      MC.S3_Y_ch[k] = aPosi;
			      s3_y_posi[k] = aPosi;
			      aqr_y[k] = aQR;
			      MC.S3_Y_t[k] = at;
			      ampn_y[k] = ampyn[ix[k].at(left)];
			      tcl_y[k] = at;

		      }

		      MC.S3_Y_N = Nclusty;
    
    }

    // else if(size == 1){
    // 	MC.S3_Y_N = 1;
    // 	MC.S3_Y_ch[0] = (ss3_2w[0]-94)*0.32;
    // 	MC.S3_Y_t[0] = ss3_2t[0];

    // 	s3_y_posi[0] = (ss3_2w[0]-94)*0.32;
    // 	Nclusty = 1;
    //   }

    else MC.S3_Y_N = 0;
 

    for (int i=0;i<MAX_NUM_HITS;i++)
      {

	MC.wc1_1n = wc1_1n;
	MC.wc1_1w[i] = wc1_1w[i];
	MC.wc1_1t[i] = wc1_1t[i];
	MC.wc1_2n = wc1_2n;
	MC.wc1_2w[i] = wc1_2w[i];
	MC.wc1_2t[i] = wc1_2t[i];
	MC.wc1_3n = wc1_3n;
	MC.wc1_3w[i] = wc1_3w[i];
	MC.wc1_3t[i] = wc1_3t[i];

	MC.wc2_1n = wc2_1n;
	MC.wc2_1w[i] = wc2_1w[i];
	MC.wc2_1t[i] = wc2_1t[i];
	MC.wc2_2n = wc2_2n;
	MC.wc2_2w[i] = wc2_2w[i];
	MC.wc2_2t[i] = wc2_2t[i];
	MC.wc2_3n = wc2_3n;
	MC.wc2_3w[i] = wc2_3w[i];
	MC.wc2_3t[i] = wc2_3t[i];


	MC.wc3_1n = wc3_1n;
	MC.wc3_1w[i] = wc3_1w[i];
	MC.wc3_1t[i] = wc3_1t[i];
	MC.wc3_2n  = wc3_2n;
	MC.wc3_2w[i] = wc3_2w[i];
	MC.wc3_2t[i] = wc3_2t[i];
	MC.wc3_3n = wc3_3n;
	MC.wc3_3w[i] = wc3_3w[i];
	MC.wc3_3t[i] = wc3_3t[i];
	
	MC.S1_X_N = ss1_1n;
	MC.S1_X_ch[i] = (ss1_1w[i]-24)*1.28;
	MC.S1_X_ch[i] = ss1_1x[i];
	MC.S1_X_t[i] = ss1_1t[i];
       	MC.S1_Y_N = ss1_2n;
	MC.S1_Y_ch[i] = (ss1_2w[i]-24)*1.28;
	MC.S1_Y_ch[i] = ss1_2y[i];
	MC.S1_Y_t[i] = ss1_2t[i];
	
	
	MC.S2_X_N = ss2_1n;
	MC.S2_X_ch[i] = (ss2_1w[i]-24)*1.28;
	MC.S2_X_ch[i] = ss2_1x[i];
	MC.S2_X_t[i] = ss2_1t[i];
       	MC.S2_Y_N = ss2_2n;
	MC.S2_Y_ch[i] = (ss2_2w[i]-24)*1.28;
	MC.S2_Y_ch[i] = ss2_2y[i];
	MC.S2_Y_t[i] = ss2_2t[i];
	
	//     MC.S3_X_N = Nclustx;
	//	MC.S3_X_ch[i] = (ss3_1w[i]-24)*1.28;
        //      MC.S3_X_t[i] = ss3_1t[i];
	
       	// MC.S3_Y_N = Nclusty;
	// MC.S3_Y_ch[i] = (ss3_2w[i]-24)*1.28;
	// MC.S3_Y_t[i] = ss3_2t[i];
	
      }




    TrReco.Process(MC);
  ntracks=0;
  ntracks = MC.trks[2].GetN();
  // Original position (commented Apr. 16/19)
  //Float_t Z = 55.85; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  //Float_t Z_1 = 51.85; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  // Aleksey's new position (Apr. 2019) Commented June 5/19
  //Float_t Z = 56.235; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  //Float_t Z_1 = 52.235; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  // Aleksey's new position (May 2019) 
  Float_t Z = 56.755; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  // WC3 reco shift test (June 17 2019) 
  //Float_t Z = 46.755; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  Float_t Z_1 = 52.581; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  // Tristan, Oct. 3/17, WC3 Z shift test
  //Float_t Z = 57.35; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  //Float_t Z_1 = 53.35; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
  Float_t minr = 10000000;
  Float_t X = 10000000;
  Float_t X_1 = 10000000;
  Float_t Y = 10000000;
  Int_t tracknum = 0; 
  Int_t tracknum_max = 0; 
  Float_t maxr = -1000000;
  // Added by Tristan, Sept. 20/13, for acceptance definition thingy
  if (ntracks > 0) 
  {
    for (int u = 0; u < ntracks; u++) 
    {    
      X = MC.trks[2].GetTrk(u).tx*Z + MC.trks[2].GetTrk(u).x0;
      Y = MC.trks[2].GetTrk(u).ty*Z + MC.trks[2].GetTrk(u).y0;
      R = sqrt(X * X + Y * Y);  
      if(u == 0) Old_Rwc3_2 = R;
      if (R < minr) {tracknum = u; minr = R;}
      if (R > minr) {tracknum_max = u; maxr = R;}

      // Tristan, June 26/17
      PTrack& trkD = MC.trks[2].GetTrk(0);
      X = (trkD.tx*Z+trkD.x0); //X at WC3_2
      Y = (trkD.ty*Z+trkD.y0); //Y at WC3_2
      Rwc3_2_besttrack = sqrt(X*X+Y*Y); // R at WC3_2

      //if (ntracks > 1 && u == 0) cout << "ntracks: " << ntracks << endl << "R1: " << R << endl;
      //if (ntracks > 1 && u == 1) cout << "R2: " << R << endl;
      //if (ntracks > 2 && u == 2) cout << "R3: " << R << endl;
    }    
    //if (ntracks > 1) cout << "tracknum: " << tracknum << endl;
  }

    PTrack& trkA = MC.trks[0].GetTrk(0);
    PTrack& trkB = MC.trks[1].GetTrk(0);
    //PTrack& trkC = MC.trks[2].GetTrk(0);
    PTrack& trkC = MC.trks[2].GetTrk(tracknum);
    // Tristan, June 26/17
    PTrack& trkE = MC.trks[2].GetTrk(tracknum_max);
    
    //Float_t Z = 55.855; // z at WC3_2 : eventually read this variable from the geometry file!!!!!
    if(MC.trks[2].GetN()>0)
      {
	//Float_t X = (trkC.tx*Z+trkC.x0); //X at WC3_2
	//Float_t Y = (trkC.ty*Z+trkC.y0); //Y at WC3_2
	//Float_t R = sqrt(X*X+Y*Y); // R at WC3_2
        X = (trkC.tx*Z+trkC.x0); //X at WC3_2
        X_1 = (trkC.tx*Z_1+trkC.x0); //X at WC3_2
        Y = (trkC.ty*Z+trkC.y0); //Y at WC3_2
        R = sqrt(X*X+Y*Y); // R at WC3_2
	Xwc3_1 =X_1;
	Xwc3_2 =X;
	Ywc3_2 =Y;
	Zwc3_2 =Z;
	Rwc3_2= R;
        // Tristan, June 26/17
        X = (trkE.tx*Z+trkE.x0); //X at WC3_2
        Y = (trkE.ty*Z+trkE.y0); //Y at WC3_2
	Rwc3_2_maxrad = sqrt(X*X + Y*Y);

	// Tristan, Oct. 24/17
	s3wc3x0 = trkC.x0;
	s3wc3y0 = trkC.y0;
	s3wc3tx = trkC.tx;
	s3wc3ty = trkC.ty;

	/*
	  cout << " eventID = " << eventID << "ntracks = " << ntracks << "  tracknum = " << tracknum;
	  cout << " X Y tx ty = " << X << "  " << Y << "  " << trkC.tx << "  " << trkC.ty << endl;
	*/
      }
    
    // Tristan July 20/17    
    if(MC.trks[0].GetN()>0)
      {
	//Float_t X = (trkC.tx*Z+trkC.x0); //X at WC3_2
	//Float_t Y = (trkC.ty*Z+trkC.y0); //Y at WC3_2
	//Float_t R = sqrt(X*X+Y*Y); // R at WC3_2
	Xwc12 = trkA.x0; //X at WC3_2
	Ywc12 = trkA.y0; //Y at WC3_2
	txwc12 = trkA.tx;
	tywc12 = trkA.ty;
      }
    
    // Tristan, July 20/17
    if (MC.trks[1].GetN() > 0)
    {
      s12x0 = trkB.x0;
      s12y0 = trkB.y0;
      s12tx = trkB.tx;
      s12ty = trkB.ty;
    }
    //  Calculating the KINK variable
    if (MC.trks[0].GetN()>0 && MC.trks[1].GetN()>0)
      {
	double cs = (trkA.tx*trkB.tx + trkA.ty*trkB.ty+1.0) /
	  sqrt((trkA.tx*trkA.tx+trkA.ty*trkA.ty+1.0)*(trkB.tx*trkB.tx+trkB.ty*trkB.ty+1.0));
	kv = acos(cs)*57.296;
      }
    

    //vertex Z 
    if(MC.trks[1].GetN() > 0 && MC.trks[2].GetN() > 0) {
      zv = -((trkB.x0-trkC.x0)*(trkB.tx-trkC.tx)+
	     (trkB.y0-trkC.y0)*(trkB.ty-trkC.ty))/
	((trkB.tx-trkC.tx)*(trkB.tx-trkC.tx)+
	 (trkB.ty-trkC.ty)*(trkB.ty-trkC.ty));
    }
    
    // Phi angle of the decay positron
    if(MC.trks[2].GetN()>0) phie = atan2(trkC.ty,trkC.tx)*180.0/M_PI;
    // Theta angle of the decay positron
    if(MC.trks[2].GetN()>0) thetae = atan(sqrt(trkC.tx*trkC.tx+trkC.ty*trkC.ty))*180.0/M_PI;
    //USI with highest energy
    double etmp=0;
    csich = -999;dphi=-999;
    for (int i=0;i<21;i++ ){
      if (csiall->USIn[i] > etmp){
	etmp = csiall->USIn[i];
	csich = i;
      }
    }

    //Various conversions:
    if (csich!=-999 && phie!=-999){
      if (csich>0) csich = -(csich-21);//couterclockwise direction (data=clockwise)
      csich -= 4.8;
      if (csich<0) csich +=21;
      csich = csich*360/21.0;
      //dphi variable:
      dphi = fabs(csich-(phie+180));
    }

    if (dphi>180) dphi -=180;
    
    if (usiZn>0) usiZ /= usiZn;

    trks[0]=(MC.trks[0]);
    trks[1]=(MC.trks[1]);
    trks[2]=(MC.trks[2]);
    
    eventnumber = entry; 

    OutputTree->Fill();
    
    energyDeposit = 0;
    
    hitwc1_1 = 0;hitwc1_2 = 0;hitwc1_3 = 0;
    hitwc2_1 = 0;hitwc2_2 = 0;hitwc2_3 = 0;
    hitwc3_1 = 0;hitwc3_2 = 0;hitwc3_3 = 0;
    
    hitss1_1 = 0;hitss1_2 = 0;
    hitss2_1 = 0;hitss2_2 = 0;
    hitss3_1 = 0;hitss3_2 = 0;
    
    hitb1 = 0;
    hitb2 = 0;
    hittg = 0;
    hitt1 = 0;
    hitt2 = 0;
    hitv2 = 0;
    hitv3 = 0;
		
    Tpos = -10000;
    postime = 100000;
    pitime = 100000;
    posmin = 100000;//shintaro
    binaph = -10000;
    binaq = -10000;
    birksph = -10000;
    birksq = -10000;
    birksphr = -10000;
    birksqr = -10000;

    for (int i=0;i<48;i++) ampx[i]=0;
    for (int i=0;i<48;i++) ampy[i]=0;
    for (int i=0;i<48;i++) ampxn[i]=-999;
    for (int i=0;i<48;i++) ampyn[i]=-999;
    for (int i=0;i<48;i++) ampxt[i]=-999;
    for (int i=0;i<48;i++) ampyt[i]=-999;
    Nclustx = 0;
    Nclusty = 0;
    for (int i=0;i<MAX_NUM_HITS;i++) s3_x_posi[i] = 0;
    for (int i=0;i<MAX_NUM_HITS;i++) s3_y_posi[i] = 0;

    
    // Tristan, Sep. 29/17
    Xwc3_1 = 999;
    Rwc3_2 = Xwc3_2 = Ywc3_2 = Zwc3_2 = 999;
    Old_Rwc3_2=999;
    // Tristan, June 26/17
    Rwc3_2_maxrad = 999;
    Rwc3_2_besttrack = 999;
    // Tristan, July 20/17
    Xwc12 = Ywc12 = txwc12 = tywc12 = 999;
    s12x0 = 999;
    s12y0 = 999;
    s12tx = 999;
    s12ty = 999;
    
    // Tristan, Oct. 24/17
    s3wc3x0 = 999;
    s3wc3y0 = 999;
    s3wc3tx = 999;
    s3wc3ty = 999;
    
    if (entry%1000==0) cout << "Entries processed: " << entry << "\r" << flush;
    
  }
  
  cout << endl;
  
}

