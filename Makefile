all: bin/main


bin/main: bin src/*.cpp include/*.hpp
	g++ -g -o bin/main -I include src/*.cpp

bin:
	mkdir -p bin
