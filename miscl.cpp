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
	if (A == 0) set_zero(true);
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
