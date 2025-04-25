#include "PPU.h"
#include "helpers.h"
#include "FIFO.h"


#define SCANLINE_CYCLES 456

//void PPU::OAMScan(){
//	//fetch oam
//	sprite_buffer.fill(Sprite(0));
//	oam_buffer = addr.getOAM();
//	auto it = sprite_buffer.begin();
//	for (int i = 0; i < 40; i ++) {
//		int sprite;
//		std::memcpy(&sprite, &oam_buffer[i * 4], 4);
//		if (sprite == 0) continue;
//		Sprite s(sprite);
//		if (s.xpos > 0 && get_scanline() + 16 >= s.ypos && get_scanline() + 16 < s.ypos + 8 * (1 + SPRSZE) && it != sprite_buffer.end()) {
//			*it = s;
//			++it;
//		}
//	}
//}



//tile data:
uint16_t get_tile_ptr(uint8_t tilenumber, bool lcd4) {
	if (lcd4) {
		uint16_t bp = 0x8000;
		uint16_t start = bp + 16 * tilenumber;
		return start;
	}
	else {
		int8_t sig = (int8_t)tilenumber;
		uint16_t bp = 0x9000;
		return bp + (sig * 16);
	}
}


//thhis will given a 

std::array<std::array<uint8_t,32>,32> PPU::get_background_map(bool is9800) {
	std::array<std::array<uint8_t, 32>, 32> map;
	auto ve = is9800 ? addr.get_range(0x9800, 0x9BFF) : addr.get_range(0x9C00, 0x9FFF);
	for (int y = 0; y < 32; ++y) {
		for (int x = 0; x < 32; ++x) {
			map[y][x] = ve[y * 32 + x];
		}
	}
	return map;
}


void PPU::writeStat() {
	uint8_t stat = addr.read(STAT);
	stat &= 0b11111000;
	stat |= (state & 0b11);              
	stat &= ((stat2 & 0x01) << 2); 
	addr.write(STAT, stat,false);
}

void PPU::readStat(){
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

	if (stat2 && lyc != scanline) {
		stat2 = false;
	}

	if (lyc == scanline) {
		stat2 = true;
	}

	writeStat();
}

//void PPU::execute_loop(){
//	while (action) {
//		int cycleCount = clock.get_cycle();
//		for (int i = 0; i < 144; i++) {
//			//oam scan
//			readStat();
//			addr.setCpuWriteable(false);
//			state = PPUSTATE::OAM; 
//			if (MODE2ENABLE) 
//				set_stat_interrupt();
//
//			block_n_cycles(80);
//
//			//drawing
//			readStat();
//			state = PPUSTATE::DRAW; renderScanline(i);
//			block_n_cycles(172);
//
//			//hblank
//			readStat();
//			setLYCFlag();
//			addr.setCpuWriteable(true);
//			addr.incr(LY);
//			state = PPUSTATE::HBLANK; 
//			if (MODE0ENABLE)
//				set_stat_interrupt();
//			block_n_cycles(204);
//		}
//		//vblank
//		readStat();
//		state = PPUSTATE::VLANK; 
//		if (MODE1ENABLE)
//			set_stat_interrupt();
//		set_vblank_interrupt();
//		addr.write(LY, 0);
//		block_n_cycles(70224 - (clock.get_cycle() - cycleCount));
//	}
//}

uint8_t PPU::get_scx(){
	return addr.read(SCX);
}

uint8_t PPU::get_scy(){
	return addr.read(SCY);
}

void PPU::read_lcdc(){
	uint8_t byte = addr.read(LCDC);
	lcdc0 = byte & 1;
	lcdc1 = byte & (1 << 1);
	lcdc2 = byte & (1 << 2);
	lcdc3 = byte & (1 << 3);
	lcdc4 = byte & (1 << 4);
	lcdc5 = byte & (1 << 5);
	lcdc6 = byte & (1 << 6);
	lcdc7 = byte & (1 << 7);
}

void PPU::set_vblank_interrupt(){
	uint8_t val = addr.read(IF);
	addr.write(IF, val | 1, false);
}


void PPU::set_stat_interrupt(){
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

void PPU::renderBackground(){
	read_lcdc();

	int currentScanline = addr.read(LY);
	int scrollX = addr.read(SCX);
	int scrollY = addr.read(SCY);

	int nonZeroPixels = 0;

	if (!lcdc0) {
		//printf("background render disabeld?\n");
		return;
	}

	//printf("current scanline: %d scroll x: %d scroll y: %d\n", currentScanline, scrollX, scrollY);

	uint16_t tile_map_address = lcdc3 ? 0x9C00 : 0x9800;

	for (int x = 0; x < 160; x++) {

		int bgX = (scrollX + x) % 256;
		int bgY = (scrollY + currentScanline) % 256;


		//printf("calculating x: %d, y: %d\n", bgX, bgY);

		int tileX = bgX / 8;
		int tileY = bgY / 8;

		//printf("fetcherX %d: fetcher Y %d", tileX, tileY);

		int mapIndex = tileY * 32 + tileX;
		uint8_t tile_index = addr.read(mapIndex + tile_map_address);

		//printf("retrieved tile index: %d\n", tile_index);

		uint16_t tiledatabase = (lcdc4) ? 0x8000 : 0x8800;
		uint16_t tileAddress = (lcdc4) ? (tiledatabase + tile_index * 16) : (tiledatabase + (int8_t(tile_index) * 16));

		int lineInTile = bgY % 8;
		int xinTile = bgX % 8;

		uint8_t blow = addr.read(tileAddress + lineInTile * 2);
		uint8_t bhigh = addr.read((tileAddress + lineInTile * 2 + 1));

		int bitIndex = 7 - xinTile;

		uint8_t bitlow = get_bit(blow, bitIndex);
		uint8_t bithigh = get_bit(bhigh, bitIndex);

		uint8_t combined = bithigh << 1 | bitlow;
		//printf("pixel (%d,%d) is colored: %d\n", x,currentScanline, combined);

		uint8_t palette = addr.read(0xFF47);

		if (combined != 0) {
			nonZeroPixels++;
		}
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
		//printf("pallette converted to %d\n", scanline_buffer[x]);
	}
}

void PPU::renderWindow()
{
	read_lcdc();
	bool use_tile_set_zero = lcdc4;
	bool use_tile_map_zero = lcdc6;

	if (!lcdc5) return;  // If window rendering is disabled, exit early

	uint16_t tile_set_address = use_tile_set_zero
		? 0x8000  // Window uses tile set 0, this should be 0x8000, not 0x9C00
		: 0x8800;  // Tile set 1

	uint16_t tile_map_address = use_tile_map_zero
		? 0x9C00  // Window map is at 0x9C00
		: 0x9800;  // Tile map 1

	uint8_t screen_y = addr.read(LY);
	uint8_t scrolled_y = screen_y - addr.read(WY);  // Subtract WY to get position relative to window start

	// If the current scanline is less than WY, don't render the window
	if (scrolled_y >= 144 || scrolled_y < 0) {
		return;
	}

	for (uint8_t screen_x = 0; screen_x < 144; screen_x++) {
		// Adjust for the window's X position using WX and subtract 7 (hardware bug)
		uint8_t scrolled_x = screen_x + addr.read(WX) - 7;

		// Skip rendering pixels that are outside the window's horizontal range
		if (scrolled_x < 0 || scrolled_x >= 160) {
			continue;
		}

		// Calculate the tile's X and Y coordinates
		uint8_t tile_x = scrolled_x / 8;
		uint8_t tile_y = scrolled_y / 8;

		// Find the index of the tile in the map
		uint8_t tile_index = tile_y * 32 + tile_x;
		uint16_t tile_id_address = tile_map_address + tile_index;

		// Get the tile ID from the map
		uint8_t tile_id = addr.read(tile_id_address);

		// Calculate the offset to the tile data in memory
		uint16_t tile_data_offset = use_tile_set_zero
			? tile_id * 16
			: (static_cast<int8_t>(tile_id) + 128) * 16;

		// Calculate the offset within the tile for the pixel we're rendering
		uint8_t tile_pixel_y = scrolled_y % 8;
		uint16_t tile_data_line_address = tile_set_address + tile_data_offset + tile_pixel_y * 2;

		// Fetch the tile data for the current row of the tile
		uint8_t pixels_1 = addr.read(tile_data_line_address);
		uint8_t pixels_2 = addr.read(tile_data_line_address + 1);

		// Get the pixel we need based on the X position within the tile
		int bitIndex = 7 - (scrolled_x % 8);

		uint8_t bitlow = get_bit(pixels_1, bitIndex);
		uint8_t bithigh = get_bit(pixels_2, bitIndex);

		uint8_t combined = bithigh << 1 | bitlow;

		uint8_t palette = addr.read(0xFF47);  // Read the palette data

		uint8_t finalColor = (palette >> (combined * 2)) & 0x03;

		// Set the final pixel color based on the palette
		switch (finalColor) {
		case 0:
			scanline_buffer[screen_x] = PIXEL::GREEN0;
			break;
		case 1:
			scanline_buffer[screen_x] = PIXEL::GREEN1;
			break;
		case 2:
			scanline_buffer[screen_x] = PIXEL::GREEN2;
			break;
		case 3:
			scanline_buffer[screen_x] = PIXEL::GREEN3;
			break;
		default:
			printf("Invalid color value!\n");
			break;
		}
	}
}


void PPU::renderSprite(){
	if (lcdc1 == 0) {
		//printf("sprite render disabeld\n");
		return;
	}

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

		int spriteHeight = (lcdc2) != 0 ? 16 : 8;
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
					printf("color pixel fucked ggs\n");
					break;
				}

			}
		}
		renderedSprites++;
	}
}

void PPU::resetBuffers(){
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
	std::copy(vram_buffer.begin() + startdex, vram_buffer.begin()+startdex + 16, tile.begin());
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

//
//void PPU::step(uint8_t cycles) {
//	// have some calculation first
//	//each LY takes say 339 cycles // we stop doing anything after cyclew 70224
//	if (cycle_counter + cycles > 70224) {
//		if (debug) {
//			printf("ppu has finished the frame\n");
//		}
//		return;
//	}
//	auto ly_cycle_count = (cycle_counter + cycles) % 339;
//	auto mod_curr = cycle_counter % 339;
//	auto ly = addr.read(LY);
//
//	if (ly > 153) {
//		if (debug) {
//			printf("cycle counter is %d executing %d cycles ", cycle_counter, cycles);
//		}
//		return;
//	}
//
//
//	if (debug) {
//		printf("cycle counter is %d executing %d cycles ", cycle_counter, cycles);
//	}
//
//
//
//	//add some extra counter to allow oam to not just happen every frame of oamscan.
//	if (ly > 143) {
//		if (ly == 144 && vblank_triggerable) {
//			state = PPUSTATE::VLANK;
//			set_vblank_interrupt();
//			vblank_triggerable = false;
//		}
//		if (ly_cycle_count < cycle_counter % 339) {
//			addr.incr(LY);
//		}
//	}
//	else if (mod_curr < DRAWSTART && oamtriggerable) {
//		readStat();
//		addr.setCpuWriteable(false);
//		state = PPUSTATE::OAM;
//		if (MODE2ENABLE)
//			set_stat_interrupt();
//		oamtriggerable = false;
//	}
//	else if (mod_curr < DRAWSTART && ly_cycle_count >= DRAWSTART) {
//		readStat();
//		state = PPUSTATE::DRAW;
//		int i = addr.read(LY);
//		renderScanline(i);
//	}
//	else if (mod_curr < HBLANKSTART && ly_cycle_count >= HBLANKSTART) {
//		readStat();
//		setLYCFlag();
//		addr.setCpuWriteable(true);
//		addr.incr(LY);
//		state = PPUSTATE::HBLANK;
//		if (MODE0ENABLE)
//			set_stat_interrupt();
//		oamtriggerable = true;
//		vblank_triggerable = true;
//	}
//
//	printf("state: %s\n", to_string(state));
//
//	cycle_counter += cycles;
//}


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




//there are 3 layers the background 32x32 the window which is a 32 x 32 above it.

//int currentScanline = addr.read(LY);
//printf("rendering scanline %d\n", currentScanline);
//int scrollx = get_scx();
//int scrolly = get_scy();
//
//if (lcdc0 == false) {
//	printf("rendering background is set disable\n");
//	return;
//}
//
//for (int i = 0; i < 160; i++) {
//	int bgX = (scrollx + i) % 256;
//	int bgY = (scrolly + currentScanline) % 256;
//
//	int tileX = bgX / 8;
//	int tileY = bgY / 8;
//
//	int tileIndex = tileY * 32 + tileX;
//
//	uint16_t tileMapBase = (lcdc3) != 0 ? 0x9C00 : 0x9800;
//	uint8_t tileNumber = addr.read(tileMapBase + tileIndex);
//
//	uint16_t tileDataBase = lcdc4 ? 0x8000 : 0x8800;
//	int16_t tileAddress = lcdc4
//		? tileDataBase + tileNumber * 16
//		: tileDataBase + static_cast<int8_t>(tileNumber) * 16;
//	int lineInTile = bgY % 8;
//
//	uint8_t tileLow = addr.read(tileAddress + lineInTile * 2);
//	uint8_t tileHigh = addr.read(tileAddress + lineInTile * 2 + 1);
//
//	int bitIndex = 7 - (bgX % 8);
//	int colorBit = ((tileHigh >> bitIndex) & 0b1) << 1 | ((tileLow >> bitIndex) & 0b1);
//
//	uint8_t bgp = addr.read(BGP);
//
//	int paletteShift = colorBit * 2;
//	int paletteColor = (bgp >> paletteShift) & 0b11;
//	printf("Tile data at line %d: %02x %02x\n", lineInTile, tileLow, tileHigh);
//	printf("bgp: %s\n", to_string(bgp));
//
//	switch (paletteColor) {
//	case 0:
//		scanline_buffer[i] = PIXEL::GREEN0;
//		break;
//	case 1:
//		scanline_buffer[i] = PIXEL::GREEN1;
//		break;
//	case 2:
//		scanline_buffer[i] = PIXEL::GREEN2;
//		break;
//	case 3:
//		scanline_buffer[i] = PIXEL::GREEN3;
//		break;
//	default:
//		printf("color pixel fucked ggs\n");
//	}
//}