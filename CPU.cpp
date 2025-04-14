#include "CPU.h"
#include "Clock.h"
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
	SP = 0xFFFE;
	PC = 0;

}

//do in its own thread whoopsies
void CPU::execute_loop(uint16_t start_ptr){
	while (isRunning) {
		try
		{
			PC = decode_execute_instruction(PC);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
		catch (...) {
			std::cout << "Unknown exception in CPU thread" << std::endl;
		}
	}
}
void CPU::execute(uint16_t start_ptr) {
	isRunning = true;
	/*cpu_thread = std::thread([this, start_ptr]() { execute_loop(start_ptr); });*/
	cpu_thread = std::thread([this, start_ptr]() {
		//printf("executing thread loop");
			execute_loop(start_ptr);
		});
}

void CPU::stop_execute(){
	printf("stopping cpu\n");
	isRunning = false;
	if (cpu_thread.joinable()) {
		try
		{
			cpu_thread.join();
		}
		catch (const std::exception& e)
		{
			std::cout << "execption occured" << e.what();
		}
		catch (...) {
			std::cout << "Unknown exception in CPU thread" << std::endl;
		}
	}
	else {
		printf("threa is not joinable");
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

uint16_t CPU::decode_execute_instruction(uint16_t program_counter){
	uint8_t opcode = address_space.read(program_counter);
	printf("opcode is "); print_binary(opcode);
	uint8_t block = opcode >> 6;
	switch (block) {
		case 0:
			printf("block 0\n\n");
			return handle_block_0(program_counter);
		case 1:
			printf("block 1\n\n");
			return handle_block_1(program_counter);
		case 2:
			printf("block 2\n\n");
			return handle_block_2(program_counter);
		case 3:
			printf("block 3\n\n");
			return handle_block_3(program_counter);
		default:
			//restart the gb
			return -1;
	}
	return 0;
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

void CPU::block_cycle_n(uint8_t n) {
	for (int i = 0; i < n; i++) {
		block_cycle_i();
	}
}

void CPU::block_cycle_i(){
	uint64_t cycle_count = clock.get_cycle() /4 ;

	while (cycle_count == clock.get_cycle() / 4) {
		std::this_thread::yield();
	}
}

bool CPU::get_bit(uint8_t byte, uint8_t bit) {
	return (byte & (1 << bit)) >> bit;
}



void CPU::arithmetic_test() {
	ldr8n8(registerCalls::A, 8);
	ldr8r8(registerCalls::B, registerCalls::A);
	addar8(registerCalls::B);
	decr8(registerCalls::A);
	incr8(registerCalls::A);
	subar8(registerCalls::B);
	print_registers();
	ldr16n16(registerCalls::HL, 100);
	ldr16a(registerCalls::HL);
	decr8(registerCalls::A);
	decr8(registerCalls::A);
	decr8(registerCalls::A);
	dechl();
	print_registers();
	ldan16(100);
	print_registers();
	dechl();
	ldan16(100);
	print_registers();
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

bool CPU::borrow(uint8_t before, uint8_t after){
	return before < after;
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

bool CPU::half_borrow(uint8_t before, uint8_t after){
	return (before & 0xF) < (after & 0xF);
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


void CPU::ccf() {
	set_n(false);
	set_half_carry(false);
	set_carry(!get_carry());
}

void CPU::scf(){
	set_carry(true);
	set_half_carry(false);
	set_n(false);
}
