#include "CPU.h"

int main() {
	AddressSpace adr = AddressSpace();
	CPU cpu = CPU(adr);
	cpu.inititialise();
	cpu.arithmetic_test();
}