#include "cpu.h"

//16 instructions
uint16_t CPU::handle_block_2(uint16_t program_counter) {
	uint8_t cycles = 0;
	uint8_t byte1 = address_space.read(program_counter);
	uint8_t reg8 = get_bit_range(byte1, 0, 2);
	registerCalls a = val_to_r8[reg8];
	byte1 = byte1 >> 3;

	if (byte1 == 32) {
			if (a == HL) {
				cycles = 2;
				addahl();
			}
			else {
				cycles = 1;
				addar8(a);
			}
	}

	else if (byte1 & 7) {
		if (a == HL) {
			cycles = 2;
			cpahl();
		}
		else {
			cycles = 1;
			cpar8(a);
		}
	}

	else if (byte1 & 6) {
		if (a == HL) {
			cycles = 2;
			orahl();
		}
		else {
			cycles = 1;
			orar8(a);
		}
	}

	else if (byte1 & 5) {
		if (a == HL) {
			cycles = 2;
			xorahl();
		}
		else {
			cycles = 1;
			xorar8(a);
		}
	}

	else if (byte1 & 4) {
		if (a == HL) {
			cycles = 2;
			andahl();
		}
		else {
			cycles = 1;
			andar8(a);
		}
	}

	else if (byte1 & 3) {
		if (a == HL) {
			cycles = 2;
			sbcahl();
		}
		else {
			cycles = 2;
			sbcar8(a);
		}
	}

	else if (byte1 & 2) {
		if (a == HL) {
			cycles = 2;
			subahl();
		}
		else {
			cycles = 1;
			subar8(a);
		}
	}

	else if (byte1 & 1) {
		if (a == HL) {
			cycles = 2;
			adcahl();
		}
		else {
			cycles = 1;
			adcar8(a);
		}
	}



	else {
		if (debug)
			printf("block 2 code invallid"); 
	}

	block_cycle_n(cycles);
	return program_counter + 1;
}