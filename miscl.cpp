#include "CPU.h"

void CPU::daa() {
	bool n = get_n();
	bool h = get_half_carry();
	bool c = get_carry();
	uint8_t adjustment = 0;
	if (debug)
		printf("daa\n");

	if (n) {
		if (h) adjustment += 0x6;
		if (c) adjustment += 0x60;
		A -= adjustment;
	}
	else {
		if (h || (A & 0xF) > 0x9) adjustment += 0x6;
		if (c || A > 0x99) {
			adjustment += 0x60;
			set_carry(true);
		}
		A += adjustment;
	}
	set_half_carry(false);
	set_zero(A == 0);
}

void CPU::nop(){
	if (debug)
		printf("nop\n");
}

void CPU::stop(){
	if (debug)
		printf("stop\n");
	if (IME) {
		//stop the clock
	}
	else {
		//stop the clock until interrupt
	}
}

uint8_t CPU::get_interrupt_count() {
	uint8_t ie = address_space.read(IE);
	uint8_t iff = address_space.read(IF);
	int count = 0;

	for (int i = 0; i < 5; i++) {
		if (get_bit(iff, i) && get_bit(ie, i)) {
			count++;
		}
	}

	return count;
}


// also unsets if bit
Interrupt CPU::get_highest_priority_interrupt(){
	uint8_t ie = address_space.read(IE);
	uint8_t iff = address_space.read(IF);
	if (get_bit(iff, 0) && get_bit(ie, 0)) {
		iff = iff & 0b1111110;
		address_space.write(IF, iff);
		return Interrupt::VBLANK;
	}
	else if (get_bit(iff, 1) && get_bit(ie, 1)) {
		iff = iff & 0b1111101;
		address_space.write(IF, iff);
		return Interrupt::LCD;
	}
	else if (get_bit(iff, 2) && get_bit(ie, 2)) {
		printf("timer interrupt received\n");
		iff = iff & 0b1111011;
		address_space.write(IF, iff);
		return Interrupt::Timer;
	}
	else if (get_bit(iff, 3) && get_bit(ie, 3)) {
		iff = iff & 0b1110111;
		address_space.write(IF, iff);
		return Interrupt::Serial;
	}
	else if (get_bit(iff, 4) && get_bit(ie, 4)) {
		iff = iff & 0b1101111;
		address_space.write(IF, iff);
		return Interrupt::JOYPAD;
	}
	else {
		printf("no highest priority\n");
		return Interrupt::Timer;
	}
}
