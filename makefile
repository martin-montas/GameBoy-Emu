# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
# ****************************************************
# Targets needed to bring the executable up to date

#// On linux compile with:
#// g++ -std=c++17 main.cpp -o prog -lSDL2 
#
gameboy: main.o CPU.o registers.o instructions.o
		g++  main.o  -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
		g++ -c main.cpp

CPU.o: ./src/cpu/CPU.cpp ./src/cpu/CPU.hpp
		g++ -c  ./src/cpu/CPU.cpp 

registers.o: ./src/registers.cpp ./src/registers.hpp
		g++ -c  src/registers.cpp 


