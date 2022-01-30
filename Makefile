CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra -Wconversion -Wsign-conversion -Wpedantic -Wfloat-equal

all: .prepare build/scala-parser

build/scala-parser: build/main.o build/tuning.o
	$(CXX) build/main.o build/tuning.o -o build/scala-parser

build/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

build/tuning.o: src/tuning.cpp src/tuning.hpp
	$(CXX) $(CXXFLAGS) -c src/tuning.cpp -o build/tuning.o

.PHONY = all .prepare clean

.prepare:
	@mkdir -p build/

clean:
	@rm -rf build/
