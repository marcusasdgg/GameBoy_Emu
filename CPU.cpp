#include "CPU.h"
#include "Clock.h"
#include <iostream>
#include <fstream>
#include "helpers.h"
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

CPU::CPU(AddressSpace& addressSpace, Clock& clock_l) : address_space(addressSpace), clock(clock_l) {

	A = 0;
	B = 0;
	C = 0;
	D = 0;
	E = 0;
	F = 0;
	H = 0;
	L = 0;


	F = 0;
	SP = 0;
	PC = 0;
	IME = 0;
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
	IME = 0;

	F = 0;
	SP = 0;
	PC = 0;

}


void CPU::step(){

	//if (PC == 0x0100)
	//	tempfuse = true;

	//if (!debug && tempfuse) {
	//	print_format();
	//}

	// HALT logic
	if (halted) {
		if (get_highest_priority_interrupt_nonedit() == None) {
			advance_cycles(4);
			return;
		}
		halted = false;
	}

	// HALT bug logic
	if (halt_bug) {
		halt_bug = false;
		uint8_t opcode = address_space.read(PC);
		decode_execute_instruction(opcode);
		PC++;
		return;
	}

	// IME delayed activation
	if (triggerIME) {
		IME = true;
		triggerIME = false;
	}

	// Execute next instruction if not halted
	PC = decode_execute_instruction(PC);

	// Check and handle interrupts
	interrupt_handler();
}

void CPU::advance_cycles(uint8_t cycles){
	for (int i = 0; i < cycles; i++) {
		clock.tick();
	}
}


static bool is_16_bit(registerCalls a) {
	return (a == registerCalls::BC || a == registerCalls::DE || a == registerCalls::HL || a == registerCalls::AF || a == registerCalls::SP || a == registerCalls::PC);
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
			A = second;
			F = first;
			break;
		case registerCalls::BC:
			B = second;
			C = first;
			break;
		case registerCalls::DE:
			D = second;
			E = first;
			break;
		case registerCalls::HL:
			H = second;
			L = first;
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
			printf("reg not accepted\n");
			return 0;
			break;
	}
}

bool CPU::half_carry(uint8_t a, uint8_t b){
	return ((a & 0x0F) + (b & 0x0F) > 0x0F);
}

bool CPU::half_carry(uint16_t a , uint16_t b){
	return ((a & 0xFFF) + (b & 0xFFF) > 0xFFF);
}

void CPU::interrupt_handler(){
	if (IME) {
		uint8_t count = get_interrupt_count();
		if (count != 0) {
			IME = false;
			Interrupt intr = get_highest_priority_interrupt();
			// unset intr bit
			advance_cycles(8);
			pushn16(PC);
			advance_cycles(8);
			switch (intr) {
			case JOYPAD:
				PC = 0x60;
				break;
			case Serial:
				PC = 0x58;
				break;
			case Timer:
				PC = 0x50;
				break;
			case LCD:
				PC = 0x48;
				break;
			case VBLANK:
				PC = 0x40;
				break;
			}
			advance_cycles(4);
		}
	}
	else {
		//printf("IME was false, trigger IME is %d\n", triggerIME);
	}
	
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

void CPU::print_format() {
	printf("A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X (%02X %02X %02X %02X)\n",
			A,F,B,C,D,E,H,L, SP, PC, address_space.read(PC), address_space.read(PC+1), address_space.read(PC+2), address_space.read(PC+3)
		);
}

uint16_t CPU::decode_execute_instruction(uint16_t program_counter){
	uint8_t opcode = address_space.read(program_counter);
	//if (debug) {
	//	printf("pc is : 0x");
	//	std::cout << std::hex << static_cast<int>(PC) << std::endl;
	//}

	if (opcode == 0xCB) {
		return prefixedCodes(program_counter + 1);
	}
	else {
		return unprefixedCodes(program_counter);
	}
}

uint8_t CPU::get_bit_range(uint8_t value, uint8_t start, uint8_t end){
	uint8_t n = end - start + 1;
	uint8_t bitmask = (1 << n) - 1;
	return (value >> start) & bitmask;
}

bool CPU::check_cond(Cond c){
	switch (c){
	case nz:
		return !get_zero();
	case z:
		return get_zero();
	case nc:
		return !get_carry();
	case Cond::c:
		return get_carry();
	default:
		printf("what");
		return false;
		break;
	}
}




bool CPU::get_bit(uint8_t byte, uint8_t bit) {
	return (byte & (1 << bit)) >> bit;
}



void CPU::arithmetic_test() {
	pushn16(0x1234, true);
	popn16(true);
	print_registers();
}




bool CPU::get_carry() {
	return get_bit(F, 4);
}

void CPU::set_carry(bool a){
	F = set_bit(F, 4, a);
}

bool CPU::borrow(uint8_t before, uint8_t after){
	return before < after;
}

bool CPU::get_half_carry() {
	return get_bit(F, 5);
}

void CPU::set_half_carry(bool a){
	F = set_bit(F, 5, a);
}

bool CPU::half_borrow(uint8_t before, uint8_t after){
	return (before & 0xF) < (after & 0xF);
}



bool CPU::get_zero(){
	return get_bit(F, 7);
}

void CPU::set_zero(bool a) {
	F = set_bit(F, 7, a);
}

void CPU::set_n(bool a){
	F = set_bit(F, 6, a);
}

bool CPU::get_n(){
	return get_bit(F, 6);
}


void CPU::ccf() {
	if (debug)
		printf("ccf\n");
	set_n(false);
	set_half_carry(false);
	set_carry(!get_carry());
}

void CPU::scf(){
	if (debug)
		printf("scf\n");
	set_carry(true);
	set_half_carry(false);
	set_n(false);
}
