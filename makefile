SRC_DIR := ./src

CXX := g++
CXXFLAGS := -I./src -std=c++20
LDFLAGS :=

gameboy: main.o cpu.o gameboy.o instructions.o extended-instructions.o mmu.o MBC.o MBC0.o timer.o serial.o
	$(CXX) main.o cpu.o gameboy.o instructions.o extended-instructions.o mmu.o MBC.o MBC0.o timer.o serial.o -o gameboy $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c main.cpp $(CXXFLAGS)

gameboy.o: ./src/gameboy.cpp ./src/gameboy.hpp
	$(CXX) -c ./src/gameboy.cpp $(CXXFLAGS)

instructions.o: ./src/instructions.cpp ./src/instructions.hpp
	$(CXX) -c ./src/instructions.cpp $(CXXFLAGS)

cpu.o: ./src/cpu.cpp ./src/cpu.hpp
	$(CXX) -c ./src/cpu.cpp $(CXXFLAGS)

extended-instructions.o: ./src/extended-instructions.cpp ./src/instructions.hpp
	$(CXX) -c ./src/extended-instructions.cpp $(CXXFLAGS)

mmu.o: ./src/mmu.cpp ./src/mmu.hpp
	$(CXX) -c ./src/mmu.cpp $(CXXFLAGS)

MBC.o: ./src/MBC.cpp ./src/MBC.hpp
	$(CXX) -c ./src/MBC.cpp $(CXXFLAGS)

MBC0.o: ./src/MBC0.cpp ./src/MBC0.hpp
	$(CXX) -c ./src/MBC0.cpp $(CXXFLAGS)

timer.o: ./src/timer.cpp ./src/timer.hpp
	$(CXX) -c ./src/timer.cpp $(CXXFLAGS)

serial.o: ./src/serial.cpp ./src/serial.hpp
	$(CXX) -c ./src/serial.cpp $(CXXFLAGS)

clean:
	rm -f *.o gameboy

format:
	find . \( -name "*.cpp" -o -name "*.hpp" \) | xargs clang-format -style=llvm -i

lint:
	cpplint $(shell find . \( -name "*.cpp" -o -name "*.hpp" \))

.PHONY: clean format lint
