# Makefile
CXX = g++
#CXX = clang++
CXXFLAGS += -g -fPIC `root-config --cflags` -I.
#CXXFLAGS += -O3
CXXFLAGS += -I$(RATROOT)/include
LDFLAGS += -lstdc++ `root-config --glibs`
LDFLAGS += -L$(RATROOT)/lib -lRATEvent -lrat 
EXE = extractMCEvent extractMCPMT
all: $(EXE)
#extractMCEvent.o: extractMCEvent.cc
#	$(CXX) $(CXXFLAGS) -c -o $@ $^
#extractMCEvent: extractMCEvent.o
#	$(CXX) $(LDFLAGS) -o $@ $^
extractMCEvent: extractMCEvent.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
extractMCPMT: extractMCPMT.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
clean:
	rm -f *.o $(EXE)
