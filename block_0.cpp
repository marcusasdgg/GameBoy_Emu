#include "CPU.h"


static void print_binary(uint8_t n) {
	unsigned int mask = 1 << (sizeof(n) * 8 - 1);  // Mask to start from the most significant bit

	// Loop through all bits
	for (int i = 0; i < sizeof(n) * 8; i++) {
		// Print the current bit
		putchar((n & mask) ? '1' : '0');

		// Shift the mask to the right
		mask >>= 1;

		// Print space every 8 bits (optional for better readability)
		if ((i + 1) % 8 == 0) {
			putchar(' ');
		}
	}
	putchar('\n');
}

// gives a new program counter and delays n-cycles
//31 instructions
uint16_t CPU::handle_block_0(uint16_t program_counter) {
	uint8_t cycles = 0;
	uint8_t byte1 = address_space.read(program_counter);

	uint8_t r1645 = get_bit_range(byte1,
		4, 5);
	uint8_t r1635 = get_bit_range(byte1, 3, 5);

	uint8_t first3 = get_bit_range(byte1, 0, 3);

	//check hard code op codes
	if (byte1 == 0b00000000) {
		cycles = 1;
		nop();
	}
	else if (byte1 == 0b00001000) {
		uint8_t byte2 = address_space.read(++program_counter);
		uint8_t byte3 = address_space.read(++program_counter);
		uint16_t imm16 = (byte3 << 8) | byte2;
		ldn16sp(imm16);
		cycles = 5;
	}
	else if (byte1 == 0b00000111) {
		cycles = 1;
		rlca();
	}
	else if (byte1 == 0b00001111) {
		cycles = 1;
		rrca();
	}
	else if (byte1 == 0b00010111) {
		cycles = 1;
		rla();
	}
	else if (byte1 == 0b00011111) {
		cycles = 1;
		rra();
	}
	else if (byte1 == 0b00100111) {
		cycles = 1;
		daa();
	}
	else if (byte1 == 0b00101111) {
		cycles = 1;
		cpl();
	}
	else if (byte1 == 0b00110111) {
		cycles = 1;
		scf();
	}
	else if (byte1 == 0b00111111) {
		cycles = 1;
		ccf();
	}
	else if (byte1 == 0b00011000) {
		uint8_t byte2 = address_space.read(++program_counter);
		cycles = 16;
		jre8(byte2);
	}
	else if (byte1 == 0b00010000) {
		//????
		stop();
	}
	else {

		if (first3 == 1) {
			//adhlr16 or ldr16im16
			registerCalls reg = val_to_r16[r1645];
			if (get_bit(byte1, 3)) {
				cycles = 2;
				addhlr16(reg);
			}
			else {
				uint8_t byte2 = address_space.read(++program_counter);
				uint8_t byte3 = address_space.read(++program_counter);
				uint16_t imm16 = (byte3 << 8) | byte2;
				cycles = 3;
				ldr16n16(reg, imm16);
			}
		}
		else if (first3 == 2) {
			//ldr16mema or ldar16mem 
			registerCalls reg = val_to_r16[r1645];
			if (get_bit(byte1,3)) {
				if (reg == HLI) {
					cycles = 2;
					ldahli();
				}
				else if (reg == HLD) {
					cycles = 2;
					ldahld();
				}
				else {
					cycles = 2;
					ldar16(reg);
				}
			}
			else {
				if (reg == HLI) {
					cycles = 2;
					ldhlia();
				}
				else if (reg == HLD) {
					cycles = 2;
					ldhlda();
				}
				else {
					cycles = 2;
					ldr16a(reg);
				}
			}
		}
		else if (first3 == 3) {
			//incr16 or decr16
			registerCalls reg = val_to_r16[r1645];
			if (get_bit(byte1, 3)) {
				if (reg == registerCalls::SP) {
					cycles = 2;
					decsp();
				}
				else if (reg == HL) {
					cycles = 2;

					decr16(reg);
				}
				else {
					cycles = 2;
					decr16(reg);
				}
			}
			else {
				if (reg == registerCalls::SP) {
					cycles = 2;
					incsp();
				}
				else if (reg == HL) {
					cycles = 2;
					incr16(reg);
				}
				else {
					cycles = 2;
					incr16(reg);
				}
			}
		}
		else if (first3 == 4) {
			//incr8
			registerCalls reg = val_to_r8[r1635];
			if (reg == HL) {
				cycles = 3;
				inchl();
			}
			else {
				cycles = 1;
				incr8(reg);
			}
		} 
		else if (first3 == 5) {
			//decr8
			registerCalls reg = val_to_r8[r1635];
			if (reg == HL) {
				cycles = 3;
				dechl();
			}
			else {
				cycles = 1;
				decr8(reg);
			}
		}
		else if (first3 == 6) {
			//ldr8imm8 
			registerCalls reg = val_to_r8[r1635];
			uint8_t byte2 = address_space.read(++program_counter);
			ldr8n8(reg,byte2);
			cycles = 2;
		}

	}

	block_cycle_n(cycles);
	return program_counter + 1;
}

////ccf
//if (byte1 & 63) {
//	cycles = 1;
//
//	ccf();
//}
//
////scf
//if (byte1 & 55) {
//	cycles = 1;
//	scf();
//}
//
//if (byte1 & 49) {
//
//}
//
//
////daa
//if (byte1 & 39) {
//	cycles = 1;
//	daa();
//}
//
////cpl
//if (byte1 & 37) {
//	cycles = 1;
//	cpl();
//}
//
////rra
//if (byte1 & 31) {
//	cycles = 1;
//	rra();
//}
//
////rla
//if (byte1 & 23) {
//	cycles = 1;
//	rla();
//}
//
////stop
//if (byte1 & 16) {
//	cycles = 1;//???
//	stop();
//}
//
////rrca
//if (byte1 & 15) {
//	cycles = 1;
//	rrca();
//}
//
//
//
//
////decr16
//if (byte1 & 11) {
//	cycles = 2;
//	registerCalls reg = val_to_r16[r1645];
//	if (reg == registerCalls::SP) {
//		decsp();
//	}
//	else {
//		decr16(reg);
//	}
//}
//
////ldar16mem
//if (byte1 & 10) {
//	cycles = 2;
//	registerCalls reg = val_to_r16mem[r1645];
//	if (reg == registerCalls::HLI) {
//		ldahli();
//	}
//	else if (reg == registerCalls::HLD) {
//		ldahld();
//	}
//	else {
//		ldar16(reg);
//	}
//}
//
////addhlr16
//if (byte1 & 9) {
//	cycles = 2;
//	registerCalls reg = val_to_r16[r1645];
//	if (reg == registerCalls::SP) {
//		addhlsp();
//	}
//	else {
//		addhlr16(reg);
//	}
//}
//
//// ldimm16sp
//if (byte1 & 8) {
//	cycles = 5;
//	uint8_t byte1 = address_space.read(++program_counter);
//	uint16_t byte2 = address_space.read(++program_counter);
//	uint16_t imm16 = (byte2 << 8) | byte1;
//	ldn16sp(imm16);
//}
//
////rlca
//if (byte1 & 7) {
//	cycles = 1;
//	rlca();
//}
//
//
//
////ldr8imm8
//if (byte1 & 6) {
//	uint8_t byte1 = address_space.read(++program_counter);
//	registerCalls reg = val_to_r8[r1635];
//
//	if (reg == HL) {
//		cycles = 3;
//		ldhln8(byte1);
//	}
//	else {
//		cycles = 2;
//		ldr8n8(reg, byte1);
//	}
//}
//
////decr8
//if (byte1 & 5) {
//	registerCalls reg = val_to_r8[r1635];
//	if (reg == HL) {
//		cycles = 3;
//		dechl();
//	}
//	else {
//		cycles = 1;
//		decr8(reg);
//	}
//}
////incr8
//if (byte1 & 4) {
//	registerCalls reg = val_to_r8[r1635];
//	if (reg == HL) {
//		cycles = 3;
//		inchl();
//	}
//	else {
//		cycles = 1;
//		incr8(reg);
//	}
//}
//
////incr16
//if (byte1 & 3) {
//	registerCalls reg = val_to_r16[r1645];
//	if (reg == registerCalls::SP) {
//		incsp();
//	}
//	else {
//		incr16(reg);
//	}
//	cycles = 2;
//}
//
////ld r16mem, a
//if (byte1 & 2) {
//	registerCalls reg = val_to_r16mem[r1645];
//	if (reg == registerCalls::HLI) {
//		cycles = 2;
//		ldhlda();
//	}
//	else if (reg == registerCalls::HLD) {
//		cycles = 2;
//		ldhlia();
//	}
//	else {
//		cycles = 2;
//		ldr16a(reg);
//	}
//}
//
////ldr16, imm16
//if (byte1 & 1) {
//	cycles = 3;
//	registerCalls reg = val_to_r16[r1645];
//	uint8_t byte1 = address_space.read(++program_counter);
//	uint16_t byte2 = address_space.read(++program_counter);
//	uint16_t imm16 = (byte2 << 8) | byte1;
//	if (reg == registerCalls::SP) {
//		ldspn16(imm16);
//	}
//	else {
//		ldr16n16(reg, imm16);
//	}
//	//calculate num_cycles or something
//}
//
//if (byte1 == 0) {
//	nop();
//	printf("nop executed\n");
//	cycles = 1;
//}
//printf("pc is %d\n", program_counter);
