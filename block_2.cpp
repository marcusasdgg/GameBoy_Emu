#include "cpu.h"

uint16_t CPU::handle_block_2(uint16_t program_counter) {
	uint8_t byte1 = address_space.read(program_counter);
	uint8_t reg8 = get_bit_range(byte1, 0, 2);
	registerCalls a = val_to_r8[reg8];
	byte1 = byte1 >> 3;

	if (byte1 & 7) {
		if (a == HL) {
			cpahl();
		}
		else {
			cpar8(a);
		}
	}

	if (byte1 & 6) {
		if (a == HL) {
			orahl();
		}
		else {
			orar8(a);
		}
	}

	if (byte1 & 5) {
		if (a == HL) {
			xorahl();
		}
		else {
			xorar8(a);
		}
	}

	if (byte1 & 4) {
		if (a == HL) {
			andahl();
		}
		else {
			andar8(a);
		}
	}

	if (byte1 & 3) {
		if (a == HL) {
			sbcahl();
		}
		else {
			sbcar8(a);
		}
	}

	if (byte1 & 2) {
		if (a == HL) {
			subahl();
		}
		else {
			subar8(a);
		}
	}

	if (byte1 & 1) {
		if (a == HL) {
			adcahl();
		}
		else {
			adcar8(a);
		}
	}

	if (byte1 == 32) {
		if (a == HL) {
			addahl();
		}
		else {
			addar8(a);
		}
	}
	return program_counter + 1;
}