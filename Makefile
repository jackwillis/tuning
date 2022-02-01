CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra -Wconversion -Wsign-conversion -Wpedantic -Wfloat-equal

all: build/scala-parser

build/scala-parser: build/main.o build/tuning.o build/math.o
	$(CXX) build/main.o build/tuning.o -o build/scala-parser

build/main.o: .prepare src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

build/tuning.o: .prepare src/tuning.cpp src/tuning.hpp
	$(CXX) $(CXXFLAGS) -c src/tuning.cpp -o build/tuning.o

build/math.o: .prepare src/math.cpp src/math.hpp
	$(CXX) $(CXXFLAGS) -c src/math.cpp -o build/math.o

.PHONY = all .prepare clean

.prepare:
	@mkdir -p build/

clean:
	@rm -rf build/
