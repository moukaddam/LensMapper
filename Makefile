

# Compilateur et options
# LDFLAGS       = -O
# CXXFLAGS     += $(ROOTCFLAGS)


# Extensions des fichiers
ObjSuf        = o
SrcSuf        = cpp
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o

# Optimisation
OPTFLAGS = -O3 -funroll-loops\
         -fgcse-lm  -fgcse-sm

# Compilateur et options
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC #-DDEBUG #-ggdb
LD            = g++
LDFLAGS       = -O $(OPTFLAGS)
SOFLAGS       = -shared 


# Options de ROOT
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs) -lMathMore -lSpectrum
ROOTGLIBS    := $(shell root-config --glibs)

CXXFLAGS     += $(ROOTCFLAGS) $(OPTFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)


# ---------------------------------- Writing and reading classes
#SHARELIB      = libExperimentalPoint.so libSimulationPoint.so 
#
#all:            $(SHARELIB)
#
#libExperimentalPoint.so:	ExperimentalPoint.o	 ExperimentalPointDict.o
#		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
#
#ExperimentalPointDict.cpp:	ExperimentalPoint.h
#		rootcint -f $@ -c $^
#
#
#libSimulationPoint.so:	SimulationPoint.o	SimulationPointDict.o
#		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
#
#SimulationPointDict.cpp:	SimulationPoint.h
#		rootcint -f $@ -c $^		
# ----------------------------------		
		
		
# Environnement
# CHANGE PATH TO THE DIRECTORY OF THE PROJECT
DIR		= ./
BIN		= ./bin

# CHANGE ADD/REMOVE CLASSES OR TOOLS
SRCS	=     ExperimentalPoint.cpp SimulationPoint.cpp \
				GraphManager.cpp HistManager.cpp Mapper.cpp

OBJECTS	=     ExperimentalPoint.o SimulationPoint.o \
				GraphManager.o   HistManager.o  Mapper.o


# CHANGE NAME OF THE EXCUTION 
PROGRAMS = mapperTool


# Par defaut on compile tout
all:		$(OBJECTS)\
		$(BIN)/$(PROGRAMS)


# Programme principal
$(BIN)/$(PROGRAMS):	$(OBJECTS)
			$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
			@echo "$@ done"  

clean : 
	rm  $(OBJECTS)

distclean:
	cd $(DIR); make clean


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

# dependances
ExperimentalPoint.o:		ExperimentalPoint.h			ExperimentalPoint.cpp	Defines.h
SimulationPoint.o:			SimulationPoint.h			SimulationPoint.cpp		Defines.h
GraphManager.o:				GraphManager.h				GraphManager.cpp	    Defines.h
HistManager.o:				HistManager.h				HistManager.cpp	        Defines.h
#OptimizeParameters.o:		OptimizeParameters.h		OptimizeParameters.cpp	Defines.h
Mapper.o:			        													Defines.h
