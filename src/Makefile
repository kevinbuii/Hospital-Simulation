CXX=g++
CXXFLAGS=-std=c++11 -pthread

debug: CXXFLAGS += -g
debug: all

all: proj_4

proj_4: project_4.cpp
	$(CXX) $(CXXFLAGS) project_4.cpp -o proj_4

clean:
	rm -rf proj_4 *.o