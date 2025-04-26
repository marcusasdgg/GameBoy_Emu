#include "PPU.h"
#include "helpers.h"

#define SCANLINE_CYCLES 339

//issues : stat interrupts don't really work, i.e LYC == LY

void PPU::writeStat() {
	uint8_t stat = addr.read(STAT);
	stat &= 0b11111000;
	stat &= ((stat2 & 0x01) << 2);
	stat |= (state & 0b11);
	addr.write(STAT, stat, false);
}

void PPU::readStat() {
	uint8_t stat = addr.read(STAT) >> 3;
	stat2 = stat & (1 < 2);
	stat3 = stat & (1 < 3);
	stat4 = stat & (1 < 4);
	stat5 = stat & (1 < 5);
	stat6 = stat & (1 < 6);
}

void PPU::updateMode() {
	uint8_t stat = (addr.read(STAT) >> 2) << 2;
	stat |= state;
	addr.write(STAT, stat, false);
}

void PPU::setLYCFlag() {
	uint8_t scanline = get_scanline();
	uint8_t lyc = addr.read(LYC);
	uint8_t iff = addr.read(IF);

	if (stat2 && lyc != scanline) {
		stat2 = false;
	}

	if (lyc == scanline) {
		stat2 = true;
		if (stat6) {
			set_stat_interrupt();
		}
	}

	writeStat();

}


uint8_t PPU::get_scx() {
	return addr.read(SCX);
}

uint8_t PPU::get_scy() {
	return addr.read(SCY);
}

void PPU::read_lcdc() {
	uint8_t uint8_t = addr.read(LCDC);
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
	uint8_t val = addr.read(IF);
	addr.write(IF, val | 1, false);
}


void PPU::set_stat_interrupt() {
	printf("triggered stat interrupt\n");
	uint8_t val = addr.read(IF);
	addr.write(IF, (val << 1) | 1, false);
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

	int currentScanline = addr.read(LY);
	int scrollX = addr.read(SCX);
	int scrollY = addr.read(SCY);

	if (!lcdc0) {
		return;
	}

	uint16_t tile_map_address = lcdc3 ? 0x9C00 : 0x9800;

	for (int x = 0; x < 160; x++) {

		int bgX = (scrollX + x) % 256;
		int bgY = (scrollY + currentScanline) % 256;


		int tileX = bgX / 8;
		int tileY = bgY / 8;


		int mapIndex = tileY * 32 + tileX;
		uint8_t tile_index = addr.read(mapIndex + tile_map_address);

		uint16_t tiledatabase = (lcdc4) ? 0x8000 : 0x8800;
		uint16_t tileAddress = (lcdc4) ? (tiledatabase + tile_index * 16) : (tiledatabase + (uint8_t(tile_index) + 128) * 16);

		int lineInTile = bgY % 8;
		int xinTile = bgX % 8;

		uint8_t blow = addr.read(tileAddress + lineInTile * 2);
		uint8_t bhigh = addr.read((tileAddress + lineInTile * 2 + 1));

		int bitIndex = 7 - xinTile;

		uint8_t bitlow = get_bit(blow, bitIndex);
		uint8_t bithigh = get_bit(bhigh, bitIndex);

		uint8_t combined = bithigh << 1 | bitlow;

		uint8_t palette = addr.read(0xFF47);

		uint8_t finalColor = (palette >> (combined * 2)) & 0x03;


		switch (finalColor) {
		case 0:
			scanline_buffer[x] = PIXEL::GREEN0;
			break;
		case 1:
			scanline_buffer[x] = PIXEL::GREEN1;
			break;
		case 2:
			scanline_buffer[x] = PIXEL::GREEN2;
			break;
		case 3:
			scanline_buffer[x] = PIXEL::GREEN3;
			break;
		default:
			printf("wtf color fucked\n");
			break;
		}
	}
}


//check if it reaches the window yet 
void PPU::renderWindow() {
	read_lcdc();

	if (lcdc5 == 0) return;

	printf("WX: %d, WY: %d\n", addr.read(WX), addr.read(WY));

	int currentScanline = addr.read(LY);
	int windowX = addr.read(WX) - 7;

	int windowY = addr.read(WY);

	if (currentScanline < windowY) return;

	uint16_t tileMapBase = (lcdc6 != 0) ? (uint16_t)0x9C00 : (uint16_t)0x9800;

	bool windowRendered = false;

	for (int x = 0; x < 160; x++)
	{
		if (x < windowX) continue;

		windowRendered = true;

		int windowColumn = x - windowX;

		int tileX = windowColumn / 8;
		int tileY = windowLineCounter / 8;

		int tileIndex = tileY * 32 + tileX;

		uint8_t tileNumber = addr.read((uint16_t)(tileMapBase + tileIndex));

		uint16_t tileDataBase = (lcdc4) != 0 || tileNumber >= 128 ? (uint16_t)0x8000 : (uint16_t)0x9000;
		uint16_t tileAddress = (uint16_t)(tileDataBase + tileNumber * 16);

		int lineInTile = windowLineCounter % 8;

		uint8_t tileLow = addr.read((uint16_t)(tileAddress + lineInTile * 2));
		uint8_t tileHigh = addr.read((uint16_t)(tileAddress + lineInTile * 2 + 1));

		int bitIndex = 7 - (windowColumn % 8);
		int colorBit = ((tileHigh >> bitIndex) & 1) << 1 | ((tileLow >> bitIndex) & 1);

		uint8_t bgp = addr.read(BGP);
		int paletteShift = colorBit * 2;
		int paletteColor = (bgp >> paletteShift) & 0b11;
		switch (paletteColor) {
		case 0:
			scanline_buffer[x] = PIXEL::GREEN0;
			break;
		case 1:
			scanline_buffer[x] = PIXEL::GREEN1;
			break;
		case 2:
			scanline_buffer[x] = PIXEL::GREEN2;
			break;
		case 3:
			scanline_buffer[x] = PIXEL::GREEN3;
			break;
		default:
			printf("wtf color fucked\n");
			break;
		}
	}

	if (windowRendered) {
		windowLineCounter++;
	}
}




void PPU::renderSprite() {
	read_lcdc();
	int currentScanline = addr.read(LY);
	if (lcdc1 == 0) return;

	int renderedSprites = 0;
	std::array<int, 160> pixelOwner;
	std::fill(pixelOwner.begin(), pixelOwner.end(), -1);
	for (int i = 0; i < 40; i++) {
		if (renderedSprites >= 10) break;

		int spriteIndex = i * 4;
		int yPos = addr.read((uint16_t)(0xFE00 + spriteIndex)) - 16;
		int xPos = addr.read((uint16_t)(0xFE00 + spriteIndex + 1)) - 8;
		uint8_t tileIndex = addr.read((uint16_t)(0xFE00 + spriteIndex + 2));
		uint8_t attributes = addr.read((uint16_t)(0xFE00 + spriteIndex + 3));

		int spriteHeight = (lcdc2) != 0 ? 16 : 8;
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
		uint8_t tileLow = addr.read(tileAddress);
		uint8_t tileHigh = addr.read((uint16_t)(tileAddress + 1));

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

				uint8_t spritePalette = isSpritePalette1 ? addr.read(OBP1) : addr.read(OBP0);
				int paletteShift = colorBit * 2;
				int paletteColor = (spritePalette >> paletteShift) & 0b11;

				switch (paletteColor) {
				case 0:
					scanline_buffer[screenX] = PIXEL::GREEN0;
					break;
				case 1:
					scanline_buffer[screenX] = PIXEL::GREEN1;
					break;
				case 2:
					scanline_buffer[screenX] = PIXEL::GREEN2;
					break;
				case 3:
					scanline_buffer[screenX] = PIXEL::GREEN3;
					break;
				default:
					printf("wtf color fucked\n");
					break;
				}
			}
		}
		renderedSprites++;
	}
}

void PPU::resetBuffers() {
	scanline_buffer.fill(PIXEL::GREEN0);
	for (int i = 0; i < 144; i++) {
		framebuffer[i].fill(PIXEL::GREEN0);
	}
}

uint8_t PPU::get_scanline()
{
	return addr.read(LY);
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

	BACKFIFO = 0;
	SPRITEFIFO = 0;
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

			if (addr.read(LY) == 144) {
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

				state = PPUSTATE::OAM;
			}

		}
		updateMode();
		break;
	case VLANK:
		if (!vblank_triggerable && addr.read(LY) == 144)
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

			if (addr.read(LY) == 153) {
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

			if (addr.read(LY) < 144) {
				renderScanline(addr.read(LY));
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

std::array<std::array<PIXEL, 160>, 144> PPU::getDisplay()
{
	return framebuffer;
}


