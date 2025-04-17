#include "CPU.h"

void CPU::di() {

	if (debug)
		printf("di\n");

	IME = false;
}

void CPU::ei() {

	if (debug)
		printf("ei\n");

	IME = true;
}

void CPU::halt() {
	if (debug)
		printf("halt\n");
	//???? probabyl some infinite loop
}