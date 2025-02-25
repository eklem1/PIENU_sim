c ... Particle masses: they must correspond exactly to G4ParticleDefinition.
c ... Unit used by the decay generators: GeV.

      REAL*8 MKCH,SQMKCH,MPI,SQMPI,MP0,SQMP0,MEL,SQMEL,MMU,SQMMU
      REAL*8 RPI,RPI2
      PARAMETER (MKCH=0.493677,     SQMKCH=MKCH**2)
      PARAMETER (MPI=0.13957010,    SQMPI=MPI**2)
      PARAMETER (MP0=0.1349766,     SQMP0=MP0**2)
      PARAMETER (MEL=0.00051099891, SQMEL=MEL**2)
      PARAMETER (MMU=0.1056583715,  SQMMU=MMU**2)
      PARAMETER (RPI=MPI/MKCH, RPI2=RPI**2)

c ... PDG particle codes
      INTEGER IDPIP, IDPIM, IDPIZ
      INTEGER IDELEP, IDELEM, IDMUP, IDMUM, IDGAM, IDNUE, IDNUM
      INTEGER IDKP, IDKM, IDEXO
      PARAMETER (IDPIP=211, IDPIM=-211, IDPIZ=111) ! pi+, pi-, pi0
      PARAMETER (IDELEP=-11, IDELEM=11)            ! e+, e-
      PARAMETER (IDMUP=-13, IDMUM=13)              ! mu+, mu-
      PARAMETER (IDGAM=22, IDNUE=12, IDNUM=14)     ! gamma, nu_e, nu_mu
      PARAMETER (IDKP=321, IDKM=-321)              ! K+, K-
      PARAMETER (IDEXO=9000)                       ! Exotic
