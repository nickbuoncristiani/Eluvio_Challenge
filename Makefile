CXX := -g++
CXXFLAGS := -std=c++17

all: strand

strand.o: strand.cc
	$(CXX) $(CXXFLAGS) strand.cc -O2 -c 
strand: strand.o
	$(CXX) $(CXXFLAGS) strand.o -o strand

clean:
	rm strand strand.o