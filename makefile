const: all
	./main US_constitution.txt

northkorea: all
	./main dprk.txt

all: main.cpp
	g++ -std=c++14 -o main main.cpp

debug: main.cpp
	g++ -g -std=c++14 -o main main.cpp

clean: 
	rm -r main 
