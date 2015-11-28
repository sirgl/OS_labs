main: main.o list.o
	g++ main.o list.o -threads -o main -lrt -std=c++11
main.o: main.cpp
	g++ -c main.cpp
list.o: list.cpp
	g++ -c list.cpp
