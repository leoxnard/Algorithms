CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -Wall -pedantic -O2 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address -fopenmp
# -D_GLIBCXX_DEBUG // bad for openmp performance

%.o: %.cpp util.h BLAST_Neighborhood.hpp
	${CXX} ${CXXFLAGS} -I . -c $*.cpp
	
	
main: BLAST_Neighborhood.o main.o util.o
	${CXX} ${CXXFLAGS} -I . $^ -o main