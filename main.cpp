#include "CPU.h"
#include "Clock.h"
#include "AddressSpace.h"
#include "PPU.h"
#include<thread>
#include <iostream>

AddressSpace addr("C:\\Users\\marcu\\Documents\\dmg_boot.bin","C:\\Users\\marcu\\Downloads\\tetris.gb");
Clock ck(4000000000,addr);
PPU ppu(addr, ck);
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;


char grayscaleToChar(uint8_t value) {
	const char gradient[] = " .+%#"; // From light to dark
  // Map 0-255 to an index
	return gradient[value];
}


int main() {
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
	cpu.stop_execute();
	cpu.print_registers();
	ppu.stop();
	printf("stopping clock\n");
	ck.stop_clock();
	addr.saveRom("C:\\Users\\marcu\\Documents\\test.gb");

}