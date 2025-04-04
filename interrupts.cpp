#include "CPU.h"

void CPU::di() {
	IME = false;
}

void CPU::ei() {
	IME = true;
}

void CPU::halt() {
	//???? probabyl some infinite loop
}