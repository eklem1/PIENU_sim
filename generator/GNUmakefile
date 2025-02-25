# B. Velghe (Aug. 2021)
.PHONY: clean debug all

FC=gfortran
CC=g++
CFLAGS=-fPIC
FCFLAGS=-O2 -fPIC -fno-automatic -fno-backslash -fno-second-underscore
LDFLAGS=-shared

SRCS = $(wildcard *.F) $(wildcard *.cc)
OBJS = $(filter %.o,$(patsubst %.F,%.o,$(SRCS))) $(filter %.o,$(patsubst %.cc,%.o,$(SRCS)))
INCLUDE= -I../include -I$(ROOTSYS)/include

TARGET = FortranGenerators.so

all: $(TARGET)

debug:
	@echo $(SRCS)
	@echo $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $? -o $@

%.o: %.F
	$(FC) $(FCFLAGS) -c $< -o $@

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm -f *.o 
