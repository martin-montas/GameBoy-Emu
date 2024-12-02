#define BOOST_TEST_MODULE isEq Test
#include <boost/test/included/unit_test.hpp>

#include "../src/instructions.hpp"
#include "../src/mmu.hpp"
#include "../src/mmu.hpp"


BOOST_AUTO_TEST_CASE( INC ) {
    MMU *mmu = new MMU("../ROM/DuckTales (USA).gb");
    InstructionSet *instructions = new InstructionSet(mmu);
    Cpu *cpu = instructions->getCpu();
    cpu->BC = 0x01;
    instructions->execute(0x03);
    BOOST_CHECK(cpu->BC == 0x02);
}

BOOST_AUTO_TEST_CASE( DEC ) {
    MMU *mmu = new MMU("../ROM/DuckTales (USA).gb");
    InstructionSet *instructions = new InstructionSet(mmu);
    Cpu *cpu = instructions->getCpu();
    cpu->BC = 0x02;
    instructions->execute(0x0B);
    BOOST_CHECK(cpu->BC == 0x01);
}
BOOST_AUTO_TEST_CASE( LD ) {
    // Find a way to test the load functions of the game boy:

    MMU *mmu = new MMU("../ROM/DuckTales (USA).gb");
    InstructionSet *instructions = new InstructionSet(mmu);
    Cpu *cpu = instructions->getCpu();
    cpu->BC = 0x01;
    instructions->execute(0x06);
}
