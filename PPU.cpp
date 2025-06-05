#include "PPU.h"
#include "helpers.h"
#include <algorithm>
#include <iterator>
#include <fstream>
#define SCANLINE_CYCLES 339

//issues : stat interrupts don't really work, i.e LYC == LY
PIXEL PPU::int_to_pixel(uint8_t pixel, uint16_t palAdd)
{

	uint8_t palette = addr.getVRAMADD(palAdd);
	int hi = 0;
	int lo = 0;

	// which bits of the colour palette does the colour id map to?
	switch (pixel)
	{
	case 0: hi = 1; lo = 0; break;
	case 1: hi = 3; lo = 2; break;
	case 2: hi = 5; lo = 4; break;
	case 3: hi = 7; lo = 6; break;
	}

	// use the palette to get the colour
	int colour = 0;
	colour = get_bit(palette, hi) << 1;
	colour |= get_bit(palette, lo);

	// convert the game colour to emulator colour
	switch (colour)
	{
	case 0: return PIXEL::GREEN0;
	case 1:	return PIXEL::GREEN1;
	case 2: return PIXEL::GREEN2;
	case 3: return PIXEL::GREEN3;
	}

}

void PPU::writeStat() {
	uint8_t stat = addr.getVRAMADD(STAT);
	stat &= 0b11111000;
	stat |= ((stat2 & 0x01) << 2);
	stat |= (state & 0b11);
	addr.setSTAT(stat);
}

void PPU::readStat() {
	uint8_t stat = addr.getVRAMADD(STAT);
	stat2 = stat & (1 << 2);
	stat3 = stat & (1 << 3);
	stat4 = stat & (1 << 4);
	stat5 = stat & (1 << 5);
	stat6 = stat & (1 << 6);
}

void PPU::updateMode() {
	uint8_t stat = (addr.getVRAMADD(STAT) >> 2) << 2;
	stat |= state;
	addr.setSTAT(stat);
}

void PPU::setLYCFlag() {
	uint8_t scanline = get_scanline();
	readStat();
	uint8_t lyc = addr.getVRAMADD(LYC);
	uint8_t iff = addr.getVRAMADD(IF);

	if (stat2 && lyc != scanline) {
		stat2 = false;
	}

	if (lyc == scanline) {
		printf("lyc == scanline %d\n", addr.getVRAMADD(LYC));
		stat2 = true;
		if (stat6) {
			set_stat_interrupt();
		}
	}

	writeStat();

}


uint8_t PPU::get_scx() {
	return addr.getVRAMADD(SCX);
}

uint8_t PPU::get_scy() {
	return addr.getVRAMADD(SCY);
}

void PPU::read_lcdc() {
	uint8_t uint8_t = addr.getVRAMADD(LCDC);
	lcdc0 = uint8_t & 1;
	lcdc1 = uint8_t & (1 << 1);
	lcdc2 = uint8_t & (1 << 2);
	lcdc3 = uint8_t & (1 << 3);
	lcdc4 = uint8_t & (1 << 4);
	lcdc5 = uint8_t & (1 << 5);
	lcdc6 = uint8_t & (1 << 6);
	lcdc7 = uint8_t & (1 << 7);
}

void PPU::set_vblank_interrupt() {
	uint8_t val = addr.getVRAMADD(IF);
	addr.setIF(val | 1);
}


void PPU::set_stat_interrupt() {
	//printf("triggered stat interrupt\n");
	uint8_t val = addr.getVRAMADD(IF);
	addr.setIF((val << 1) | 1);
}



//void PPU::fetch_vram(){
//	vram_buffer = addr.getVRAM();
//}

void PPU::renderScanline(uint8_t scanlineno) {
	renderBackground();
	renderWindow();
	renderSprite();

	std::copy(scanline_buffer.begin(), scanline_buffer.end(), framebuffer[scanlineno].begin());
}

void PPU::renderBackground() {
	read_lcdc();
	if (!lcdc0)
		return; //means both bg and window are disabled so we skip rendering here.

	uint8_t currentScanline = addr.getVRAMADD(LY);
	uint8_t scrollX = addr.getVRAMADD(SCX);
	uint8_t scrollY = addr.getVRAMADD(SCY);

	//check lcdc3 for the tilemap address

	uint16_t tile_map_address = (lcdc3) ? 0x9C00 : 0x9800;
	uint16_t tile_address_mode = (lcdc4) ? 0x8000 : 0x9000;

	for (uint8_t x = 0; x < 160; x++) {
		// this simulates the fifo internal x position counter

		// in this loop we loop over each pixel, get the appropiate background tile index for it,
		// retrieve the background tile then read the subsequent pixel in said tile and read that for
		// the scanline_buffer pixel position.

		//basically add the appropiate scx and scxy offset to the tile_index calculation grab the tile, render it

		uint16_t xoffset = ((x + scrollX) / 8) & 0x1F;
		uint16_t yoffset = 32 * (((currentScanline + scrollY) & 0xFF) / 8);
		uint16_t totaloffset = (xoffset + yoffset) & 0x3FF;

		uint8_t tile_index = addr.getVRAMADD(tile_map_address + totaloffset);
		int8_t signed_tile_index = tile_index;
		// now we calculate the tiles address
		uint16_t tile_address = (lcdc4) ? tile_address_mode + 16 * tile_index : tile_address_mode + 16 * signed_tile_index;
		tile_address += 2 * ((currentScanline + scrollY) % 8);

		uint8_t tilelo = addr.getVRAMADD(tile_address);
		uint8_t tilehi = addr.getVRAMADD(tile_address + 1);

		//tile in x coordinate
		uint8_t xinTile = 7 - ((x + scrollX) % 8);

		uint8_t bitlo = get_bit(tilelo, xinTile);
		uint8_t bithi = get_bit(tilehi, xinTile);

		uint8_t combined = (bithi << 1) | bitlo;
		scanline_buffer[x] = int_to_pixel(combined,BGP);
	}
	
}


//check if it reaches the window yet 
void PPU::renderWindow() {
	read_lcdc();
	if (!lcdc0)
		return; //means both bg and window are disabled so we skip rendering here.

	if (!lcdc5)
		return; //window disabled specifically

	uint16_t tile_map_address = (lcdc6) ? 0x9C00 : 0x9800;
	uint16_t tile_address_mode = (lcdc4) ? 0x8000 : 0x9000;

	uint8_t ly = addr.getVRAMADD(LY);
	uint8_t windowStartY = addr.getVRAMADD(WY);
	int winX = addr.getVRAMADD(WX) - 7; //weird edge cases
	uint8_t windowStartX = addr.getVRAMADD(WX) - 7;
	bool rendered = false;

	//if ly is above where the window starts then return
	if (ly < windowStartY)
		return;

	if (ly == windowStartY) {
		windowLineCounter = 0;
	}

	for (uint8_t x = 0; x < 160; x++) {
		// now do the same loop through the pixels of the screen except check if window x overlaps screen or skip.
		if (x < winX)
			continue;
		rendered = true;

		// need to do some slightly weird calculations to calculate where in that window we are
		int windowColumn = x - winX;

		int tileX = windowColumn / 8;
		int tileY = windowLineCounter / 8;

		int totalOffset = tileY * 32 + tileX;

		uint8_t tile_index = addr.getVRAMADD(tile_map_address + totalOffset);
		int8_t signed_tile_index = tile_index;

		uint16_t tile_address = (lcdc4) ? tile_address_mode + 16 * tile_index : tile_address_mode + 16 * signed_tile_index;

		tile_address += 2 * (windowLineCounter % 8);

		uint8_t xinTile = 7 - (windowColumn % 8);
		uint8_t tilelo = addr.getVRAMADD(tile_address);
		uint8_t tilehi = addr.getVRAMADD(tile_address + 1);


		uint8_t bitlo = get_bit(tilelo, xinTile);
		uint8_t bithi = get_bit(tilehi, xinTile);

		uint8_t combined = (bithi << 1) | bitlo;
		scanline_buffer[x] = int_to_pixel(combined, BGP);
	}

	if (rendered)
		windowLineCounter++;
}




void PPU::renderSprite() {
	int currentScanline = addr.getVRAMADD(LY);
	if (!lcdc1) return;

	int renderedSprites = 0;
	std::array<int,160> pixelOwner;
	pixelOwner.fill(-1);

	for (int i = 0; i < 40; i++) {
		if (renderedSprites >= 10) break;

		int spriteIndex = i * 4;
		int yPos = addr.getVRAMADD((uint16_t)(0xFE00 + spriteIndex)) - 16;
		int xPos = addr.getVRAMADD((uint16_t)(0xFE00 + spriteIndex + 1)) - 8;
		uint8_t tileIndex = addr.getVRAMADD((uint16_t)(0xFE00 + spriteIndex + 2));
		uint8_t attributes = addr.getVRAMADD((uint16_t)(0xFE00 + spriteIndex + 3));

		int spriteHeight = (lcdc2) ? 16 : 8;
		if (currentScanline < yPos || currentScanline >= yPos + spriteHeight) {
			continue;
		}

		int lineInSprite = currentScanline - yPos;
		if ((attributes & (1 << 6)) != 0) {
			lineInSprite = spriteHeight - 1 - lineInSprite;
		}

		if (spriteHeight == 16) {
			tileIndex &= 0xFE;
			if (lineInSprite >= 8) {
				tileIndex += 1;
				lineInSprite -= 8;
			}
		}

		uint16_t tileAddress = (uint16_t)(0x8000 + tileIndex * 16 + lineInSprite * 2);
		uint8_t tileLow = addr.getVRAMADD(tileAddress);
		uint8_t tileHigh = addr.getVRAMADD((uint16_t)(tileAddress + 1));

		for (int x = 0; x < 8; x++) {
			int bitIndex = (attributes & (1 << 5)) != 0 ? x : 7 - x;
			int colorBit = ((tileHigh >> bitIndex) & 1) << 1 | ((tileLow >> bitIndex) & 1);

			if (colorBit == 0) continue;

			int screenX = xPos + x;
			if (screenX < 0 || screenX >= 160) continue;

			bool bgOverObj = (attributes & (1 << 7)) != 0;
			if (bgOverObj && !(scanline_buffer[screenX] == PIXEL::GREEN0)) {
				continue;
			}

			if (pixelOwner[screenX] == -1 || xPos < pixelOwner[screenX]) {
				pixelOwner[screenX] = xPos;
				bool isSpritePalette1 = (attributes & (1 << 4)) != 0;

				scanline_buffer[screenX] = int_to_pixel(colorBit, isSpritePalette1 ? OBP1 : OBP0);
			}
		}
		renderedSprites++;
		
	}
}

void PPU::resetBuffers() {
	scanline_buffer.fill(PIXEL::GREEN0);
	//for (int i = 0; i < 144; i++) {
	//	framebuffer[i].fill(PIXEL::GREEN0);
	//}
}

uint8_t PPU::get_scanline()
{
	return addr.getVRAMADD(LY);
}





//false is 8000 method
std::array<uint8_t, 16> PPU::get_tile(int index, bool indexingMethod) {
	std::array<uint8_t, 16> tile;
	int startdex = 0;
	if (indexingMethod) {
		startdex = 0x9000 - 0x8000 + index;
	}
	else {
		startdex = 0x8000 - 0x8000 + 16 * index;
	}
	std::copy(vram_buffer.begin() + startdex, vram_buffer.begin() + startdex + 16, tile.begin());
	return tile;
}

PPU::PPU(AddressSpace& addressSpace, Clock& clock_l) : addr(addressSpace), clock(clock_l) {
}

void PPU::step(uint8_t cycles) {
	read_lcdc();
	readStat();
	if (!lcdc7)
		return;


	cycle_counter += cycles;

	switch (state)
	{
	case HBLANK:
		if (cycle_counter >= 204) {
			scanline_buffer.fill(PIXEL::GREEN0);
			cycle_counter -= 204;
			addr.incr(LY);
			readStat();

			setLYCFlag();

			if (addr.getVRAMADD(LY) == 144) {
				state = PPUSTATE::VLANK;
				vblank_triggerable = false;

				if (stat4) {
					set_stat_interrupt();
				}
			}
			else {
				if (stat5) {
					set_stat_interrupt();
				}
				scanline_buffer.fill(PIXEL::GREEN0);
				state = PPUSTATE::OAM;
			}

		}
		updateMode();
		break;
	case VLANK:
		if (!vblank_triggerable && addr.getVRAMADD(LY) == 144)
		{
			windowLineCounter = 0;
			if (lcdc7) {
				set_vblank_interrupt();
				vblank_triggerable = true;
			}
		}

		if (cycle_counter >= SCANLINE_CYCLES) {
			cycle_counter -= SCANLINE_CYCLES;
			addr.incr(LY);
			setLYCFlag();
			if (addr.getVRAMADD(LY) == 153) {
				addr.write(LY, 0, false);
				state = OAM;
				vblank_triggerable = false;

				updateMode();
				if (stat5) {
					set_stat_interrupt();
				}
			}
		}
		break;
	case OAM:
		// do oam checks?
		if (cycle_counter >= 80) {
			cycle_counter -= 80;
			state = DRAW;
			updateMode();
		}
		break;
	case DRAW:
		if (cycle_counter >= 172) {
			cycle_counter -= 172;
			state = HBLANK;
			readStat();
			updateMode();

			if (addr.getVRAMADD(LY) < 144) {
				//should only render scanline once per scanline
				renderScanline(addr.getVRAMADD(LY));
			}
			if (stat3) {
				set_stat_interrupt();
			}
		}
		break;
	default:
		break;
	}

	//printf("mode: %s\n", to_string(state));
}

void PPU::reset_cycle_counter() {
	if (debug)
		printf("reset ppu internal cycle counter\n");
	cycle_counter = 0;
	state = OAM;
}

std::vector<uint8_t> PPU::saveBytes()
{
	std::vector<uint8_t> butes;
	butes.push_back(static_cast<uint8_t>(state));

	butes.push_back(cycle_counter >> 24);
	butes.push_back(cycle_counter >> 16);
	butes.push_back(cycle_counter >> 8);
	butes.push_back(cycle_counter);

	butes.push_back(windowLineCounter >> 24);
	butes.push_back(windowLineCounter >> 16);
	butes.push_back(windowLineCounter >> 8);
	butes.push_back(windowLineCounter);

	butes.push_back(static_cast<uint8_t>(oamtriggerable));
	butes.push_back(static_cast<uint8_t>(vblank_triggerable));

	// scanline_buffer: 160 PIXEL values -> cast if safe
	for (auto px : scanline_buffer) {
		butes.push_back(static_cast<uint8_t>(px));
	}

	// framebuffer: 144x160 PIXEL values
	for (const auto& row : framebuffer) {
		for (auto px : row) {
			butes.push_back(static_cast<uint8_t>(px));
		}
	}

	return butes;
}


void PPU::loadSave(std::string savePath)
{
	std::ifstream inputFile(savePath, std::ios::binary);
	std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());

	if (buffer.size() < 36 + 160 + 160 * 144) {
		throw std::runtime_error("Save file too small for PPU state.");
	}

	state = static_cast<PPUSTATE>(buffer[25]);
	cycle_counter = (buffer[26] << 24) | (buffer[27] << 16) | (buffer[28] << 8) | buffer[29];
	windowLineCounter = (buffer[30] << 24) | (buffer[31] << 16) | (buffer[32] << 8) | buffer[33];
	oamtriggerable = buffer[34];
	vblank_triggerable = buffer[35];

	// Scanline buffer: next 160 bytes
	for (int i = 0; i < 160; i++) {
		scanline_buffer[i] = static_cast<PIXEL>(buffer[36 + i]);
	}

	// Framebuffer: next 160 * 144 bytes
	for (int i = 0; i < 160 * 144; i++) {
		int x = i % 160;
		int y = i / 160;
		framebuffer[y][x] = static_cast<PIXEL>(buffer[36 + 160 + i]);
	}
}


std::array<std::array<PIXEL, 160>, 144> PPU::getDisplay()
{
	return framebuffer;
}




//int currentScanline = addr.getVRAMADD(LY);
//if (lcdc1 == 0) return;
//
//int renderedSprites = 0;
//std::array<int, 160> pixelOwner;
//std::fill(pixelOwner.begin(), pixelOwner.end(), -1);
//for (int i = 0; i < 40; i++) {
//	if (renderedSprites >= 10) break;
//
//	int spriteIndex = i * 4;
//	int yPos = addr.getVRAMADD((uint16_t)(0xFE00 + spriteIndex)) - 16;
//	int xPos = addr.getVRAMADD((uint16_t)(0xFE00 + spriteIndex + 1)) - 8;
//	uint8_t tileIndex = addr.getVRAMADD((uint16_t)(0xFE00 + spriteIndex + 2));
//	uint8_t attributes = addr.getVRAMADD((uint16_t)(0xFE00 + spriteIndex + 3));
//
//	int spriteHeight = (lcdc2) != 0 ? 16 : 8;
//	if (currentScanline < yPos || currentScanline >= yPos + spriteHeight) {
//		continue;
//	}
//
//	int lineInSprite = currentScanline - yPos;
//	if ((attributes & (1 << 6)) != 0) {
//		lineInSprite = spriteHeight - 1 - lineInSprite;
//	}
//
//	if (spriteHeight == 16) {
//		tileIndex &= 0xFE;
//		if (lineInSprite >= 8) {
//			tileIndex += 1;
//			lineInSprite -= 8;
//		}
//	}
//
//	uint16_t tileAddress = (uint16_t)(0x8000 + tileIndex * 16 + lineInSprite * 2);
//	uint8_t tileLow = addr.getVRAMADD(tileAddress);
//	uint8_t tileHigh = addr.getVRAMADD((uint16_t)(tileAddress + 1));
//
//	for (int x = 0; x < 8; x++) {
//		int bitIndex = (attributes & (1 << 5)) != 0 ? x : 7 - x;
//		int colorBit = ((tileHigh >> bitIndex) & 1) << 1 | ((tileLow >> bitIndex) & 1);
//
//		if (colorBit == 0) continue;
//
//		int screenX = xPos + x;
//		if (screenX < 0 || screenX >= 160) continue;
//
//		bool bgOverObj = (attributes & (1 << 7)) != 0;
//		if (bgOverObj && !(scanline_buffer[screenX] == PIXEL::GREEN0)) {
//			continue;
//		}
//
//		if (pixelOwner[screenX] == -1 || xPos < pixelOwner[screenX]) {
//			pixelOwner[screenX] = xPos;
//			bool isSpritePalette1 = (attributes & (1 << 4)) != 0;
//
//			uint8_t spritePalette = isSpritePalette1 ? addr.getVRAMADD(OBP1) : addr.getVRAMADD(OBP0);
//			int paletteShift = colorBit * 2;
//			int paletteColor = (spritePalette >> paletteShift) & 0b11;
//
//			switch (paletteColor) {
//			case 0:
//				scanline_buffer[screenX] = PIXEL::GREEN0;
//				break;
//			case 1:
//				scanline_buffer[screenX] = PIXEL::GREEN1;
//				break;
//			case 2:
//				scanline_buffer[screenX] = PIXEL::GREEN2;
//				break;
//			case 3:
//				scanline_buffer[screenX] = PIXEL::GREEN3;
//				break;
//			default:
//				printf("wtf color fucked\n");
//				break;
//			}
//		}
//	}
//	renderedSprites++;
//}