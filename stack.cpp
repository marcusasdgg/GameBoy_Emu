#include "CPU.h"
#include "helpers.h"

void CPU::popaf() {
	if (debug)
		printf("pop af\n");
	F = address_space.read(SP) & 0xF0; //TODO surely u dont need this in the future????
	SP++;
	A = address_space.read(SP);
	SP++;
}

void CPU::popr16(registerCalls a){
	uint16_t temp = SP;
	uint8_t byte1 = address_space.read(SP);
	SP++;
	uint8_t byte2 = address_space.read(SP);
	SP++;
	uint16_t imm16 = (byte2 << 8) | byte1;
	if (debug) {
		printf("pop %s popped val: %s\n", to_string(a), to_string(imm16));
	}
		
	store_in_register(a, imm16);
}

uint16_t CPU::popn16(bool print) {
	uint16_t byte1 = address_space.read(SP);
	SP++;
	uint8_t byte2 = address_space.read(SP);
	SP++;
	uint16_t imm16 = (byte2 << 8) | byte1;
	if (debug && print)
		printf("pop n16 val: %s\n", to_string(imm16));
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

void CPU::pushn8(uint8_t val, bool print) {
	if (debug && print)
		printf("push %s\n", to_string(val));
	SP--;
	address_space.write(SP, val);
}

void CPU::pushn16(uint16_t val, bool print){
	if (debug && print)
		printf("push %s \n",to_string(val));
	uint8_t high = val >> 8;
	uint8_t low = val;
	SP--;
	address_space.write(SP, high);
	SP--;
	address_space.write(SP, low);
}

void CPU::pushr16(registerCalls a) {
	uint16_t v = retrieve_register_16(a);
	if (debug) {
		printf("push %s pushed val: %s\n", to_string(a), to_string(v));
	}
		
	uint16_t bytes = v;
	uint8_t high = bytes >> 8;
	uint8_t low = bytes;
	SP--;
	address_space.write(SP,high);
	SP--;
	address_space.write(SP,low);
	if (debug) {
		printf("to %s\n", to_string(SP));
	}
}