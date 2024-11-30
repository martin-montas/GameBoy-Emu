#include <boost/test/included/unit_test.hpp>
#include "../src/instructions.hpp"
#include "../src/cpu/cpu.hpp"
#include "../src/memory/mmu.hpp"

BOOST_AUTO_TEST_SUITE(CPU_Test)

BOOST_AUTO_TEST_CASE(ld_instruction_test) {
    MMU mmu("../ROM/cpu_instrs.gb");

    InstructionSet *instruction = new InstructionSet(&mmu);
    uint8_t opcode = 0x3E;  
    uint8_t operand = 0x42;

    instruction->execute(opcode);
    BOOST_CHECK_EQUAL(cpu.A,operand);
}

BOOST_AUTO_TEST_SUITE_END()
