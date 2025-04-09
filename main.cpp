#include "CPU.h"
#include "Clock.h"
#include "AddressSpace.h"
#include<thread>

AddressSpace addr;
Clock ck(1000);


int main() {
	addr.readRom("C:\\Users\\marcu\\Documents\\dmg_boot.bin");
	CPU cpu(addr, ck);
	ck.start_clock();
	cpu.inititialise();
	cpu.execute();
	std::this_thread::sleep_for(std::chrono::seconds(3));
	cpu.stop_execute();
	ck.stop_clock();
	addr.saveRom("C:\\Users\\marcu\\Documents\\test.gb");
	cpu.print_registers();
}