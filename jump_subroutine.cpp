#include "CPU.h"

void CPU::calln16(uint16_t val) {
	uint16_t pushed_ad = PC + 3;
	pushn16(pushed_ad);
	jpn16(val);
}

void CPU::callccn16(Cond c, uint16_t val){
	if (check_cond(c)) {
		calln16(val);
	}
}

void CPU::jphl(){
	PC = HL;
}

void CPU::jre8(uint8_t val) {
	int8_t signe = val;
	uint16_t add = PC + 3 + signe;
	PC = add -1;
}

void CPU::jrcce8(Cond c, uint8_t val){
	if (check_cond(c)) jre8(val);
}

void CPU::retcc(Cond c){
	if (check_cond(c)) ret();
}

void CPU::ret() {
	PC = popn16();
}

void CPU::reti(){
	ei();
	ret();
}

void CPU::rstvec(uint8_t val){
	if (val != 0x00 && val != 0x08 && val != 0x10 && val != 0x18 && val != 0x20 && val != 0x28 && val != 0x30 && val != 0x38) {
		printf("rst val improper %d", val);
	}
	calln16(val);
}

void CPU::jpn16(uint16_t val) {
	PC = val-1;
}

void CPU::jpccn16(Cond c, uint16_t val){
	if (check_cond(c)) jpn16(val);
}
