#include "CPU.h"
#include <iostream>

//every function is required to check for interrupts before then after.

void CPU::inititialise() {
	A = 0;
	B = 0;
	C = 0;
	D = 0;
	E = 0;
	F = 0;
	H = 0;
	L = 0;

	F = 0;
	SP = 0xFFFE;
	PC = 0;

}

static bool is_16_bit(registerCalls a) {
	return (a == registerCalls::BC || a == registerCalls::DE || a == registerCalls::HL || a == registerCalls::AF);
}

void CPU::store_in_register(registerCalls a, uint16_t value) {
	if (!is_16_bit(a)) {
		std::cout << "register a is too small for 16 bit value\n";
		return;
	}

	uint8_t first = static_cast<uint8_t>(value);
	uint8_t second = static_cast<uint8_t>(value >> 8);

	switch (a){
		case registerCalls::AF:
			A = first;
			F = second;
			break;
		case registerCalls::BC:
			B = first;
			C = second;
			break;
		case registerCalls::DE:
			D = first;
			E = second;
			break;
		case registerCalls::HL:
			H = first;
			L = second;
			break;
		case registerCalls::SP:
			SP = value;
			break;
		case registerCalls::PC:
			PC = value;
			break;
		default:
			break;
	}
}

void CPU::store_in_register(registerCalls a, uint8_t value) {
	if (is_16_bit(a)) {
		std::cout << "register a is too big for 16 bit value\n";
		return;
	}

	switch (a){
		case registerCalls::A:
			A = value;
			break;
		case registerCalls::B:
			B = value;
			break;
		case registerCalls::C:
			C = value;
			break;
		case registerCalls::D:
			D = value;
			break;
		case registerCalls::E:
			E = value;
			break;
		case registerCalls::H:
			H = value;
			break;
		case registerCalls::L:
			L = value;
			break;
		default:
			break;
	}
}


uint8_t CPU::retrieve_register_8(registerCalls a) {
	if (is_16_bit(a)) {
		std::cout << "trying to retrieve 8 bits from 16\n";
		return 0;
	}

	switch (a) {
		case registerCalls::A:
			return A;
			break;
		case registerCalls::B:
			return B;
			break;
		case registerCalls::C:
			return C;
			break;
		case registerCalls::D:
			return D;
			break;
		case registerCalls::E:
			return E;
			break;
		case registerCalls::H:
			return H;
			break;
		case registerCalls::L:
			return L;
			break;
		default:
			break;
	}
}

uint16_t CPU::retrieve_register_16(registerCalls a) {
	if (!is_16_bit(a)) {
		std::cout << "trying to retrieve 16 bits from 8\n";
		return 0;
	}

	switch (a) {
	case registerCalls::AF:
		return (A << 8) | F;
		break;
	case registerCalls::BC:
		return (B << 8) | C;
		break;
	case registerCalls::DE:
		return (D << 8) | E;
		break;
	case registerCalls::HL:
		return (H << 8) | L;
		break;
	case registerCalls::SP:
		return SP;
		break;
	case registerCalls::PC:
		return PC;
		break;
	default:
		return 0;
		break;
	}
}

void CPU::print_registers() {
	std::cout << "Registers Report\n";
	printf("\tA: %d\n\tB: %d\n\tC: %d\n\tD: %d\n\tE: %d\n\tH: %d\n\tL: %d\n\tF: %d\n\tSP: %d\n\tPC: %d\n",A,B,C,D,E,H,L,F,SP,PC);
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

void CPU::ldhln8(uint8_t val) {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
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

void CPU::ldhac() {
	address_space.write(0xFF00 + C, retrieve_register_8(registerCalls::A));
}

void CPU::ldhlia() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	address_space.write(ad, A);
	inc(registerCalls::HL);
}

void CPU::ldhlda() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	address_space.write(ad, A);
	dec(registerCalls::HL);
}

void CPU::ldahld() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	store_in_register(registerCalls::A, temp);
	dec(registerCalls::A);
}

void CPU::ldahli() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	store_in_register(registerCalls::A, temp);
	inc(registerCalls::A);
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

void CPU::arithmetic_test() {
	ldr8n8(registerCalls::A, 8);
	print_registers();
	ldr8r8(registerCalls::B, registerCalls::A);
	print_registers();
}

void CPU::ldhlspe8(int16_t e8) {
	int16_t signed_e8 = static_cast<int16_t>(e8);
	uint16_t result = SP + signed_e8;

	store_in_register(registerCalls::HL, result);
	F = 0; // Z and N are always 0

	//checking half carry
	if (((SP & 0xF) + (signed_e8 & 0xF)) > 0xF) F |= 0b00001000; 

	//checking full carry
	if (((SP & 0xFF) + (signed_e8 & 0xFF)) > 0xFF) F |= 0b00010000;
}

void CPU::ldsphl() {
	int temp = retrieve_register_16(registerCalls::HL);
	SP = temp;
}

void CPU::NOP() {
	//nothing
}

CPU::CPU(AddressSpace& addressSpace): address_space(addressSpace) {

}