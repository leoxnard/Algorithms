CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -Wall -pedantic -O2 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address -fopenmp
//CXXFLAGS = -std=c++17 -Wall -pedantic -O2 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address
# -D_GLIBCXX_DEBUG // bad for openmp performance


%.o: %.cpp QGramIndex.h
	${CXX} ${CXXFLAGS} -I . -c $*.cpp
	
	
main: QGramIndex.o main.o util.o
	${CXX} ${CXXFLAGS} -I . $^ -o main