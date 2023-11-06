.POSIX:

CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -lm --std=gnu++2b -fopenmp
SRC=src/*.cpp
BUILD=build

build: src/main.cpp
	$(CXX) $(SRC) -o $(BUILD)/a.out $(CXXFLAGS)

run: build
	./$(BUILD)/a.out

format:
	clang-format -i $(SRC)

.PHONY: build run format
