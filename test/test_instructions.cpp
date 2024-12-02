#define BOOST_TEST_MODULE isEq Test
#include <boost/test/included/unit_test.hpp>

#include "../src/instructions.hpp"
#include "../src/mmu.hpp"
#include "../src/mmu.hpp"


BOOST_AUTO_TEST_CASE( testA ) {
    MMU *mmu = new MMU("../ROM/dummy_file.gb");
    InstructionSet *instructions = new InstructionSet(mmu);
    instructions->execute(0x3E);
    Cpu *cpu = instructions->getCpu();

    BOOST_CHECK(cpu->A == 0x3E);
}

