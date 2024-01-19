CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -g -Wall -pedantic -O2 -D_GLIBCXX_DEBUG -fsanitize=address

%.o: %.cpp
	${CXX} ${CXXFLAGS} -I . -c $*.cpp

main: Alignment.o main.o
	${CXX} ${CXXFLAGS} -I . $^ -o main