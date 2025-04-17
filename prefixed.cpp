#include "CPU.h"

uint16_t CPU::prefixedCodes(uint8_t program_counter) {
	uint8_t code = address_space.read(program_counter);
	uint8_t cycles = 0;

	block_cycle_n(cycles);
}