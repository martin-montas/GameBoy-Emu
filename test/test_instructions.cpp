#include <boost/test/included/unit_test.hpp>
#include "../src/instructions.hpp"
#include "../src/cpu/cpu.hpp"
#include "../src/memory/mmu.hpp"
#include <memory> // For smart pointers
#include <filesystem> // For file existence check (C++17)

BOOST_AUTO_TEST_SUITE(CPU_Test)

BOOST_AUTO_TEST_CASE(ld_instruction_test) {
    // Validate ROM path
    BOOST_REQUIRE(std::filesystem::exists("../ROM/cpu_instrs.gb"));

    // Initialize MMU and InstructionSet
    MMU mmu("../ROM/cpu_instrs.gb");
    std::unique_ptr<InstructionSet> instruction = std::make_unique<InstructionSet>(&mmu);

    // Define opcode and operand
    uint8_t opcode = 0x3E;  
    uint8_t operand = 0x42;

    // Simulate fetching operand
    mmu.write8(0x0100, operand); // Stub memory to fetch the operand
    instruction->execute(opcode);

    // Access CPU and verify
    Cpu* cpu = instruction->getCpu();
    BOOST_CHECK_EQUAL(cpu->A, operand);
}

BOOST_AUTO_TEST_SUITE_END()
