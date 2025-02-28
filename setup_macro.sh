module purge
module load StdEnv/2020
module load gcc/9.3.0
module load root/6.20.04

cd /home/eklem1/projects/def-chloe/eklem1/PIENU/pienu_2025/macro
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/mischke/mc/log4cpp/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib

ln -s /home/tristan/dicksmc/reco/reco_classesDict_rdict.pcm .

