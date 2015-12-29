################################################################################
################################################################################
#
# Makefile
#
# Makefile for the sim executable.
# 
################################################################################
################################################################################

TOPDIR := ..

RUNDIR = $(TOPDIR)/run

EXENAME := p3dof

EXEPATH := $(RUNDIR)/$(EXENAME)

SRCDIR1 := $(TOPDIR)/src

SRC1 := \
main.cpp \
Autopilot.cpp \
FcRadar.cpp \
FcTseO.cpp \
FcTse6.cpp \
Guidance.cpp \
MathUtils.cpp \
MdlBase.cpp \
Missile.cpp \
MslTse6.cpp \
Seeker.cpp \
SimExec.cpp \
Target.cpp

OBJ1 := $(subst .cpp,.o,$(SRC1))

OBJ := $(OBJ1) 

INC1 := $(SRCDIR1)

INC2 := $(TOPDIR)/Matrix/src

INC = -I$(INC1) -I$(INC2)

FLAGS = -c -g 

LIBS := $(TOPDIR)/lib/libMatrix.a

all :	tgtExe

tgtExe : $(OBJ)
	g++ -g -o $(EXEPATH) $(OBJ) $(LIBS) -lm

$(OBJ1) :	%.o: $(SRCDIR1)/%.cpp
	g++ $(FLAGS) $(INC) $< -o $@

#
# Create dependency files
#

%.d : $(SRCDIR1)/%.cpp
	g++ -MM $(INC) $< -o $@		

#
# Include dependency files
#
include $(OBJ:.o=.d)

clean :
	rm -f *.o *.d  $(EXEPATH) $(RUNDIR)/core


################################################################################
################################################################################
