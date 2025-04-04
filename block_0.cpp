#include "CPU.h"




// gives a new program counter and delays n-cycles
//31 instructions
uint16_t CPU::handle_block_0(uint16_t program_counter) {
	uint8_t cycles = 0;
	uint8_t byte1 = address_space.read(program_counter);
	uint8_t r1645 = get_bit_range(byte1,
		4, 5);
	uint8_t r1635 = get_bit_range(byte1, 3, 5);

	//ccf
	if (byte1 & 63) {
		cycles = 1;
		ccf();
	}

	//scf
	if (byte1 & 55) {
		cycles = 1;
		scf();
	}



	//daa
	if (byte1 & 39) {
		cycles = 1;
		daa();
	}

	//cpl
	if (byte1 & 37) {
		cycles = 1;
		cpl();
	}

	//rra
	if (byte1 & 31) {
		cycles = 1;
		rra();
	}

	//rla
	if (byte1 & 23) {
		cycles = 1;
		rla();
	}

	//stop
	if (byte1 & 16) {
		cycles = 1;//???
		stop();
	}

	//rrca
	if (byte1 & 15) {
		cycles = 1;
		rrca();
	}




	//decr16
	if (byte1 & 11) {
		cycles = 2;
		registerCalls reg = val_to_r16[r1645];
		if (reg == registerCalls::SP) {
			decsp();
		}
		else {
			decr16(reg);
		}
	}

	//ldar16mem
	if (byte1 & 10) {
		cycles = 2;
		registerCalls reg = val_to_r16mem[r1645];
		if (reg == registerCalls::HLI) {
			ldahli();
		}
		else if (reg == registerCalls::HLD) {
			ldahld();
		}
		else {
			ldar16(reg);
		}
	}

	//addhlr16
	if (byte1 & 9) {
		cycles = 2;
		registerCalls reg = val_to_r16[r1645];
		if (reg == registerCalls::SP) {
			addhlsp();
		}
		else {
			addhlr16(reg);
		}
	}

	// ldimm16sp
	if (byte1 & 8) {
		cycles = 5;
		uint8_t byte1 = address_space.read(++program_counter);
		uint16_t byte2 = address_space.read(++program_counter);
		uint16_t imm16 = (byte2 << 8) | byte1;
		ldn16sp(imm16);
	}

	//rlca
	if (byte1 & 7) {
		cycles = 1;
		rlca();
	}



	//ldr8imm8
	if (byte1 & 6) {
		uint8_t byte1 = address_space.read(++program_counter);
		registerCalls reg = val_to_r8[r1635];
		
		if (reg == HL) {
			cycles = 3;
			ldhln8(byte1);
		}
		else {
			cycles = 2;
			ldr8n8(reg, byte1);
		}
	}

	//decr8
	if (byte1 & 5) {
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
	//incr8
	if (byte1 & 4) {
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

	//incr16
	if (byte1 & 3) {
		registerCalls reg = val_to_r16[r1645];
		if (reg == registerCalls::SP) {
			incsp();
		}
		else {
			incr16(reg);
		}
		cycles = 2;
	}

	//ld r16mem, a
	if (byte1 & 2) {
		registerCalls reg = val_to_r16mem[r1645];
		if (reg == registerCalls::HLI) {
			cycles = 2;
			ldhlda();
		}
		else if (reg == registerCalls::HLD) {
			cycles = 2;
			ldhlia();
		}
		else {
			cycles = 2;
			ldr16a(reg);
		}
	}

	//ldr16, imm16
	if (byte1 & 1) {
		cycles = 3;
		registerCalls reg = val_to_r16[r1645];
		uint8_t byte1 = address_space.read(++program_counter);
		uint16_t byte2 = address_space.read(++program_counter);
		uint16_t imm16 = (byte2 << 8) | byte1;
		if (reg == registerCalls::SP) {
			ldspn16(imm16);
		}
		else {
			ldr16n16(reg, imm16);
		}
		//calculate num_cycles or something
	}

	if (byte1 == 0) {
		nop();
		printf("nop\n");
		cycles = 1;
	}
	printf("block 0 new pc is %d", program_counter);

	block_cycle_n(cycles);
	return program_counter + 1;
}