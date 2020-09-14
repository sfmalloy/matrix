#############################################################
# Sean Malloy                                               #
# Matrix Makefile                                           #
#############################################################

#############################################################
# Variables                                                 #
#############################################################

# C++ compiler
CXX      := g++

# C++ compiler flags
# CXXFLAGS := -g -Wall
CXXFLAGS := -O3 -Wall

#############################################################
# Rules                                                     #
#############################################################

main.out: main.cpp 
	$(CXX) $(CXXFLAGS) $^ -o $@

#############################################################

.PHONY: *.out

clean :
	@$(RM) *.out
	@$(RM) *.exe
	@$(RM) *.o
	@$(RM) *~ 

#############################################################
