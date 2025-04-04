#include "CPU.h"
#include "Clock.h"
#include "AddressSpace.h"

AddressSpace addr;
Clock ck(1.0);


int main() {
	CPU cpu(addr, ck);
	ck.start_clock();
	cpu.inititialise();
	cpu.execute();
	while (true);
}