#include "PPU.h"
#include "helpers.h"
#include "FIFO.h"

void PPU::OAMScan(){
	//fetch oam
	sprite_buffer.fill(Sprite(0));
	oam_buffer = addr.getOAM();
	auto it = sprite_buffer.begin();
	for (int i = 0; i < 40; i ++) {
		int sprite;
		std::memcpy(&sprite, &oam_buffer[i * 4], 4);
		if (sprite == 0) continue;
		Sprite s(sprite);
		if (s.xpos > 0 && get_scanline() + 16 >= s.ypos && get_scanline() + 16 < s.ypos + 8 * (1 + SPRSZE) && it != sprite_buffer.end()) {
			*it = s;
			++it;
		}
	}
}


void PPU::writeStat() {
	uint8_t stat = addr.read(STAT);
	stat &= 0b11111000;
	stat |= (state & 0b11);              
	stat |= ((COINCIDENCE & 0x01) << 2); 
	addr.write(STAT, stat);
}

void PPU::readStat(){
	uint8_t stat = addr.read(STAT) >> 3;
	MODE0ENABLE = stat & 1;
	MODE1ENABLE = stat & (1 < 1);
	MODE2ENABLE = stat & (1 < 2);
	LYCLYENABLE = stat & (1 < 3);
}

void PPU::updateMode() {
	uint8_t stat = (addr.read(STAT) >> 2) << 2;
	stat |= state;
	addr.write(STAT, stat);
}

void PPU::setLYCFlag() {
	uint8_t scanline = get_scanline();
	uint8_t lyc = addr.read(LYC);

	if (COINCIDENCE && lyc != scanline) {
		COINCIDENCE = false;
	}

	if (lyc == scanline) {
		COINCIDENCE = true;
	}

	writeStat();
}

void PPU::execute_loop(){
	while (action) {
		int cycleCount = clock.get_cycle();
		for (int i = 0; i < 144; i++) {
			//oam scan
			readStat();
			addr.setCpuWriteable(false);
			state = PPUSTATE::OAM; 
			if (MODE2ENABLE) 
				set_stat_interrupt();

			block_n_cycles(80);

			//drawing
			readStat();
			state = PPUSTATE::DRAW; renderScanline(i);
			block_n_cycles(172);

			//hblank
			readStat();
			setLYCFlag();
			addr.setCpuWriteable(true);
			addr.incr(LY);
			state = PPUSTATE::HBLANK; 
			if (MODE0ENABLE)
				set_stat_interrupt();
			block_n_cycles(204);
		}
		//vblank
		readStat();
		state = PPUSTATE::VLANK; 
		if (MODE1ENABLE)
			set_stat_interrupt();
		set_vblank_interrupt();
		addr.write(LY, 0);
		block_n_cycles(70224 - (clock.get_cycle() - cycleCount));
	}
}

uint8_t PPU::get_scx(){
	return addr.read(SCX);
}

uint8_t PPU::get_scy(){
	return addr.read(SCY);
}

void PPU::set_vblank_interrupt(){
	uint8_t val = addr.read(IF);
	addr.write(IF, val | 1);
}


void PPU::set_stat_interrupt(){
	uint8_t val = addr.read(IF);
	addr.write(IF, (val << 1) | 1);
}

void PPU::block_n_cycles(int num){
	for (int i = 0; i < num * 2; i++) {
		block_cycles_i();
	}
}

void PPU::block_cycles_i(){
	uint64_t cycle_count = clock.get_cycle();

	while (cycle_count == clock.get_cycle()) {
		std::this_thread::yield();
	}
}

void PPU::fetch_vram(){
	vram_buffer = addr.getVRAM();
}

void PPU::renderScanline(uint8_t scanlineno) {
	renderBackground();
	renderWindow();
	renderSprite();

	std::copy(scanline_buffer.begin(), scanline_buffer.end(), framebuffer[scanlineno].begin());
}

void PPU::renderBackground(){
	int scrollx = get_scx();
	int scrolly = get_scy();

	if (BGENABLE) {
		return;
	}

	for (int i = 0; i < 160; i++) {
		int bgX = (scrollx + i) % 256;
		int bgY = (scrolly + get_scanline()) % 256;

		int tileX = bgX / 8;
		int tileY = bgY / 8;

		int tileIndex = tileY * 32 + tileX;

		uint16_t tileMapBase = (BGSEL) != 0 ? 0x9C00 : 0x9800;
		uint8_t tileNumber = addr.read(tileMapBase + tileIndex);

		uint16_t tileDataBase = TILESEL != 0 || tileNumber >= 128 ? 0x8000 : 0x9000;
		uint16_t tileAddress = tileDataBase + tileNumber * 16;
		int lineInTile = bgY % 8;

		uint8_t tileLow = addr.read(tileAddress + lineInTile * 2);
		uint8_t tileHigh = addr.read(tileAddress + lineInTile * 2 + 1);

		int bitIndex = 7 - (bgX % 8);
		int colorBit = ((tileHigh >> bitIndex) & 0b1) << 1 | ((tileLow >> bitIndex) & 0b1);
		uint8_t bgp = addr.read(BGP);

		int paletteShift = colorBit * 2;
		int paletteColor = (bgp >> paletteShift) & 0b11;

		switch(paletteColor) {
		case 0:
			scanline_buffer[i] = PIXEL::GREEN0;
			return;
		case 1:
			scanline_buffer[i] = PIXEL::GREEN1;
			return;
		case 2:
			scanline_buffer[i] = PIXEL::GREEN2;
			return;
		case 3:
			scanline_buffer[i] = PIXEL::GREEN3;
			return;
		default:
			printf("color pixel fucked ggs\n");
			return;
		}
	}

}

void PPU::renderWindow(){
	if (WINENABLE == 0) {
		return;
	}

	int currentScanline = get_scanline();
	int windowX = addr.read(WX) - 7;
	int windowY = addr.read(WY);

	if (currentScanline < windowY) return;

	if (currentScanline == windowY) {
		windowLineCounter = 0;
	}

	if (currentScanline < windowY) return;

	if (currentScanline == windowY) {
		windowLineCounter = 0;
	}

	uint16_t tileMapBase = (WINMAPSEL) != 0 ? 0x9C00 : 0x9800;

	bool windowRendered = false;

	for (int x = 0; x < 160; x++)
	{
		if (x < windowX) continue;

		windowRendered = true;

		int windowColumn = x - windowX;

		int tileX = windowColumn / 8;
		int tileY = windowLineCounter / 8;

		int tileIndex = tileY * 32 + tileX;

		uint8_t tileNumber = addr.read((tileMapBase + tileIndex));

		uint16_t tileDataBase = (TILESEL) != 0 || tileNumber >= 128 ? (uint16_t)0x8000 : (uint16_t)0x9000;
		uint16_t tileAddress = (uint16_t)(tileDataBase + tileNumber * 16);

		int lineInTile = windowLineCounter % 8;

		uint8_t tileLow = addr.read(tileAddress + lineInTile * 2);
		uint8_t  tileHigh = addr.read(tileAddress + lineInTile * 2 + 1);

		int bitIndex = 7 - (windowColumn % 8);
		int colorBit = ((tileHigh >> bitIndex) & 1) << 1 | ((tileLow >> bitIndex) & 1);

		uint8_t bgp = addr.read(BGP);
		int paletteShift = colorBit * 2;
		int paletteColor = (bgp >> paletteShift) & 0b11;

		switch (paletteColor) {
		case 0:
			scanline_buffer[x] = PIXEL::GREEN0;
			return;
		case 1:
			scanline_buffer[x] = PIXEL::GREEN1;
			return;
		case 2:
			scanline_buffer[x] = PIXEL::GREEN2;
			return;
		case 3:
			scanline_buffer[x] = PIXEL::GREEN3;
			return;
		default:
			printf("color pixel fucked ggs\n");
			return;
		}
	}

	if (windowRendered) {
		windowLineCounter++;
	}
}

void PPU::renderSprite(){
	if (SPRENABLE == 0) return;

	int renderedSprites = 0;
	std::array<int, 160> pixelOwner;
	pixelOwner.fill(-1);

	for (int i = 0; i < 40; i++) {
		if (renderedSprites >= 10) break;

		int spriteIndex = i * 4;
		int yPos = addr.read((uint16_t)(0xFE00 + spriteIndex)) - 16;
		int xPos = addr.read((uint16_t)(0xFE00 + spriteIndex + 1)) - 8;
		uint8_t tileIndex = addr.read((uint16_t)(0xFE00 + spriteIndex + 2));
		uint8_t attributes = addr.read((uint16_t)(0xFE00 + spriteIndex + 3));

		int spriteHeight = (SPRSZE) != 0 ? 16 : 8;
		if (get_scanline() < yPos || get_scanline() >= yPos + spriteHeight) {
			continue;
		}

		int lineInSprite = get_scanline() - yPos;
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
			if (bgOverObj && scanline_buffer[screenX] != PIXEL::GREEN0) {
				continue;
			}

			if (pixelOwner[screenX] == -1 || xPos < pixelOwner[screenX]) {
				pixelOwner[screenX] = xPos;
				bool isSpritePalette1 = (attributes & (1 << 4)) != 0;

				uint8_t spritePalette = isSpritePalette1 ? addr.read(OBP0) : addr.read(OBP1);
				int paletteShift = colorBit * 2;
				int paletteColor = (spritePalette >> paletteShift) & 0b11;

				switch (paletteColor) {
				case 0:
					scanline_buffer[x] = PIXEL::GREEN0;
					return;
				case 1:
					scanline_buffer[x] = PIXEL::GREEN1;
					return;
				case 2:
					scanline_buffer[x] = PIXEL::GREEN2;
					return;
				case 3:
					scanline_buffer[x] = PIXEL::GREEN3;
					return;
				default:
					printf("color pixel fucked ggs\n");
					return;
				}
				
			}
		}
		renderedSprites++;
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
	std::copy(vram_buffer.begin() + startdex, vram_buffer.begin()+startdex + 16, tile.begin());
	return tile;
}

PPU::PPU(AddressSpace& addressSpace, Clock& clock_l) : addr(addressSpace), clock(clock_l) {
	LCDENABLE = false;
	WINMAPSEL = false;
	WINENABLE = false;
	TILESEL = false;
	BGSEL = false;
	SPRSZE = false;
	SPRENABLE = false;
	BGENABLE = false;

	//STAT register - mirror of the address map
	LYCLYENABLE = false;
	MODE2ENABLE = false;
	MODE1ENABLE = false;
	MODE0ENABLE = false;
	COINCIDENCE = false;

	BACKFIFO = 0;
	SPRITEFIFO = 0;
}

void PPU::execute(){
	ppu_thread = std::thread([this] {
		//printf("executing thread loop");
		execute_loop();
	});
}

void PPU::stop(){
	printf("stopping ppu\n");
	action = false;
	if (ppu_thread.joinable()) {
		ppu_thread.join();
	}
	else {
		printf("not joinable wtf\n");
	}
}

std::array<std::array<PIXEL, 160>, 144> PPU::getDisplay()
{
	return framebuffer;
}




//there are 3 layers the background 32x32 the window which is a 32 x 32 above it.

