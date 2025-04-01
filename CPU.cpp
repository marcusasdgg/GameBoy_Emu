#include "CPU.h"
#include <iostream>

//every function is required to check for interrupts before then after.
static void print_binary(uint8_t n) {
	unsigned int mask = 1 << (sizeof(n) * 8 - 1);  // Mask to start from the most significant bit

	// Loop through all bits
	for (int i = 0; i < sizeof(n) * 8; i++) {
		// Print the current bit
		putchar((n & mask) ? '1' : '0');

		// Shift the mask to the right
		mask >>= 1;

		// Print space every 8 bits (optional for better readability)
		if ((i + 1) % 8 == 0) {
			putchar(' ');
		}
	}
	putchar('\n');
}



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

bool CPU::half_carry(uint8_t a, uint8_t b){
	return ((a & 0x0F) + (b & 0x0F) > 0x0F);
}

bool CPU::half_carry(uint16_t a , uint16_t b){
	return ((a & 0xFFF) + (b & 0xFFF) > 0xFFF);
}

bool CPU::full_carry(uint8_t a, uint8_t b){
	return (a + b > 0xFF);
}

bool CPU::full_carry(uint16_t a , uint16_t b){
	return (a + b > 0xFFFF);
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
	printf("\tA: %d\n\tB: %d\n\tC: %d\n\tD: %d\n\tE: %d\n\tH: %d\n\tL: %d\n\tSP: %d\n\tPC: %d\n\tF: ",A,B,C,D,E,H,L,SP,PC);
	print_binary(F);
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

void CPU::arithmetic_test() {
	ldr8n8(registerCalls::A, 8);
	print_registers();
	ldr8r8(registerCalls::B, registerCalls::A);
	print_registers();
	addar8(registerCalls::B);
	print_registers();
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


void CPU::NOP() {
	//nothing
}

bool CPU::get_carry() {
	return 0b00010000 & F;
}

void CPU::set_carry(bool a){
	if (a) {
		F |= 0b00010000;
	}
	else {
		F ^= 0b00010000;
	}
}

bool CPU::get_half_carry() {
	return 0b00001000 & F;
}

void CPU::set_half_carry(bool a){
	if (a) {
		F |= 0b00001000;
	}
	else {
		F ^= 0b00001000;
	}
}


bool CPU::get_zero(){
	return 0b00000010 & F;
}

void CPU::set_zero(bool a) {
	if (a) {
		F |= 0b00000010;
	}
	else {
		F ^= 0b00000010;
	}
}

void CPU::set_n(bool a){
	if (a) {
		F |= 0b00000100;
	}
	else {
		F ^= 0b00000100;
	}
}

bool CPU::get_n(){
	return F & 0b00000100;
}


void CPU::adcar8(registerCalls a){
	uint8_t temp = retrieve_register_8(a);
	bool carry = get_carry();
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp + carry;
}

void CPU::adcahl(){
	uint16_t add = retrieve_register_8(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	bool carry = get_carry();
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp + carry;
}

void CPU::adcan8(uint8_t temp){
	bool carry = get_carry();
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp + carry;
}

void CPU::addar8(registerCalls a){
	uint8_t temp = retrieve_register_8(a);
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp;
}

void CPU::addahl(){
	uint16_t add = retrieve_register_8(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp;
}

void CPU::addan8(uint8_t temp){
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp;
}

void CPU::addhlr16(registerCalls val){
	uint16_t hl = retrieve_register_16(registerCalls::HL);
	uint16_t temp = retrieve_register_16(val);
	store_in_register(registerCalls::HL, (uint16_t)(hl + temp));
	if (half_carry(hl, temp)) {
		set_half_carry(true);
	}

	if (full_carry(hl, temp)) {
		set_carry(true);
	}
	set_zero(false);
}

void CPU::addhlsp(){
	uint16_t temp = SP;
	uint16_t hl = retrieve_register_16(registerCalls::HL);
	store_in_register(registerCalls::HL, (uint16_t)(hl + temp));
	if (half_carry(hl, temp)) {
		set_half_carry(true);
	}

	if (full_carry(hl, temp)) {
		set_carry(true);
	}
	set_zero(false);
}

void CPU::addspe8(int8_t e8){
	int16_t s = e8 + SP;
	SP = s;
	set_zero(false);
	set_n(false);
	if (((SP & 0xF) + (e8 & 0xF)) > 0xF) F |= 0b00001000;

	//checking full carry
	if (((SP & 0xFF) + (e8 & 0xFF)) > 0xFF) F |= 0b00010000;

}





CPU::CPU(AddressSpace& addressSpace): address_space(addressSpace) {

}