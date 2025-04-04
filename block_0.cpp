#include "CPU.h"
// maybe make it return a tuple of pc and cycles consumed
uint16_t CPU::handle_block_0(uint16_t program_counter) {
	uint8_t byte1 = address_space.read(program_counter);
	uint8_t r1645 = get_bit_range(byte1,
		4, 5);
	uint8_t r1635 = get_bit_range(byte1, 3, 5);

	//ccf
	if (byte1 & 63) {
		ccf();
	}

	//scf
	if (byte1 & 55) {
		scf();
	}



	//daa
	if (byte1 & 39) {
		daa();
	}

	//cpl
	if (byte1 & 37) {
		cpl();
	}

	//rra
	if (byte1 & 31) {
		rra();
	}

	//rla
	if (byte1 & 23) {
		rla();
	}

	//stop
	if (byte1 & 16) {
		//stop();
	}

	//rrca
	if (byte1 & 15) {
		rrca();
	}




	//decr16
	if (byte1 & 11) {
		registerCalls reg = val_to_r16[r1645];
		decr16(reg);
	}

	//ldar16mem
	if (byte1 & 10) {
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
		registerCalls reg = val_to_r16[r1645];
		addhlr16(reg);
	}

	// ldimm16sp
	if (byte1 & 8) {
		uint8_t byte1 = address_space.read(++program_counter);
		uint16_t byte2 = address_space.read(++program_counter);
		uint16_t imm16 = (byte2 << 8) | byte1;
		ldn16sp(imm16);
	}

	//rlca
	if (byte1 & 7) {
		rlca();
	}



	//ldr8imm8
	if (byte1 & 6) {
		uint8_t byte1 = address_space.read(++program_counter);
		registerCalls reg = val_to_r8[r1635];
		if (reg == HL) {
			ldhln8(byte1);
		}
		else {
			ldr8n8(reg, byte1);
		}
	}

	//decr8
	if (byte1 & 5) {
		registerCalls reg = val_to_r8[r1635];
		if (reg == HL) {
			dechl();
		}
		else {
			decr8(reg);
		}
	}
	//incr8
	if (byte1 & 4) {
		registerCalls reg = val_to_r8[r1635];
		if (reg == HL) {
			inchl();
		}
		else {
			incr8(reg);
		}
	}

	//incr16
	if (byte1 & 3) {
		registerCalls reg = val_to_r16[r1645];
		incr16(reg);
	}

	//ld r16mem, a
	if (byte1 & 2) {
		registerCalls reg = val_to_r16mem[r1645];
		if (reg == registerCalls::HLI) {
			ldhlda();
		}
		else if (reg == registerCalls::HLD) {
			ldhlia();
		}
		else {
			ldr16a(reg);
		}
	}

	//ldr16, imm16
	if (byte1 & 1) {
		registerCalls reg = val_to_r16[r1645];
		uint8_t byte1 = address_space.read(++program_counter);
		uint16_t byte2 = address_space.read(++program_counter);
		uint16_t imm16 = (byte2 << 8) | byte1;
		ldr16n16(reg, imm16);
		//calculate num_cycles or something
	}

	if (byte1 == 0) {
		//nop
	}
	printf("block 0 new pc is %d", program_counter);
	return program_counter + 1;
}