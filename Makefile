CXX=g++
CXXFLAGS=-Wall -Wextra

all: ppmm filter random convert_to_ppm

ppmm: ppmm.cpp
	$(CXX) $(CXXFLAGS) -o ppmm ppmm.cpp

filter: filter.cpp
	$(CXX) $(CXXFLAGS) -o filter filter.cpp

random: random.cpp
	$(CXX) $(CXXFLAGS) -o random random.cpp

convert_to_ppm: convert_to_ppm.cpp
	$(CXX) $(CXXFLAGS) -o convert_to_ppm convert_to_ppm.cpp

clean:
	rm -rf ppmm
	rm -rf filter
	rm -rf random
	rm -rf convert_to_ppm
