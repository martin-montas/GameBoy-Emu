# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
# ****************************************************
# Targets needed to bring the executable up to date

#// On linux compile with:
#// g++ -std=c++17 main.cpp -o prog -lSDL2 
#
game-boy: main.o game-boy.o

		g++  main.o game-boy.o -lsfml-graphics -lsfml-window -lsfml-system
main.o: main.cpp
		g++ -c main.cpp

game-boy.o: game-boy.cpp game-boy.hpp
		g++ -c  game-boy.cpp 


