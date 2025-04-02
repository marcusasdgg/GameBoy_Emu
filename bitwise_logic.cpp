#include "CPU.h"

void CPU::andar8(registerCalls a) {
	uint8_t temp = retrieve_register_8(a);
	A &= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(true);
	set_carry(false);
}

void CPU::andahl(){
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	A &= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(true);
	set_carry(false);
}

void CPU::andan8(uint8_t temp){
	A &= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(true);
	set_carry(false);
}

void CPU::orar8(registerCalls a){
	uint8_t temp = retrieve_register_8(a);
	A |= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::orahl(){
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	A |= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::oran8(uint8_t temp){
	A |= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(true);
	set_carry(false);
}

void CPU::xorar8(registerCalls a){
	uint8_t temp = retrieve_register_8(a);
	A ^= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::xorahl(){
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	A ^= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
}

void CPU::xoran8(uint8_t temp){
	A ^= temp;
	if (A == 0) set_zero(true);
	set_n(false);
	set_half_carry(true);
	set_carry(false);
}
















