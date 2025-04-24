#include "CPU.h"

void CPU::andar8(registerCalls a) {
	if (debug)
		printf("and r8\n");
	uint8_t temp = retrieve_register_8(a);
	A &= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(true);
	set_carry(false);
}

void CPU::andahl(){
	if (debug)
		printf("and a hl\n");
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	A &= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(true);
	set_carry(false);
}

void CPU::andan8(uint8_t temp){
	if (debug)
		printf("and a n8\n");
	A &= temp;
	set_zero(A == 0);
	set_n(false);
	set_half_carry(true);
	set_carry(false);
}

void CPU::orar8(registerCalls a){
	if (debug)
		printf("or a r8\n");
	uint8_t temp = retrieve_register_8(a);
	A |= temp;
	set_zero(A == 0);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::orahl(){
	if (debug)
		printf("or a hl\n");
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	A |= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::oran8(uint8_t temp){
	if (debug)
		printf("or a n8\n");
	A |= temp;
	set_zero(A == 0);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::xorar8(registerCalls a){
	if (debug)
		printf("xor a r8\n");
	uint8_t temp = retrieve_register_8(a);
	A ^= temp;

	if (A == 0) set_zero(true);
	else set_zero(false);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::xorahl(){
	if (debug)
		printf("xor a hl\n");
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	A ^= temp;
	set_zero(A == 0);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::xoran8(uint8_t temp){
	if (debug)
		printf("xor a n8\n");
	A ^= temp;
	set_zero(A == 0);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}
















