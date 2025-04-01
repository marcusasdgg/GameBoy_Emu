#include "CPU.h"

void CPU::adcar8(registerCalls a) {
	uint8_t temp = retrieve_register_8(a);
	bool carry = get_carry();
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp + carry;
	if (A == 0) set_zero(true);
}

void CPU::adcahl() {
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
	if (A == 0) set_zero(true);
}

void CPU::adcan8(uint8_t temp) {
	bool carry = get_carry();
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp + carry;
	if (A == 0) set_zero(true);
}

void CPU::addar8(registerCalls a) {
	uint8_t temp = retrieve_register_8(a);
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp;
	if (A == 0) set_zero(true);
}

void CPU::addahl() {
	uint16_t add = retrieve_register_8(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp;
	if (A == 0) set_zero(true);
}

void CPU::addan8(uint8_t temp) {
	if (full_carry(temp + get_carry(), A)) {
		set_carry(true);
	}
	if (half_carry(temp + get_carry(), A)) {
		set_half_carry(true);
	}
	A += temp;
	if (A == 0) set_zero(true);
}

void CPU::addhlr16(registerCalls val) {
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

void CPU::addhlsp() {
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

void CPU::addspe8(int8_t e8) {
	int16_t s = e8 + SP;
	SP = s;
	set_zero(false);
	set_n(false);
	if (((SP & 0xF) + (e8 & 0xF)) > 0xF) F |= 0b00001000;

	//checking full carry
	if (((SP & 0xFF) + (e8 & 0xFF)) > 0xFF) F |= 0b00010000;

}

void CPU::subar8(registerCalls a) {
	uint8_t temp = retrieve_register_8(a);
	uint8_t before = A;

	A -= temp;

	set_n(true);
	if (half_borrow(before, A)) set_half_carry(true);
	if (borrow(before, A)) set_carry(true);

	if (A == 0) set_zero(true);
}

void CPU::subahl() {
	uint16_t add = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	uint8_t before = A;
	A -= temp;
	if (borrow(before, A)) set_carry(true);
	if (half_borrow(before, A)) set_half_carry(true);
	set_n(true);
	if (A == 0) set_zero(true);
}

void CPU::suban8(uint8_t temp) {
	uint8_t before = A;
	A -= temp;
	if (borrow(before, A)) set_carry(true);
	if (half_borrow(before, A)) set_half_carry(true);
	set_n(true);
	if (A == 0) set_zero(true);
}

void CPU::sbcar8(registerCalls a) {
	uint8_t temp = retrieve_register_8(a);
	uint8_t before = A;
	bool cond = get_carry();

	A -= temp;
	A -= cond;

	set_n(true);
	if (half_borrow(before, A)) set_half_carry(true);
	if (borrow(before, A)) set_carry(true);

	if (A == 0) set_zero(true);
}

void CPU::sbcahl() {
	uint16_t add = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(add);
	bool cond = get_carry();
	uint8_t before = A;
	A -= temp;
	A -= cond;
	if (borrow(before, A)) set_carry(true);
	if (half_borrow(before, A)) set_half_carry(true);
	set_n(true);
	if (A == 0) set_zero(true);
}

void CPU::sbcn8(uint8_t temp) {
	uint8_t before = A;
	bool cond = get_carry();
	A -= temp;
	A -= cond;
	if (borrow(before, A)) set_carry(true);
	if (half_borrow(before, A)) set_half_carry(true);
	set_n(true);
	if (A == 0) set_zero(true);
}

void CPU::incr8(registerCalls a) {
	uint8_t temp = retrieve_register_8(a);
	store_in_register(a, (uint8_t)(temp + 1));
	if (half_carry(temp, 1)) {
		set_half_carry(true);
	}
	temp = retrieve_register_8(a);
	set_n(false);
	if (temp == 0) {
		set_zero(true);
	}

}

void CPU::inchl() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	address_space.write(ad, temp + 1);
	if (half_carry(temp, 1)) set_half_carry(true);
	if (address_space.read(ad) == 0) set_zero(true);
	set_n(false);
}

void CPU::incr16(registerCalls a) {
	uint16_t temp = retrieve_register_16(a) + 1;
	store_in_register(a, temp);
}

void CPU::incsp() {
	SP += 1;
}

void CPU::decr8(registerCalls a) {
	uint8_t temp = retrieve_register_8(a);
	store_in_register(a, (uint8_t)(temp - 1));
	if ((temp & 0xF) == 0) {
		set_half_carry(true);
	}
	temp = retrieve_register_8(a);
	set_n(false);
	if (temp == 0) {
		set_zero(true);
	}
	set_n(true);
}

void CPU::dechl() {
	uint16_t ad = retrieve_register_16(registerCalls::HL);
	uint8_t temp = address_space.read(ad);
	if ((temp & 0xF) == 0) {
		set_half_carry(true);
	}
	address_space.write(ad, temp - 1);
	if (address_space.read(ad) == 0) set_zero(true);
	set_n(true);
}

void CPU::decr16(registerCalls a) {
	uint8_t temp = retrieve_register_8(a) - 1;
	store_in_register(a, temp);
}

void CPU::decsp() {
	SP -= 1;
}
