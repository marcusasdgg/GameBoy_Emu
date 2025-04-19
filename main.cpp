#include "CPU.h"
#include "Clock.h"
#include "AddressSpace.h"
#include "PPU.h"
#include<thread>
#include <iostream>
#include <cstdio>

AddressSpace addr("C:\\Users\\marcu\\Documents\\dmg_boot.bin","C:\\Users\\marcu\\Downloads\\tetris.gb");
Clock ck(4000000,addr);
PPU ppu(addr, ck);
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;
FILE* logFile;



char grayscaleToChar(uint8_t value) {
	const char gradient[] = " .+%#"; // From light to dark
  // Map 0-255 to an index
	return gradient[value];
}
//IMPORATNT
// for the cpu, make each jump instruction return a PC instead of editing pc direclty.

// fix teh issue of:
// editing the PC when jumping.

//reWRITE ENTIRE PROJECT, to USE A single thread that goes by fps timing and not 100% accurate cpu/ppu timings

// could do something like: the jump inbstructions return a PC, check if PC is negative -1, if so return it back into the normal PC else don't change, also make these cases return immediately in their own cases as well.
int main() {

	//freopen_s(&logFile, "C:\\Users\\marcu\\Desktop\\log.txt", "w", stdout);
	CPU cpu(addr, ck);
	cpu.inititialise();
	cpu.execute();
	ppu.execute();
	ck.start_clock();

	//while (true) {
	//	auto pixels = ppu.getDisplay();
	//	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
	//		for (int x = 0; x < SCREEN_WIDTH; ++x) {
	//			// Convert grayscale value to ASCII character and print it
	//			std::cout << grayscaleToChar(pixels[y][x]);
	//		}
	//		std::cout << "\n";  // Newline after each row
	//	}
	//	std::this_thread::sleep_for(std::chrono::milliseconds(16));
	//}

	std::this_thread::sleep_for(std::chrono::seconds(10));
	printf("clock has ticked %llu\n", ck.get_cycle());
	cpu.print_registers();
	//fclose(logFile);
	cpu.stop_execute();
	ppu.stop();

	printf("stopping clock\n");
	ck.stop_clock();
	addr.saveRom("C:\\Users\\marcu\\Documents\\test.gb");

}


//while (cyclesThisFrame < 70224) {
//	uint8_t opcode = memory.read(cpu.PC);
//	int cycles = cpu.execute(opcode);        // Execute 1 instruction
//	cpu.handle_interrupts();                 // Handle if VBlank or others are triggered
//	timer.tick(cycles);                      // Update timers
//	ppu.tick(cycles);                        // Update scanlines, potentially enter VBlank
//	input.tick();
//
//	cyclesThisFrame += cycles;
//}
// follow this architecture in your loop.