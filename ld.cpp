#include "CPU.h"

void CPU::ldhln8(uint8_t val) {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	address_space.write(ad, val);
}


void CPU::ldr8r8(registerCalls a, registerCalls b) {
	uint8_t temp = retrieve_register_8(b);
	store_in_register(a, temp);
}

void CPU::ldr8n8(registerCalls a, uint8_t val) {
	store_in_register(a, val);
}

void CPU::ldr16n16(registerCalls a, uint16_t val) {
	store_in_register(a, val);
}

void CPU::ldhlr8(registerCalls a) {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t val = retrieve_register_8(a);
	address_space.write(ad, val);
}



void CPU::ldr8hl(registerCalls a) {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	store_in_register(a, address_space.read(ad));
}

void CPU::ldr16a(registerCalls a) {
	uint16_t ad = retrieve_register_16(a);
	address_space.write(ad, retrieve_register_8(registerCalls::A));
}

void CPU::ldn16a(uint16_t a) {
	address_space.write(a, retrieve_register_8(registerCalls::A));
}

void CPU::ldhn16a(uint16_t a) {
	if (a >= 0xFF00 && a <= 0xFFFF) {
		address_space.write(a, retrieve_register_8(registerCalls::A));
	}
}

void CPU::ldhca() {
	address_space.write(A, 0xFF00 + C);
}

void CPU::ldar16(registerCalls a) {
	uint16_t temp = retrieve_register_16(a);
	uint8_t val = address_space.read(temp);
	A = val;
}

void CPU::ldan16(uint16_t val) {
	uint8_t temp = address_space.read(val);
	A = temp;
}

void CPU::ldhan16(uint16_t val) {
	if (val <= 0xFFFF && val >= 0xFF00) {
		uint8_t temp = address_space.read(val);
		A = temp;
	}
}

void CPU::ldhac() {
	address_space.write(0xFF00 + C, retrieve_register_8(registerCalls::A));
}

void CPU::ldhlia() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	address_space.write(ad, A);
	//inc(registerCalls::HL);
}

void CPU::ldhlda() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	address_space.write(ad, A);
	//dec(registerCalls::HL);
}

void CPU::ldahld() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	store_in_register(registerCalls::A, temp);
	//dec(registerCalls::A);
}

void CPU::ldahli() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	store_in_register(registerCalls::A, temp);
	//inc(registerCalls::A);
}

void CPU::ldspn16(uint16_t a) {
	store_in_register(registerCalls::SP, a);
}

void CPU::ldn16sp(uint16_t a) {
	uint16_t value = retrieve_register_16(registerCalls::SP);
	uint8_t first = static_cast<uint8_t>(value);
	uint8_t second = static_cast<uint8_t>(value >> 8);
	address_space.write(a, first);
	address_space.write(a + 1, second);
}



void CPU::ldhlspe8(int16_t e8) {
	int16_t signed_e8 = static_cast<int16_t>(e8);
	int16_t result = SP + signed_e8;

	store_in_register(registerCalls::HL, static_cast<uint16_t>(result));
	set_zero(false);
	set_n(false);// Z and N are always 0

	//checking half carry
	if (((SP & 0xF) + (signed_e8 & 0xF)) > 0xF) F |= 0b00001000;

	//checking full carry
	if (((SP & 0xFF) + (signed_e8 & 0xFF)) > 0xFF) F |= 0b00010000;
}

void CPU::ldsphl() {
	int temp = retrieve_register_16(registerCalls::HL);
	SP = temp;
}