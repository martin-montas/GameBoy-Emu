SRC_DIR := ./src

CXX := g++
CXXFLAGS := -I./src -std=c++20 
LDFLAGS := -lSDL2

COMMON_OBJS := main.o  ppu.o
               
               

gameboy: $(COMMON_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# =====================================================
# COMPILATION RULES
# =====================================================
main.o: main.cpp
	$(CXX) -c $< $(CXXFLAGS)


ppu.o: ppu.cpp ppu.hpp
	$(CXX) -c $< $(CXXFLAGS)


# =====================================================
# UTILITIES
# =====================================================

clean:
	rm -f *.o gameboy

format:
	find . \( -name "*.cpp" -o -name "*.hpp" \) | \
	xargs clang-format -style=llvm -i

lint:
	cpplint $(shell find . \( -name "*.cpp" -o -name "*.hpp" \))

count:
	find . \( -name "*.cpp" -o -name "*.hpp"  \) | xargs wc -l

t:
	./sm83.sh

.PHONY: clean format lint
