      SUBROUTINE PICH2ENUG_GATTI(JPION, MODE)
C-------------------------------------------------------------C
C PI+- -> E NU GAMMA DECAY                                    C
C                                                             C
C IB matrix element: Bijnens,Ecker,Gasser,hep-ph/9209261      C
C Higher-order corrections: C.Gatti, EPJC45 (2006) 417        C
C This wrapper just calls the KLOE generators,                C
C boosts daughters into lab frame & interfaces to GEANT       C
C E.Goudzovski 3/08/2009, 28/06/2011                          C
C                                                             C
C modified to pass muon polarization to GEANT4                C
C by: M.Koval, 14/8/2013, michal.koval@cern.ch                C
C For details, see internal note NA62-13-09.                  C
C                                                             C
C Original function: KCH2LNUG_IB(JKAON, LTYPE)                C
C Adapted for pion decay by D.Protopopescu & I.Skillicorn     C
C Dan.Protopopescu@cern.ch 22/10/2014                         C
C For details, see CERN internal note NA62-14-10.             C
C                                                             C
C MODE: 1=IB, 2=SD+, 3=SD-, 4=INT+, 5=INT-, 6=ALL             C 
C-------------------------------------------------------------C

#include "common_blocks.f"
#include "masses.f"

      INTEGER MODE, JPION, JELEC, JGAMMA, istat, i
      real*8 PPCM(4,3)
      REAL*8 p4e(4), p4g(4), x, y

      INTEGER PIE2G_GATTI

      if (mode.lt.1.or.mode.gt.6) mode = 6
      istat = PIE2G_GATTI (PPCM, MODE)
      do i=1,4
         p4g(i) = PPCM(i,1)
         p4e(i) = PPCM(i,2)
      enddo

      x = 2.0*p4g(4)/MPI
      y = 2.0*p4e(4)/MPI

C --- FILL MC PARTICLE LIST
      JELEC  = MCADD4GEN(JPION, IDELEP, p4e, 0)
      JGAMMA = MCADD4GEN(JPION, IDGAM, p4g, 0)

C --- BOOST TO THE LAB-SYSTEM
      CALL DBOOST(P4INI(1,JPION),MPI,p4e,p4e)
      if (x.gt.1.0e-10) CALL DBOOST(P4INI(1,JPION),MPI,p4g,p4g)

C --- FILL Pie2 MC PARTICLE LIST
      JELEC = MCADD4(IDELEP, p4e)
      if (x.gt.1.0e-10) JGAMMA = MCADD4(IDGAM, p4g)

      RETURN
      END

C -------------------------------------------------------------

      Function PIE2G_GATTI(PCM, MODE)
C---------------------------------------------------------C
C PI+- --> E NU (GAMMA) DECAY                             C
C Includes IB consistently the with RK definition         C
C IB matrix element: Bijnens,Ecker,Gasser,hep-ph/9209261  C
C Higher-order corrections: C.Gatti, EPJC45 (2006) 417    C
C Imported from the KLOE library with minimal changes     C
C (thanks to Tommaso Spadaro)                             C
C E.Goudzovski 3/08/2009, 28/06/2011                      C       
C                                                         C
C Original function: KE2G_IB_KLOE(PCM)                    C
C Adapted for pion decay by D.Protopopescu & I.Skillicorn C
C Dan.Protopopescu@cern.ch 22/10/2014                     C
C For details, see CERN internal note NA62-14-10          C
C---------------------------------------------------------C

      IMPLICIT NONE

#include "masses.f"

      real*8 PCM(4,3), Amp, Amax
      integer MODE, status, PIE2G_GATTI

      real*8 Fa, Fv, Fva, Fv0
      real*8 betae, b, rl
      real*8 x, y, Ctheta, Eg, El
      real*8 rando(2), pb(1), angles(3)
      real*8 g_ib, g_sd_p, g_sd_m, g_int_p, g_int_m
      real*8 c_sd_p, c_sd_m, c_int_p, c_int_m
      real*8 ctg, stg, cpg, spg, cpl, spl
      real*8 RCM(3,2)

C     Parameters
      real*8    pi, alpha, fPI
      Parameter (pi = 3.1415927d+00)
      Parameter (alpha=1.d+00/137.03599968d+00)
      Parameter (fPI = 0.13041)   ! from J. Rosner et al. (2012)
C
C  Form-factors taken from the PDG Review of Particle Physics
C  http://journals.aps.org/prd/pdf/10.1103/PhysRevD.86.010001, page 34
C
      Fv0 = 0.0254  ! +/- 0.0017    
      Fva = 0.10    ! +/- 0.06    - Fv slope parameter
C     Fv  = Fv0*(1 + Fva*(1-x)) ! - calculated within loop                   
      Fa  = 0.0119  ! +/- 0.0001  - axial-vector form factor  

C  These need to be calculated for all modes if Amp is rescaled
      if (mode.eq.1) Amax = 1.985   ! IB
      if (mode.eq.2) Amax = 2.418   ! SD+
      if (mode.eq.3) Amax = 0.051   ! SD-
      if (mode.eq.4) Amax = 0.002   ! INT+
      if (mode.eq.5) Amax = 0.014   ! INT-
      if (mode.eq.6) Amax = 2.421   ! Full

C  Lepton mass and max value of the non-peaking factor
      rl = mel*mel/SQMPI    
      
C  Bond factor
      betae= dsqrt(1.d+00-(2.d+00*MPI*mel/(mel**2+MPI**2))**2)
      b = - 2.d+00*alpha/pi*( 1.d+00-dlog((1.d+00+betae)/(1.d+00-betae))
     >     /(2.d+00*betae))

      status = 0
      Do while (status.eq.0)

C  Energy Distribution + y

         CALL RANLUX(rando,2)

C  Extraction of y = (El - Pl*Ctheta)/m_K
         y = rl**(1.d+00 - dble(rando(2)))
         
C  Photon Energy
         x = (1.d+00 - rl)*dble(rando(1))**(1.d+00/b)
         Eg = x*MPI/2.d+00
         
C  x-dependant parms        
         Fv  = Fv0*(1 + Fva*(1-x))

C  Lepton energy
         El = (MPI**2+mel**2+2.d+00*MPI*Eg*(y-1.d+00))/(2.d+00*MPI)

C  Ctheta
         if (El.gt.mel) then
            Ctheta = (El-MPI* y)/(dsqrt(El**2-mel**2))
         Else
            goto 654
         Endif

         If (Ctheta.lt.-1.d+00.or.Ctheta.gt.1.d+00) goto 654
C
C  Amplitude split into IB, SD+/-, INT+/- terms 
C
         g_ib    = (1.d+00 - y)*(x*x + 2.d+00*(1.d+00 - rl)*
     >        (1.d+00 - x - rl/y))
         g_sd_p  = -x*x*x*x*y*y*(rl - y + x*y)
         g_sd_m  = -x*x*x*x*y*(y - 1.d+00)*
     >        (rl - y + x*y - x + 1.d+00)
         g_int_p = -x*x*(y - 1.d+00)*(rl - y + x*y)
         g_int_m = x*x*(y - 1.d+00)*(rl - y + x*y - x)
C
C  Coefficients
C
         c_sd_p = (MPI/fPI)*(MPI/fPI)/4./rl*(Fv+Fa)*(Fv+Fa)
         c_sd_m = (MPI/fPI)*(MPI/fPI)/4./rl*(Fv-Fa)*(Fv-Fa)
         c_int_p = MPI/fPI*(Fv+Fa)
         c_int_m = MPI/fPI*(Fv-Fa)
         
         if (mode.eq.1) Amp = g_ib          
         if (mode.eq.2) Amp = c_sd_p*g_sd_p
         if (mode.eq.3) Amp = c_sd_m*g_sd_m
         if (mode.eq.4) Amp = -c_int_p*g_int_p ! changed sign
         if (mode.eq.5) Amp = c_int_m*g_int_m
         
         if (mode.eq.6) Amp = g_ib + c_sd_p*g_sd_p 
     >                      + c_sd_m*g_sd_m
     >                      + c_int_p*g_int_p 
     >                      + c_int_m*g_int_m  ! all terms

C --- To match original KLOE implementation we would need
C     Amp = (fPI*fPI*MPI*MPI*mel*mel/2) * Amp / 1.0d-9
C     with all Amax parameters recalculated

         If (Amp.gt.Amax) then
            write (*,*) '@ piG:ERROR prob>1', Amp
         Endif

         CALL RANLUX(pb,1)      ! hit or miss
         If (dble(pb(1))*Amax.le.Amp) then
            Status = 1
         Endif

 654     continue
      enddo                     ! end of "do while"
c      If (Ctheta.lt.0.76604.and.Eg.gt.0.01) then
c         PRINT *, Eg, El, Ctheta
c      Endif
      CALL RANLUX(angles,3)

      ctg=dble(angles(1))*2.d+00-1.d+00
      stg=dsqrt(1.d+00-ctg**2)
      cpg=dcos(dble(angles(2))*2.d+00*pi)
      spg=dsin(dble(angles(2))*2.d+00*pi)
      cpl=dcos(dble(angles(3))*2.d+00*pi)
      spl=dsin(dble(angles(3))*2.d+00*pi)

C     photon
      RCM(1,1) = 0.d+00
      RCM(2,1) = 0.d+00
      RCM(3,1) = Eg

C     lepton
      RCM(1,2) = dsqrt(El**2-mel**2)*dsqrt(1.d+00-Ctheta**2)*cpl
      RCM(2,2) = dsqrt(El**2-mel**2)*dsqrt(1.d+00-Ctheta**2)*spl
      RCM(3,2) = dsqrt(El**2-mel**2)*Ctheta

C Rotation

C     photon
      PCM(1,1) = cpg*stg*RCM(3,1)
      PCM(2,1) = spg*stg*RCM(3,1)
      PCM(3,1) = ctg*RCM(3,1)
      PCM(4,1) = Eg

C     lepton
      PCM(1,2) = cpg*ctg*RCM(1,2)-spg*RCM(2,2)+cpg*stg*RCM(3,2)
      PCM(2,2) = spg*ctg*RCM(1,2)+cpg*RCM(2,2)+spg*stg*RCM(3,2)
      PCM(3,2) = -stg*RCM(1,2)+ctg*RCM(3,2)
      PCM(4,2) = El

C     neutrino
      PCM(1,3) = - PCM(1,1) - PCM(1,2)
      PCM(2,3) = - PCM(2,1) - PCM(2,2)
      PCM(3,3) = - PCM(3,1) - PCM(3,2)
      PCM(4,3) = MPI - Eg - El

      PIE2G_GATTI = 0
      RETURN
      END

