#include <fstream>
#include <vector>
#include <ConfigFile.h>
#include "TrReco.h"
//#include "EventClass.h"
#include "MCtruthClass.h"

TrRecoClass::TrRecoClass() {

  tkrs[0] = PTracker("WC12",6);
  tkrs[0].GetPln(0).SetName("WC1_1");
  tkrs[0].GetPln(1).SetName("WC1_2");
  tkrs[0].GetPln(2).SetName("WC1_3");
  tkrs[0].GetPln(3).SetName("WC2_1");
  tkrs[0].GetPln(4).SetName("WC2_2");
  tkrs[0].GetPln(5).SetName("WC2_3");
        
  tkrs[1] = PTracker("S12",4);
  tkrs[1].GetPln(0).SetName("SS1_Y");
  tkrs[1].GetPln(1).SetName("SS1_X");
  tkrs[1].GetPln(2).SetName("SS2_Y");
  tkrs[1].GetPln(3).SetName("SS2_X");

  tkrs[2] = PTracker("TgS3WC3",7);
  tkrs[2].GetPln(0).SetName("Tg_X");   // Artificial Tg center hit to have tracks chi2-ordered 
  tkrs[2].GetPln(1).SetName("Tg_Y");   // even one of the true planes is missing
  tkrs[2].GetPln(2).SetName("SS3_X");
  tkrs[2].GetPln(3).SetName("SS3_Y");
  tkrs[2].GetPln(4).SetName("WC3_1");
  tkrs[2].GetPln(5).SetName("WC3_2");
  tkrs[2].GetPln(6).SetName("WC3_3");
        
  /*tkrs[2] = PTracker("S3WC3",5);
  tkrs[2].GetPln(0).SetName("SS3_Y");
  tkrs[2].GetPln(1).SetName("SS3_X");
  tkrs[2].GetPln(2).SetName("WC3_1");
  tkrs[2].GetPln(3).SetName("WC3_2");
  tkrs[2].GetPln(4).SetName("WC3_3");*/

  tkrs[3] = PTracker("TgS3",4);
  tkrs[3].GetPln(0).SetName("Tg_Y");
  tkrs[3].GetPln(1).SetName("Tg_X");
  tkrs[3].GetPln(2).SetName("SS3_Y");
  tkrs[3].GetPln(3).SetName("SS3_X");
  
  tkrs[4] = PTracker("S3WC3m",4);
  tkrs[4].GetPln(0).SetName("SS3_X");
  tkrs[4].GetPln(1).SetName("SS3_Y");
  tkrs[4].GetPln(2).SetName("WC3_1");
  tkrs[4].GetPln(3).SetName("WC3_2");
}

void TrRecoClass::Init(ConfigFile &Conf) {

  for(int i=0; i<NTKRS; i++) {
 
    man().ISrv().GeomInit(tkrs[i], Conf);
 
    man().ISrv().LoadInit(tkrs[i], Conf);
 
    trks.push_back(new PTracks());
  }
}


void TrRecoClass::setBranches(TTree *T) {
  T->Branch("TrksWC12","PTracks",&trks[0],16000,1);
  T->Branch("TrksSs12","PTracks",&trks[1],16000,1);
  T->Branch("TrksDwsm","PTracks",&trks[2],16000,1);
}



void TrRecoClass::Process(MCtruthClass &MC) {

  //  if(E.BeamPi_VT_N<1 || E.e_time_VT_N<1) {
  //  for(int i=0; i<NTKRS; i++) {
  //    trks[i]->Clear(); 
  //    trks[i]->N=0;
  //    trks[i]->status =  trks[i]->status | PISrv::kTR_NOTIMINFO;
  //  }
  //  This was a no hit condition.
  // return;
    //  }

double PiTime = MC.PiTime; //E.BeamPi_VT_t[0]*0.625;
double eTime  = MC.PosTime; //e_time_VT_t[0]*0.625;

  // Loading hit info into trackers
  for(int i=0; i<NTKRS; i++) {
    man().ISrv().Load(tkrs[i],MC);
  }

  // Clusterize hits in WC
  tkrs[0].Clusterize(PiTime-35.0,PiTime+30.0);
  for(int i=0; i<tkrs[2].GetN(); i++) {
    if(tkrs[2].GetPln(i).GetName().substr(0,2) == "WC")
      tkrs[2].GetPln(i).Clusterize(eTime-20.0,eTime+35.0);
  }
  for(int i=0; i<tkrs[4].GetN(); i++) {
    if(tkrs[4].GetPln(i).GetName().substr(0,2) == "WC")
      tkrs[4].GetPln(i).Clusterize(eTime-20.0,eTime+35.0);
  }

  // Count real missed planes in S3WC3 tracker
  int nzpl=0;
  for(int i=2; i<tkrs[2].GetN(); i++) {
    if(tkrs[2].GetPln(i).GetN()>0) nzpl++;
  }
  

  // not use "virtual" Tg hits except when one or more planes are missed
  if(nzpl>=5) {
    tkrs[2].GetPln(0).GetHits().clear();
    tkrs[2].GetPln(1).GetHits().clear();
  }

  // Tracking starts
  
  for(int i=0; i<NTKRS; i++) {
    if(tkrs[i].GetName()=="TgS3") continue;

 //   if(i==2 && nzpl<4) { // no fit in S3WC3 when more than one plane is missed
 //     trks[i]->Clear(); 
 //     continue;
 //   }   
    
   if (i == 2 && nzpl >= 4)
   {
    man().CSrv().FindTrks(tkrs[i], *trks[i]); // Find tracks; trks[i] is clean inside
    man().CSrv().Fit(*trks[i]);               // Fit all found tracks
    man().ISrv().CleanTrks(*trks[i]);         // Clean of fake tracks

    // Fill the memeber N, number of tracks, 
    // for convinience to work with trees interactively
    trks[i]->N = trks[i]->GetN();

    if(i<3)    MC.trks[i] = *trks[i];
   }
   else if (i == 2 && nzpl < 4)
   {
     trks[i]->Clear();

     trks[i]->N = trks[i]->GetN();
     if(i<3)    MC.trks[i] = *trks[i];
   }
   else 
   {
    man().CSrv().FindTrks(tkrs[i], *trks[i]); // Find tracks; trks[i] is clean inside
    man().CSrv().Fit(*trks[i]);               // Fit all found tracks
    man().ISrv().CleanTrks(*trks[i]);         // Clean of fake tracks

    // Fill the memeber N, number of tracks, 
    // for convinience to work with trees interactively
    trks[i]->N = trks[i]->GetN();

    if(i<3)    MC.trks[i] = *trks[i];     
   }
    //    if(i==2)  cout<< "Number of tracks found in DS tracker i="<<i<<" : N="<<trks[i]->GetN()<<endl;

     




    
    /*
    if(i!=2) continue;
    cout<<"**** Check Tracking, Event "<<E.eventID<<endl;
    printf("Tracker: %d, Trks found: %d, Status: 0x%x\n",i+1,trks[i]->GetN(),trks[i]->status);
    for(int j=0; j<trks[i]->GetN(); j++) {
      cout<<"tx ty x0 y0 "<<trks[i]->GetTrk(j).tx<<" "<<trks[i]->GetTrk(j).ty<<" "
	  <<trks[i]->GetTrk(j).x0<<" "<<trks[i]->GetTrk(j).y0<<endl;
    }
    //trks[i]->DumpTrks();

    cout<<"proot tracks\n";
    for(int j=0; j<E.trks[i].GetN(); j++) {
      cout<<"tx ty x0 y0 "<<E.trks[i].GetTrk(j).tx<<" "<<E.trks[i].GetTrk(j).ty<<" "
	  <<E.trks[i].GetTrk(j).x0<<" "<<E.trks[i].GetTrk(j).y0<<endl;
    }



    */
  }




   
}
