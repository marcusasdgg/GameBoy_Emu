#pragma once
#include "Clock.h"
#include "AddressSpace.h"
#include "Sprite.h"
#include <array>
#include "FIFO.h"

// the vblank works by drawing scanline by scan line to buffer
// each line goes through modes 2 -> 3 -> 0
// after all lines are written than hblank occurs


//things to do:
//handle interrupts and set them properly
// implement stat and vblank interrupt set
// create stat read and lcdc read funtions taht read those into intenral storage.

enum PPUSTATE {
	HBLANK,
	VLANK,
	OAM,
	DRAW
};

//in the future stray away from buffers and use references

#define HBLANKSTART 252
#define HBLANKEND 339
#define OAMSTART 0
#define DRAWSTART 80

class PPU {
	private:
		//current state
		int cycle_counter = 0;
		PPUSTATE state = OAM;

		int xpos = 0;


		//reference to shared objects
		Clock& clock;
		AddressSpace& addr;

		int windowLineCounter = 0;

		std::array<uint8_t, 8192> vram_buffer;

		//sprite buffer
		std::array<Sprite,10> sprite_buffer;

		//oam buffer figure out when u have to refresh this
		std::array<uint8_t, 160> oam_buffer;

		std::array<PIXEL, 160> scanline_buffer;

		std::array<std::array<PIXEL, 160>, 144> framebuffer;
		//registers
		//LCDC register - a mirror of the address map
		bool lcdc0;
		bool lcdc1;
		bool lcdc2;
		bool lcdc3;
		bool lcdc4;
		bool lcdc5;
		bool lcdc6;
		bool lcdc7;

		//STAT register - mirror of the address map
		bool stat2;
		bool stat3;
		bool stat4;
		bool stat5;
		bool stat6;

		// 4 functions to perform 4 different modes.
		//87-204 cycles

		std::array<std::array<uint8_t, 32>, 32>  get_background_map(bool);

		PIXEL int_to_pixel(uint8_t pixel, uint16_t palAdd);

		void writeStat();

		void readStat();

		void updateMode();

		void setLYCFlag();


		//helper functions
		uint8_t get_scx();
		uint8_t get_scy();



		void set_vblank_interrupt();
		void set_stat_interrupt();

		void renderBackground();
		
		void renderWindow();

		void renderSprite();

		
		
		
		uint8_t get_scanline();

		std::array<uint8_t,16> get_tile(int index, bool indexingMethod);
		

		uint8_t BACKFIFO;
		uint8_t SPRITEFIFO;
		bool oamtriggerable = true;
		bool vblank_triggerable = true;


	public:
		void resetBuffers();
		void read_lcdc();
		void renderScanline(uint8_t line);

		PPU(AddressSpace& addressSpace, Clock& clock_l);
		void step(uint8_t cycles);
		void reset_cycle_counter();
		void stop();
		std::array<std::array<PIXEL,160>,144> getDisplay();
};

