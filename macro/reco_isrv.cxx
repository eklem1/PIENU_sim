/** \file reco_isrv.cpp Interface Services implementation. */

#include <reco_isrv.h>

#include <reco_manager.h>
#include <cmath>
//#include "EventClass.h"
#include "MCtree.h"


const double d2r = 0.01745329252;

void PISrv::GeomInit(PTracker& tkr, ConfigFile& config) {

  // Get info from config file

  cout<<"PISrv::GeomInit: Reading geometry info...\n";
  for(int i=0; i<tkr.GetN(); i++) {
    PPlane& pln = tkr.GetPln(i);
    string  name = pln.GetName();
    pln.a3 = config.read<double>(name+"/a3");
    pln.x  = config.read<double>(name+"/x");
    pln.y  = config.read<double>(name+"/y");
    pln.z  = config.read<double>(name+"/z");
    pln.x0 = config.read<double>(name+"/x0");
    pln.step = config.read<double>(name+"/step");
    pln.sig = config.read<double>(name+"/sig");

    pln.offset = cos(d2r*pln.a3)*pln.x+sin(d2r*pln.a3)*pln.y;
  }

  // debug print out
  for(int i=0; i<tkr.GetN(); i++) {
    PPlane& pln = tkr.GetPln(i);
    string  name = pln.GetName();
    cout << name<<" z="<< pln.z<<", phi="<<pln.a3<<", step="<<pln.step<<", x0="<<pln.x0<<endl;
    
  }
}

void PISrv::LoadInit(PTracker& tkr, ConfigFile& config) {

  cout<<"PISrv::LoadInit: Reading time offsets info...\n";
  for(int i=0; i<tkr.GetN(); i++) {
    PPlane& pln = tkr.GetPln(i);
    string  name = pln.GetName();
    t0[name] = config.read<double>(name+"/t0");
    cout << name << " t0=" << t0[name] << endl;
  }

}

void PISrv::Load(PTracker& tkr, MCtruthClass& MC) {
  load(tkr.ChePln("WC1_1"), MC.wc1_1n,  MC.wc1_1w, MC.wc1_1t, t0["WC1_1"] );
   load(tkr.ChePln("WC1_2"), MC.wc1_2n, MC.wc1_2w, MC.wc1_2t, t0["WC1_2"] );
   load(tkr.ChePln("WC1_3"), MC.wc1_3n, MC.wc1_3w, MC.wc1_3t, t0["WC1_3"] );
   load(tkr.ChePln("WC2_1"), MC.wc2_1n, MC.wc2_1w, MC.wc2_1t, t0["WC2_1"] );
   load(tkr.ChePln("WC2_2"), MC.wc2_2n, MC.wc2_2w, MC.wc2_2t, t0["WC2_2"] );
   load(tkr.ChePln("WC2_3"), MC.wc2_3n, MC.wc2_3w, MC.wc2_3t, t0["WC2_3"] );
   load(tkr.ChePln("WC3_1"), MC.wc3_1n, MC.wc3_1w, MC.wc3_1t, t0["WC3_1"] );
   load(tkr.ChePln("WC3_2"), MC.wc3_2n, MC.wc3_2w, MC.wc3_2t, t0["WC3_2"] );
   load(tkr.ChePln("WC3_3"), MC.wc3_3n, MC.wc3_3w, MC.wc3_3t, t0["WC3_3"] );
   
   load(tkr.ChePln("SS1_Y"), MC.S1_Y_N, (MC.S1_Y_ch), MC.S1_Y_t, t0["SS1_Y"], 1.);
   load(tkr.ChePln("SS1_X"), MC.S1_X_N, (MC.S1_X_ch), MC.S1_X_t, t0["SS1_X"], 1.);
   load(tkr.ChePln("SS2_Y"), MC.S2_Y_N, (MC.S2_Y_ch), MC.S2_Y_t, t0["SS2_Y"], 1.);
   load(tkr.ChePln("SS2_X"), MC.S2_X_N, (MC.S2_X_ch), MC.S2_X_t, t0["SS2_X"], 1.);
   load(tkr.ChePln("SS3_Y"), MC.S3_Y_N, (MC.S3_Y_ch), MC.S3_Y_t, t0["SS3_Y"],1.);
   load(tkr.ChePln("SS3_X"), MC.S3_X_N, (MC.S3_X_ch), MC.S3_X_t, t0["SS3_X"],1.);

    PPlane &pln = tkr.GetPln(0);
    string name = pln.GetName();

    // Commented by Tristan, Mar. 21, to test effect because Dick's code on Cedar doesn't have this. To be uncommented.

    // Special Tg "fiducial" hit
    if(name.substr(0,2)=="Tg") {

      vector<PHit>& hits = pln.GetHits();
      hits.assign(1,PHit()); // reinitizalize the vector of hits
      hits[0].x = 0;
      hits[0].t = 0;
      hits[0].s = 70/sqrt(12.0); // sigma corresponds to tranverse dismension of the Tg
      hits[0].w = 1.0/(hits[0].s*hits[0].s);
      hits[0].ppln = &pln;

      PPlane &pln = tkr.GetPln(1);

      // Changed by Tristan, June 14/19, after learning the old code, when used in Clark, doesn't reproduce the proot tracking
      vector<PHit>& hits2 = pln.GetHits();
      hits2.assign(1,PHit()); // reinitizalize the vector of hits
      hits2[0].x = 0;
      hits2[0].t = 0;
      hits2[0].s = 70/sqrt(12.0); // sigma corresponds to tranverse dismension of the Tg
      hits2[0].w = 1.0/(hits2[0].s*hits2[0].s);
      hits2[0].ppln = &pln;

      /*hits = pln.GetHits();
      hits.assign(1,PHit()); // reinitizalize the vector of hits
      hits[0].x = 0;
      hits[0].t = 0;
      hits[0].s = 70/sqrt(12.0); // sigma corresponds to tranverse dismension of the Tg
      hits[0].w = 1.0/(hits[0].s*hits[0].s);
      hits[0].ppln = &pln;*/
    } // End: Tg


//    cout << " Tracker1=========================="<< endl;
//    cout << " hits in WC1_1 " << MC.wc1_1n << endl;
//    cout << " hits in WC1_2 " << MC.wc1_2n << endl;
//    cout << " hits in WC1_3 " << MC.wc1_3n << endl;
//    cout << " hits in WC2_1 " << MC.wc2_1n << endl;
//    cout << " hits in WC2_2 " << MC.wc2_2n << endl;
//    cout << " hits in WC2_3 " << MC.wc2_3n << endl;
//    cout << " Tracker2=========================== "<< endl;
//    cout << " hits in SS1_X " << MC.S1_X_N << endl;
//    cout << " hits in SS1_Y " << MC.S1_Y_N << endl;
//    cout << " hits in SS2_X " << MC.S2_X_N << endl;
//    cout << " hits in SS2_Y " << MC.S2_Y_N << endl;
//    if(MC.S1_X_N>0 && MC.S1_Y_N>0) cout <<"S1:  X="<<MC.S1_X_ch[0]<< " Y="<<MC.S1_Y_ch[0]<<endl;

//    if(MC.S2_X_N>0 && MC.S2_Y_N>0) cout <<"S2:  X="<<MC.S2_X_ch[0]<< " Y="<<MC.S2_Y_ch[0]<<endl;



 // cout << " Tpos is:" << Tpos << endl;
 //" "<< E.WC1_1_W<<" "<< E.WC1_1_t <<" "<<t0["WC1_1"]<< endl;
 /*
 catch(out_of_range& err) {
   cout << "caught: " << err.what() << endl;
   exit(1);
 }*/
}

void PISrv::load(PPlane* pln, int N, int* W, float* t, double t0) {
  if(!pln) return;
  
  string name = pln->GetName();
  if(name.substr(0,2) != "WC") return;

  bool wc3 = false;
  if(name.substr(0,3) == "WC3") wc3 = true;

  vector<PRawHit>& hits = pln->GetRawHits();
  hits.assign(N,PRawHit()); // reinitizalize the vector of hits
  for(int i=0; i<N; i++) {
    hits[i].i = W[i];
    hits[i].t = t[i]; // AS *0.625-t0;

    // Wire swap correction with WC3
    //    if(wc3 && (W[i]/8)%2 == 1) {
    //  if(W[i]%2 == 0) hits[i].i = W[i]+1;   // AS commented out on Dima's hint
    //  else hits[i].i = W[i]-1;
    // }

  }
}

void PISrv::load(PPlane* pln, int N, float* x, float* t, double t0,double q) {
  if(!pln) return;
  
  string name = pln->GetName();
  if(name.substr(0,2) != "SS") return;


  vector<PHit>& hits = pln->GetHits();
  hits.clear(); // reinitizalize the vector of hits
  for(int i=0; i<N; i++) {
    //    if(name.substr(0,3) == "SS3" &&
    //   ! (15<t[i]&&t[i]<26)) continue; // hardcoded time window for S3

    hits.push_back(PHit());
    //    if(run<31682) hits.back().x = x[i]*1.28/1.20+pln->x0;
    //    else
          hits.back().x = x[i]+pln->x0;
	  hits.back().t = t[i];///0.06-t0;
    double frac, strip;
    frac = modf((x[i]+30.08)/1.28,&strip);
    hits.back().ic.push_back(int(strip));
    hits.back().tc.push_back(double(t[i]));
    if(frac>0.01) {
      hits.back().ic.push_back(int(strip+1));
      hits.back().tc.push_back(double(t[i]));
    }
    
    hits.back().s = pln->sig;
    //    if(0.25<Q[i]/Q2[i] && Q[i]/Q2[i]<=0.75) hits.back().s = pln->sig/4.0;
    hits.back().w = 1.0/(hits.back().s*hits.back().s);
    hits.back().ppln = pln;
  }
}


void PISrv::CleanTrks(PTracks& trks) {
  string nam = trks.GetTracker()->GetName();
  if(nam == "WC12")  cleanTrks1(trks);
  if(nam == "S3WC3") cleanTrks3(trks);
  if(nam == "TgS3WC3") cleanTrks3tg(trks);

  return;
}

void PISrv::cleanTrks1(PTracks& trks) {
  const double kMinProb = 0.001;

  PTracker* tkr = trks.GetTracker(); 

  // This fuction is specifically desinged for WC12 tracker,
  // should not be used on others.
  if(tkr->GetName() != "WC12") return;

  trks.status = 0;

  // No trakcs, nothing to do
  if(trks.GetN()<1) return;

  // (1) Fix one track events with bad "prob" by eliminating 
  // a worse outlying point
  if(trks.GetN()==1&&trks.GetTrk(0).npts==tkr->GetN()&&trks.GetTrk(0).prob<kMinProb) {
    PTrack& trk = trks.GetTrk(0);
    double max = -1;
    int ix=-1; // find index of a hist with worse nomalized residual
    for(int i=0; i<trk.npts; i++) {
      if(fabs(trk.resn[i])>max) {
        max = fabs(trk.resn[i]);
        ix = i;
      }
    }
    trk.GetHit(ix) = 0; // remove the worst point
    man().CSrv().Fit(trk); // refit

    trks.status = trks.status | kTR_1TRKREFIT;
    return;
  }

  // Do nothing if one good track is found right away
  if(trks.GetN()==1) return;

  // No tools for  4 points tracks
  if(trks.GetN()>0&&trks.GetTrk(0).npts<=4) return;
  
  // (2) Kills tracks with insane chi2. Doesn't care if all are killed
  // (that too messy and unprobable) 
  for(int i=0; i<trks.GetN(); i++) {
    if(trks.GetTrk(i).prob<kMinProb) {
      trks.KillTrk(i); // mark for killing
    }
  }
  trks.Clean(); // remove marked
  trks.status = trks.status | kTR_CHI2CLEANUP;
  if(trks.GetN()<=1) return; // ambiguity fixed, return
  

  // (3) Start with best tracks and remove its "doubles", ones different
  // by 1 point only
  trks.Sort(); // from best to worse
  for(int k=0; k<trks.GetN(); k++) { // first, second best track, etc
    for(int i=k+1; i<trks.GetN(); i++) {
      if(man().CSrv().TrkDiff(trks.GetTrk(k),trks.GetTrk(i))==1) {
        trks.KillTrk(i); // mark for killing
      }
    }
    trks.Clean(); // Note that this generally changes trks.GetN(), but it's ok
  }
  trks.status = trks.status | kTR_RMDOUBLES;
  if(trks.GetN()<=1) return; // ambiguity fixed, return

  // (4) If there are two tracks and one is much better only the best survive
  if(trks.GetN()==2 && trks.GetTrk(1).prob/trks.GetTrk(0).prob<0.16) {
    trks.KillTrk(1);
    trks.Clean();
    trks.status = trks.status | kTR_BESTOFTWO;
  }

}

void PISrv::cleanTrks3(PTracks& trks) {
  const double kMinProb = 0.001;

  PTracker* tkr = trks.GetTracker(); 

  // This fuction is specifically desinged for S3WC3 tracker,
  // should not be used on others.
  if(tkr->GetName() != "S3WC3") return;

  trks.status = 0;

  // No trakcs, nothing to do
  if(trks.GetN()<1) return;

  // (1) Fix all points one track events with bad "prob" by eliminating 
  // a worse outlying point
  if(trks.GetN()==1&&trks.GetTrk(0).npts==tkr->GetN()&&trks.GetTrk(0).prob<kMinProb) {
    PTrack& trk = trks.GetTrk(0);
    double max = -1;
    int ix=-1; // find index of a hist with worse nomalized residual
    for(int i=0; i<trk.npts; i++) {
      if(fabs(trk.resn[i])>max) {
        max = fabs(trk.resn[i]);
        ix = i;
      }
    }
    trk.GetHit(ix) = 0; // remove the worst point
    man().CSrv().Fit(trk); // refit

    trks.status = trks.status | kTR_1TRKREFIT;
    return;
  }

  // Do nothing with other one track events
  if(trks.GetN()==1) return;

  // No tools for  4 points tracks
  if(trks.GetN()>0&&trks.GetTrk(0).npts<=4) return;


  // (2) Kills tracks with insane chi2. Doesn't care if all are killed
  // (that too messy and unprobable) 
  for(int i=0; i<trks.GetN(); i++) {
    if(trks.GetTrk(i).prob<kMinProb) {
      trks.KillTrk(i); // mark for killing
    }
  }
  trks.Clean(); // remove marked
  trks.status = trks.status | kTR_CHI2CLEANUP;
  if(trks.GetN()<=1) return; // ambiguity fixed, return
  
  // (3) Start with best tracks and remove its "doubles", ones different
  // by 1 point only
  trks.Sort(); // from best to worse
  for(int k=0; k<trks.GetN(); k++) { // first, second best track, etc
    for(int i=k+1; i<trks.GetN(); i++) {
      if(man().CSrv().TrkDiff(trks.GetTrk(k),trks.GetTrk(i))==1) {
        trks.KillTrk(i); // mark for killing
      }
    }
    trks.Clean(); // Note that this generally changes trks.GetN(), but it's ok
  }
  trks.status = trks.status | kTR_RMDOUBLES;
  if(trks.GetN()<=1) return; // ambiguity fixed, return

  // (4) If there are two tracks and one is much better only the best survive
  if(trks.GetN()==2 && trks.GetTrk(1).prob/trks.GetTrk(0).prob<0.16) {
    trks.KillTrk(1);
    trks.Clean();
    trks.status = trks.status | kTR_BESTOFTWO;
  }

}


void PISrv::cleanTrks3tg(PTracks& trks) {
  const double kMinProb = 0.0001;

  PTracker* tkr = trks.GetTracker();

  // This fuction is specifically desinged for S3WC3 tracker,
  // should not be used on others.
  if(tkr->GetName() != "TgS3WC3") return;

  trks.status = 0;

  // No trakcs, nothing to do
  if(trks.GetN()<1) return;


  // Do nothing with other one track events
  if(trks.GetN()==1) return;

  // No tools for  4 points tracks
  if(trks.GetN()>0&&trks.GetTrk(0).npts<=4) return;


  // (1) Kills tracks with insane chi2. Doesn't care if all are killed
  // (that too messy and unprobable) 
  for(int i=0; i<trks.GetN(); i++) {
    if(trks.GetTrk(i).prob<kMinProb) {
      trks.KillTrk(i); // mark for killing
    }
  }
  trks.Clean(); // remove marked
  trks.status = trks.status | kTR_CHI2CLEANUP;
  if(trks.GetN()<=1) return; // ambiguity fixed, return

  // (2) Start with best tracks and remove its "doubles", ones different
  // by 1 point only
  trks.Sort(); // from best to worse
  for(int k=0; k<trks.GetN(); k++) { // first, second best track, etc
    for(int i=k+1; i<trks.GetN(); i++) {
      if(man().CSrv().TrkDiff(trks.GetTrk(k),trks.GetTrk(i))==1) {
        trks.KillTrk(i); // mark for killing
      }
    }
    trks.Clean(); // Note that this generally changes trks.GetN(), but it's ok
  }
  trks.status = trks.status | kTR_RMDOUBLES;
  if(trks.GetN()<=1) return; // ambiguity fixed, return

}


