c --------------------------------------------------------------------
c ATTENTION! In the generators, ALL Lorentz transformations of decay
c products from the kaon rest frame to the lab frame (DBOOST) should
c always be made before passing the first daughter to Geant4 (MCADD4).
c
c This is because the kaon 4-momentum is stored in a temporary place,
c and is overwritten by the first daughter by the first MCADD4 call.
c
c E Goudzovski, 22 August 2018.
c
c -----------------------------------------------------------------------------------
c     Add a particle with no polarization to MC particle list, to be passed to Geant.
c -----------------------------------------------------------------------------------

      FUNCTION MCADD4(IPID, P4)
#include "common_blocks.f"
      INTEGER J, IPID
      REAL*8  P4(4)

      IF (NPART.GE.MXPART) THEN
         PRINT *, '[MCADD4] Warning: particle list overflow'
         MCADD4 = NPART
         RETURN
      ENDIF

      NPART        = NPART + 1
      PID  (NPART) = IPID
      KEEP (NPART) = 1 ! should be passed to Geant4 for tracking
      DO J = 1, 4
         p4ini (j,npart) = p4(j)
      ENDDO
      DO J = 1, 3
         POL3INI (j,npart) = 0.0
      ENDDO
      MCADD4 = NPART
      RETURN
      END

c --------------------------------------------------------------------------------
c     Add a particle with polarization to MC particle list, to be passed to Geant.
c --------------------------------------------------------------------------------

      FUNCTION MCADD4POL3(IPID, P4, POL3)
#include "common_blocks.f"
      INTEGER J, IPID
      REAL*8  P4(4), POL3(3)

      IF (NPART.GE.MXPART) THEN
         PRINT *, '[MCADD4POL3] Warning: particle list overflow'
         MCADD4POL3 = NPART
         RETURN
      ENDIF

      NPART        = NPART + 1
      PID  (NPART) = IPID
      KEEP (NPART) = 1 ! should be passed to Geant4 for tracking
      DO J = 1, 4
         p4ini (j,npart) = p4(j)
      ENDDO
      DO J = 1, 3
         POL3INI (j,npart) = pol3(j)
      ENDDO
      MCADD4POL3 = NPART
      RETURN
      END

c----------------------------------------------------------------------------------
c     Save the true 4-momentum of a generated particle in its parent's rest frame.
c     This applies to the products of beam kaon, beam pion and secondary pi0 decays
c     produced by the NA62MC (but not Geant4) generators.
c----------------------------------------------------------------------------------

      FUNCTION MCADD4GEN(IPID, P4, IFLAG)
#include "common_blocks.f"
      INTEGER J, IPID, IFLAG
      REAL*8  P4(4)

c ... IFLAG is used to store a "particle group" identifier.
c ... Group encoding: 0 = products of the beam K+ decay,
c ...                 1 = products of the daughter pi0 decays,
c ...                 2 = radiative photons produced by Photos from beam K+ (or pi+) decay,
c ...                 3 = radiative photons produced by Photos from daughter pi0 decays,
c ...                 4 = products of the beam or daughter pi decays.
c ...
c ... This flag is propagated to KineParts in EventAction::EndOfEventAction().

      IF (NPARTGEN.GE.MXPART) THEN
         PRINT *,'[MCADD4GEN] Warning: GenePart list overflow'
         MCADD4GEN = NPARTGEN
         RETURN
      ENDIF

      NPARTGEN          = NPARTGEN + 1
      PIDGEN (NPARTGEN) = IPID
      FLAG   (NPARTGEN) = IFLAG
      DO J = 1, 4
         p4inigen (j,npartgen) = 1.0e3 * p4(j) ! GeV --> MeV
      ENDDO
      MCADD4GEN = NPARTGEN
      RETURN
      END
