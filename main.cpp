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

#define SAVEPATH "C:\\Users\\marcu\\Downloads\\saveFiles\\poo.POOSAVE"

AddressSpace addr("C:\\Users\\marcu\\Documents\\dmg_boot.bin","C:\\Users\\marcu\\Downloads\\tetris.gb");
Clock ck(4000000,addr);
CPU cpu(addr, ck);
PPU ppu(addr,ck);
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;
FILE* logFile;
sf::Color a[] = {sf::Color(181,175,66),sf::Color(145,155,58),sf::Color(93,120,46), sf::Color(58,81, 34) };


int FACTOR = 2;

//changes to make: ppu can return a texture or inplace mod a texture isntead.

// somethin about delayed movement?


int main() {
	//cpu.loadSave(SAVEPATH);
	//ck.loadSave(SAVEPATH);
	//ppu.loadSave(SAVEPATH);
	//addr.loadSave(SAVEPATH);

	//up down left right a b select start
	std::array<bool, 8> buttonstate = { 0,0,0,0,0,0,0,0 };

	//cpu.inititialise();
	int missedFrames = 0;
	int fps = 60 * FACTOR; // changes fps
	uint64_t frequency = 4194304 * FACTOR; //4194304; //changes frequency

	uint64_t cycles_per_frame = frequency / fps;
	auto target_frame_time = std::chrono::microseconds(1000000 / fps);


	std::cout << "Target frame time: " << target_frame_time.count() << " microseconds\n";
	printf("cycles per frame %lld \n", cycles_per_frame);
	//freopen_s(&logFile, "C:\\Users\\marcu\\Desktop\\log.txt", "w", stdout);

	auto start = std::chrono::high_resolution_clock::now();
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(160 * 4, 144 * 4)), "Game Boy Emulator");

	sf::Texture texture(sf::Vector2u(160, 144));

	sf::Sprite sprite(texture);
	sprite.scale(sf::Vector2f(4.0, 4.0));

	// Optional: scale it up to make it more visible (Game Boy screen is tiny)
	sf::Image frameImage(sf::Vector2u(160, 144));

	const int gradient[] = { 0,255 };


	int frames = 0;


	while (window.isOpen()) {
		auto frame_start = std::chrono::high_resolution_clock::now();
		int cycles_frame = 0;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			buttonstate[0] = true;
		}
		else {
			buttonstate[0] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			buttonstate[1] = true;
		}
		else {
			buttonstate[1] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			buttonstate[2] = true;
		}
		else {
			buttonstate[2] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			buttonstate[3] = true;
		}
		else {
			buttonstate[3] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
		{
			buttonstate[4] = true;
		}
		else {
			buttonstate[4] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
		{
			buttonstate[5] = true;
		}
		else {
			buttonstate[5] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		{
			buttonstate[6] = true;
		}
		else {
			buttonstate[6] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
		{
			buttonstate[7] = true;
		}
		else {
			buttonstate[7] = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		{
			FACTOR = 20;
		}
		else {
			FACTOR = 2;
		}
		addr.mapbuttons(buttonstate);


		while (cycles_frame < cycles_per_frame) {

			auto curr_cycle = ck.get_cycle();
			cpu.step();
			int cycles_taken = (int)(ck.get_cycle() - curr_cycle);
			ppu.step(cycles_taken);
			addr.tickAPU(cycles_taken);
			cycles_frame += cycles_taken;
		}

		//addr.write(LY, 0);
		//reset cycle counter for the next frame

		auto display = ppu.getDisplay();
		//printf("vblank writing lcd\n");

		for (int y = 0; y < 144; ++y) {
			for (int x = 0; x < 160; ++x) {
				PIXEL val = display[y][x];
				frameImage.setPixel(sf::Vector2u(x, y), a[val]);
			}
		}




		fps = 60 * FACTOR;
		target_frame_time = std::chrono::microseconds(1000000 / fps);


		texture.update(frameImage);
		//window.clear();
		window.draw(sprite);
		window.display();

		ppu.resetBuffers();

		auto frame_end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);
		frames++;

		auto sleep_til = frame_start + target_frame_time;

		if (sleep_til < std::chrono::high_resolution_clock::now()) {
			missedFrames++;
			//printf("exceeded frame time\n");
		}


		while (sleep_til > std::chrono::high_resolution_clock::now()) {
			const std::optional event = window.pollEvent();
			if (event.has_value() && event->is<sf::Event::Closed>()) {
				window.close();
			}
			std::this_thread::yield();
		}


		auto now = std::chrono::high_resolution_clock::now();
		auto frame_total = std::chrono::duration_cast<std::chrono::microseconds>(now - frame_start);
	}


	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
	printf("In %lld cycles missed %d frames\n", ck.get_cycle(), missedFrames);

	//std::vector<uint8_t> cpubytes = cpu.saveBytes();
	//printf("clock bytes starts at index %lld\n", cpubytes.size());
	//std::vector<uint8_t> clockbytes = ck.saveBytes();
	//printf("ppu bytes starts at index %lld\n", clockbytes.size() + cpubytes.size());
	//std::vector<uint8_t> ppuBytes = ppu.saveBytes();
	//printf("addr bytes starts at index %lld\n", ppuBytes.size() + cpubytes.size() + clockbytes.size());
	//std::vector<uint8_t> addrbytes = addr.saveBytes();

	//printf("saving %lld bytes", cpubytes.size() + clockbytes.size() + ppuBytes.size() + addrbytes.size());
	//// NOW COMBINE AND SAVE
	//std::ofstream outputFile(SAVEPATH, std::ios::binary);
	//outputFile.write(reinterpret_cast<const char*>(cpubytes.data()), cpubytes.size());
	//outputFile.write(reinterpret_cast<const char*>(clockbytes.data()), clockbytes.size());
	//outputFile.write(reinterpret_cast<const char*>(ppuBytes.data()), ppuBytes.size());
	//outputFile.write(reinterpret_cast<const char*>(addrbytes.data()), addrbytes.size());
	/*outputFile.close();*/
	//fclose(logFile);
	cpu.print_registers();
}


//each save file contains cpu register values, PC SP etc then clock info then ppu info then address space info which in itself contains mbc info.

//int main() {
//		sf::RenderWindow window(sf::VideoMode(sf::Vector2u(160*4, 144*4)), "Game Boy Emulator");
//	ppu.loadSave("C:\\Users\\marcu\\Downloads\\poo.POOSAVE");
//	sf::Texture texture(sf::Vector2u(160, 144));
//
//	sf::Sprite sprite(texture);
//	sprite.scale(sf::Vector2f(4.0, 4.0));
//
//	// Optional: scale it up to make it more visible (Game Boy screen is tiny)
//	sf::Image frameImage(sf::Vector2u(160, 144)); 
//			auto display = ppu.getDisplay();
//	
//	for (int y = 0; y < 144; ++y) {
//		for (int x = 0; x < 160; ++x) {
//			PIXEL val = display[y][x];
//			frameImage.setPixel(sf::Vector2u(x, y), a[val]);
//		}
//	}
//			texture.update(frameImage);
//		//window.clear();
//		window.draw(sprite);
//		window.display();
//	std::this_thread::sleep_for(std::chrono::seconds(5));
//}