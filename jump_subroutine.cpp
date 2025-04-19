#include "CPU.h"
#include "helpers.h"


//32868

uint16_t  CPU::calln16(uint16_t val, uint16_t program_counter) {
	uint16_t pushed_ad = program_counter + 1;
	if (debug){
		printf("call %s", to_string(val));
		printf(" pushed PC = %s\n", to_string(program_counter));
	}
	pushn16(pushed_ad, false);
	return val - 1;
}

uint16_t CPU::callccn16(Cond c, uint16_t val, uint16_t current_PC, bool* jumped){
	if (debug)
		printf("call %s %s pushed pc: %s\n", to_string(c), to_string(val),to_string(current_PC));
	if (check_cond(c)) {
		*jumped = true;
		uint16_t pushed_ad = current_PC+1;
		pushn16(pushed_ad, false);
		return val - 1;
	}
	else {
		return current_PC;
	}
}

uint16_t CPU::jphl(){
	if (debug)
		printf("jp hl\n");
	return HL - 1;
}

uint16_t CPU::jre8(uint8_t val) {
	int8_t signe = val;
	if (debug)
		printf("jr %d\n", signe);
	uint16_t add = PC + 3 + signe;
	return add - 1;
}

uint16_t  CPU::jrcce8(Cond c, uint8_t val, uint16_t current_PC, bool* jumped){
	if (debug)
		printf("jr %s %s\n",to_string(c), to_string(val));
	if (check_cond(c)) {
		*jumped = true;
		int8_t signe = val;
		uint16_t add = current_PC + 1 + signe;

		return add - 1;
	}
	else {
		return current_PC;
	}
}

uint16_t CPU::retcc(Cond c, uint16_t current_PC, bool* jumped){
	if (debug)
		printf("ret %s\n",to_string(c));
	if (check_cond(c)) {
		*jumped = true;
		return ret(false);
	}
	else return current_PC;
}

uint16_t CPU::ret(bool print) {
	uint16_t spc = popn16(false) - 1;
	if (debug && print)
		printf("ret PC = %s\n",to_string(spc));
	return spc;
}

uint16_t CPU::reti(){
	if (debug)
		printf("reti \n");
	ei();
	return ret(false);
}

uint16_t CPU::rstvec(uint8_t val){
	if (debug)
		printf("rst %s \n",to_string(val));
	if (val != 0x00 && val != 0x08 && val != 0x10 && val != 0x18 && val != 0x20 && val != 0x28 && val != 0x30 && val != 0x38) {
		printf("rst val improper %d", val);
	}
	uint16_t pushed_ad = PC + 1;
	pushn16(pushed_ad, false);
	return val - 1;
}

uint16_t CPU::jpn16(uint16_t val) {
	if (debug)
		printf("jp %s\n",to_string(val));
	return val-1;
}

uint16_t CPU::jpccn16(Cond c, uint16_t val, uint16_t current_PC, bool* jumped){
	if (debug)
		printf("jp %s %s\n",to_string(c),to_string(val));
	if (check_cond(c)) {
		*jumped = true;
		return val - 1;
	}
	else return current_PC;
}
