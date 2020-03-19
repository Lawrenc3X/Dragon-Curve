CC = gcc
CXX = g++
CFLAGS = -g -lncurses
CXXFLAGS = -g -lncurses

Dragon_Curve: Dragon_Curve.cpp
	$(CXX) $(CXXFLAGS) -o Dragon_Curve Dragon_Curve.cpp

all: Dragon_Curve.cpp 
	$(CXX) $(CXXFLAGS) -o Dragon_Curve Dragon_Curve.cpp

clean:
	rm -r Dragon_Curve Dragon_curve.dSYM
