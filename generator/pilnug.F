      SUBROUTINE PILNUG(JPION, LTYPE, MODE)
C---------------------------------------------------------------C
C pi+- -> L NU GAMMA decay                                      C
C                                                               C
C LTYPE:  1=electron, 2=muon                                    C
C MODE:   1=IB, 2=SD+, 3=SD-, 4=INT+, 5=INT-, 6=ALL             C
C                                                               C
C Evgueni.Goudzovski@cern.ch                                    C
C 02/09/2007: original version                                  C
C 13/02/2008: form-factor x-dependence introduced               C
C 25/02/2011: INT terms added                                   C
C 03/02/2011: adapted for NA62MC                                C  
C                                                               C
C Adapted for pion decay by D.Protopopescu & I.Skillicorn       C
C Dan.Protopopescu@cern.ch  30/06/2014                          C
C For details, see internal note NA62-14-10                     C 
C---------------------------------------------------------------C

#include "common_blocks.f"
#include "masses.f"

      INTEGER LTYPE, MODE

      INTEGER i, JPION, JLEP, JGAM, JNU, IDLEP, ffmode
      REAL*8 vec(3), P1(4,4), P2(4,4), P3(4,4), P4(4,4)
      REAL*8 PHI, SINPHI, COSPHI, COSTHE, SINTHE, COSPSI, SINPSI
      REAL*8 MLEP, Eg, Ee, Pe, En, Pgx, Pgy
      REAL*8 plep(4), pgam(4), pnu(4)
      REAL*8 x, y, ymin, rl, f1, f2, wtcomp, wtmax
      REAL*8 Fa, Fv, Fva, Fv0, alpha, fPI
      REAL*8 f_ib, f_sd_p, f_sd_m, f_int_p, f_int_m, f_here, f_tot
      REAL*8 c_sd_p, c_sd_m, c_int_p, c_int_m, c_g, xb, yb, z, rcf
      REAL*8 rc_ib, rc_sd_p, rc_sd_m, rc_int_p, rc_int_m, rc_tot
      REAL*8 pi
      REAL*8 rando(2)
      Parameter (pi = 3.141592653589793)

c ............................................

      if (mode.lt.1.or.mode.gt.6)    mode  = 6
      if (ltype.ne.1.and.ltype.ne.2) ltype = 1

      if (ltype.eq.1) MLEP = MEL
      if (ltype.eq.2) MLEP = MMU
      rl = MLEP*MLEP/SQMPI    

      if (ltype.eq.1.and.mode.eq.1) wtmax = 5.00e5 !  IB
      if (ltype.eq.1.and.mode.eq.2) wtmax = 4.610  !  SD+
      if (ltype.eq.1.and.mode.eq.3) wtmax = 0.650  !  SD-
      if (ltype.eq.1.and.mode.eq.4) wtmax = 0.036  !  INT+
      if (ltype.eq.1.and.mode.eq.5) wtmax = 275.0  !  INT-
      if (ltype.eq.1.and.mode.eq.6) wtmax = 5.00e5 !  FULL

      if (ltype.eq.2.and.mode.eq.1) wtmax = 1.09e6 !  IB
      if (ltype.eq.2.and.mode.eq.2) wtmax = 0.0001 !  SD+
      if (ltype.eq.2.and.mode.eq.3) wtmax = 0.0001 !  SD-
      if (ltype.eq.2.and.mode.eq.4) wtmax = 0.0025 !  INT+
      if (ltype.eq.2.and.mode.eq.5) wtmax = 0.0013 !  INT-
      if (ltype.eq.2.and.mode.eq.6) wtmax = 1.09e6 !  FULL

c ... Generate (x,y) uniformly over their physical allowed regions
c ... Where x=2Eg/MPI , y=2El/MPI, rl=(MLEP/MPI)^2      ! See PDG reference
c
 1    CONTINUE
      CALL RANLUX(rando,2)
      x    = rando(1) * (1-rl)
      y    = rando(2) * (1+rl)
     
c      x    = ranf() * (1-rl)
c      y    = ranf() * (1+rl)
      ymin = 1-x+rl/(1-x)
      if (y.lt.ymin) goto 1

      Eg = 0.5 * MPI * x
      if (Eg.lt.0.010) goto 1  ! infrared cutoff Eg > 0.01 GeV optional
      
c  -----  FORM-FACTORS etc. -----
c
c  Form-factors taken from the PDG Review of Particle Physics
c  http://journals.aps.org/prd/pdf/10.1103/PhysRevD.86.010001, page 34
c
      Fv0 = 0.0254  ! +/- 0.0017    
      Fva = 0.10    ! +/- 0.06         - Fv slope parameter
      Fv  = Fv0*(1 + Fva*(1-x))      ! - vector form factor                   
      Fa  = 0.0119  ! +/- 0.0001       - axial-vector form factor
      fPI = 0.13041 ! from J. Rosner et al. (2012)           
      alpha = 1./137.036

c  ------  MATRIX ELEMENT  ------
c
      f1      = 1-y+rl
      f2      = x+y-1-rl                                ! Notations from PDG reference:
      f_ib    = f1/(x*x*f2)*(x*x+2*(1-x)*(1-rl)-2*x*rl*(1-rl)/f2) ! IB(x,y)
      f_sd_p  = f2*((x+y-1)*(1-x)-rl)                             ! SD+(x,y)
      f_sd_m  = (1-y+rl)*((1-x)*(1-y)+rl)                         ! SD-(x,y)
      f_int_p = f1/x/f2*((1-x)*(1-x-y)+rl)                        ! INT+(x,y)
      f_int_m = f1/x/f2*(x*x-(1-x)*(1-x-y)-rl)                    ! INT-(x,y)

c -------- COEFFICIENTS ---------
c
      c_sd_p = (MPI/fPI)*(MPI/fPI)/4/rl*(Fv+Fa)*(Fv+Fa)
      c_sd_m = (MPI/fPI)*(MPI/fPI)/4/rl*(Fv-Fa)*(Fv-Fa)
      c_int_p = MPI/fPI*(Fv+Fa)
      c_int_m = MPI/fPI*(Fv-Fa)
c     c_g = alpha/(2.*Pi)/(1-rl)/(1-rl)

      f_tot = f_ib + c_sd_p*f_sd_p + c_sd_m*f_sd_m
     >      + c_int_p*f_int_p + c_int_m*f_int_m         ! psi^(0) terms

c ------  RADIATIVE CORRECTIONS -----
c See Bystritsky et al. arXiv:hep-ph/0310275v3 (2004-2013), page 14
c
      xb = 1.-x
      yb = 1.-y
      z = x+y-1.
      if(xb.lt.0.00001) xb = 0.00001
      if(yb.lt.0.00001) yb = 0.00001
      if( z.lt.0.00001)  z = 0.00001
     
      rcf = alpha/(2.*Pi)*(log(y*y/rl) - 1.)
      rc_ib = ((1+xb*xb)/x*x)*(3/2*yb/z+yb/xb -(xb+x*y)/xb**2*log(y)
     >        + 2.*yb/z*log(yb/y)-x*(xb*xb+y**2)/(xb*xb*z)*log(x/z))
      rc_sd_p = xb*(3*z*z/2 + (1-y*y)/2 + yb*(y-2*xb) 
     >        + xb*(xb-2*y)*log(y) - xb*xb*yb + 2*z*z*log(yb/y))
      rc_sd_m = xb*(3*yb*yb/2 + (1-y*y)/2. + yb*(y-3.) + (1-2*y)*log(y)
     >	      + 2*yb*yb*log(yb/y))
      rc_int_p = (xb/x)*(yb/2 - yb*log(y) - 2*yb*log(yb/y));
      rc_int_m = (1./x)*(-xb*yb/2. + 3.*x*x*yb/(z*2)  
     >	      + xb*(yb*log(y) + 2*yb*log(yb/y)) 
     >	      + x*x*(yb/x - (xb+x*y)/(xb*xb)*log(y) + 2*yb/z*log(yb/y)
     >        - x*(xb*xb + y*y)/(xb*xb*z)*log(x/z)))

      rc_tot = rcf*(rc_ib + c_sd_p*rc_sd_p + c_sd_m*rc_sd_m
     >	      + c_int_p*rc_int_p + c_int_m*rc_int_m)        ! psi^(1) terms

      if (mode.eq.1) f_here = f_ib + rcf*rc_ib
      if (mode.eq.2) f_here = c_sd_p*(f_sd_p + rcf*rc_sd_p)
      if (mode.eq.3) f_here = c_sd_m*(f_sd_m + rcf*rc_sd_m)
      if (mode.eq.4) f_here = c_int_p*(f_int_p + rcf*rc_int_p)
      if (mode.eq.5) f_here = -c_int_m*(f_int_m + rcf*rc_int_m) ! changed sign
      if (mode.eq.6) f_here = f_tot + rc_tot                ! full calculation

      CALL RANLUX(rando,2)
      wtcomp = rando(1)*wtmax
c      wtcomp = ranf()*wtmax
      if (wtcomp.gt.f_here) goto 1

c --- Transform (x,y) into 4-momenta in pion rest frame
c ... Lepton momentum is aligned along the X axis
      Eg  = 0.5 * MPI * x
      Ee  = 0.5 * MPI * y
      En  = MPI - Eg - Ee
      Pe  = sqrt(Ee*Ee - MLEP*MLEP)
      Pgx = -0.5 * (En*En - Eg*Eg - Pe*Pe) / Pe
      Pgx = sqrt(Eg*Eg - Pgx*Pgx)

      plep(1) = Pe
      plep(2) = 0.0
      plep(3) = 0.0
      plep(4) = Ee
      pgam(1) = Pgx
      pgam(2) = Pgy
      pgam(3) = 0.0
      pgam(4) = Eg
      pnu(1)  = -Pe-Pgx
      pnu(2)  = -Pgy
      pnu(3)  = 0.0
      pnu(4)  = En

c --- Finally, perform a rotation

c --- For rotation of momenta into a random direction, let us use
c --- the Euler angles: rotations around z, unrotated x, unrotated z

      DO I = 1, 4
         P1(1, I) = Plep(I)
         P1(2, I) = Pgam(I)
         P1(3, I) = Pnu(I)
      ENDDO

c --- a) Counterclockwise rotation around Z axis (PHI)
      PHI    = rando(2)*2.0*PI
c      PHI    = RANF()*2.0*PI
      SINPHI = DSIN(PHI)
      COSPHI = DCOS(PHI)
      DO I = 1, 3
         P2(I,1) =  P1(I,1)*COSPHI + P1(I,2)*SINPHI
         P2(I,2) = -P1(I,1)*SINPHI + P1(I,2)*COSPHI
         P2(I,3) =  P1(I,3)
         P2(I,4) =  P1(I,4)
      ENDDO

c --- b) Generate uniformly the new direction of Z axis,
c ---    define the corresponding Euler angles THETA, PSI
      CALL GENSPH(VEC)
      COSTHE = VEC(3)/SQRT(VEC(1)**2+VEC(2)**2+VEC(3)**2)
      SINTHE = SQRT((VEC(1)**2+VEC(2)**2)/
     >              (VEC(1)**2+VEC(2)**2+VEC(3)**2))
      COSPSI = VEC(2)/SQRT(VEC(1)**2+VEC(2)**2)
      SINPSI = VEC(1)/SQRT(VEC(1)**2+VEC(2)**2)

c --- c) Clockwise rotation around X axis (THETA)
      DO I = 1, 3
         P3(I,1) = P2(I,1)
         P3(I,2) = P2(I,2)*COSTHE - P2(I,3)*SINTHE
         P3(I,3) = P2(I,2)*SINTHE + P2(I,3)*COSTHE
         P3(I,4) = P2(I,4)
      ENDDO

c --- d) Counterclockwise rotation around Z axis (PSI)
      DO I = 1, 3
         P4(I,1) = P3(I,1)*COSPSI - P3(I,2)*SINPSI
         P4(I,2) = P3(I,1)*SINPSI + P3(I,2)*COSPSI
         P4(I,3) = P3(I,3)
         P4(I,4) = P3(I,4)
      ENDDO

c --- put the results back into the old vectors
      do i = 1, 4
         Plep(i) = P4(1, I)
         Pgam(i) = P4(2, I)
         Pnu(i)  = P4(3, I)
      enddo

c-------------------------------------------------------

c --- FILL MC PARTICLE LIST
      if (ltype.eq.1) IDLEP = IDELEP
      if (ltype.eq.2) IDLEP = IDMUP
      JLEP = MCADD4GEN(JPION, IDLEP, plep, 0)
      JGAM = MCADD4GEN(JPION, IDGAM, pgam, 0)
      JNU  = MCADD4GEN(JPION, IDNUE,  pnu,  0)

c --- BOOST TO THE LAB-SYSTEM
      CALL DBOOST(P4INI(1,JPION),MPI,plep,plep)
      CALL DBOOST(P4INI(1,JPION),MPI,pgam,pgam)

c --- FILL MC PARTICLE LIST
      JLEP = MCADD4(IDLEP, plep)
      JGAM = MCADD4(IDGAM, pgam)

      RETURN
      END
