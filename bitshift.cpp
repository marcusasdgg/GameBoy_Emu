#include "CPU.h"
const uint8_t LASTBITMASK = 0b10000000;
const uint8_t FIRSTBITMASK = 1;


void CPU::rlr8(registerCalls a) {
	if (debug)
		printf("rl r8\n");
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
	if (debug)
		printf("rl hl\n");
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
	if (debug)
		printf("rl a\n");
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
	if (debug)
		printf("rlc r8\n");
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
	if (debug)
		printf("rlc hl\n");
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
	if (debug)
		printf("rlc a\n");
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
	if (debug)
		printf("rr r8\n");
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
	if (debug)
		printf("rr hl\n");
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
	if (debug)
		printf("rr a\n");
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
	if (debug)
		printf("rrc r8\n");
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
	if (debug)
		printf("rrc hl\n");
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
	if (debug)
		printf("rrc a\n");
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
	if (debug)
		printf("sla r8\n");
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
	if (debug)
		printf("sla hl\n");
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
	if (debug)
		printf("sra r8\n");
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
	if (debug)
		printf("sra hl\n");
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
	if (debug)
		printf("srl r8\n");
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
	if (debug)
		printf("srl hl\n");
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
	if (debug)
		printf("swap r8\n");
	uint8_t temp = retrieve_register_8(a);
	temp = (temp >> 4) | (temp << 4);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
	store_in_register(a, temp);
}

void CPU::swaphl(){
	if (debug)
		printf("swap hl\n");
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	temp = (temp >> 4) | (temp << 4);
	if (temp == 0) set_zero(true);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
	address_space.write(ad, temp);
}
















