CXX=g++
CXXFLAGS=-Wall -Wextra

all: ppmm probe
probe: filter random convert_to_ppm rle

ppmm: main.cpp ppmm.cpp
	$(CXX) $(CXXFLAGS) -o ppmm main.cpp ppmm.cpp

filter: probe/filter.cpp
	$(CXX) $(CXXFLAGS) -o filter probe/filter.cpp

random: probe/random.cpp
	$(CXX) $(CXXFLAGS) -o random probe/random.cpp

convert_to_ppm: probe/convert_to_ppm.cpp
	$(CXX) $(CXXFLAGS) -o convert_to_ppm probe/convert_to_ppm.cpp

rle: probe/rle.cpp
	$(CXX) $(CXXFLAGS) -o rle probe/rle.cpp ppmm.cpp

clean:
	rm -rf ppmm
	rm -rf filter
	rm -rf random
	rm -rf convert_to_ppm
	rm -rf rle
