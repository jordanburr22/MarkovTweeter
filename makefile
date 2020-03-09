all: main.cpp
	g++ -std=c++14 -o main main.cpp

test: all
	./main

debug: main.cpp
	g++ -g -std=c++14 -o main main.cpp

clean: 
	rm -r main 
