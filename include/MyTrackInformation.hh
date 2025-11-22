#ifndef MYTRACKINFORMATION_HH
#define MYTRACKINFORMATION_HH

#include "G4VUserTrackInformation.hh"
#include "globals.hh"

class MyTrackInformation : public G4VUserTrackInformation {
public:
    MyTrackInformation() : passedVolume(false) {}
    MyTrackInformation(G4bool flag) : passedVolume(flag) {}

    G4bool GetPassedVolume() const { return passedVolume; }
    void SetPassedVolume(G4bool val) { passedVolume = val; }

private:
    G4bool passedVolume;
};

#endif
