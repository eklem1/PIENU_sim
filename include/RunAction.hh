#ifndef RunAction_h
#define RunAction_h 1

#include "G4ThreeVector.hh"

#include "G4UserRunAction.hh"

class TFile;
class TNtuple;
class TH1D;
class TH2D;
class TTree;

class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction {

  public:

    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

    void SetAutoSeed (const G4bool val) { autoSeed = val; }

    void OpenRoot();
    void CloseRoot();

    void FillTuple(G4double E1,
                   G4double E2,
                   G4double E3,
                   G4double E4,
                   G4double E5,
                   G4double E6,
                   G4double E7,
                   G4double E8,
                   G4double E9,
                   G4double E10,
                   G4double E11,
                   G4double E12,
                   G4double E13,
                   G4double E14,
                   G4double E15,
                   G4double E16);
    void FillTree();

    void ClearVariable();

    void SetEventID(G4int eID) { eventID=eID; }

    void SPionStart(G4ThreeVector, G4double, G4ThreeVector, G4double);
    void SPionDecay(G4ThreeVector, G4double, G4ThreeVector, G4double);
    void SMuonStart(G4ThreeVector, G4double, G4ThreeVector, G4double);
    void SMuonDecay(G4ThreeVector, G4double, G4ThreeVector, G4double);
    void SPositronStart(G4ThreeVector, G4double, G4ThreeVector, G4double);
    void SPositronStop(G4ThreeVector, G4double, G4ThreeVector, G4double);
    void SPhotonuclear(G4ThreeVector);
    void SBhabha(G4double);
    void SMuPolarization(G4ThreeVector);
    void SEmomentum (G4ThreeVector);

    // void AnnihilSeen();
    // void BremsSeen();
    // void MSCSeen();

    void SavePionStart(G4ThreeVector, G4ThreeVector);
    void SavePionStop(G4ThreeVector);
    void SaveMuonStart(G4ThreeVector, G4ThreeVector);
    void SavePositronStart(G4ThreeVector, G4ThreeVector);
    void SaveT1(G4ThreeVector, G4ThreeVector);
    void SaveT3(G4ThreeVector, G4ThreeVector);
    void SaveT1dEdX(G4double, G4double);
    void SaveT3dEdX(G4double, G4double);

    void FillUntriggeredHistos();
    void FillTargetStopHistos();
    void FillSemiTriggeredHistos();
    void FillTriggeredHistos();

  private:

    RunActionMessenger* runActionMessenger;

    G4bool autoSeed;

    TFile*   file;
    TNtuple* hitTuple;
    TTree*   aTree;

    //Diagnostic Histograms

    // Particle Start Position/Momentum and Stop Position
    TH1D* pionStartR; // Pion Start Position R Coordinate
    TH1D* pionStartZ; // Pion Start Position Z Coordinate
    TH1D* pionStartPR; // Pion Start Momentum R Component
    TH1D* pionStartPZ; // Pion Start Momentum Z Component
    TH1D* pionStartPRP; // Pion Start Momentum Pr/P
    TH1D* pionStartPZP; // Pion Start Momentum Pz/P
    TH1D* pionStopR; // Pion Stop Position R Coordinate
    TH1D* pionStopZ; // Pion Stop Position Z Coordinate
    TH1D* muonStartR; // Muon Start Position R Coordinate (Same As Pion Stop Coordinate - Redundant)
    TH1D* muonStartZ; // Muon Start Position Z Coordinate (Same As Pion Stop Coordinate - Redundant)
    TH1D* muonStartPR; // Muon Start Momentum R Component
    TH1D* muonStartPZ; // Muon Start Momentum Z Component
    TH1D* muonStartPRP; // Muon Start Momentum Pr/P
    TH1D* muonStartPZP; // Muon Start Momentum Pz/P
    TH1D* positronStartR; // Positron Start Position R Coordinate
    TH1D* positronStartZ; // Positron Start Position Z Coordinate
    TH1D* positronStartPR; // Positron Start Momentum R Component
    TH1D* positronStartPZ; // Positron Start Momentum Z Component
    TH1D* positronStartPRP; // Positron Start Momentum Pr/P
    TH1D* positronStartPZP; // Positron Start Momentum Pz/P
    TH1D* triggeredPositronStartZ; // Triggered Positron Start Position Z Coordinate
    TH1D* triggeredPositronStartPZ; // Triggered Positron Momentum Z Component
    TH1D* triggeredPositronStartPZP; // Triggered Positron Momentum Pz/P

    // All T1 Histograms require hits in B1, B2, Target, and T1 to fill
    TH1D* positronT1R; // Positron T1 Position R Coordinate
    TH1D* positronT1Z; // Positron T1 Position Z Coordinate
    TH1D* positronT1PR; // Positron T1 Momentum R Component
    TH1D* positronT1PZ; // Positron T1 Momentum Z Component
    TH1D* positronT1PRP; // Positron T1 Momentum Pr/P
    TH1D* positronT1PZP; // Positron T1 Momentum Pz/P
    TH1D* positronT1ExpectedR; //Expected Radius at center of T1 from positron start direction
    TH1D* positronT1DifferenceR; // Difference between expected and actual radial coordinates
    TH1D* positronT1DifferencePZP; // Difference between start PZ/P and PZ/P at T1 entrance
    TH1D* positronT1dEdX; // dE/dX for positrons in T1
    TH2D* positronT1dEdXvPathLength; // dE/dX vs. Path Length for positrons in T1

    // All T3 Histograms require hits in B1, B2, Target, T1, and T3 to fill
    TH1D* positronT3R; // Positron T3 Position R Coordinate
    TH1D* positronT3Z; // Positron T3 Position Z Coordinate
    TH1D* positronT3PR; // Positron T3 Momentum R Component
    TH1D* positronT3PZ; // Positron T3 Momentum Z Component
    TH1D* positronT3PRP; // Positron T3 Momentum Pr/P
    TH1D* positronT3PZP; // Positron T3 Momentum Pz/P
    TH1D* positronT3ExpectedR; // Expected Radius at T3 Entrance from positron start direction
    TH1D* positronT3DifferenceR; // Difference between expected and actual radial coordinates
    TH1D* positronT3DifferencePZP; // Difference between start PZ/P and PZ/P at T3 entrance
    TH1D* positronT3dEdX; // dE/dX for positrons in T3
    TH2D* positronT3dEdXvPathLength; // dE/dX vs. Path Length for positrons in T3
    TH1D* triggeredPositronT3ExpectedR; // Triggered Positron Expected Radius At T3 Entrance
    TH1D* triggeredPositronT3DifferenceR; // Difference between expected and actual radial coordinates for triggered events at T3
    TH1D* triggeredPositronT3DifferencePZP; // Difference between start PZ/P and PZ/P at T3 entrance

    G4int eventID;

    G4double PStartX[4],PStartP[4];
    G4double PDecayX[4],PDecayP[4];
    G4double MStartX[4],MStartP[4];
    G4double MDecayX[4],MDecayP[4];
    G4double EStartX[4],EStartP[4];
    G4double EStopX[4],EStopP[4];
    G4double EPhNX[3];
    G4double EBh;

    // G4int annihilCount;
    // G4int bremsCount;
    // G4int mscCount;

    G4double MuPol[3];
    G4double EMom[3];

    G4ThreeVector pionStartPosition;
    G4ThreeVector pionStartMomentum;
    G4ThreeVector pionStopPosition;
    G4ThreeVector muonStartPosition;
    G4ThreeVector muonStartMomentum;
    G4ThreeVector positronStartPosition;
    G4ThreeVector positronStartMomentum;
    G4ThreeVector t1Position;
    G4ThreeVector t1Momentum;
    G4ThreeVector t3Position;
    G4ThreeVector t3Momentum;
    G4double t1energyDeposit;
    G4double t1pathLength;
    G4double t3energyDeposit;
    G4double t3pathLength;

    char* itoa(int val, int base);

};

#endif
