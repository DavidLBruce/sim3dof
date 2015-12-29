################################################################################
################################################################################
#
# Makefile
#
# Makefile for the Matrix library.
#
################################################################################

TOPDIR := ../..

LIBDIR := $(TOPDIR)/lib

TGTLIB := $(LIBDIR)/libMatrix.a

SRCDIR := $(TOPDIR)/Matrix/src

INCL := -I$(SRCDIR)

SRC  := Matrix.cpp

OBJ := $(subst .cpp,.o,$(SRC))

FLAGS := -c -g 

all  : tgtLib

$(OBJ) :	%.o: $(SRCDIR)/%.cpp
		g++ $(FLAGS) $(INCL) $< -o $@

#
# Create dependency files
#
%.d : 		$(SRCDIR)/%.cpp
		g++ -MM $(INCL) $< -o $@

#
# Include dependency files
#
include $(OBJ:.o=.d)

tgtLib :	$(OBJ)
		ar -r -o $(TGTLIB) $(OBJ) 

.PHONY : clean

clean :
		rm -f *.o *.d  $(TGTLIB)

################################################################################
################################################################################
