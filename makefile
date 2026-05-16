s

mmu.o: ./src/mmu.cpp ./src/mmu.hpp
	$(CXX)  -c ./src/mmu.cpp #$(CXXFLAGS)

clean:
	rm -f *.o gameboy test_binary
f: 
	find . -name "*.cpp" -o -name "*.hpp" | xargs clang-format -style=llvm -i
