#include "CPU.h"

void CPU::di() {

	if (debug)
		printf("di\n");

	IME = false;
	triggerIME = false;
}

void CPU::ei() {

	if (debug)
		printf("ei\n");
	
	triggerIME = true;
}

void CPU::halt() {
	if (debug)
		printf("halt\n");
	Interrupt s = get_highest_priority_interrupt_nonedit();
	if (IME || s == None) {
		halted = true;
	}
	else {
		halt_bug = true;
	}
}