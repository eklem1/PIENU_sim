module purge
module load StdEnv/2020
module load gcc/9.3.0
module load geant4/10.06
module load cmake
module load root

source $G4INSTALL/share/Geant4-10.6.0/geant4make/geant4make.sh

cd /home/eklem1/projects/def-chloe/eklem1/PIENU/pienu_2025/pienu_build
export G4WORKDIR=$PWD
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/mischke/mc/log4cpp/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib

