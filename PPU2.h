#pragma once
#include "PPU.h"
class PPU2
{
private:
	PPUSTATE mode = OAM;
	int m_ScanlineCounter = 0;

	PIXEL int_to_pixel(uint8_t, uint16_t palAdd);

	void set_stat_interrupt();

	void set_vblank_interrupt();

	void setLCDSTATUS();

	bool isLCDENABLED();

	void renderScanline();
	void renderTiles(uint8_t lcdControl);
	void renderSprites(uint8_t lcdControl);

	AddressSpace& addr;

	//buffers
	std::array<PIXEL, 160> scanline_buffer;
	std::array<std::array<PIXEL, 160>, 144> frame_buffer;
public:
	PPU2(AddressSpace&);
	std::array<std::array<PIXEL, 160>, 144> get_display();
	void resetBuffers();
	void step(uint8_t cycles);

};

