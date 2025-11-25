#ifndef MYTRACKINFORMATION_HH
#define MYTRACKINFORMATION_HH

#include "G4VUserTrackInformation.hh"
#include "globals.hh"

class MyTrackInformation : public G4VUserTrackInformation {
public:
    MyTrackInformation() : passedVolume(false), grandparentID(0) {}
    MyTrackInformation(G4bool flag, G4int gpID)
        : passedVolume(flag), grandparentID(gpID) {}

    G4bool GetPassedVolume() const { return passedVolume; }
    void SetPassedVolume(G4bool val) { passedVolume = val; }

    G4int GetGrandparentID() const { return grandparentID; }
    void SetGrandparentID(G4int id) { grandparentID = id; }

private:
    G4bool passedVolume;
    G4int grandparentID;
};

#endif
