#include "cpu.h"

uint16_t CPU::handle_block_0(uint16_t program_counter) {
	uint8_t byte1 = address_space.read(program_counter);
	uint8_t r16 = get_bit_range(byte1, 4, 5);
	

	if (byte1 & 8) {

	}

	if (byte1 & 10) {

	}

	//ld r16mem, a
	if (byte1 & 2) {
		registerCalls reg = val_to_r16mem[r16];
		if (reg == registerCalls::HLI) {
			ldhlda();
		}
		else if (reg == registerCalls::HLD) {
			ldhlia();
		}
		else {
			ldr16a(reg);
		}
		return program_counter + 1;
	}

	//ldr16, imm16
	if (byte1 & 1) {
		registerCalls reg = val_to_r16[r16];
		uint8_t byte1 = address_space.read(++program_counter);
		uint16_t byte2 = address_space.read(++program_counter);
		uint16_t imm16 = (byte2 << 8) | byte1;
		ldr16n16(reg, imm16);
		//calculate num_cycles or something
		return program_counter + 1;
	}
}