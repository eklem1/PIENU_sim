//////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Anthony Hillairet, April 2008
///
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MCtruthClass_h
#define MCtruthClass_h
// Include de C++
using namespace std;

#include <iostream>
#include <string>
#include <map>
#include <math.h>

// Include de ROOT
// #include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TBranch.h"
// #include "TMath.h"
#include "TObjArray.h"
#include "TVectorT.h"
#include "TObjString.h"

// Include Program
#include "log4cpp/Category.hh"
#include "ConfigFile.h"
//#include "FuncLib.h"
//#include "DetectorGeo.h"
#include <reco_classes.h>

//##########################################################
// Copy the following lines at the beginning
// of MCtruthClass.h right after the #include calls

#define MAXB1_N         100
#define MAXB2_N         100
#define MAXTG_N         100
#define MAXT1_N         100
#define MAXT2_N         100
#define MAXWC1_1_N              100
#define MAXWC1_2_N              100
#define MAXWC1_3_N              100
#define MAXWC2_1_N              100
#define MAXWC2_2_N              100
#define MAXWC2_3_N              100
#define MAXWC3_1_N              100
#define MAXWC3_2_N              100
#define MAXWC3_3_N              100
#define MAXS1_X_N               100
#define MAXS1_Y_N               100
#define MAXS2_X_N               100
#define MAXS2_Y_N               100
#define MAXS3_X_N               100
#define MAXS3_Y_N               100



class MCtruthClass{
	public :
		MCtruthClass()	{};
		~MCtruthClass()	{};

		void Init();// ConfigFile &C, log4cpp::Category *L );
				bool InitGeometry(ConfigFile &C);
		void InitVar( TTree* T);
		void GetVar( TTree* T, const char* Branch, const char* Leaf, void* V);
		bool Load( );

		bool Exists( const char* Name)	{return ExistList[Name];}

                log4cpp::Category *Log;
		// Geometry class of the detector
		//DetectorGeo *geo;

		// Track info is read in these objects
		PTracks         trks[3];
		PTracks*        ptrks[3];


		
		//__________________________ Parameters _________________________//
		// Here are the general parameters, fixed like a physical constant
		// or parameters defined in the method Init

		
	//##########################################################
// Copy the following lines in the public section
// of the definition of the class MCtruthClass in MCtruthClass.h

		Int_t static const MAX_NUM_HITS = 50;

                Float_t         PosTime;
                Float_t         PiTime;
	        Float_t         Tpos;
	       	Float_t         PosMin;


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


  Int_t S1_Y_N;
  Float_t S1_Y_ch[MAX_NUM_HITS];
  Float_t S1_Y_t[MAX_NUM_HITS];
  Int_t S1_X_N;
  Float_t S1_X_ch[MAX_NUM_HITS];
  Float_t S1_X_t[MAX_NUM_HITS];



  Int_t S2_Y_N;
  Float_t S2_Y_ch[MAX_NUM_HITS];
  Float_t S2_Y_t[MAX_NUM_HITS];
  Int_t S2_X_N;
  Float_t S2_X_ch[MAX_NUM_HITS];
  Float_t S2_X_t[MAX_NUM_HITS];


  Int_t S3_Y_N;
  Float_t S3_Y_ch[MAX_NUM_HITS];
  Float_t S3_Y_t[MAX_NUM_HITS];
  Int_t S3_X_N;
  Float_t S3_X_ch[MAX_NUM_HITS];
  Float_t S3_X_t[MAX_NUM_HITS];

  Float_t kv;
  Float_t zv;
  Float_t Rmin;



		//______________ New variables (not tree variables) ______________//

		// Define here variables that will be filled in InitVar at each entry


		Double_t	BeamSum;

	private :
		map <const char*, bool>ExistList;



};


#endif
