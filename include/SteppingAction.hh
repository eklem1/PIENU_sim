
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "globals.hh"

#include "G4UserSteppingAction.hh"

class RunAction;

class SteppingAction : public G4UserSteppingAction {

    public:

        SteppingAction(RunAction* run);
        ~SteppingAction();

        void UserSteppingAction(const G4Step*);

    private:

        bool PointingIntoBina(G4ThreeVector, G4ThreeVector);
        bool insideWC3_1(G4ThreeVector);
        bool insideWC3_2(G4ThreeVector);
        bool insideWC3_3(G4ThreeVector);

        G4int DecayInFlightCounter;
           
        RunAction* runAction;
        G4bool savedT1Once;
        G4bool savedT3Once;

        G4int mutracknumber;
        G4bool WC3_1eflag;
        G4bool WC3_2eflag;
        G4bool WC3_3eflag;
        G4bool WC3_1primposflag;
        G4bool WC3_2primposflag;
        G4bool WC3_3primposflag;
        G4bool WC3_1secposflag;
        G4bool WC3_2secposflag;
        G4bool WC3_3secposflag;

};

#endif
