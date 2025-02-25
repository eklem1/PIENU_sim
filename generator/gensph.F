      SUBROUTINE GENSPH(VEC)
C--------------------------------------------------------C
C GENERATES AN UNIT-LENGTH VECTOR UNIFORMLY IN 4PI.      C
C USES 1 SQRT AND NO SIN,COS'S. HITOSHI OCT-86           C
C--------------------------------------------------------C
      implicit none
      REAL*8 VEC(3), U1, U2, S, CO2
C
      REAL*8 RRR(2)
    1 CALL RANLUX(RRR,2)
      U1 = 2.0*RRR(1)-1.0
      U2 = 2.0*RRR(2)-1.0
      S  = U1*U1+U2*U2
      IF (S.GE.1.) GOTO 1
C
      CO2=2.0*SQRT(1.0-S)
      VEC(1)=U1*CO2
      VEC(2)=U2*CO2
      VEC(3)=1.0-2.0*S
C
      RETURN
      END
