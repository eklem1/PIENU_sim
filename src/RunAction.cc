
#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4ios.hh"

#include "TFile.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TTree.h"

#include "RunActionMessenger.hh"

#include "Randomize.hh"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
// for lock file
#include <fcntl.h> // for open()
#include <cerrno> // for errno 
#include <cstdio> // for perror()

// Default Constructor
//
RunAction::RunAction()
  : autoSeed(false)
{
    runActionMessenger = new RunActionMessenger(this);

    t1energyDeposit = 0;
    t1pathLength = 0;
    t3energyDeposit = 0;
    t3pathLength = 0;

    //    pionStopPosition = 0;
}

// Default Destructor
//
RunAction::~RunAction() {

    delete runActionMessenger;

}

// Open a ROOT file and create an Ntuple
//

void RunAction::OpenRoot() {


  int fd;

  do{
  fd=open("password.lck", O_WRONLY | O_CREAT | O_EXCL);
  
  //  G4cout << "We are opening the lock file"<<G4endl;
  //  G4cout << "fd = " << fd<<G4endl;
  }
  while (fd<0);

  
  // Intelligently select filenames to use for output
  std::ifstream fin;
  bool isOpen = false;
    int i = 1;
    std::string filename = "";
    std::string extension=".root";
    //char env;
    
    while (isOpen == false) {
      
        filename.clear();
        filename.insert(0,itoa(i,10));
	char *env = getenv("MCOUTPUT");
	filename.insert(0,env);
        filename.append(extension);
        fin.open(filename.c_str(),std::ios::in);

        if (fin.fail()) {

            std::cout << "Saving to: " << filename << std::endl;
            isOpen = true;

        } else {

            i++;

        }

        fin.close();
    }

    // Create a new Root file
    file = new TFile(filename.c_str(),"RECREATE");

    remove("password.lck");
    // Create TNtuple to store the hits
    hitTuple = new TNtuple("hits","Hits","eventID:volumeID0:volumeID1:volumeID2:energyDeposit:startX:startY:startZ:startT:stopX:stopY:stopZ:stopT:Ebirk:PID:CreatorProcess",40000);

    aTree =new TTree("tree","pienu");
    aTree = ((TTree*)gROOT->FindObject("tree"));
    aTree->Branch("eventID",&eventID,"eventID/I");
    aTree->Branch("PiStartX",PStartX,"PiStartX[4]/D");
    aTree->Branch("PiStartP",PStartP,"PiStartP[4]/D");
    aTree->Branch("PiDecayX",PDecayX,"PiDecayX[4]/D");
    aTree->Branch("PiDecayP",PDecayP,"PiDecayP[4]/D");
    aTree->Branch("MuStartX",MStartX,"MuStartX[4]/D");
    aTree->Branch("MuStartP",MStartP,"MuStartP[4]/D");
    aTree->Branch("MuDecayX",MDecayX,"MuDecayX[4]/D");
    aTree->Branch("MuDecayP",MDecayP,"MuDecayP[4]/D");
    aTree->Branch("PoStartX",EStartX,"PoStartX[4]/D");
    aTree->Branch("PoStartP",EStartP,"PoStartP[4]/D");
    aTree->Branch("PoStopX",EStopX,"PoStopX[4]/D");
    aTree->Branch("PoStopP",EStopP,"PoStopP[4]/D");

    /// Additions based on Tristan's work /////
    aTree->Branch("PosBremPreX",PosBremPreX,"PosBremPreX[4]/D");
    aTree->Branch("PosBremPreP",PosBremPreP,"PosBremPreP[4]/D");
    aTree->Branch("PosBremPostX",PosBremPostX,"PosBremPostX[4]/D");
    aTree->Branch("PosBremPostP",PosBremPostP,"PosBremPostP[4]/D");
    aTree->Branch("PosBhabhaPreX",PosBhabhaPreX,"PosBhabhaPreX[4]/D");
    aTree->Branch("PosBhabhaPreP",PosBhabhaPreP,"PosBhabhaPreP[4]/D");
    aTree->Branch("PosBhabhaPostX",PosBhabhaPostX,"PosBhabhaPostX[4]/D");
    aTree->Branch("PosBhabhaPostP",PosBhabhaPostP,"PosBhabhaPostP[4]/D");
    aTree->Branch("PosAnnihilPreX",PosAnnihilPreX,"PosAnnihilPreX[4]/D");
    aTree->Branch("PosAnnihilPreP",PosAnnihilPreP,"PosAnnihilPreP[4]/D");
    aTree->Branch("PosAnnihilPostX",PosAnnihilPostX,"PosAnnihilPostX[4]/D");
    aTree->Branch("PosAnnihilPostP",PosAnnihilPostP,"PosAnnihilPostP[4]/D");
    aTree->Branch("ElecBremPreX",ElecBremPreX,"ElecBremPreX[4]/D");
    aTree->Branch("ElecBremPreP",ElecBremPreP,"ElecBremPreP[4]/D");
    aTree->Branch("ElecBremPostX",ElecBremPostX,"ElecBremPostX[4]/D");
    aTree->Branch("ElecBremPostP",ElecBremPostP,"ElecBremPostP[4]/D");
    aTree->Branch("PosScatterPreX",PosScatterPreX,"PosScatterPreX[4]/D");
    aTree->Branch("PosScatterPreP",PosScatterPreP,"PosScatterPreP[4]/D");
    aTree->Branch("PosScatterPostX",PosScatterPostX,"PosScatterPostX[4]/D");
    aTree->Branch("PosScatterPostP",PosScatterPostP,"PosScatterPostP[4]/D");
    aTree->Branch("ElecScatterPreX",ElecScatterPreX,"ElecScatterPreX[4]/D");
    aTree->Branch("ElecScatterPreP",ElecScatterPreP,"ElecScatterPreP[4]/D");
    aTree->Branch("ElecScatterPostX",ElecScatterPostX,"ElecScatterPostX[4]/D");
    aTree->Branch("ElecScatterPostP",ElecScatterPostP,"ElecScatterPostP[4]/D");
    aTree->Branch("PosTotalBremE",&TotalBremEpos,"PosTotalBremE/D");
    aTree->Branch("ElecTotalBremE",&TotalBremEelec,"ElecTotalBremE[4]/D");
    aTree->Branch("PosTotalBhabhaE",&TotalBhabhaEpos,"PosTotalBhabhaE/D");

    // aTree->Branch("ElecinWC3PreX", ElecinWC3PreX, "ElecinWC3PreX[4]/D");
    // aTree->Branch("ElecinWC3PreP", ElecinWC3PreP, "ElecinWC3PreP[4]/D");
    // aTree->Branch("PrimPosinWC3PreX", PrimPosinWC3PreX, "PrimPosinWC3PreX[4]/D");
    // aTree->Branch("PrimPosinWC3PreP", PrimPosinWC3PreP, "PrimPosinWC3PreP[4]/D");
    // aTree->Branch("SecPosinWC3PreX", SecPosinWC3PreX, "SecPosinWC3PreX[4]/D");
    // aTree->Branch("SecPosinWC3PreP", SecPosinWC3PreP, "SecPosinWC3PreP[4]/D");

    aTree->Branch("PhotonuclearX",EPhNX,"PhotonuclearX[3]/D");
    aTree->Branch("BhabhaE",&EBh,"BhabhaE/D");
    aTree->Branch("MuPolarization",MuPol,"MuPolarization[3]/D");
    aTree->Branch("EMomentum",EMom,"EMomentum[3]/D");

    //Emma's addition - Feb 2025
    // aTree->Branch("annihil",&annihilCount,"annihil/I");
    // aTree->Branch("brems",&bremsCount,"brems/I");    
    // aTree->Branch("msc",&mscCount,"msc/I");    

  
    //Diagnostic Histograms

    // Particle Start Position/Momentum and Stop Position
    pionStartR = new TH1D("pionStartR", "Pion Start Position R Coordinate", 40, 30, 30);
    pionStartZ = new TH1D("pionStartZ", "Pion Start Position Z Coordinate", 20, -175, -125);
    pionStartPR = new TH1D("pionStartPR", "Pion Start Momentum R Component", 300, -75, 75);
    pionStartPZ = new TH1D("pionStartPZ", "Pion Start Momentum Z Component", 200, 50, 100);
    pionStartPRP = new TH1D("pionStartPRP", "Pion Start Momentum Pr/P", 100, -1, 1);
    pionStartPZP = new TH1D("pionStartPZP", "Pion Start Momentum Pz/P", 100, -1, 1);
    pionStopR = new TH1D("pionStopR", "Pion Stop Position R Coordinate", 50, 0, 50);
    pionStopZ = new TH1D("pionStopZ", "Pion Stop Position Z Coordinate", 20, -0.4, 0.6);
    muonStartR = new TH1D("muonStartR", "Muon Start Position R Coordinate", 50, 0, 50);
    muonStartZ = new TH1D("muonStartZ", "Muon Start Position Z Coordinate", 20, -0.4, 0.6);
    muonStartPR = new TH1D("muonStartPR", "Muon Start Momentum R Component", 300, -75, 75);
    muonStartPZ = new TH1D("muonStartPZ", "Muon Start Momentum Z Component", 300, -75, 75);
    muonStartPRP = new TH1D("muonStartPRP", "Muon Start Momentum Pr/P", 100, -1, 1);
    muonStartPZP = new TH1D("muonStartPZP", "Muon Start Momentum Pz/P", 100, -1, 1);
    positronStartR = new TH1D("positronStartR", "Positron Start Position R Coordinate", 50, 0, 50);
    positronStartZ = new TH1D("positronStartZ", "Positron Start Position Z Coordinate", 20, -0.4, 0.6);
    positronStartPR = new TH1D("positronStartPR", "Positron Start Momentum R Component", 300, -75, 75);
    positronStartPZ = new TH1D("positronStartPZ", "Positron Start Momentum Z Component", 300, -75, 75);
    positronStartPRP = new TH1D("positronStartPRP", "Positron Start Momentum Pr/P", 120, -1.2, 1.2);
    positronStartPZP = new TH1D("positronStartPZP", "Positron Start Momentum Pz/P", 50, -1, 1);
    triggeredPositronStartZ = new TH1D("triggeredPositronStartZ", "Triggered Positron Start Position Z Coordinate", 20, -0.4, 0.6);
    triggeredPositronStartPZ = new TH1D("triggeredPositronStartPZ", "Triggered Positron Start Momentum Z Component", 75, -75, 75);
    triggeredPositronStartPZP = new TH1D("triggeredPositronStartPZP", "Triggered Positron Start Momentum Pz/P", 50, -1, 1);

    // All T1 Histograms require hits in B1, B2, Target, and T1 to fill
    positronT1R = new TH1D("positronT1R", "Positron T1 Position R Coordinate", 50, 0, 10);
    positronT1Z = new TH1D("positronT1Z", "Positron T1 Position Z Coordinate", 50, 0, 7);
    positronT1PR = new TH1D("positronT1PR", "Positron T1 Momentum R Component", 300, -75, 75);
    positronT1PZ = new TH1D("positronT1PZ", "Positron T1 Momentum Z Component", 300, -75, 75);
    positronT1PRP = new TH1D("positronT1PRP", "Positron T1 Momentum Pr/P", 100, -1, 1);
    positronT1PZP = new TH1D("positronT1PZP", "Positron T1 Momentum Pz/P", 100, -1, 1);
    positronT1ExpectedR = new TH1D("positronT1ExpectedR", "Expected Radius at center of T1 from positron start direction", 50, 0, 10);
    positronT1DifferenceR = new TH1D("positronT1DifferenceR", "Difference between expected and actual radial coordinates", 50,0,50);
    positronT1DifferencePZP = new TH1D("positronT1DifferencePZP", "Difference between start PZ/P and PZ/P at T1 center", 20, 0, 0.8);
    positronT1dEdX = new TH1D("positronT1dEdX", "dE/dX for positrons in T1", 100, 0, 0.4);
    positronT1dEdXvPathLength = new TH2D("positronT1dEdXvPathLength", "Edep vs. Path Length for positrons in T1", 100, 0, 10, 100, 0, 6);

    // All T3 Histograms require hits in B1, B2, Target, T1, and T3 to fill
    positronT3R = new TH1D("positronT3R", "Positron T3 Position R Coordinate", 50, 0, 20);
    positronT3Z = new TH1D("positronT3Z", "Positron T3 Position Z Coordinate", 50, 50, 70);
    positronT3PR = new TH1D("positronT3PR", "Positron T3 Momentum R Component", 300, -75, 75);
    positronT3PZ = new TH1D("positronT3PZ", "Positron T3 Momentum Z Component", 300, -75, 75);
    positronT3PRP = new TH1D("positronT3PRP", "Positron T3 Momentum Pr/P", 50, -1, 1);
    positronT3PZP = new TH1D("positronT3PZP", "Positron T3 Momentum Pz/P", 50, -1, 1);
    positronT3ExpectedR = new TH1D("positronT3ExpectedR", "Expected Radius at entrance to T3 from positron start direction", 50, 0, 50);
    positronT3DifferenceR = new TH1D("positronT3DifferenceR", "Difference between expected and actual radial coordinates at entrance to T3", 50,0,50);
    positronT3DifferencePZP = new TH1D("positronT3DifferencePZP", "Difference between start Pz/P and PZ/P at T3 center", 20, 0, 0.8);
    positronT3dEdX = new TH1D("positronT3dEdX", "dE/dX for positrons in T3", 100, 0, 0.4);
    positronT3dEdXvPathLength = new TH2D("positronT3dEdXvPathLength","Edep vs. Path Length for positrons in T3", 100, 0, 10, 100, 0, 6);
    triggeredPositronT3ExpectedR = new TH1D("triggeredPositronT3ExpectedR", "Expected Radius at entrance to T3 for triggered events", 50, 0, 50);
    triggeredPositronT3DifferenceR = new TH1D("triggeredPositronT3DifferenceR", "Difference between expected and actual radial coordinates at entrance to T3", 50, 0, 50);
    triggeredPositronT3DifferencePZP = new TH1D("triggeredPositronT3DifferencePZP", "Difference between start Pz/P and Pz/P at T3 center for triggered events", 20, 0, 0.8);

    hitTuple->SetAutoSave(32000);
    aTree->SetAutoSave(32000);
}

// Write any remaining ROOT objects and close the file.
//
void RunAction::CloseRoot() {

    hitTuple->AutoSave();

    // Particle Start Position/Momentum and Stop Position
    pionStartR->Write(); // Pion Start Position R Coordinate
    pionStartZ->Write(); // Pion Start Position Z Coordinate
    pionStartPR->Write(); // Pion Start Momentum R Component
    pionStartPZ->Write(); // Pion Start Momentum Z Component
    pionStartPRP->Write(); // Pion Start Momentum Pr/P
    pionStartPZP->Write(); // Pion Start Momentum Pz/P
    pionStopR->Write(); // Pion Stop Position R Coordinate
    pionStopZ->Write(); // Pion Stop Position Z Coordinate
    muonStartR->Write(); // Muon Start Position R Coordinate (Same As Pion Stop Coordinate - Redundant)
    muonStartZ->Write(); // Muon Start Position Z Coordinate (Same As Pion Stop Coordinate - Redundant)
    muonStartPR->Write(); // Muon Start Momentum R Component
    muonStartPZ->Write(); // Muon Start Momentum Z Component
    muonStartPRP->Write(); // Muon Start Momentum Pr/P
    muonStartPZP->Write(); // Muon Start Momentum Pz/P
    positronStartR->Write(); // Positron Start Position R Coordinate
    positronStartZ->Write(); // Positron Start Position Z Coordinate
    positronStartPR->Write(); // Positron Start Momentum R Component
    positronStartPZ->Write(); // Positron Start Momentum Z Component
    positronStartPRP->Write(); // Positron Start Momentum Pr/P
    positronStartPZP->Write(); // Positron Start Momentum Pz/P
    triggeredPositronStartZ->Write(); // Positron Start Position Z Coordinate
    triggeredPositronStartPZ->Write(); // Positron Start Momentum Z Component
    triggeredPositronStartPZP->Write(); // Positron Start Momentum Pz/P

    // All T1 Histograms require hits in B1, B2, Target, and T1 to fill
    positronT1R->Write(); // Positron T1 Position R Coordinate
    positronT1Z->Write(); // Positron T1 Position Z Coordinate
    positronT1PR->Write(); // Positron T1 Momentum R Component
    positronT1PZ->Write(); // Positron T1 Momentum Z Component
    positronT1PRP->Write(); // Positron T1 Momentum Pr/P
    positronT1PZP->Write(); // Positron T1 Momentum Pz/P
    positronT1ExpectedR->Write(); //Expected Radius at center of T1 from positron start direction
    positronT1DifferenceR->Write(); // Difference between expected and actual radial coordinates
    positronT1DifferencePZP->Write(); // Difference between start PZ/P and PZ/P at T1 center
    positronT1dEdX->Write(); // dE/dX for positrons in T1
    positronT1dEdXvPathLength->Write(); // dE/dX vs. Path Length for positrons in T1

    // All T3 Histograms require hits in B1, B2, Target, T1, and T3 to fill
    positronT3R->Write(); // Positron T3 Position R Coordinate
    positronT3Z->Write(); // Positron T3 Position Z Coordinate
    positronT3PR->Write(); // Positron T3 Momentum R Component
    positronT3PZ->Write(); // Positron T3 Momentum Z Component
    positronT3PRP->Write(); // Positron T3 Momentum Pr/P
    positronT3PZP->Write(); // Positron T3 Momentum Pz/P
    positronT3ExpectedR->Write(); // Expected Radius at center of T3 from positron start direction
    positronT3DifferenceR->Write(); // Difference between expected and actual radial coordinates
    positronT3DifferencePZP->Write(); // Difference between start PZ/P and PZ/P at T1 center
    positronT3dEdX->Write(); // dE/dX for positrons in T3
    positronT3dEdXvPathLength->Write(); // dE/dX vs. Path Length for positrons in T3
    triggeredPositronT3ExpectedR->Write();
    triggeredPositronT3DifferenceR->Write();
    triggeredPositronT3DifferencePZP->Write();

    aTree->Write();
    file->Close();
}

// Fill the hit Ntuple
//

void RunAction::FillTuple(G4double E1, G4double E2, G4double E3, G4double E4,
                          G4double E5, G4double E6, G4double E7, G4double E8,
                          G4double E9, G4double E10, G4double E11,
			  G4double E12, G4double E13, G4double E14,
                          G4double E15, G4double E16) {
  
  E16 = 0;
  hitTuple->Fill(E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15);

}

void RunAction::FillTree(){aTree->Fill();}

void RunAction::SPionStart(G4ThreeVector position, 
                           G4double time, 
                           G4ThreeVector momentum, 
                           G4double Energy)
{
  PStartX[0]=position.x();
  PStartX[1]=position.y();
  PStartX[2]=position.z();
  PStartX[3]=time;
  PStartP[0]=momentum.x();
  PStartP[1]=momentum.y();
  PStartP[2]=momentum.z();
  PStartP[3]=Energy;
}

void RunAction::SPionDecay(G4ThreeVector position,
                           G4double time,
                           G4ThreeVector momentum,
                           G4double Energy)
{
  PDecayX[0]=position.x();
  PDecayX[1]=position.y();
  PDecayX[2]=position.z();
  PDecayX[3]=time;
  PDecayP[0]=momentum.x();
  PDecayP[1]=momentum.y();
  PDecayP[2]=momentum.z();
  PDecayP[3]=Energy;
}

void RunAction::SMuonStart(G4ThreeVector position,
                           G4double time,
                           G4ThreeVector momentum,
                           G4double Energy)
{
  MStartX[0]=position.x();
  MStartX[1]=position.y();
  MStartX[2]=position.z();
  MStartX[3]=time;
  MStartP[0]=momentum.x();
  MStartP[1]=momentum.y();
  MStartP[2]=momentum.z();
  MStartP[3]=Energy;
}

void RunAction::SMuonDecay(G4ThreeVector position,
                           G4double time,
                           G4ThreeVector momentum,
                           G4double Energy)
{
  MDecayX[0]=position.x();
  MDecayX[1]=position.y();
  MDecayX[2]=position.z();
  MDecayX[3]=time;
  MDecayP[0]=momentum.x();
  MDecayP[1]=momentum.y();
  MDecayP[2]=momentum.z();
  MDecayP[3]=Energy;
}

void RunAction::SPositronStart(G4ThreeVector position,
                               G4double time,
                               G4ThreeVector momentum,
                               G4double Energy)
{
  EStartX[0]=position.x();
  EStartX[1]=position.y();
  EStartX[2]=position.z();
  EStartX[3]=time;
  EStartP[0]=momentum.x();
  EStartP[1]=momentum.y();
  EStartP[2]=momentum.z();
  EStartP[3]=Energy;
}

void RunAction::SPositronStop(G4ThreeVector position,
                              G4double time,
                              G4ThreeVector momentum,
                              G4double Energy)
{
  EStopX[0]=position.x();
  EStopX[1]=position.y();
  EStopX[2]=position.z();
  EStopX[3]=time;
  EStopP[0]=momentum.x();
  EStopP[1]=momentum.y();
  EStopP[2]=momentum.z();
  EStopP[3]=Energy;
}

void RunAction::SPhotonuclear(G4ThreeVector position)
{
  EPhNX[0]=position.x();
  EPhNX[1]=position.y();
  EPhNX[2]=position.z();
}
    

void RunAction::SBhabha(G4double Energy)
{
  EBh=Energy;
 
}


/// Additions based on Tristan's work /////

void RunAction::SPosBrem(G4double pretime, G4double posttime, G4ThreeVector prepos, G4ThreeVector postpos, G4ThreeVector premom, G4ThreeVector postmom, G4double preE, G4double postE)
{
    //G4cout << "MaxBremEpos: " << MaxBremEpos << G4endl;
    //G4cout << "preE - postE: " << preE - postE << G4endl;

    if (preE - postE > MaxBremEpos)
    {    
        PosBremPreX[0] = prepos.x();
        PosBremPreX[1] = prepos.y();
        PosBremPreX[2] = prepos.z();
        PosBremPreX[3] = pretime;
        PosBremPreP[0] = premom.x();
        PosBremPreP[1] = premom.y();
        PosBremPreP[2] = premom.z();
        PosBremPreP[3] = preE;

        PosBremPostX[0] = postpos.x();
        PosBremPostX[1] = postpos.y();
        PosBremPostX[2] = postpos.z();
        PosBremPostX[3] = posttime;
        PosBremPostP[0] = postmom.x();
        PosBremPostP[1] = postmom.y();
        PosBremPostP[2] = postmom.z();
        PosBremPostP[3] = postE;

        MaxBremEpos = preE - postE;
    }

    TotalBremEpos += preE - postE;
}
 

void RunAction::SPosBhabha(G4double pretime, G4double posttime, G4ThreeVector prepos, G4ThreeVector postpos, G4ThreeVector premom, G4ThreeVector postmom, G4double preE, G4double postE)
{
    //G4cout << "MaxBhabhaEpos: " << MaxBhabhaEpos << G4endl;
    //G4cout << "preE - postE: " << preE - postE << G4endl;

    if (preE - postE > MaxBhabhaEpos)
    {    
        PosBhabhaPreX[0] = prepos.x();
        PosBhabhaPreX[1] = prepos.y();
        PosBhabhaPreX[2] = prepos.z();
        PosBhabhaPreX[3] = pretime;
        PosBhabhaPreP[0] = premom.x();
        PosBhabhaPreP[1] = premom.y();
        PosBhabhaPreP[2] = premom.z();
        PosBhabhaPreP[3] = preE;

        PosBhabhaPostX[0] = postpos.x();
        PosBhabhaPostX[1] = postpos.y();
        PosBhabhaPostX[2] = postpos.z();
        PosBhabhaPostX[3] = posttime;
        PosBhabhaPostP[0] = postmom.x();
        PosBhabhaPostP[1] = postmom.y();
        PosBhabhaPostP[2] = postmom.z();
        PosBhabhaPostP[3] = postE;

        MaxBhabhaEpos = preE - postE;
    }

    TotalBhabhaEpos += preE - postE;
}
                   
void RunAction::SPosAnnihil(G4double pretime, G4double posttime, G4ThreeVector prepos, G4ThreeVector postpos, G4ThreeVector premom, G4ThreeVector postmom, G4double preE, G4double postE)
{
    PosAnnihilPreX[0] = prepos.x();
    PosAnnihilPreX[1] = prepos.y();
    PosAnnihilPreX[2] = prepos.z();
    PosAnnihilPreX[3] = pretime;
    PosAnnihilPreP[0] = premom.x();
    PosAnnihilPreP[1] = premom.y();
    PosAnnihilPreP[2] = premom.z();
    PosAnnihilPreP[3] = preE;

    PosAnnihilPostX[0] = postpos.x();
    PosAnnihilPostX[1] = postpos.y();
    PosAnnihilPostX[2] = postpos.z();
    PosAnnihilPostX[3] = posttime;
    PosAnnihilPostP[0] = postmom.x();
    PosAnnihilPostP[1] = postmom.y();
    PosAnnihilPostP[2] = postmom.z();
    PosAnnihilPostP[3] = postE;
}
                   
void RunAction::SElecBrem(G4double pretime, G4double posttime, G4ThreeVector prepos, G4ThreeVector postpos, G4ThreeVector premom, G4ThreeVector postmom, G4double preE, G4double postE)
{
    //G4cout << "MaxBremEelec: " << MaxBremEelec << G4endl;
    //G4cout << "preE - postE: " << preE - postE << G4endl;

    if (preE - postE > MaxBremEelec)
    {    
        ElecBremPreX[0] = prepos.x();
        ElecBremPreX[1] = prepos.y();
        ElecBremPreX[2] = prepos.z();
        ElecBremPreX[3] = pretime;
        ElecBremPreP[0] = premom.x();
        ElecBremPreP[1] = premom.y();
        ElecBremPreP[2] = premom.z();
        ElecBremPreP[3] = preE;

        ElecBremPostX[0] = postpos.x();
        ElecBremPostX[1] = postpos.y();
        ElecBremPostX[2] = postpos.z();
        ElecBremPostX[3] = posttime;
        ElecBremPostP[0] = postmom.x();
        ElecBremPostP[1] = postmom.y();
        ElecBremPostP[2] = postmom.z();
        ElecBremPostP[3] = postE;

        MaxBremEelec = preE - postE;
    }

    TotalBremEelec += preE - postE;
}

void RunAction::SPosScatter(G4double pretime, G4double posttime, G4ThreeVector prepos, G4ThreeVector postpos, G4ThreeVector premom, G4ThreeVector postmom, G4double preE, G4double postE)
{
    PosScatterPreX[0] = prepos.x();
    PosScatterPreX[1] = prepos.y();
    PosScatterPreX[2] = prepos.z();
    PosScatterPreX[3] = pretime;
    PosScatterPreP[0] = premom.x();
    PosScatterPreP[1] = premom.y();
    PosScatterPreP[2] = premom.z();
    PosScatterPreP[3] = preE;

    PosScatterPostX[0] = postpos.x();
    PosScatterPostX[1] = postpos.y();
    PosScatterPostX[2] = postpos.z();
    PosScatterPostX[3] = posttime;
    PosScatterPostP[0] = postmom.x();
    PosScatterPostP[1] = postmom.y();
    PosScatterPostP[2] = postmom.z();
    PosScatterPostP[3] = postE;
}
                    
void RunAction::SElecScatter(G4double pretime, G4double posttime, G4ThreeVector prepos, G4ThreeVector postpos, G4ThreeVector premom, G4ThreeVector postmom, G4double preE, G4double postE)
{
    ElecScatterPreX[0] = prepos.x();
    ElecScatterPreX[1] = prepos.y();
    ElecScatterPreX[2] = prepos.z();
    ElecScatterPreX[3] = pretime;
    ElecScatterPreP[0] = premom.x();
    ElecScatterPreP[1] = premom.y();
    ElecScatterPreP[2] = premom.z();
    ElecScatterPreP[3] = preE;

    ElecScatterPostX[0] = postpos.x();
    ElecScatterPostX[1] = postpos.y();
    ElecScatterPostX[2] = postpos.z();
    ElecScatterPostX[3] = posttime;
    ElecScatterPostP[0] = postmom.x();
    ElecScatterPostP[1] = postmom.y();
    ElecScatterPostP[2] = postmom.z();
    ElecScatterPostP[3] = postE;
}
                    
/*
void RunAction::SeinWC3(G4ThreeVector position, G4double time, G4ThreeVector momentum, G4double Energy)
{
    ElecinWC3PreX[0] = position.x();
    ElecinWC3PreX[1] = position.y();
    ElecinWC3PreX[2] = position.z();
    ElecinWC3PreX[3] = time;

    ElecinWC3PreP[0] = momentum.x();
    ElecinWC3PreP[1] = momentum.y();
    ElecinWC3PreP[2] = momentum.z();
    ElecinWC3PreP[3] = Energy;
}

void RunAction::SprimposinWC3(G4ThreeVector position, G4double time, G4ThreeVector momentum, G4double Energy)
{
    PrimPosinWC3PreX[0] = position.x();
    PrimPosinWC3PreX[1] = position.y();
    PrimPosinWC3PreX[2] = position.z();
    PrimPosinWC3PreX[3] = time;

    PrimPosinWC3PreP[0] = momentum.x();
    PrimPosinWC3PreP[1] = momentum.y();
    PrimPosinWC3PreP[2] = momentum.z();
    PrimPosinWC3PreP[3] = Energy;
}

void RunAction::SsecposinWC3(G4ThreeVector position, G4double time, G4ThreeVector momentum, G4double Energy)
{
    SecPosinWC3PreX[0] = position.x();
    SecPosinWC3PreX[1] = position.y();
    SecPosinWC3PreX[2] = position.z();
    SecPosinWC3PreX[3] = time;

    SecPosinWC3PreP[0] = momentum.x();
    SecPosinWC3PreP[1] = momentum.y();
    SecPosinWC3PreP[2] = momentum.z();
    SecPosinWC3PreP[3] = Energy;
}
*/

/*
void RunAction::AnnihilSeen()
{
  annihilCount=1; // if you see annihilation, put a 1 here
 
}


void RunAction::BremsSeen()
{
  bremsCount=1; // if you see Brems, put a 1 here
}

void RunAction::MSCSeen()
{
  mscCount=1; // if you see msc, put a 1 here
}
 
*/
                    
void RunAction::SMuPolarization(G4ThreeVector mupolarization)
{
  //G4cout << "Pol = " << mupolarization << G4endl;
  MuPol[0] = mupolarization.x();
  MuPol[1] = mupolarization.y();
  MuPol[2] = mupolarization.z();
}
      
void RunAction::SEmomentum(G4ThreeVector emomentum)
{
  //G4cout << "Mom = " << emomentum << G4endl;
  EMom[0] = emomentum.x();
  EMom[1] = emomentum.y();
  EMom[2] = emomentum.z();

  double N1=0;
  double N2=0;
  double Q = 0;
  for (int i=0;i<3;i++){
    N1 += MuPol[i]*MuPol[i];
    N2 += EMom[i]*EMom[i];
    
    Q += MuPol[i]*EMom[i];
  }
  
  Q /= sqrt(N1*N2);
  
  //G4cout << Q << G4endl;
  
  EMom[0] = Q;
  EMom[1] = emomentum.y();
  EMom[2] = emomentum.z();

}    

void RunAction::ClearVariable(){
  G4ThreeVector tV(-10000,-10000,-10000);
  G4double temp=-10000;
  RunAction::SPionStart(tV,temp,tV,temp);
  RunAction::SPionDecay(tV,temp,tV,temp);
  RunAction::SMuonStart(tV,temp,tV,temp);
  RunAction::SMuonDecay(tV,temp,tV,temp);
  RunAction::SPositronStart(tV,temp,tV,temp);
  RunAction::SPositronStop(tV,temp,tV,temp);
  RunAction::SPhotonuclear(tV);
  RunAction::SBhabha(temp);

  RunAction::SPosBrem(temp, temp, tV, tV, tV, tV, temp, temp);
  RunAction::SPosBhabha(temp, temp, tV, tV, tV, tV, temp, temp);
  RunAction::SPosAnnihil(temp, temp, tV, tV, tV, tV, temp, temp);
  RunAction::SElecBrem(temp, temp, tV, tV, tV, tV, temp, temp);
  RunAction::SPosScatter(temp, temp, tV, tV, tV, tV, temp, temp);
  RunAction::SElecScatter(temp, temp, tV, tV, tV, tV, temp, temp);

  // RunAction::SeinWC3(tV, temp, tV, temp);
  // RunAction::SprimposinWC3(tV, temp, tV, temp);
  // RunAction::SsecposinWC3(tV, temp, tV, temp);

  MaxBremEpos = 0;
  MaxBremEelec = 0;
  TotalBremEpos = 0;
  TotalBremEelec = 0;

  MaxBhabhaEpos = 0;
  TotalBhabhaEpos = 0;
}

void RunAction::SavePionStart(G4ThreeVector position, G4ThreeVector momentum)
{
  pionStartPosition = position;
  pionStartMomentum = momentum;
}

void RunAction::SavePionStop(G4ThreeVector position)
{
  pionStopPosition = position;
}

void RunAction::SaveMuonStart(G4ThreeVector position, G4ThreeVector momentum)
{
  muonStartPosition = position;
  muonStartMomentum = momentum;
}

void RunAction::SavePositronStart(G4ThreeVector position, 
                                  G4ThreeVector momentum)
{
  positronStartPosition = position;
  positronStartMomentum = momentum;
}

void RunAction::SaveT1(G4ThreeVector position, G4ThreeVector momentum)
{
  t1Position = position;
  t1Momentum = momentum;
}

void RunAction::SaveT1dEdX(G4double energyDeposit, G4double pathLength)
{
  t1energyDeposit = energyDeposit;
  t1pathLength = pathLength;
}

void RunAction::SaveT3(G4ThreeVector position, G4ThreeVector momentum)
{
  t3Position = position;
  t3Momentum = momentum;
}

void RunAction::SaveT3dEdX(G4double energyDeposit, G4double pathLength)
{
  t3energyDeposit = energyDeposit;
  t3pathLength = pathLength;
}

void RunAction::FillUntriggeredHistos() {
    // Particle Start Position/Momentum and Stop Position
    pionStartR->Fill( std::sqrt( std::pow(pionStartPosition.x(),2) +
                                 std::pow(pionStartPosition.y(),2) )/10. );
    pionStartZ->Fill(pionStartPosition.z()/10.);
    pionStartPR->Fill( std::sqrt( std::pow(pionStartMomentum.x(),2) +
                                  std::pow(pionStartMomentum.y(),2) ) );
    pionStartPZ->Fill(pionStartMomentum.z());
    pionStartPRP->Fill( std::sqrt( std::pow(pionStartMomentum.x(),2) +
                                   std::pow(pionStartMomentum.y(),2) )/
                                             pionStartMomentum.mag() );
    pionStartPZP->Fill(pionStartMomentum.z()/pionStartMomentum.mag());
}

void RunAction::FillTargetStopHistos() {

  if (1/*pionStopPosition != 0*/) {
       pionStopR->Fill(std::sqrt(std::pow(pionStopPosition.x(),2) +
                                 std::pow(pionStopPosition.y(),2))/10.);
       pionStopZ->Fill(pionStopPosition.z()/10.);
       muonStartR->Fill(std::sqrt(std::pow(muonStartPosition.x(),2) +
                                  std::pow(muonStartPosition.y(),2))/10.);
       muonStartZ->Fill(muonStartPosition.z()/10.);
       muonStartPR->Fill(std::sqrt(std::pow(muonStartMomentum.x(),2) +
                                   std::pow(muonStartMomentum.y(),2)));
       muonStartPZ->Fill(muonStartMomentum.z());
       muonStartPRP->Fill(std::sqrt(std::pow(muonStartMomentum.x(),2) +
                                    std::pow(muonStartMomentum.y(),2))/
                                                muonStartMomentum.mag());
       muonStartPZP->Fill(muonStartMomentum.z()/muonStartMomentum.mag());
       positronStartR->Fill(std::sqrt(std::pow(positronStartPosition.x(),2) +
                                      std::pow(positronStartPosition.y(),2))/
                                                                         10.);
       positronStartZ->Fill(positronStartPosition.z()/10.);
       positronStartPR->Fill(std::sqrt(std::pow(positronStartMomentum.x(),2) +
                                       std::pow(positronStartMomentum.y(),2)));
       positronStartPZ->Fill(positronStartMomentum.z());
       positronStartPRP->Fill(std::sqrt(std::pow(positronStartMomentum.x(),2) +
                                        std::pow(positronStartMomentum.y(),2))/
                                                  positronStartMomentum.mag());
       positronStartPZP->Fill(positronStartMomentum.z()/
                              positronStartMomentum.mag());

       G4double t3Scaling = (66.0 - positronStartPosition.z())/
                    (positronStartMomentum.z()/positronStartMomentum.mag());
       G4double t3ExpectedX = t3Scaling*positronStartMomentum.x()/
                    positronStartMomentum.mag() + positronStartPosition.x();
       G4double t3ExpectedY = t3Scaling*positronStartMomentum.y()/
                    positronStartMomentum.mag() + positronStartPosition.y();
       G4double t3ExpectedRadius = std::sqrt(std::pow(t3ExpectedX,2) + 
                                          std::pow(t3ExpectedY,2));

       positronT3ExpectedR->Fill(t3ExpectedRadius/10.);
       positronT3DifferenceR->Fill(std::abs(t3ExpectedRadius -
                                std::sqrt(std::pow(t3Position.x(),2) + 
                                          std::pow(t3Position.y(),2)))/10.);
       positronT3DifferencePZP->Fill(std::abs((positronStartMomentum.z()/
         positronStartMomentum.mag()) - (t3Momentum.z()/t3Momentum.mag())));

    }
  //    pionStopPosition = 0;
}

void RunAction::FillSemiTriggeredHistos() {

    // All T1 Histograms require hits in B1, B2, Target, and T1 to fill
    positronT1R->Fill(std::sqrt(std::pow(t1Position.x(),2) + 
                                std::pow(t1Position.y(),2))/10.);
    positronT1Z->Fill(t1Position.z()/10.);
    positronT1PR->Fill(std::sqrt( std::pow(t1Momentum.x(),2) + 
                                  std::pow(t1Momentum.y(),2)));
    positronT1PZ->Fill(t1Momentum.z());
    positronT1PRP->Fill(std::sqrt( std::pow(t1Momentum.x(),2) + 
                                   std::pow(t1Momentum.y(),2))/
                                                t1Momentum.mag());
    positronT1PZP->Fill(t1Momentum.z()/t1Momentum.mag());

    G4double t1Scaling = (t1Position.z() - positronStartPosition.z())/
                      (positronStartMomentum.z()/positronStartMomentum.mag());
    G4double t1ExpectedX = t1Scaling*positronStartMomentum.x()/
                      positronStartMomentum.mag() + positronStartPosition.x();
    G4double t1ExpectedY = t1Scaling*positronStartMomentum.y()/
                      positronStartMomentum.mag() + positronStartPosition.y();
    G4double t1ExpectedRadius = std::sqrt(std::pow(t1ExpectedX,2) + 
                                          std::pow(t1ExpectedY,2));

    positronT1ExpectedR->Fill(t1ExpectedRadius/10.);
    positronT1DifferenceR->Fill(std::abs(t1ExpectedRadius - 
                                std::sqrt(std::pow(t1Position.x(),2) + 
                                          std::pow(t1Position.y(),2)))/10.);
    positronT1DifferencePZP->Fill(std::abs((positronStartMomentum.z()/
                                      positronStartMomentum.mag()) - 
                                      (t1Momentum.z()/t1Momentum.mag()) ) );

    positronT1dEdX->Fill(t1energyDeposit/t1pathLength);
    positronT1dEdXvPathLength->Fill(t1pathLength/3.0, t1energyDeposit);
}

void RunAction::FillTriggeredHistos() {

    triggeredPositronStartZ->Fill(positronStartPosition.z()/10.);
    triggeredPositronStartPZ->Fill(positronStartMomentum.z());
    triggeredPositronStartPZP->Fill(positronStartMomentum.z()/
                                    positronStartMomentum.mag());

    // All T3 Histograms require hits in B1, B2, Target, T1, and T3 to fill
    positronT3R->Fill(std::sqrt(std::pow(t3Position.x(),2) + 
                                std::pow(t3Position.y(),2))/10.);
    positronT3Z->Fill(t3Position.z()/10.);
    positronT3PR->Fill(std::sqrt(std::pow(t3Momentum.x(),2) + 
                                 std::pow(t3Momentum.y(),2)));
    positronT3PZ->Fill(t3Momentum.z());
    positronT3PRP->Fill(std::sqrt(std::pow(t3Momentum.x(),2) + 
                        std::pow(t3Momentum.y(),2))/t3Momentum.mag());
    positronT3PZP->Fill(pionStartPosition.z());

    G4double t3Scaling = (t3Position.z() - positronStartPosition.z())/
                     (positronStartMomentum.z()/positronStartMomentum.mag());
    G4double t3ExpectedX = t3Scaling*positronStartMomentum.x()/
                     positronStartMomentum.mag() + positronStartPosition.x();
    G4double t3ExpectedY = t3Scaling*positronStartMomentum.y()/
                     positronStartMomentum.mag() + positronStartPosition.y();
    G4double t3ExpectedRadius = std::sqrt(std::pow(t3ExpectedX,2) + 
                                          std::pow(t3ExpectedY,2));

    triggeredPositronT3ExpectedR->Fill(t3ExpectedRadius/10.);
    triggeredPositronT3DifferenceR->Fill(std::abs(t3ExpectedRadius - 
                                  std::sqrt(std::pow(t3Position.x(),2) +
                                            std::pow(t3Position.y(),2)))/10.);
    triggeredPositronT3DifferencePZP->Fill(std::abs((positronStartMomentum.z()/
          positronStartMomentum.mag()) - (t3Momentum.z()/t3Momentum.mag())));

    positronT3dEdX->Fill(t3energyDeposit/t3pathLength);
    positronT3dEdXvPathLength->Fill(t3pathLength/3.0, t3energyDeposit);
}


// Called by Geant4 at the beginning of a run
//
void RunAction::BeginOfRunAction(const G4Run* aRun) {

    G4cout << "### Run " << aRun->GetRunID() << " start ###" << G4endl;

    // Open Root
    OpenRoot();

        G4cout << "### Run " << aRun->GetRunID() << " started with OpenRoot() ###" << G4endl;

    // save random number status
    //    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

    if (autoSeed) {
       // automatic (time-based) random seeds for each run
       G4cout << "*******************" << G4endl;
       G4cout << "*** AUTOSEED ON ***" << G4endl;
       G4cout << "*******************" << G4endl;
       long seeds[2];
       time_t systime = time(NULL);
       seeds[0] = (long) systime;
       seeds[1] = (long) (systime*G4UniformRand());
       CLHEP::HepRandom::setTheSeeds(seeds);
    }

}

// Called by Geant4 at the end of a run
//
void RunAction::EndOfRunAction(const G4Run* aRun)
{

    G4cout << "### Run " << aRun->GetRunID() << " end ###" << G4endl;

    // Close ROOT
    CloseRoot();

    // save Rndm status
    CLHEP::HepRandom::saveEngineStatus("random/EndOfRun.rndm");

}

// Convert an integer to a character
//
char* RunAction::itoa(int val, int base) {

    static char buf[32] = {0};

    int i = 30;

    for(; val && i; --i, val /= base) {

        buf[i] = "0123456789abcdef"[val % base];

    }

    return &buf[i+1];

}
