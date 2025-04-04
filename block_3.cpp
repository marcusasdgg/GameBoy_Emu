#include "CPU.h"

//53 instructions
uint16_t CPU::handle_block_3(uint16_t program_counter) {
	uint8_t byte1 = address_space.read(program_counter);
	uint8_t cycles = 0;

	if (byte1 == 0b11000110) {
		uint8_t byte2 = address_space.read(++program_counter);
		addan8(byte2);
		cycles = 2;
	}
	else if(byte1 == 0b11001110) {
		uint8_t byte2 = address_space.read(++program_counter);
		cycles = 2;
		adcan8(byte2);
	}
	else if (byte1 == 0b11010110) {
		cycles = 2;
		uint8_t byte2 = address_space.read(++program_counter);
		suban8(byte2);
	}
	else if (byte1 == 0b11011110) {
		cycles = 2;
		uint8_t byte2 = address_space.read(++program_counter);
		sbcn8(byte2);
	}
	else if (byte1 == 0b11100110) {
		cycles = 2;
		uint8_t byte2 = address_space.read(++program_counter);
		andan8(byte2);
	}
	else if (byte1 == 0b11101110) {
		cycles = 2;
		uint8_t byte2 = address_space.read(++program_counter);
		xoran8(byte2);
	}
	else if (byte1 == 0b11110110) {
		cycles = 2;
		uint8_t byte2 = address_space.read(++program_counter);
		oran8(byte2);
	}
	else if (byte1 == 0b11111110) {
		cycles = 2;
		uint8_t byte2 = address_space.read(++program_counter);
		cpan8(byte2);
	} 
	else if (byte1 == 0b11100010) {
		cycles = 2;
		ldhca();
	}
	else if (byte1 == 0b11100000) {
		cycles = 2;
		uint8_t byte2 = address_space.read(++program_counter);
		ldhn8a(byte2);
	}
	else if (byte1 == 0b11101010) {
		uint8_t byte2 = address_space.read(++program_counter);
		uint8_t byte3 = address_space.read(++program_counter);
		uint16_t imm16 = (byte3 << 8) | byte2;
		cycles = 4;
		ldn16a(imm16);
	}
	else if (byte1 == 0b11110010) {
		cycles = 2;
		ldhac();
	}
	else if (byte1 == 0b11110000) {
		uint8_t byte2 = address_space.read(++program_counter);
		cycles = 2;
		ldhan8(byte2);
	}
	else if (byte1 == 0b11111010) {
		uint8_t byte2 = address_space.read(++program_counter);
		uint8_t byte3 = address_space.read(++program_counter);
		uint16_t imm16 = (byte3 << 8) | byte2;
		ldan16(imm16);
		cycles = 4;
	}
	else if (byte1 == 0b11101000) {
		uint8_t byte2 = address_space.read(++program_counter);
		cycles = 4;
		addspe8(byte2);
	}
	else if (byte1 == 0b11111000) {
		uint8_t byte2 = address_space.read(++program_counter);
		cycles = 3;
		ldhlspe8(byte2);
	}
	else if (byte1 == 0b11111001) {
		cycles = 2;
		ldsphl();
	}
	else if (byte1 == 0b11110011) {
		cycles = 1;
		di();
	}
	else if (byte1 == 0b11001001) {
		cycles = 4;
		ret();
	}
	else if (byte1 == 0b11011001) {
		cycles = 4;
		reti();
	}
	else if (byte1 == 0b11101001) {
		cycles = 1;
		jphl();
	}
	else if (byte1 == 0b11111011) {
		ei();
		cycles = 1;
		//22
	}
	else if (byte1 == 0b11001011) {
		//cb prefix
		uint8_t byte2 = address_space.read(++program_counter);
		uint8_t bit02 = get_bit_range(byte2, 0, 2);
		uint8_t bit35 = get_bit_range(byte2, 3, 5);
		if (byte2 >> 6 == 0) {
			switch (byte2 >> 2) {
			case 0:
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					rlchl();
				}
				else {
					cycles = 2;
					rlcr8(val_to_r8[bit02]);
				}
			case 1:
				cycles = 4;
				if (val_to_r8[bit02] == HL) {
					rrchl();
				}
				else {
					cycles = 2;
					rrcr8(val_to_r8[bit02]);
				}
				
			case 2:
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					rlhl();
				}
				else {
					cycles = 2;
					rlr8(val_to_r8[bit02]);
				}
			case 3:
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					rrhl();
				}
				else {
					cycles = 2;
					rrr8(val_to_r8[bit02]);
				}
			case 4:
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					slahl();
				}
				else {
					cycles = 2;
					slar8(val_to_r8[bit02]);
				}
			case 5:
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					srahl();
				}
				else {
					cycles = 2;
					srar8(val_to_r8[bit02]);
				}
			case 6:
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					swaphl();
				}
				else {
					cycles = 2;
					swapr8(val_to_r8[bit02]);
				}
			case 7:
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					srlhl();
				}
				else {
					cycles = 2;
					srlr8(val_to_r8[bit02]);
				}
			}
		}
		else {
			if (byte2 >> 6 == 1) {
				if (val_to_r8[bit02] == HL) {
					bitu3hl(bit35);
					cycles = 3;
				}
				else {
					cycles = 2;
					bitu3r8(bit35, val_to_r8[bit02]);
				}
			}
			else if (byte2 >> 6 == 2) {
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					resu3hl(bit35);
				}
				else {
					cycles = 2;
					resu3r8(bit35, val_to_r8[bit02]);
				}
			}
			else {
				if (val_to_r8[bit02] == HL) {
					cycles = 4;
					setu3hl(bit35);
				}
				else {
					cycles = 2;
					setu3r8(bit35, val_to_r8[bit02]);
					//44
				}
			}
		}
	}
	else {
		//none of block 3 part 1
		uint8_t last3 = get_bit_range(byte1, 0, 2);
		uint8_t cond34 = get_bit_range(byte1, 3, 4);
		uint8_t cond35 = get_bit_range(byte1, 3, 5);
		uint8_t cond45 = get_bit_range(byte1, 4, 5);


		if (last3 == 0) {
			//ret cond
			Cond c = val_to_cond[cond34];
			retcc(c); //5 taken 2 untaken??
		}
		else if (last3 == 1) {
			//pop16stk
			registerCalls reg = val_to_r16stk[cond45];
			if (reg == registerCalls::AF) {
				popaf();
			}
			else {
				popr16(reg);
			}
			cycles = 3;
			
		}
		else if (last3 == 2) {
			//jpconfimm16 
			// 4 or 3 untaken
			Cond c = val_to_cond[cond34];
			uint8_t byte2 = address_space.read(++program_counter);
			uint8_t byte3 = address_space.read(++program_counter);
			uint16_t imm16 = (byte3 << 8) | byte2;
			jpccn16(c, imm16);
		}
		else if (last3 == 3) {
			uint8_t byte2 = address_space.read(++program_counter);
			uint8_t byte3 = address_space.read(++program_counter);
			uint16_t imm16 = (byte3 << 8) | byte2;
			jpn16(imm16);
			cycles = 4;
		}
		else if (last3 == 4) {
			//callcondimm16
			Cond c = val_to_cond[cond34];
			uint8_t byte2 = address_space.read(++program_counter);
			uint8_t byte3 = address_space.read(++program_counter);
			uint16_t imm16 = (byte3 << 8) | byte2;
			callccn16(c, imm16);
			// 6 or 3;
		}

		else if (last3 == 5) {
			//pushr16stk
			registerCalls reg = val_to_r16stk[cond45];
			if (reg == registerCalls::AF) {
				pushaf();
			}
			else {
				pushr16(reg);
			}
			cycles = 4;
		}
		else if (last3 == 7) {
			//rsttgt3
			cycles = 4;
			rstvec(cond35 * 8);
		}

	}

	block_cycle_n(cycles);

	return program_counter + 1;
}