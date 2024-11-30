# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
# *****************************************************
SRC_DIR := ./src
CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.hpp')

# Compiler and flags
CXX := g++
CXXFLAGS := -I./src -std=c++17
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Target for the main program
gameboy: main.o cpu.o game-boy.o instructions.o extended-instructions.o mmu.o
	$(CXX) main.o cpu.o game-boy.o instructions.o extended-instructions.o mmu.o -o gameboy $(LDFLAGS)

# Object file rules
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

game-boy.o: ./src/game-boy.cpp ./src/game-boy.hpp
	$(CXX) $(CXXFLAGS) -c ./src/game-boy.cpp

instructions.o: ./src/instructions.cpp ./src/instructions.hpp
	$(CXX) $(CXXFLAGS) -c ./src/instructions.cpp

cpu.o: ./src/cpu.cpp ./src/cpu.hpp
	$(CXX) $(CXXFLAGS) -c ./src/cpu.cpp
	
extended-instructions.o: ./src/extended-instructions.cpp ./src/instructions.hpp
	$(CXX) $(CXXFLAGS) -c ./src/extended-instructions.cpp

mmu.o: ./src/mmu.cpp ./src/mmu.hpp
	$(CXX) $(CXXFLAGS) -c ./src/mmu.cpp

# Clean rule
clean:
	rm -f *.o gameboy test_binary

# Lint rule
lint:
	@cpplint $(CPP_FILES)

