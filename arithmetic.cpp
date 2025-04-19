#include "CPU.h"


void CPU::adcar8(registerCalls a) {
	if (debug)
		printf("adc a r8\n");
	uint8_t temp = retrieve_register_8(a);
	bool carry = get_carry();
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	A += temp + carry;
	if (A == 0) set_zero(true);
	else  set_zero(false);
}

void CPU::adcahl() {
	if (debug)
		printf("adc a hl\n");
	uint16_t add = retrieve_register_8(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	bool carry = get_carry();
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	A += temp + carry;
	if (A == 0) set_zero(true);
	else  set_zero(false);
}

void CPU::adcan8(uint8_t temp) {
	if (debug)
		printf("adc a n8\n");
	bool carry = get_carry();
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	A += temp + carry;
	if (A == 0) set_zero(true);
	else  set_zero(false);
}

void CPU::addar8(registerCalls a) {
	if (debug)
		printf("add a r8\n");
	uint8_t temp = retrieve_register_8(a);
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	A += temp;
	if (A == 0) set_zero(true);
	else  set_zero(false);
}

void CPU::addahl() {
	if (debug)
		printf("add a hl\n");
	uint16_t add = retrieve_register_8(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	A += temp;
	if (A == 0) set_zero(true);
	else  set_zero(false);
}

void CPU::addan8(uint8_t temp) {
	if (debug)
		printf("add a n8\n");
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	} else {
		set_carry(false);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	A += temp;
	if (A == 0) set_zero(true);
	else set_zero(false);
}

void CPU::addhlr16(registerCalls val) {
	if (debug)
		printf("add hl r16\n");
	uint16_t hl = retrieve_register_16(registerCalls::HL);
	uint16_t temp = retrieve_register_16(val);
	store_in_register(registerCalls::HL, (uint16_t)(hl + temp));
	if (half_carry(hl, temp)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}

	if (full_carry(hl, temp)) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
	set_zero(false);
}

void CPU::addhlsp() {
	if (debug)
		printf("add hl sp\n");
	uint16_t temp = SP;
	uint16_t hl = retrieve_register_16(registerCalls::HL);
	store_in_register(registerCalls::HL, (uint16_t)(hl + temp));
	if (half_carry(hl, temp)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}

	if (full_carry(hl, temp)) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
	set_zero(false);
}

void CPU::addspe8(int8_t e8) {
	if (debug)
		printf("add sp e8\n");
	int16_t s = e8 + SP;
	SP = s;
	set_zero(false);
	set_n(false);
	if (((SP & 0xF) + (e8 & 0xF)) > 0xF) set_carry(true);
	else set_carry(false);

	//checking full carry
	if (((SP & 0xFF) + (e8 & 0xFF)) > 0xFF) set_half_carry(true);
	else set_half_carry(false);

}

void CPU::subar8(registerCalls a) {
	if (debug)
		printf("sub a r8\n");
	uint8_t temp = retrieve_register_8(a);
	uint8_t before = A;

	A -= temp;

	set_n(true);
	if (half_borrow(before, A)) set_half_carry(true);
	else set_half_carry(false);
	if (borrow(before, A)) set_carry(true);
	else set_carry(false);

	if (A == 0) set_zero(true);
	else set_zero(false);
}

void CPU::subahl() {
	if (debug)
		printf("sub a hl\n");
	uint16_t add = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	uint8_t before = A;
	A -= temp;
	if (borrow(before, A)) set_carry(true);
	else set_carry(false);
	if (half_borrow(before, A)) set_half_carry(true);
	else set_half_carry(false);
	set_n(true);
	if (A == 0) set_zero(true);
	else set_zero(false);
}

void CPU::suban8(uint8_t temp) {
	if (debug)
		printf("sub a n8\n");
	uint8_t before = A;
	A -= temp;
	if (borrow(before, A)) set_carry(true);
	else set_carry(false);
	if (half_borrow(before, A)) set_half_carry(true);
	else set_half_carry(false);
	set_n(true);
	if (A == 0) set_zero(true);
	else set_zero(false);
}

void CPU::sbcar8(registerCalls a) {
	if (debug)
		printf("sbc a r8\n");
	uint8_t temp = retrieve_register_8(a);
	uint8_t before = A;
	bool cond = get_carry();

	A -= temp;
	A -= cond;

	set_n(true);
	if (half_borrow(before, A)) set_half_carry(true);
	else set_half_carry(false);
	if (borrow(before, A)) set_carry(true);
	else set_carry(false);

	if (A == 0) set_zero(true);
	else set_zero(false);
}

void CPU::sbcahl() {
	if (debug)
		printf("sbc a hl\n");
	uint16_t add = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	bool cond = get_carry();
	uint8_t before = A;
	A -= temp;
	A -= cond;
	if (borrow(before, A)) set_carry(true);
	else set_carry(false);
	if (half_borrow(before, A)) set_half_carry(true);
	else set_half_carry(false);
	set_n(true);
	if (A == 0) set_zero(true);
	else set_zero(false);
}

void CPU::sbcn8(uint8_t temp) {
	if (debug)
		printf("sbc n8\n");
	uint8_t before = A;
	bool cond = get_carry();
	A -= temp;
	A -= cond;
	if (borrow(before, A)) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
	if (half_borrow(before, A)) set_half_carry(true);
	else set_half_carry(false);
	set_n(true);
	if (A == 0) {
		set_zero(true);
	}
	else {
		set_zero(false);
	}
}

void CPU::incr8(registerCalls a) {
	if (debug)
		printf("inc r8\n");
	uint8_t temp = retrieve_register_8(a);
	store_in_register(a, (uint8_t)(temp + 1));
	if (half_carry(temp, 1)) {
		set_half_carry(true);
	} else{
		set_half_carry(false);
	}
	temp = retrieve_register_8(a);
	set_n(false);
	if (temp == 0) {
		set_zero(true);
	}
	else {
		set_zero(false);
	}

}

void CPU::inchl() {
	if (debug)
		printf("inc hl\n");
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	address_space.write(ad, temp + 1);
	if (half_carry(temp, 1)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	if (address_space.read(ad) == 0) {
		set_zero(true);
	}
	else {
		set_zero(false);
	}
	set_n(false);
}

void CPU::incr16(registerCalls a) {
	if (debug)
		printf("inc r16\n");
	uint16_t temp = retrieve_register_16(a) + 1;
	store_in_register(a, temp);
}

void CPU::incsp() {
	if (debug)
		printf("inc sp\n");
	SP += 1;
}

void CPU::decr8(registerCalls a) {
	if (debug)
		printf("dec r8\n");
	uint8_t temp = retrieve_register_8(a);
	store_in_register(a, (uint8_t)(temp - 1));
	if ((temp & 0xF) == 0) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	temp = retrieve_register_8(a);
	set_n(false);
	if (temp == 0) {
		set_zero(true);
	}
	else {
		set_zero(false);
	}
	set_n(true);
}

void CPU::dechl() {
	if (debug)
		printf("dec hl\n");
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	if ((temp & 0xF) == 0) {
		set_half_carry(true);
	}
	address_space.write(ad, temp - 1);
	if (address_space.read(ad) == 0) set_zero(true);
	set_n(true);
}

void CPU::decr16(registerCalls a, bool print) {
	if (debug && print)
		printf("dec r16\n");
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
		printf("cp a n8\n");
	if (half_borrow(A, val)) {
		set_half_carry(true);
	}
	uint8_t final = A - val;
	if (final == 0) {
		set_zero(true);
	}
	else {
		set_zero(false);
	}
	set_n(true);
	if (val > A) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
}

void CPU::cpl() {
	if (debug)
		printf("\n");
	set_n(true);
	set_half_carry(true);
	A = ~ A;
}

void CPU::cpar8(registerCalls a) {
	if (debug)
		printf("cp a r8\n");
	uint8_t val = retrieve_register_8(a);
	if (half_borrow(A, val)) {
		set_half_carry(true);
	}
	uint8_t final = A - val;
	if (final == 0) {
		set_zero(true);
	}
	else {
		set_zero(false);
	}
	set_n(true);
	if (val > A) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
}

void CPU::cpahl() {
	if (debug)
		printf("cp a hl\n");
	uint8_t val = address_space.read(retrieve_register_16(HL));
	if (half_borrow(A, val)) {
		set_half_carry(true);
	}
	else {
		set_half_carry(false);
	}
	uint8_t final = A - val;
	if (final == 0) {
		set_zero(true);
	}
	else {
		set_zero(false);
	}
	set_n(true);
	if (val > A) {
		set_carry(true);
	}
	else {
		set_carry(false);
	}
}