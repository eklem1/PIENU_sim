# First get the general variable definitions.

RECOPKG = /home/tristan/dicksmc/reco

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)


DEPENDENCIES  = $(shell ls *.cpp)
DEPENDENCIES2 = $(shell ls *.h)

# includes and libs
INCLUDE = -I. -I/home/mischke/mc/log4cpp/include -I$(RECOPKG)/

# CXXFLAGS=-O2 -fPIC -Wall -I$(ROOTSYS)/include -I$(RECOPKG)/



CXX = g++

# Flags for the compiler
CXXFLAGS = -g -O2
#CXXFLAGS = -g
# CXXFLAGS = -g -O2 -m64 
# CXXFLAGS = -g -O2 -m32 
#CXXFLAGS+=-O2 -fPIC -Wall -I$(ROOTSYS)/include 

CXXFLAGS += $(ROOTCFLAGS) -DEXE

# Flags for the linker
LDFLAGS = -g -O2
#LDFLAGS = -g
# LDFLAGS = -g -O2 -m64
# LDFLAGS = -g -O2 -m32

LINKER = g++

AR = ar  -rc
RANLIB = ranlib





LIBS		+= $(ROOTLIBS) -L/home/mischke/mc/log4cpp/lib -llog4cpp  -lm  -L$(RECOPKG) -lreco

OBJ = ConfigFile.o  Resolution.o  CsI.o  MCtoData.o MCtree.o reco_isrv.o TrReco.o  MCtruthClass.o MCtoDataDict.o
EXE = MCtoData




all : link




%.o : %.C $(DEPENDENCIES) $(DEPENDENCIES2)
	@echo "  CC  $<"
	$(CXX) -o $@ $(CXXFLAGS) -c ${INCLUDE} $<

%.o : %.cc 
	@echo "  CC  $<"
	$(CXX) -o $@ $(CXXFLAGS) -c ${INCLUDE} $<

%.o : %.cxx %.h
	@echo "  CC  $<"
	$(CXX) -o $@ $(CXXFLAGS) -c ${INCLUDE} $<

MCtoDataDict.cc: CsI.h DetectorsLinkDef.h
	        	  rootcint -f $@ -c -I$(ROOTSYS)/include -I. -I$(RECOPKG) TTree.h CsI.h reco_classes.h DetectorsLinkDef.h



link: $(OBJ)
	@echo "  LD  ${EXE}"
	${LINKER} -o $(EXE) ${LDFLAGS} $(OBJ) $(OBJ2) ${LIBS}


clean:
	@echo "  RM  ${EXE} ${OBJ} core.*"
	@rm -rf ${EXE} ${OBJ} core.* *Dict*

# install:
# 	@mkdir -p ${BINPREFIX}
# 	@chmod 755 ${EXE}
# 	@cp -f ${EXE} ${BINPREFIX}
# 	@echo installed ${EXE} to ${BINPREFIX}


