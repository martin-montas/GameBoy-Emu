# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
# ****************************************************
# Targets needed to bring the executable up to date

#// On linux compile with:
#// g++ -std=c++17 main.cpp -o prog -lSDL2 
SRC_DIR := ./src
CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.hpp')
gameboy: main.o cpu.o game-boy.o instructions.o
		g++  main.o  -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
		g++ -c main.cpp

game-boy.o:./src/game-boy.cpp ./src/game-boy.hpp
		g++ -I ./src/ -c  ./src/game-boy.cpp

instructions.o:./src/instructions.cpp ./src/instructions.hpp
		g++ -I ./src/ -c  ./src/instructions.cpp

cpu.o: ./src/cpu/cpu.cpp ./src/cpu/cpu.hpp
		g++ -I ./src/cpu/ -c  ./src/cpu/cpu.cpp

clean:
		rm *.o gameboy

lint:
		@cpplint $(CPP_FILES)
