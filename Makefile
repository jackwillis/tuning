CXX = g++
CXXFLAGS = -std=c++20 -g -Wall -Wextra -Wconversion -Wsign-conversion -Wpedantic -Wfloat-equal

all: .prepare build/scala-parser

build/scala-parser: src/main.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) $< -o $@

.PHONY = all .prepare clean

.prepare:
	@mkdir -p build/

clean:
	@rm -rf build/
