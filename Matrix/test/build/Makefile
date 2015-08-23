################################################################################
################################################################################
#
# Makefile
#
# Makefile for the test executable
#
################################################################################
################################################################################

TOPDIR := ../..

BINDIR := ../bin

EXENAME := testMat

TGTEXE := $(BINDIR)/$(EXENAME)

RUNDIR := ../run

SRCDIR1 := ../src

SRC1 := \
main.cpp \
TestMat.cpp

OBJ1 := $(subst .cpp,.o,$(SRC1))

OBJ := $(OBJ1)

LIBS := $(TOPDIR)/lib/libMatrix.a

INCL := \
-I. \
-I$(TOPDIR)/src

FLAGS := -c -g 

all : tgtExe symLink

tgtExe : 	$(OBJ)
		g++ -g -o $(TGTEXE) $(OBJ) $(LIBS) -lm

$(OBJ) : 	%.o: $(SRCDIR1)/%.cpp
		g++ $(FLAGS) $(INCL) $< -o $@

#
# Create dependency files
#
%.d : 		$(SRCDIR1)/%.cpp
		g++ -MM $(INCL) $< -o $@

#
# Include dependency files
#
include $(OBJ:.o=.d)

symLink :
		ln -sf $(TGTEXE) $(RUNDIR)

.PHONY : clean

clean :
		rm -f *.o *.d $(TGTEXE) $(RUNDIR)/core $(RUNDIR)/$(EXENAME)

################################################################################
################################################################################
