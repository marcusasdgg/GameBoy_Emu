#include "CPU.h"
const uint8_t LASTBITMASK = 0b10000000;
const uint8_t FIRSTBITMASK = 1;

void CPU::rlr8(registerCalls a) {
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(a);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	temp |= carry;
	set_carry(last);
	store_in_register(a,temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::rlhl(){
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	temp |= carry;
	set_carry(last);
	address_space.write(ad, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::rla(){
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(registerCalls::A);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	temp |= carry;
	set_carry(last);
	store_in_register(registerCalls::A, temp);
	set_zero(false);
	set_n(false);
	set_half_carry(false);
}

void CPU::rlcr8(registerCalls a){
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(a);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	temp |= last;
	set_carry(last);
	store_in_register(a, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::rlchl(){
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	temp |= last;
	set_carry(last);
	address_space.write(ad, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::rlca(){
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(registerCalls::A);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	temp |= last;
	set_carry(last);
	store_in_register(registerCalls::A, temp);
	set_zero(false);
	set_n(false);
	set_half_carry(false);
}

void CPU::rrr8(registerCalls a){
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(a);
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp |= (carry << 7);
	set_carry(first);
	store_in_register(a, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::rrhl(){
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp |= (carry << 7);
	set_carry(first);
	address_space.write(ad, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::rra(){
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(registerCalls::A);
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp |= (carry << 7);
	set_carry(first);
	store_in_register(registerCalls::A, temp);
	set_zero(false);
	set_n(false);
	set_half_carry(false);
}

void CPU::rrcr8(registerCalls a){
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(a);
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp |= (first << 7);
	set_carry(first);
	store_in_register(a, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::rrchl() {
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp |= (first << 7);
	set_carry(first);
	address_space.write(ad, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::rrca(){
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(registerCalls::A);
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp |= (first << 7);
	set_carry(first);
	store_in_register(registerCalls::A, temp);
	set_zero(false);
	set_n(false);
	set_half_carry(false);
}

void CPU::slar8(registerCalls a){
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(a);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	set_carry(last);
	store_in_register(a, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::slahl(){
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	set_carry(last);
	address_space.write(ad, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}


void CPU::srar8(registerCalls a) {
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(a);
	uint8_t last = temp & LASTBITMASK;
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp |= (last << 7);
	set_carry(first);
	store_in_register(a, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::srahl() {
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	uint8_t last = temp & LASTBITMASK;
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp |= (last << 7);
	set_carry(first);
	address_space.write(ad, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::srlr8(registerCalls){
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	set_carry(first);
	address_space.write(ad, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}

void CPU::srlhl(){
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	set_carry(first);
	address_space.write(ad, temp);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
}


void CPU::swapr8(registerCalls a){
	uint8_t temp = retrieve_register_8(a);
	temp = (temp >> 4) | (temp << 4);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
	store_in_register(a, temp);
}

void CPU::swaphl(){
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	temp = (temp >> 4) | (temp << 4);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
	address_space.write(ad, temp);
}
















