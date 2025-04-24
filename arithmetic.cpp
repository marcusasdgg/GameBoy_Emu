#include "CPU.h"
#include "helpers.h"

void CPU::adcar8(registerCalls a) {
	if (debug)
		printf("adc a %s\n", to_string(a));
	uint8_t temp = retrieve_register_8(a);
	uint8_t carry = get_carry();
	uint8_t result = A + temp + carry;
	uint16_t temp_wide = (uint16_t)temp + (uint16_t)carry;
	set_n(false);
	set_zero(result == 0);
	set_half_carry(((A & 0xF) + (temp & 0xF) + carry) > 0xF);
	set_carry((uint16_t)A + temp_wide > 0xFF);
	A = result;
}

void CPU::adcahl() {
	if (debug)
		printf("adc a hl\n");
	uint16_t add = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	uint8_t carry = get_carry();
	uint8_t result = A + temp + carry;
	uint16_t temp_wide = (uint16_t)temp + (uint16_t)carry;
	set_n(false);
	set_zero(result == 0);
	set_half_carry(((A & 0xF) + (temp & 0xF) + carry) > 0xF);
	set_carry((uint16_t)A + temp_wide > 0xFF);
	A = result;
}

void CPU::adcan8(uint8_t temp) {
	if (debug)
		printf("adc a %s\n", to_string(temp));
	uint8_t carry = get_carry();
	uint8_t result = A + temp + carry;
	uint16_t temp_wide = (uint16_t)temp + (uint16_t)carry;
	set_n(false);
	set_zero(result == 0);
	set_half_carry(((A & 0xF) + (temp & 0xF) + carry) > 0xF);
	set_carry((uint16_t)A + temp_wide > 0xFF);
	A = result;
}

void CPU::addar8(registerCalls a) {
	if (debug)
		printf("add a %s\n", to_string(a));
	uint8_t temp = retrieve_register_8(a);
	set_n(false);
	set_half_carry(half_carry(A, temp));
	set_carry(full_carry(A, temp));
	A += temp;
	set_zero(A == 0);
}

void CPU::addahl() {
	if (debug)
		printf("add a hl\n");
	uint16_t add = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	set_carry(full_carry(A, temp));
	set_half_carry(half_carry(A, temp));
	A += temp;
	set_zero(A == 0);
	set_n(false);
}

void CPU::addan8(uint8_t temp) {
	if (debug)
		printf("add a %s\n", to_string(temp));
	set_half_carry(half_carry(A, temp));
	set_carry(full_carry(A, temp));
	set_n(false);
	A += temp;
	set_zero(A == 0);
}

void CPU::addhlr16(registerCalls val) {
	if (debug)
		printf("add hl %s\n", to_string(val));
	uint16_t hl = retrieve_register_16(registerCalls::HL);
	uint16_t temp = retrieve_register_16(val);
	set_n(false);
	set_half_carry(half_carry(hl, temp));
	set_carry(full_carry(hl, temp));
	store_in_register(registerCalls::HL, (uint16_t)(hl + temp));
}

void CPU::addhlsp() {
	if (debug)
		printf("add hl sp\n");
	uint16_t temp = SP;
	uint16_t hl = retrieve_register_16(registerCalls::HL);
	store_in_register(registerCalls::HL, (uint16_t)(hl + temp));
	set_n(false);
	set_carry(full_carry(hl, temp));
	set_half_carry(half_carry(hl, temp));
}

void CPU::addspe8(int8_t e8) {
	if (debug)
		printf("add sp %s\n", to_string((uint8_t)e8));
	int16_t final = SP + e8;
	set_zero(false);
	set_n(false);
	set_carry(full_carry((uint8_t)SP, e8));
	set_half_carry(half_carry((uint8_t)SP, e8));
	SP = final;
}

void CPU::subar8(registerCalls a) {
	if (debug)
		printf("sub a %s\n", to_string(a));
	uint8_t temp = retrieve_register_8(a);
	set_n(true);
	set_half_carry(half_borrow(A, temp));
	set_carry(borrow(A, temp));
	A -= temp;
	set_zero(A == 0);
}

void CPU::subahl() {
	if (debug)
		printf("sub a hl\n");
	uint16_t add = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	set_n(true);
	set_half_carry(half_borrow(A, temp));
	set_carry(borrow(A, temp));
	A -= temp;
	set_zero(A == 0);
}

void CPU::suban8(uint8_t temp) {
	if (debug)
		printf("sub a %s\n", to_string(temp));
	set_n(true);
	set_half_carry(half_borrow(A, temp));
	set_carry(borrow(A, temp));
	A -= temp;
	set_zero(A == 0);
}

void CPU::sbcar8(registerCalls a) {
	if (debug)
		printf("sbc a %s\n", to_string(a));
	uint8_t temp = retrieve_register_8(a);
	uint8_t carry = get_carry();
	uint16_t full_sub = (uint16_t)A - (uint16_t)temp - carry;
	uint8_t result = full_sub & 0xFF;

	set_n(true);
	set_zero(result == 0);
	set_half_carry(((A & 0xF) - (temp & 0xF) - carry) < 0);
	set_carry((A < temp + carry));
	A = result;
}

void CPU::sbcahl() {
	if (debug)
		printf("sbc a hl\n");
	uint16_t add = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	uint8_t carry = get_carry();
	uint16_t full_sub = (uint16_t)A - (uint16_t)temp - carry;
	uint8_t result = full_sub & 0xFF;

	set_n(true);
	set_zero(result == 0);
	set_half_carry(((A & 0xF) - (temp & 0xF) - carry) < 0);
	set_carry((A < temp + carry));
	A = result;
}

void CPU::sbcn8(uint8_t temp) {
	if (debug)
		printf("sbc a %s\n", to_string(temp));

	uint8_t carry = get_carry();
	uint16_t full_sub = (uint16_t)A - (uint16_t)temp - carry;
	uint8_t result = full_sub & 0xFF;

	set_n(true);
	set_zero(result == 0);
	set_half_carry(((A & 0xF) - (temp & 0xF) - carry) < 0);
	set_carry((A < temp + carry));
	A = result;
}

void CPU::incr8(registerCalls a) {
	if (debug)
		printf("inc %s\n", to_string(a));
	uint8_t temp = retrieve_register_8(a);
	
	set_zero((uint8_t)(temp + 1) == 0);
	set_half_carry(half_carry(temp, 1));
	store_in_register(a, (uint8_t)(temp + 1));
	set_n(false);

}

void CPU::inchl() {
	if (debug)
		printf("inc hl\n");
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	set_n(false);
	set_zero(temp + 1 == 0);
	set_half_carry(half_carry(temp, 1));
	address_space.write(ad, temp + 1);
}


void CPU::incr16(registerCalls a, bool print) {
	if (debug && print)
		printf("inc %s\n", to_string(a));
	uint16_t temp = retrieve_register_16(a) + 1;
	store_in_register(a, temp);
}

void CPU::incsp() {
	if (debug)
		printf("inc sp\n");
	SP += 1;
}

void CPU::decr8(registerCalls a) {

	uint8_t temp = retrieve_register_8(a);
	if (debug)
	{
		printf("dec %s (%s -> ", to_string(a), to_string(uint8_t(temp)));
		printf("%s)\n", to_string(uint8_t(temp - 1)));
	}
	store_in_register(a, (uint8_t)(temp - 1));
	set_n(true);
	set_half_carry(half_borrow(temp, 1));
	set_zero(temp - 1 == 0);
}

void CPU::dechl() {
	if (debug)
		printf("dec hl\n");
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	set_n(true);
	set_zero(temp - 1 == 0);
	set_half_carry(half_borrow(temp, 1));
	address_space.write(ad, temp - 1);
}

void CPU::decr16(registerCalls a, bool print) {
	if (debug && print)
		printf("dec %s\n", to_string(a));
	uint16_t temp = retrieve_register_16(a) - 1;
	store_in_register(a, temp);
}

void CPU::decsp() {
	if (debug)
		printf("dec sp\n");
	SP -= 1;
}


void CPU::cpan8(uint8_t val){
	if (debug)
		printf("cp a %s\n",to_string(val));
	uint8_t final = A - val;
	set_zero(final == 0);
	set_n(true);
	set_half_carry(half_borrow(A, val));
	set_carry(borrow(A, val));
}

void CPU::cpl() {
	if (debug)
		printf("cpl\n");
	set_n(true);
	set_half_carry(true);
	A = ~ A;
}

void CPU::cpar8(registerCalls a) {
	if (debug)
		printf("cp a %s\n", to_string(a));
	uint8_t val = retrieve_register_8(a);
	uint8_t final = A - val;
	set_half_carry(half_borrow(A, val));
	set_n(true);
	set_carry(borrow(A, val));
	set_zero(final == 0);
}

void CPU::cpahl() {
	if (debug)
		printf("cp a hl\n");
	uint8_t val = address_space.read(retrieve_register_16(HL));

	set_half_carry(half_borrow(A, val));
	set_carry(val > A);
	uint8_t final = A - val;
	set_zero(final == 0);
	set_n(true);
	
}