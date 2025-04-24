#include "CPU.h"
#include "helpers.h"
const uint8_t LASTBITMASK = 0b10000000;
const uint8_t FIRSTBITMASK = 1;

std::string print_binary(uint8_t n) {
	unsigned int mask = 1 << (sizeof(n) * 8 - 1);  // Mask to start from the most significant bit
	std::string s;
	// Loop through all bits
	for (int i = 0; i < sizeof(n) * 8; i++) {
		// Print the current bit
		s += (n & mask) ? '1' : '0';

		// Shift the mask to the right
		mask >>= 1;

		// Print space every 8 bits (optional for better readability)
		if ((i + 1) % 8 == 0) {
			s += ' ';
		}
	}
	return s;
}

std::string print_binary(uint16_t n) {
	unsigned int mask = 1 << (sizeof(n) * 8 - 1);  // Mask to start from the most significant bit
	std::string s;
	// Loop through all bits
	for (int i = 0; i < sizeof(n) * 8; i++) {
		// Print the current bit
		s += (n & mask) ? '1' : '0';

		// Shift the mask to the right
		mask >>= 1;

		// Print space every 8 bits (optional for better readability)
		if ((i + 1) % 8 == 0) {
			s += ' ';
		}
	}
	return s;
}


void CPU::rlr8(registerCalls a) {
	uint8_t og = retrieve_register_8(a);
	uint8_t carry = get_carry();
	uint8_t temp = retrieve_register_8(a);
	uint8_t last = LASTBITMASK & temp;
	set_carry(last);
	temp = temp << 1;
	temp |= set_bit(temp, 0, carry);
	
	store_in_register(a,temp);

	if (temp == 0) set_zero(true);
	else set_zero(false);

	set_n(false);
	set_half_carry(false);
	if (debug)
		printf("rl %s (%s -> %s) carry: ( %d -> %d )\n", to_string(a), print_binary(og).c_str(), print_binary(temp).c_str(), carry, get_carry());

}

void CPU::rlhl(){
	if (debug)
		printf("rl hl\n");
	uint16_t ad = retrieve_register_16(HL);
	bool carry = get_carry();
	uint8_t temp = address_space.read(ad);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	temp = set_bit(temp, 0, carry);
	set_carry(last);
	address_space.write(ad, temp);
	set_zero(temp == 0);
	set_n(false);
	set_half_carry(false);
}

void CPU::rla(){
	uint8_t og = A;
	bool carry = get_carry();
	uint8_t temp = A;
	uint8_t last = LASTBITMASK & temp;
	temp = temp << 1;
	temp = set_bit(temp, 0,carry);
	set_carry(last);
	A = temp;
	set_zero(false);
	set_n(false);
	set_half_carry(false);
	if (debug)
		printf("rla (%s -> %s) carry: (%d -> %d)\n",print_binary(og).c_str(), print_binary(temp).c_str(), carry,get_carry());
}

void CPU::rlcr8(registerCalls a){
	if (debug)
		printf("rlc r8\n");
	bool carry = get_carry();
	uint8_t temp = retrieve_register_8(a);
	bool last = LASTBITMASK & temp;
	temp = temp << 1;
	temp |= (uint8_t) last;
	set_carry(last);
	store_in_register(a, temp);
	set_zero(temp == 0);
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
	temp |= (uint8_t) last;
	set_carry(last);
	address_space.write(ad, temp);
	set_zero(temp == 0);
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
	temp |= (uint8_t) last;
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
	temp = set_bit(temp, 7, carry);
	set_carry(first);
	store_in_register(a, temp);
	set_zero(temp == 0);
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
	temp = set_bit(temp, 7, carry);
	set_carry(first);
	address_space.write(ad, temp);
	set_zero(temp == 0);
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
	set_zero(temp == 0);
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
	set_zero(temp == 0);
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
	set_zero(temp == 0);
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
	set_zero(temp == 0);
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
	temp = set_bit(temp, 7, last);
	set_carry(first);
	store_in_register(a, temp);
	set_zero(temp == 0);
	set_n(false);
	set_half_carry(false);
}

void CPU::srahl() {
	if (debug)
		printf("sra hl\n");
	uint16_t ad = retrieve_register_16(HL);
	uint8_t temp = address_space.read(ad);
	bool carry = get_carry();
	
	uint8_t last = temp & LASTBITMASK;
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	temp = set_bit(temp, 7, last);
	set_carry(first);
	address_space.write(ad, temp);
	set_zero(temp == 0);
	set_n(false);
	set_half_carry(false);
}

void CPU::srlr8(registerCalls a){
	if (debug)
		printf("srl r8\n");

	uint8_t temp = retrieve_register_8(a);
	bool carry = get_carry();
	uint8_t first = temp & FIRSTBITMASK;
	temp = temp >> 1;
	set_carry(first);
	store_in_register(a, temp);
	set_zero(temp == 0);
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
	set_zero(temp == 0);
	set_n(false);
	set_half_carry(false);
}


void CPU::swapr8(registerCalls a){
	if (debug)
		printf("swap r8\n");
	uint8_t temp = retrieve_register_8(a);
	temp = (temp >> 4) | (temp << 4);
	set_zero(temp == 0);
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
	set_zero(temp == 0);
	set_n(false);
	set_half_carry(false);
	set_carry(false);
	address_space.write(ad, temp);
}
















