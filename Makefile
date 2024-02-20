CXX=g++
CXXFLAGS=-Wall -Wextra

all: ppmm filter random convert_to_ppm
probe: filter random convert_to_ppm

ppmm: ppmm.cpp
	$(CXX) $(CXXFLAGS) -o ppmm ppmm.cpp

filter: probe/filter.cpp
	$(CXX) $(CXXFLAGS) -o filter probe/filter.cpp

random: probe/random.cpp
	$(CXX) $(CXXFLAGS) -o random probe/random.cpp

convert_to_ppm: probe/convert_to_ppm.cpp
	$(CXX) $(CXXFLAGS) -o convert_to_ppm probe/convert_to_ppm.cpp

clean:
	rm -rf ppmm
	rm -rf filter
	rm -rf random
	rm -rf convert_to_ppm
