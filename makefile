# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
# *****************************************************
SRC_DIR := ./src
CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.hpp')

# Compiler and flags
CXX := g++
CXXFLAGS := -I./src -std=c++20
#LDFLAGS := #-lsfml-graphics -lsfml-window -lsfml-system

# Target for the main program
gameboy: main.o cpu.o game-boy.o instructions.o extended-instructions.o mmu.o MBC.o MBC0.o
	$(CXX) main.o cpu.o game-boy.o instructions.o extended-instructions.o mmu.o MBC.o MBC0.o -o gameboy #$(LDFLAGS)

# Object file rules
main.o: main.cpp
	$(CXX)  -c main.cpp #$(CXXFLAGS)

game-boy.o: ./src/game-boy.cpp ./src/game-boy.hpp
	$(CXX)  -c ./src/game-boy.cpp #$(CXXFLAGS)

instructions.o: ./src/instructions.cpp ./src/instructions.hpp
	$(CXX)  -c ./src/instructions.cpp #$(CXXFLAGS)

cpu.o: ./src/cpu.cpp ./src/cpu.hpp
	$(CXX)  -c ./src/cpu.cpp #$(CXXFLAGS)
extended-instructions.o: ./src/extended-instructions.cpp ./src/instructions.hpp
	$(CXX)  -c ./src/extended-instructions.cpp #$(CXXFLAGS)

mmu.o: ./src/mmu.cpp ./src/mmu.hpp
	$(CXX)  -c ./src/mmu.cpp #$(CXXFLAGS)

MBC.o: ./src/MBC.cpp ./src/MBC.hpp
	$(CXX)  -c ./src/MBC.cpp #$(CXXFLAGS)

MBC0.o: ./src/MBC0.cpp ./src/MBC0.hpp
	$(CXX)  -c ./src/MBC0.cpp #$(CXXFLAGS)
clean:
	rm -f *.o gameboy 
format: 
	find . -name "*.cpp" -o -name "*.hpp" | xargs clang-format -style=llvm -i
lint:
	cpplint $(shell find . -name "*.cpp" -o -name "*.hpp")

