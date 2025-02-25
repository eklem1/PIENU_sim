C ... RanLux
      SUBROUTINE RANLUX(RVEC,LENV)
      IMPLICIT NONE
      INTEGER LENV
      REAL*8 RVEC(LENV)
      CALL RANLUXCPP(LENV,RVEC)
      END



C ... Call the Geant4 G4UniformRand() function
      FUNCTION RANF()
      IMPLICIT NONE
      REAL*8 RANF, OUTPUT
      CALL RNDMCPP(OUTPUT)
      RANF = OUTPUT
      RETURN
      END
