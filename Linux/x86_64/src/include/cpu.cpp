// 3rd Party Libraries

// Platform Libraries

// Project Libraries
#include "cpu.hpp"
#include "logger.hpp"
#include "rom.hpp"

// Standard Libraries

void CPU::reset() {
    registers.fill(0);
    instruction_register = 0;
    program_counter = PROGRAM_START_ADDR;
    stack_pointer = program_counter;
}

void CPU::fetch() {
    uint8_t high = {};
    uint8_t low  = {};

    if (program_counter + 1 >= MAX_RAM_SIZE) {
        ERRO(stdout, "Program counter out of bounds: %u", program_counter);
        instruction_register = PROGRAM_START_ADDR;
        return;
    }

    high = machine->ram[program_counter];
    low  = machine->ram[program_counter + 1];
    instruction_register = (high << 8) | low;
    program_counter += 2;

    /* TODO: Perform the action as per the "low" byte */
}

void CPU::decode() {
    fetch();
}

void CPU::execute() {
    decode();
}
