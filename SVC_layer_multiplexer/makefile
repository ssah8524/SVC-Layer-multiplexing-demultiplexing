# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g
OBJECTS = Multiplex.o LargeFile.o Parameter.o ReadBitstreamFull.o \
         ReAttach.o WriteBitstreamToFile.o
	
# ****************************************************

all: Multiplex

Multiplex: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o Multiplex $(OBJECTS)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: 
	rm $(OBJECTS) Multiplex