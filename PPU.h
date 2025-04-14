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

class PPU {
	private:
		//current state
		PPUSTATE state;

		int xpos = 0;

		//reference to shared objects
		Clock& clock;
		AddressSpace& addr;
		Fifo& fifo;

		int windowLineCounter = 0;

		std::array<uint8_t, 6144> vram_buffer;

		//sprite buffer
		std::array<Sprite,10> sprite_buffer;

		//oam buffer figure out when u have to refresh this
		std::array<uint8_t, 160> oam_buffer;

		std::array<PIXEL, 160> scanline_buffer;

		//registers
		//LCDC register - a mirror of the address map
		bool LCDENABLE;
		bool WINMAPSEL;
		bool WINENABLE;
		bool TILESEL;
		bool BGSEL;
		bool SPRSZE;
		bool SPRENABLE;
		bool BGENABLE;

		//STAT register - mirror of the address map
		bool LYCLYENABLE;
		bool MODE2ENABLE;
		bool MODE1ENABLE;
		bool MODE0ENABLE;
		bool COINCIDENCE;

		// 4 functions to perform 4 different modes.
		//87-204 cycles
		void HBlank();

		//4560 cycles
		void VBlank();

		//80 cycles
		void OAMScan();

		//172-289 cycles
		void Drawing();


		void writeStat();

		void setLYCFlag();

		//execution_loop
		void execute_loop();

		//helper functions
		uint8_t get_scx();
		uint8_t get_scy();

		void get_lcdc();

		void set_vblank_interrupt();
		void set_stat_interrupt();

		void block_n_cycles(int);

		void block_cycles_i();

		void fetch_vram();

		void renderScanline();

		void renderBackground();
		
		void renderWindow();

		void renderSprite();

		uint8_t get_scanline();

		std::array<uint8_t,16> get_tile(int index, bool indexingMethod);
		

		uint8_t BACKFIFO;
		uint8_t SPRITEFIFO;





	public:
		PPU(AddressSpace& addressSpace, Clock& clock_l, Fifo& fifo_l);
		void execute();
};

