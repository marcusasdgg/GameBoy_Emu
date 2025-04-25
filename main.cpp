#include "CPU.h"
#include "Clock.h"
#include "AddressSpace.h"
#include "PPU.h"
#include<thread>
#include <iostream>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include <fstream>

#include "helpers.h"

AddressSpace addr("C:\\Users\\marcu\\Documents\\dmg_boot.bin","C:\\Users\\marcu\\Downloads\\tetris.gb");
Clock ck(4000000,addr);
PPU ppu(addr, ck);
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;
FILE* logFile;


#define SECONDS 40

// diagnose the cpu because it is broken and does not decomnpress data properly into vram

//IMPORATNT
// for the cpu, make each jump instruction return a PC instead of editing pc direclty.

// fix teh issue of:
// editing the PC when jumping.

//reWRITE ENTIRE PROJECT, to USE A single thread that goes by fps timing and not 100% accurate cpu/ppu timings

// could do something like: the jump inbstructions return a PC, check if PC is negative -1, if so return it back into the normal PC else don't change, also make these cases return immediately in their own cases as well.
//00:C69D
//current main
int main() {

	
	CPU cpu(addr, ck);
	cpu.inititialise();

	int fps = 60; // changes fps
	uint64_t factor = 60 / 4194304;
	uint64_t frequency = 4194304; //changes frequency
	
	uint64_t cycles_per_frame = frequency / fps;
	auto target_frame_time = std::chrono::microseconds(1000000 / fps);
	std::cout << "Target frame time: " << target_frame_time.count() << " microseconds\n";
	printf("cycles per frame %lld \n", cycles_per_frame);
	freopen_s(&logFile, "C:\\Users\\marcu\\Desktop\\log.txt", "w", stdout);

	auto start = std::chrono::high_resolution_clock::now();
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(160*4, 144*4)), "Game Boy Emulator");

	sf::Texture texture(sf::Vector2u(160, 144));

	sf::Sprite sprite(texture);
	sprite.scale(sf::Vector2f(4.0, 4.0));

	// Optional: scale it up to make it more visible (Game Boy screen is tiny)
	sf::Image frameImage(sf::Vector2u(160, 144)); 

	const int gradient[] = {0,255};


	int frames = 0;
	cycles_per_frame = 69764;


	while (window.isOpen()) {
		auto frame_start = std::chrono::high_resolution_clock::now();
		int cycles_frame = 0;
		while (cycles_frame < cycles_per_frame) {
			const std::optional event = window.pollEvent();

			if (event.has_value() && event->is<sf::Event::Closed>())
				window.close();
			auto curr_cycle = ck.get_cycle();
			cpu.step();
			int cycles_taken = (int) (ck.get_cycle() - curr_cycle);
			ppu.step(cycles_taken);
			cycles_frame += cycles_taken;
		}

		ppu.reset_cycle_counter();
		addr.write(LY, 0);
		//reset cycle counter for the next frame

		auto display = ppu.getDisplay();
		//printf("vblank writing lcd\n");

		for (int y = 0; y < 144; ++y) {
			for (int x = 0; x < 160; ++x) {
				PIXEL val = display[y][x];
				if (val == PIXEL::GREEN0) {
					frameImage.setPixel(sf::Vector2u(x, y), sf::Color(255, 255, 255)); // grayscale
				}
				else {
					frameImage.setPixel(sf::Vector2u(x, y), sf::Color(0, 0, 0)); // grayscale
				}
				
			}
		}
		
		//frameImage.setPixel(sf::Vector2u(100, 100), sf::Color(256, 256, 0));

		texture.update(frameImage);
		window.clear();
		window.draw(sprite);
		window.display();

		ppu.resetBuffers();

		auto frame_end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);
		frames++;

		if (std::chrono::duration_cast<std::chrono::seconds>(frame_start - start) > std::chrono::seconds(SECONDS))
			break;

		auto sleep_til = frame_start + target_frame_time;
		while (sleep_til > std::chrono::high_resolution_clock::now()) {
			const std::optional event = window.pollEvent();

			if (event.has_value() && event->is<sf::Event::Closed>())
				window.close();
			std::this_thread::yield();
		}

		auto now = std::chrono::high_resolution_clock::now();
		auto frame_total = std::chrono::duration_cast<std::chrono::microseconds>(now - frame_start);

		//printf("Work: %lld mms | Total: %lld mms\n", elapsed.count(), frame_total.count());
	}


	auto memory = addr.getVRAM();
	std::ofstream outputFile("C:\\Users\\marcu\\Downloads\\vramdumpd.txt", std::ios::binary);
	outputFile.write(reinterpret_cast<const char*>(memory.data()), 6144);
	outputFile.close();

	auto memory2 = addr.get_range(0x9800, 0x9BFF);
	std::ofstream outputFile2("C:\\Users\\marcu\\Downloads\\map1_dump.txt", std::ios::binary);
	outputFile2.write(reinterpret_cast<const char*>(memory2.data()), 1024);
	outputFile2.close();


	auto memory3 = addr.get_range(0x9C00, 0x9FFF);
	std::ofstream outputFile3("C:\\Users\\marcu\\Downloads\\map2_dump.txt", std::ios::binary);
	outputFile3.write(reinterpret_cast<const char*>(memory3.data()), 1024);
	outputFile3.close();

	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
	//printf("%d frames generated in %llds", frames, elapsed_sec);
	//cpu.print_registers();
	printf("joypad is %02x\n", addr.read(0xFF00));
	fclose(logFile);
	cpu.print_registers();
}


//void DecompressAndCopy(uint8_t data, uint16_t addrs) {
//	uint8_t mask0 = 0b00000001;
//	uint8_t mask1 = 0b00000011;
//	uint8_t res = 0;
//	for (int i = 0; i < 4; ++i) {
//		res |= (data & mask0) ? mask1 : 0;
//		mask0 <<= 1;
//		mask1 <<= 2;
//	}
//	addr.write(addrs, res);
//	addr.write(addrs + 2, res);
//}
//
//
//int main() {
//	uint16_t vram = 0x8010;
//
//	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(160*4, 144*4)), "Game Boy Emulator");
//
//	sf::Texture texture(sf::Vector2u(160, 144));
//
//	sf::Sprite sprite(texture);
//	sprite.scale(sf::Vector2f(4.0, 4.0));
//
//	// Optional: scale it up to make it more visible (Game Boy screen is tiny)
//	sf::Image frameImage(sf::Vector2u(160, 144)); 
//
//	addr.write(BGP, 0xFC);
//
//
//	for (uint16_t logo = 0x0104; logo < 0x0134; logo++) {
//		uint8_t data = addr.read(logo);
//		DecompressAndCopy(data, vram);
//		vram += 4;
//		DecompressAndCopy(data >> 4, vram);
//		vram += 4;
//	}
//
//	vram = 0x80d0;
//	for (uint16_t logo = 0xd8; logo < 0xe0; ++logo) {
//		addr.write(vram, addr.read(logo));
//		vram += 2;
//	}
//
//	int a = 25;
//	uint16_t mem = 0x9910;
//	addr.write(mem, a);
//	mem = 0x992f;
//	for (int j = 0; j < 2; ++j) {
//		for (int i = 12; i > 0; --i) {
//			a--;
//			addr.write(mem, a);
//			mem--;
//		}
//		mem = 0x990f;
//	}
//
//
//	addr.write(SCY,0x48);
//	addr.write(LCDC, 0x91);
//
//
//	ppu.read_lcdc();
//	auto memory = addr.getVRAM();
//	
//
//	for (int i = 0; i < 144; i++) {
//		ppu.renderScanline(i);
//		addr.incr(LY);
//	}
//
//	auto display = ppu.getDisplay();
//
//	for (int y = 0; y < 144; ++y) {
//		for (int x = 0; x < 160; ++x) {
//			PIXEL val = display[y][x];
//			if (val != PIXEL::GREEN0) {
//				printf("found non 0 pixel!!\n");
//			}
//			frameImage.setPixel(sf::Vector2u(x, y), sf::Color(val *255, val * 255, val * 255)); // grayscale
//		}
//	}
//
//	texture.update(frameImage);
//	window.clear();
//	window.draw(sprite);
//	window.display();
//
//	
//

//
//	std::this_thread::sleep_for(std::chrono::seconds(5));
//}