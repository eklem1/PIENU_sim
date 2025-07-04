#ifndef HitSegment_h
#define HitSegment_h 1

#include "globals.hh"

#include "G4VHit.hh"

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"

#include "VolumeID.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Step;

class HitSegment : public G4VHit
{

  public:

    HitSegment();
    virtual ~HitSegment();

    HitSegment(const HitSegment &right);

    inline void *operator new(size_t);
    inline void  operator delete(void*);

    HitSegment operator=(const HitSegment&);
    HitSegment operator+=(const HitSegment&);

    /// Check the condition whether or not a hit should be merged.
    virtual G4bool CheckMergeCondition(HitSegment* ) { return false; }

    /// Add the effects of a step to this hit.
    virtual void AddStep(G4Step* theStep);

    /// Hits for the same track in the same phys. volume belong to the same hit.
    virtual G4bool SameHit(G4Step* theStep);

    /// Return the VolumeID of the trajectory that made this hit.
    const VolumeID& GetVolumeID(void) const { return fHitVolume; }

    /// Return the TrackID of the trajectory that made this hit.
    G4int GetTrackID(void) const { return fTrackID; }

    /// Return the ParentID of the trajectory that made this hit.
    G4int GetParentID(void) const { return fParentID; }

    /// Get the total energy deposited in this hit.
    G4double GetEnergyDeposit(void) const { return fEnergyDeposit; }

    /// Get the energy weighted hit position.
    G4ThreeVector GetEnergyWeightedPosition(void) const { return fPosition; }

    /// Get the total charged track length in this hit.
    G4double GetTrackLength(void) const { return fTrackLength; }

    /// The X position of the hit starting point.  Note that a hit by
    /// definition is in a single volume.
    G4double GetStartX(void) const { return fStartX; }

    /// The Y position of the hit starting point.  Note that a hit by
    /// definition is in a single volume.
    G4double GetStartY(void) const { return fStartY; }

    /// The Z position of the hit starting point.  Note that a hit by
    /// definition is in a single volume.
    G4double GetStartZ(void) const { return fStartZ; }

    /// The time of the hit starting point.  Note that a hit by
    /// definition is in a single volume.
    G4double GetStartT(void) const { return fStartT; }

    /// The X position of the hit stopping point.  Note that a hit by
    /// definition is in a single volume.
    G4double GetStopX(void) const { return fStopX; }

    /// The Y position of the hit stopping point.  Note that a hit by
    /// definition is in a single volume.
    G4double GetStopY(void) const { return fStopY; }

    /// The Z position of the hit stopping point.  Note that a hit by
    /// definition is in a single volume.
    G4double GetStopZ(void) const { return fStopZ; }

    /// The time of the hit stopping point.  Note that a hit by
    /// definition is in a single volume.
    G4double GetStopT(void) const { return fStopT; }

    /// The PDG Encoding of the particle producing the hit.
    G4int GetPDG(void) const { return fPDG; }

    /// The particle name producing the hit.
    G4String GetParticleName(void) const { return fParticleName; }

    /// The Decay flag
    G4int GetCreatorFlag(void) const { return fCreatorFlag; }

    /// The creator process
    G4String GetCreatorProcessName(void) const { return fCreatorProcessName; }

    /// The step process ID
    G4int GetProcessID(void) const { return fProcessID; }

    /// Get the visible energy deposited in this hit.
    G4double GetEbirk(void) const { return fEbirk; }

    void SetMergedFlag(G4bool status) { fMerged = status; }
    G4bool GetMergedFlag() const { return fMerged; }

    ///Clear the track id and time
    void Clear(void);

    void Print(void);

#ifdef G4VIS_USE
    void Draw(void);
#endif

  protected:

    /// The VolumeID that contains the hit.
    VolumeID fHitVolume;

    /// The TrackID of the trajectory that made this hit.
    G4int fTrackID;

    /// The ParentID of the trajectory that made this hit.
    G4int fParentID;

    /// The total energy deposit in this hit.
    G4double fEnergyDeposit;

    /// The energy weighted hit position.
    G4ThreeVector fPosition;

    /// The total charged track length in this hit.
    G4double fTrackLength;

    /// The X position of the hit starting point.
    G4double fStartX;

    /// The Y position of the hit starting point.
    G4double fStartY;

    /// The Z position of the hit starting point.
    G4double fStartZ;

    /// The time of the hit starting point.
    G4double fStartT;

    /// The X position of the hit stopping point.
    G4double fStopX;

    /// The Y position of the hit stopping point.
    G4double fStopY;

    /// The Z position of the hit stopping point.
    G4double fStopZ;

    /// The time of the hit stopping point.
    G4double fStopT;

    /// The PDG Encoding of the particle
    G4int fPDG;

    /// The  particle name
    G4String fParticleName;

    /// The  decayID (Flag whether this hit is generated by decay or not)
    G4int fCreatorFlag;

    /// The  process name
    G4String fCreatorProcessName;

    // /// The process ID
    G4int fProcessID;

    /// The  visible energy with birks effect
    G4double fEbirk;

    /// Flag that determines if the hit was merged into another hit
    G4bool fMerged;

#ifdef G4VIS_USE
    /// Coordinate transformations needed for hit-volume drawing
    G4RotationMatrix fRotation;
    G4ThreeVector fTranslation;
#endif

};

typedef G4THitsCollection<HitSegment> HitSegmentCollection;

extern G4Allocator<HitSegment> HitSegmentAllocator;

inline void* HitSegment::operator new(size_t)
{
  void *aHit;
  aHit = (void *) HitSegmentAllocator.MallocSingle();
  return aHit;
}

inline void HitSegment::operator delete(void *aHit)
{
  HitSegmentAllocator.FreeSingle((HitSegment*) aHit);
}

#endif
