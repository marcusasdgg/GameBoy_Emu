#include "CPU.h"

void CPU::popaf() {
	if (debug)
		printf("pop af\n");
	F = address_space.read(SP);
	SP++;
	A = address_space.read(SP);
	SP++;
}

void CPU::popr16(registerCalls a){
	uint16_t byte1 = address_space.read(SP);
	SP++;
	uint8_t byte2 = address_space.read(SP);
	SP++;
	uint16_t imm16 = (byte2 << 8) | byte1;
	if (debug)
		printf("pop r16\n");
	store_in_register(a, imm16);
}

uint16_t CPU::popn16() {
	uint16_t byte1 = address_space.read(SP);
	SP++;
	uint8_t byte2 = address_space.read(SP);
	SP++;
	uint16_t imm16 = (byte2 << 8) | byte1;
	if (debug)
		printf("pop n16\n");
	return imm16;
}

void CPU::pushaf(){
	if (debug)
		printf("push af\n");
	SP--;
	address_space.write(SP,A);
	SP--;
	address_space.write(SP,F);
}

void CPU::pushn8(uint8_t val){
	if (debug)
		printf("push n8\n");
	SP--;
	address_space.write(SP, val);
}

void CPU::pushn16(uint16_t val){
	if (debug)
		printf("push n16 \n");
	uint8_t high = val >> 8;
	uint8_t low = val;
	SP--;
	address_space.write(SP, high);
	SP--;
	address_space.write(SP, low);
}

void CPU::pushr16(registerCalls a) {
	if (debug)
		printf("push r16 \n");
	uint16_t bytes = retrieve_register_16(a);
	uint8_t high = bytes >> 8;
	uint8_t low = bytes;
	SP--;
	address_space.write(SP,high);
	SP--;
	address_space.write(SP,low);
}