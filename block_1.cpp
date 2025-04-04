#include "CPU.h"


uint16_t CPU::handle_block_1(uint16_t program_counter) {
	uint8_t byte = address_space.read(program_counter);

	uint8_t dest = get_bit_range(byte, 3, 5);
	uint8_t source = get_bit_range(byte, 0, 2);
	registerCalls des = val_to_r8[dest];
	registerCalls sourc = val_to_r8[source];
	
	if (des == HL && sourc == HL) {
		//halt();
	}
	else {
		if (des == HL) {
			ldhlr8(sourc);
		}
		else if (sourc == HL) {
			ldr8hl(des);
		}
		else {
			ldr8r8(des, sourc);
		}
	}

	return program_counter + 1;
}