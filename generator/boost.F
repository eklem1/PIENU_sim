      SUBROUTINE DBOOST(P0,U0,PA,PB)
C---------------------------------------------------------C
C 4-MOMENTUM PA IN THE REST FRAME OF PARTICLE 0 IS        C
C EVALUATED IN THE FRAME WHERE PARTILE 0 HAS A 4-MOMENTUM C
C P0 AND RETURNED AS PB. U0 IS THE MASS OF THE PARTICLE 0.C
C PA AND PB CAN SHARE THE SAME MEMORY. OCT-86 HITOSHI     C
C THIS IS THE DOUBLE PRECISION VERSION OF LBOOST.         C
C---------------------------------------------------------C
      DOUBLE PRECISION P0(4),PA(4),PB(4),U0
      DOUBLE PRECISION EP,CONS
      EP=(PA(4)*P0(4)+PA(1)*P0(1)+PA(2)*P0(2)+PA(3)*P0(3))/U0
      CONS=(PA(4)+EP)/(U0+P0(4))
      PB(1)=PA(1)+CONS*P0(1)
      PB(2)=PA(2)+CONS*P0(2)
      PB(3)=PA(3)+CONS*P0(3)
      PB(4)=EP
      RETURN
      END

c
c     Boost using different indices for 4-vectors
c

      SUBROUTINE BOOST2 (PC,PD,AMASS,PL)

c   Lorentz boost of PC from PD rest frame to LAB
c   in double precision (W. Wislicki 29/11/95)

      IMPLICIT  NONE
      REAL*8 AMASS
      REAL*8 DMASS, SCP, FACT
     +,    PC(0:3),PD(0:3),PL(0:3)
      DMASS = DBLE (AMASS)
      SCP   = PD(1)*PC(1) + PD(2)*PC(2) + PD(3)*PC(3)
      FACT  = (SCP/(PD(0) + DMASS) + PC(0))/DMASS
      PL(1) = PC(1) + FACT*PD(1)
      PL(2) = PC(2) + FACT*PD(2)
      PL(3) = PC(3) + FACT*PD(3)
      PL(0) = (PC(0)*PD(0) + SCP)/DMASS
      RETURN
      END
