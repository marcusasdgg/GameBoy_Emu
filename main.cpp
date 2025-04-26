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

AddressSpace addr("C:\\Users\\marcu\\Documents\\dmg_boot.bin","C:\\Users\\marcu\\Downloads\\dmg-acid2.gb");
Clock ck(4000000,addr);
PPU ppu(addr, ck);
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;
FILE* logFile;


#define SECONDS 120

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

	//up down left right a b select start
	std::array<bool,8> buttonstate = { 0,0,0,0,0,0,0,0 };
	
	CPU cpu(addr, ck);
	cpu.inititialise();

	int fps = 60; // changes fps
	uint64_t frequency = 4194304; //4194304; //changes frequency
	
	uint64_t cycles_per_frame = frequency / fps;
	auto target_frame_time = std::chrono::microseconds(1000000 / fps);


	std::cout << "Target frame time: " << target_frame_time.count() << " microseconds\n";
	printf("cycles per frame %lld \n", cycles_per_frame);
	//freopen_s(&logFile, "C:\\Users\\marcu\\Desktop\\log.txt", "w", stdout);

	auto start = std::chrono::high_resolution_clock::now();
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(160*4, 144*4)), "Game Boy Emulator");

	sf::Texture texture(sf::Vector2u(160, 144));

	sf::Sprite sprite(texture);
	sprite.scale(sf::Vector2f(4.0, 4.0));

	// Optional: scale it up to make it more visible (Game Boy screen is tiny)
	sf::Image frameImage(sf::Vector2u(160, 144)); 

	const int gradient[] = {0,255};


	int frames = 0;


	while (window.isOpen()) {
		auto frame_start = std::chrono::high_resolution_clock::now();
		int cycles_frame = 0;
		while (cycles_frame < cycles_per_frame) {
			
			const std::optional event = window.pollEvent();
			if (event.has_value() && event->is<sf::Event::Closed>()) {
				window.close();
			}
				
			auto curr_cycle = ck.get_cycle();
			cpu.step();
			int cycles_taken = (int) (ck.get_cycle() - curr_cycle);
			ppu.step(cycles_taken);
			cycles_frame += cycles_taken;
		}

		addr.write(LY, 0);
		//reset cycle counter for the next frame

		auto display = ppu.getDisplay();
		//printf("vblank writing lcd\n");

		for (int y = 0; y < 144; ++y) {
			for (int x = 0; x < 160; ++x) {
				PIXEL val = display[y][x];
				switch (val)
				{
				case GREEN0:
					frameImage.setPixel(sf::Vector2u(x, y), sf::Color(255, 255, 255)); // Dark Green
					break;
				case GREEN1:
					frameImage.setPixel(sf::Vector2u(x, y), sf::Color(255, 0, 0)); // Medium Green
					break;
				case GREEN2:
					frameImage.setPixel(sf::Vector2u(x, y), sf::Color(0, 255, 0)); // Light Green
					break;
				case GREEN3:
					frameImage.setPixel(sf::Vector2u(x, y), sf::Color(0, 0, 255)); // Very Light Green
					break;
				default:
					break;
				}
			}
		}


		// poll for keypresses
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			buttonstate[0] = true;
			printf("up pressed\n");
		}
		else {
			buttonstate[0] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			buttonstate[1] = true;
			printf("down pressed\n");
		}
		else {
			buttonstate[1] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			printf("left pressed\n");
			buttonstate[2] = true;
		}
		else {
			buttonstate[2] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			printf("right pressed\n");
			buttonstate[3] = true;
		}
		else {
			buttonstate[3] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
		{
			printf("A pressed\n");
			buttonstate[4] = true;
		}
		else {
			buttonstate[4] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
		{
			printf("B pressed\n");
			buttonstate[5] = true;
		}
		else {
			buttonstate[5] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		{
			printf("select pressed\n");
			buttonstate[6] = true;
		}
		else {
			buttonstate[6] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
		{
			printf("start pressed\n");
			buttonstate[7] = true;
		}
		else {
			buttonstate[7] = false;
		}


		addr.mapbuttons(buttonstate);

		texture.update(frameImage);
		window.clear();
		window.draw(sprite);
		window.display();

		ppu.resetBuffers();

		auto frame_end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);
		frames++;

		auto sleep_til = frame_start + target_frame_time;

		if (sleep_til < std::chrono::high_resolution_clock::now())
			printf("exceeded frame time\n");

		while (sleep_til > std::chrono::high_resolution_clock::now()) {
			const std::optional event = window.pollEvent();

			if (event.has_value() && event->is<sf::Event::Closed>()) {
				window.close();
			}
				
			//std::this_thread::yield();
		}

		auto now = std::chrono::high_resolution_clock::now();
		auto frame_total = std::chrono::duration_cast<std::chrono::microseconds>(now - frame_start);
	}


	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
	//fclose(logFile);
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