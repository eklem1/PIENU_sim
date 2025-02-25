      SUBROUTINE KCH2LNUG_IB(JKAON, PTYPE, LTYPE, PCHARGE)
C--------------------------------------------------------C
C K+- ->L NU GAMMA DECAY                                 C
C Includes IB consistently with the RK definition        C
C IB matrix element: Bijnens,Ecker,Gasser,hep-ph/9209261 C
C Higher-order corrections: C.Gatti, EPJC45 (2006) 417   C
C This wrapper just calls the KLOE generators,           C
C boosts daughters into lab frame & interfaces to GEANT  C
C LTYPE:  1=electron, 2=muon                             C
C E.Goudzovski 3/08/2009, 28/06/2011                     C
C                                                        C
C Modified to pass muon polarization to GEANT4           C
C by M.Koval, 14/8/2013, michal.koval@cern.ch            C
C For details, see internal note NA62-13-09.             C
C                                                        C
C Unified Kl2g generator added (based on Km2g_IB_kloe)   C
C by K. Massri, 24/01/2018, karim.massri@cern.ch         C
C                                                        C
C Updated muon polarization vector calculation           C
C Now it works not only for 2-body decay                 C
C Radiative corrections to the muon polarization are     C
C very small as shown in E. Ginsberg and R. Pratt,       C
C “On the radiative corrections to the muon polarization C
C from pion decay”, http://dx.doi.org/10.1007/BF02750561 C
C Muon polarization vector direction is identical        C
C to the direction of the kaon momentum in the           C
C muon rest frame                                        C
C by A. Shaikhiev 04/08/2018 shaykhiev@inr.ru            C
C--------------------------------------------------------C

#include "common_blocks.f"
#include "masses.f"

      INTEGER PTYPE, LTYPE, IDLEP, J, IDNEUTRINO, PCHARGE
      INTEGER JELEC, JNU, JGAMMA, istat, i
      REAL*8 MPARENT, PPCM(4,3)
      REAL*8 p4e(4), p4n(4), p4g(4), x, POL(3)
      REAL*8 scale, P0(4), p4e_op(4)

      INTEGER KE2G_IB_KLOE, KM2G_IB_KLOE, KL2G_IB_KLOE

      if (ptype.ne.1.and.ptype.ne.2) ptype = 1 ! parent type: kaon (1) or pion (2)
      if (ltype.ne.1.and.ltype.ne.2) ltype = 1 ! lepton type: electron (1) or muon (2)

      if (ptype.eq.1) MPARENT = MKCH
      if (ptype.eq.2) MPARENT = MPI

c     if (ltype.eq.1) istat = ke2g_ib_kloe (PPCM, MPARENT) !separated generators (original)
c     if (ltype.eq.2) istat = km2g_ib_kloe (PPCM, MPARENT) !separated generators (original)
      if (ltype.eq.1) istat = kl2g_ib_kloe (PPCM, MPARENT, MEL)
      if (ltype.eq.2) istat = kl2g_ib_kloe (PPCM, MPARENT, MMU)

      do i=1,4
         p4g(i) = PPCM(i,1) ! photon
         p4e(i) = PPCM(i,2) ! lepton
         p4n(i) = PPCM(i,3) ! neutrino
      enddo

      x = 2.0*p4g(4)/MPARENT
c     y = 2.0*p4e(4)/MPARENT

c --- FILL MC PARTICLE LIST
      if (pcharge.eq.1.and.ltype.eq.1) then
        IDLEP = IDELEP
        IDNEUTRINO = IDNUE
      endif
      if (pcharge.eq.-1.and.ltype.eq.1) then
        IDLEP = IDELEM
        IDNEUTRINO = IDNUE
      endif
      if (pcharge.eq.1.and.ltype.eq.2) then
        IDLEP = IDMUP
        IDNEUTRINO = IDNUM
      endif
      if (pcharge.eq.-1.and.ltype.eq.2) then
        IDLEP = IDMUM
        IDNEUTRINO = IDNUM
      endif
      if (MainDecay.eq.1) then
        if (ptype.eq.1) then
          JELEC  = MCADD4GEN(IDLEP, p4e, 0)
          JNU    = MCADD4GEN(IDNEUTRINO,  p4n, 0)
          JGAMMA = MCADD4GEN(IDGAM, p4g, 0)
        else
          JELEC  = MCADD4GEN(IDLEP, p4e, 4)
          JNU    = MCADD4GEN(IDNEUTRINO,  p4n, 4)
          JGAMMA = MCADD4GEN(IDGAM, p4g, 4)
        endif
      endif

c --- BOOST TO THE LAB-SYSTEM
      CALL DBOOST(P4INI(1,JKAON),MPARENT,p4e,p4e)
      CALL DBOOST(P4INI(1,JKAON),MPARENT,p4n,p4n)
      if (x.gt.1.0e-10) CALL DBOOST(P4INI(1,JKAON),MPARENT,p4g,p4g)

c --- FILL MC PARTICLE LIST
      if (ltype.eq.1) then ! Ke2: add the positron
         JELEC = MCADD4(IDLEP, p4e)
      else ! Kmu2 and pimu2: calculate muon polarization in its rest frame; add the polarized muon
         DO J = 1, 4
            P0(J) = P4INI(J,JKAON)
         ENDDO
c        boost kaon/pion to the muon rest frame
         P4E_OP(1) = -P4E(1)
         P4E_OP(2) = -P4E(2)
         P4E_OP(3) = -P4E(3)
         P4E_OP(4) = P4E(4)
         CALL DBOOST(P4E_OP,MMU,P0,P0)
         scale = 1./sqrt(P0(1)*P0(1)+P0(2)*P0(2)+P0(3)*P0(3))
         POL(1) = scale*P0(1)
         POL(2) = scale*P0(2)
         POL(3) = scale*P0(3)
         JELEC = MCADD4POL3(IDLEP, P4E, POL)
      endif
c ... add the neutrino and the photon
      JNU = MCADD4(IDNEUTRINO, p4n)
      if (x.gt.1.0e-10) JGAMMA = MCADD4(IDGAM, p4g)
      RETURN
      END

c----------------------------------------------------------------------

      Function KE2G_IB_KLOE(PCM, MPAR)
C--------------------------------------------------------C
C K+- --> E NU (GAMMA) DECAY                             C
C Includes IB consistently the with RK definition        C
C IB matrix element: Bijnens,Ecker,Gasser,hep-ph/9209261 C
C Higher-order corrections: C.Gatti, EPJC45 (2006) 417   C
C Imported from the KLOE library with minimal changes    C
C (thanks to Tommaso Spadaro)                            C
C E.Goudzovski 3/08/2009, 28/06/2011                     C
C--------------------------------------------------------C

      IMPLICIT NONE

#include "masses.f"

      real*8 MPAR, PCM(4,3), Amp
      integer status, KE2G_IB_KLOE
      real*8 betae,b
      real*8 Eg,Emax,El
      real*8 Cteta,y
      real*8 rando(2),pb(1),angoli(3)
      real*8 Ampiezza,factor
      real*8 ctg,stg,cpg,spg,cpl,spl
      real*8 RCM(3,2), MLEP

C     Parameters
      real*8    pi, alfa, Fk, LH, RH
      Parameter (pi = 3.1415927d+00)
      Parameter (alfa=1.d+00/137.03599968d+00)
      Parameter (Fk=0.1136d+00) ! GeV
      Parameter (RH=0.d+00)! -0.097d+00/mkch) ! (V+A)=0 for IB
      Parameter (LH=0.d+00)! -0.037/mkch)     ! (V-A)=0 for IB

C     lepton mass and max value of the non-peaking factor
      mlep   = MEL
      factor = 1.0d-9

C     bond factor
      betae= dsqrt(1.d+00-(2.d+00*mpar*mlep/(mlep**2+mpar**2))**2)
      b = - 2.d+00*alfa/pi*( 1.d+00-dlog((1.d+00+betae)/(1.d+00-betae))
     $     /(2.d+00*betae))

C     max photon energy
      Emax = 0.5d+00*(mpar-mlep**2/mpar)

      status = 0
      Do while (status.eq.0)

C     Energy Distribution + y

         CALL RANLUX(rando,2)

C     extraction of y=(El-Pl Cteta)/Mk

         y = (mpar/mlep)**(2.d+00*(dble(rando(2))-1.d+00))

C     photon Energy
         Eg = Emax*dble(rando(1))**(1.d+00/b)

C     lepton energy
         El= (mpar**2+mlep**2+2.d+00*mpar*Eg*(y-1.d+00))/(2.d+00*mpar)

C     Cteta
         if (El.gt.mlep) then
            Cteta = (El -mpar* y)/(dsqrt(El**2-mlep**2))
         Else
            Cteta = 0.d+00
         Endif

         If (Cteta.lt.-1.d+00.or.Cteta.gt.1.d+00) goto 654

C     Ampiezza^2 kl2gamma
C
C     RH = V+A
C     LH = V-A
C
C     K(p) -> l(l) u(u) gamma(q)   |q| = Eg
C
C     l*q = y Eg mk = Eg (El - l Cteta)
C     l*p = (mk^2+mel^2-2 Eg mk+ 2 y Eg mk)/2
C
C     epsilon^(0123)=-1
C     16*Gf**2*Pi*Vus**2*alfa*

         Ampiezza =
     $        -(-2.d+00*Eg**2*Fk*mpar**2*mlep**2*(-1.d+00 + y)*y*
     $        ((LH - RH)*(mlep**2 - mpar**2*y) +
     $        2.d+00*Eg*mpar*(LH*(-1.d+00 + y) - RH*y))
     $        + 2.d+00*Eg**4*mpar**4*y**2*
     $        (LH**2*(mlep**2 - mpar**2*(-1.d+00 + y))*(-1.d+00 + y) -
     $        RH**2*y*(-mlep**2 + mpar**2*y) +
     $        2.d+00*Eg*mpar*(LH**2*(-1.d+00 + y)**2 + RH**2*y**2)) +
     $        Fk**2*mlep**2*(-1.d+00 + y)*
     $        (mlep**4 - 2.d+00*Eg*mpar**3*y + mpar**4*y +
     $        2.d+00*Eg*mpar*mlep**2*y -
     $        mpar**2*(-2.d+00*Eg**2*y + mlep**2*(1.d+00 + y))))
     $        /(mpar**2*y)

         Amp = Ampiezza / factor

         If (Amp.gt.1.01) then
            write (0,*) '@ KL2G:ERROR prob>1',Amp
         Endif

         CALL RANLUX(pb,1)      ! hit or miss
         If (dble(pb(1))*1.01.le.Amp) then
            Status = 1
         Endif

 654     continue
      enddo ! end "do while"

      CALL RANLUX(angoli,3)

      ctg=dble(angoli(1))*2.d+00-1.d+00
      stg=dsqrt(1.d+00-ctg**2)
      cpg=dcos(dble(angoli(2))*2.d+00*pi)
      spg=dsin(dble(angoli(2))*2.d+00*pi)
      cpl=dcos(dble(angoli(3))*2.d+00*pi)
      spl=dsin(dble(angoli(3))*2.d+00*pi)

C     photon
      RCM(1,1) = 0.d+00
      RCM(2,1) = 0.d+00
      RCM(3,1) = Eg

C     lepton
      RCM(1,2) = dsqrt(El**2-mlep**2)*dsqrt(1.d+00-Cteta**2)*cpl
      RCM(2,2) = dsqrt(El**2-mlep**2)*dsqrt(1.d+00-Cteta**2)*spl
      RCM(3,2) = dsqrt(El**2-mlep**2)*Cteta

C rotation

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
      PCM(4,3) = mpar - Eg - El

      KE2G_IB_KLOE = 0
      Return
      End

c---------------------------------------------------------------

      Integer Function KM2G_IB_KLOE(PCM, MPAR)
C--------------------------------------------------------C
C K+- --> MU NU (GAMMA) DECAY                            C
C Includes IB consistently the with RK definition        C
C IB matrix element: Bijnens,Ecker,Gasser,hep-ph/9209261 C
C Higher-order corrections: C.Gatti, EPJC45 (2006) 417   C
C Imported from the KLOE library with minimal changes    C
C (thanks to Tommaso Spadaro)                            C
C E.Goudzovski 28/06/2011                                C
C--------------------------------------------------------C
C                                                        C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Author: C. Gatti     11/12/03
C     MC generator for K+ --> mu+ nu gamma
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      IMPLICIT NONE

#include "masses.f"

      real*8 MPAR, PCM(4,3), Amp
      integer status, KM2_IB_KLOE
      real*8 betae,b
      real*8 Eg,Emax,Elgmax,El
      real*8 Cteta,Steta,delta,boundary,y
      real*8 rando(2),pb(1),angoli(3)
      real*8 Ampiezza,PSF,factor
      real*8 ctg,stg,cpg,spg,cpl,spl
      real*8 RCM(3,2)

C     Parameters
      real*8 pi, alfa, Fk, LH, RH
      Parameter (pi = 3.1415927d+00)
      Parameter (alfa=1.d+00/137.03599976d+00)
C MeV
      Parameter (Fk=0.1136d+00) ! GeV
      Parameter (RH=0.d+00) ! -0.097d+00/mkch) ! (V+A)=0 for IB
      Parameter (LH=0.d+00) ! -0.037d+00/mkch) ! (V-A)=0 for IB

C     bond factor
      betae= dsqrt(1.d+00-(2.d+00*mpar*mmu/(mmu**2+mpar**2))**2)
      b = - 2.d+00*alfa/pi*( 1.d+00-dlog((1.d+00+betae)/(1.d+00-betae))
     $     /(2.d+00*betae))

C     max photon energy
      Emax = 0.5d+00*(mpar-mmu**2/mpar)

      status = 0
      Do while (status.eq.0)

C     Energy Distribution + Angular Flat
         CALL RANLUX(rando,2)

C     Angle
         Cteta = dble(rando(2))*2.d+00-1.d+00
         if ((1.d+00 - Cteta**2).lt.0.d+00) then
           Steta = 0.d+00
         else
           Steta = dsqrt(1.d+00 - Cteta**2)
         end if

C     lepton+ energy which gives the maximum photon energy given Cteta
         Elgmax = -((mmu*(Steta*
     $        (mpar**2 - mmu**2)**2 -
     $        2.d+00*Cteta**2*mpar*mmu*(mpar**2 + mmu**2)))/
     $        ((-1.d+00 + Cteta**2)*mpar**4 +
     $        2.d+00*(1.d+00 + Cteta**2)*mpar**2*mmu**2 +
     $        (-1.d+00 + Cteta**2)*mmu**4))

C     boundary is the photon energy as a function of El+ and Cteta
C     when El+ is Elgmax
         boundary = (-2.d+00*Elgmax*mpar + mpar**2 + mmu**2)/
     $        (-2.d+00*Elgmax + 2.d+00*mpar +
     $        2.d+00*Cteta*dsqrt(Elgmax**2 - mmu**2))

C     photon Energy
         Eg = Emax*dble(rando(1))**(1.d+00/b)
c         Eg = Emax*dble(0.995+rando(1)*0.005)**(1.d+00/b)

C If outside the dalitz plot
         If (Eg.gt.boundary) Goto 654
         delta = (mpar**2 - mmu**2)**2 +
     $        4.d+00*Eg**2*(mpar**2 + (-1.d+00 + Cteta**2)*mmu**2)-
     $        4.d+00*Eg*(mpar**3 - mpar*mmu**2)

C out of Dplot
         If (delta.lt.0.d+00) goto 654
         El = (-2.d+00*Eg**2*mpar - mpar*(mpar**2 + mmu**2) +
     $        Eg*(3.d+00*mpar**2 + mmu**2 +
     $        Cteta*dsqrt(delta)))/
     $        (2.d+00*(-1.d+00 + Cteta**2)*Eg**2
     $        + 4.d+00*Eg*mpar - 2*mpar**2)

C     Ampiezza^2 kl2gamma
C
C     RH = V+A
C     LH = V-A
C
C     K(p) -> l(l) u(u) gamma(q)   |q| = Eg
C
C     l*q = y Eg mk = Eg (El - l Cteta)
C     l*p = (mk^2+mmu^2-2 Eg mk+ 2 y Eg mk)/2
C
C     epsilon^(0123)=-1
C     16*Gf**2*Pi*Vus**2*alfa*
         y = (El - dsqrt(El**2-mmu**2)* Cteta)/mpar

c$$$         write(*,*) 'inside km2gene',El,Eg,Cteta,y,rando(1),rando(2)

         Ampiezza =
     $        -(-2.d+00*Eg**2*Fk*mpar**2*mmu**2*(-1.d+00 + y)*y*
     $        ((LH - RH)*(mmu**2 - mpar**2*y) +
     $        2.d+00*Eg*mpar*(LH*(-1.d+00 + y) - RH*y))
     $        + 2.d+00*Eg**4*mpar**4*y**2*
     $        (LH**2*(mmu**2 - mpar**2*(-1.d+00 + y))*(-1.d+00 + y) -
     $        RH**2*y*(-mmu**2 + mpar**2*y) +
     $        2.d+00*Eg*mpar*(LH**2*(-1.d+00 + y)**2 + RH**2*y**2)) +
     $        Fk**2*mmu**2*(-1.d+00 + y)*
     $        (mmu**4 - 2.d+00*Eg*mpar**3*y + mpar**4*y +
     $        2.d+00*Eg*mpar*mmu**2*y -
     $        mpar**2*(-2.d+00*Eg**2*y + mmu**2*(1.d+00 + y))))
     $        /(mpar**2*y**2)

C     phase space factor  1/(b (2pi)^3 8 mkch)
         PSF = (El**2-mmu**2)/(mpar*dsqrt(El**2-mmu**2)-
     $        Eg*(dsqrt(El**2-mmu**2)-El*Cteta))
         factor= (Fk**2*(mpar**2 - mmu**2)**4)/(8.d+00*mpar**4)

c         write (*,*) 'km2gene ',PSF,factor,1./y,ampiezza

         Amp = PSF * Ampiezza / factor
         If (Amp.gt.1.01) then
C            write (0,*) 'KCMU2G:ERROR prob>1',Amp
         Endif
C hit or miss
         CALL RANLUX(pb,1)
         If (pb(1)*1.01.le.Amp) then
            Status = 1
         Endif

 654     continue
      enddo ! end "do while"

C-- Prevent underflow problems with optimizer
      if (Eg.lt.1.d-12) Eg = 0.d+00
      CALL RANLUX(angoli,3)
      ctg=dble(angoli(1))*2.d+00-1.d+00
      if ((1.d+00-ctg**2).lt.0.d+00) then
        stg = 0.d+00
      else
        stg=dsqrt(1.d+00-ctg**2)
      end if
      cpg=dcos(dble(angoli(2))*2.d+00*pi)
      spg=dsin(dble(angoli(2))*2.d+00*pi)
      cpl=dcos(dble(angoli(3))*2.d+00*pi)
      spl=dsin(dble(angoli(3))*2.d+00*pi)

C     photon
      RCM(1,1) = 0.d+00
      RCM(2,1) = 0.d+00
      RCM(3,1) = Eg

C     lepton
      RCM(1,2) = dsqrt(El**2-mmu**2)*Steta*cpl
      RCM(2,2) = dsqrt(El**2-mmu**2)*Steta*spl
      RCM(3,2) = dsqrt(El**2-mmu**2)*Cteta

C rotation

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
      PCM(4,3) = mpar - Eg - El

      KM2G_IB_KLOE = 0
      Return
      End

c---------------------------------------------------------------

      Integer Function KL2G_IB_KLOE(PCM, MPAR, MLEP)
C--------------------------------------------------------C
C K+- --> L NU (GAMMA) DECAY                            C
C Includes IB consistently the with RK definition        C
C IB matrix element: Bijnens,Ecker,Gasser,hep-ph/9209261 C
C Higher-order corrections: C.Gatti, EPJC45 (2006) 417   C
C Imported from the KLOE library with minimal changes    C
C (thanks to Tommaso Spadaro)                            C
C E.Goudzovski 28/06/2011                                C
C--------------------------------------------------------C
C                                                        C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     Author: C. Gatti     11/12/03
C     MC generator for K+ --> l+ nu gamma
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      IMPLICIT NONE

c #include "masses.f"

      real*8 MPAR, MLEP, PCM(4,3), Amp
      integer status, KL2_IB_KLOE
      real*8 betae,b
      real*8 Eg,Emax,Elgmax,El
      real*8 Cteta,Steta,delta,boundary,y
      real*8 rando(2),pb(1),angoli(3)
      real*8 Ampiezza,PSF,factor
      real*8 ctg,stg,cpg,spg,cpl,spl
      real*8 RCM(3,2)

C     Parameters
      real*8 pi, alfa, Fk, LH, RH
      Parameter (pi = 3.1415927d+00)
      Parameter (alfa=1.d+00/137.03599976d+00)
C MeV
      Parameter (Fk=0.1136d+00) ! GeV
      Parameter (RH=0.d+00) ! -0.097d+00/mkch) ! (V+A)=0 for IB
      Parameter (LH=0.d+00) ! -0.037d+00/mkch) ! (V-A)=0 for IB

C     bond factor
      betae= dsqrt(1.d+00-(2.d+00*mpar*mlep/(mlep**2+mpar**2))**2)
      b = - 2.d+00*alfa/pi*( 1.d+00-dlog((1.d+00+betae)/(1.d+00-betae))
     $     /(2.d+00*betae))

C     max photon energy
      Emax = 0.5d+00*(mpar-mlep**2/mpar)

      status = 0
      Do while (status.eq.0)

C     Energy Distribution + Angular Flat
         CALL RANLUX(rando,2)

C     Angle
         Cteta = dble(rando(2))*2.d+00-1.d+00
         if ((1.d+00 - Cteta**2).lt.0.d+00) then
           Steta = 0.d+00
         else
           Steta = dsqrt(1.d+00 - Cteta**2)
         end if

C     lepton+ energy which gives the maxilepm photon energy given Cteta
         Elgmax = -((mlep*(Steta*
     $        (mpar**2 - mlep**2)**2 -
     $        2.d+00*Cteta**2*mpar*mlep*(mpar**2 + mlep**2)))/
     $        ((-1.d+00 + Cteta**2)*mpar**4 +
     $        2.d+00*(1.d+00 + Cteta**2)*mpar**2*mlep**2 +
     $        (-1.d+00 + Cteta**2)*mlep**4))

C     boundary is the photon energy as a function of El+ and Cteta
C     when El+ is Elgmax
         boundary = (-2.d+00*Elgmax*mpar + mpar**2 + mlep**2)/
     $        (-2.d+00*Elgmax + 2.d+00*mpar +
     $        2.d+00*Cteta*dsqrt(Elgmax**2 - mlep**2))

C     photon Energy
         Eg = Emax*dble(rando(1))**(1.d+00/b)
c     Eg = Emax*dble(0.995+rando(1)*0.005)**(1.d+00/b)
c         if(Eg.lt.0.001) Goto 654

C If outside the dalitz plot
         If (Eg.gt.boundary) Goto 654
         delta = (mpar**2 - mlep**2)**2 +
     $        4.d+00*Eg**2*(mpar**2 + (-1.d+00 + Cteta**2)*mlep**2)-
     $        4.d+00*Eg*(mpar**3 - mpar*mlep**2)

C out of Dplot
         If (delta.lt.0.d+00) goto 654
         El = (-2.d+00*Eg**2*mpar - mpar*(mpar**2 + mlep**2) +
     $        Eg*(3.d+00*mpar**2 + mlep**2 +
     $        Cteta*dsqrt(delta)))/
     $        (2.d+00*(-1.d+00 + Cteta**2)*Eg**2
     $        + 4.d+00*Eg*mpar - 2*mpar**2)

C     Ampiezza^2 kl2gamma
C
C     RH = V+A
C     LH = V-A
C
C     K(p) -> l(l) u(u) gamma(q)   |q| = Eg
C
C     l*q = y Eg mk = Eg (El - l Cteta)
C     l*p = (mk^2+mlep^2-2 Eg mk+ 2 y Eg mk)/2
C
C     epsilon^(0123)=-1
C     16*Gf**2*Pi*Vus**2*alfa*
         y = (El - dsqrt(El**2-mlep**2)* Cteta)/mpar

c$$$         write(*,*) 'inside kl2gene',El,Eg,Cteta,y,rando(1),rando(2)

         Ampiezza =
     $        -(-2.d+00*Eg**2*Fk*mpar**2*mlep**2*(-1.d+00 + y)*y*
     $        ((LH - RH)*(mlep**2 - mpar**2*y) +
     $        2.d+00*Eg*mpar*(LH*(-1.d+00 + y) - RH*y))
     $        + 2.d+00*Eg**4*mpar**4*y**2*
     $        (LH**2*(mlep**2 - mpar**2*(-1.d+00 + y))*(-1.d+00 + y) -
     $        RH**2*y*(-mlep**2 + mpar**2*y) +
     $        2.d+00*Eg*mpar*(LH**2*(-1.d+00 + y)**2 + RH**2*y**2)) +
     $        Fk**2*mlep**2*(-1.d+00 + y)*
     $        (mlep**4 - 2.d+00*Eg*mpar**3*y + mpar**4*y +
     $        2.d+00*Eg*mpar*mlep**2*y -
     $        mpar**2*(-2.d+00*Eg**2*y + mlep**2*(1.d+00 + y))))
     $        /(mpar**2*y**2)

C     phase space factor  1/(b (2pi)^3 8 mkch)
         PSF = (El**2-mlep**2)/(mpar*dsqrt(El**2-mlep**2)-
     $        Eg*(dsqrt(El**2-mlep**2)-El*Cteta))
         factor= (Fk**2*(mpar**2 - mlep**2)**4)/(8.d+00*mpar**4)

c         write (*,*) 'kl2gene ',PSF,factor,1./y,ampiezza

         Amp = PSF * Ampiezza / factor
         If (Amp.gt.1.01) then
C            write (0,*) 'KCL2G:ERROR prob>1',Amp
         Endif
C hit or miss
         CALL RANLUX(pb,1)
         If (pb(1)*1.01.le.Amp) then
            Status = 1
         Endif

 654     continue
      enddo ! end "do while"

C-- Prevent underflow problems with optimizer
      if (Eg.lt.1.d-12) Eg = 0.d+00
      CALL RANLUX(angoli,3)
      ctg=dble(angoli(1))*2.d+00-1.d+00
      if ((1.d+00-ctg**2).lt.0.d+00) then
        stg = 0.d+00
      else
        stg=dsqrt(1.d+00-ctg**2)
      end if
      cpg=dcos(dble(angoli(2))*2.d+00*pi)
      spg=dsin(dble(angoli(2))*2.d+00*pi)
      cpl=dcos(dble(angoli(3))*2.d+00*pi)
      spl=dsin(dble(angoli(3))*2.d+00*pi)

C     photon
      RCM(1,1) = 0.d+00
      RCM(2,1) = 0.d+00
      RCM(3,1) = Eg

C     lepton
      RCM(1,2) = dsqrt(El**2-mlep**2)*Steta*cpl
      RCM(2,2) = dsqrt(El**2-mlep**2)*Steta*spl
      RCM(3,2) = dsqrt(El**2-mlep**2)*Cteta

C rotation

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
      PCM(4,3) = mpar - Eg - El

      KL2G_IB_KLOE = 0
      Return
      End
