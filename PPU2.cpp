#include "PPU2.h"
#include "helpers.h"

PIXEL PPU2::int_to_pixel(uint8_t pixel,uint16_t palAdd)
{

	uint8_t palette = addr.read(palAdd);
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

void PPU2::set_stat_interrupt() {
	//printf("triggered stat interrupt\n");
	uint8_t val = addr.read(IF);
	addr.write(IF, set_bit(val,1,true), false);
}

void PPU2::set_vblank_interrupt() {
	uint8_t val = addr.read(IF);
	addr.write(IF, val | 1, false);
}


void PPU2::setLCDSTATUS(){
	uint8_t ly = addr.read(LY);
	uint8_t status = addr.read(STAT);
	if (!isLCDENABLED())
	{
		// set the mode to 1 during lcd disabled and reset scanline
		m_ScanlineCounter = 456;
		addr.write(0xFF44, 0);
		status &= 252;
		status = set_bit(status, 0,true);
		addr.write(0xFF41, status);
		return;
	}

	uint8_t currentline = addr.read(0xFF44);
	uint8_t currentmode = status & 0x3;

	uint8_t mode = 0;
	bool reqInt = false;

	// in vblank so set mode to 1
	if (currentline >= 144)
	{
		mode = 1;
		status = set_bit(status, 0,true);
		status = set_bit(status, 1,false);
		reqInt = get_bit(status, 4);
	}
	else
	{
		int mode2bounds = 456 - 80;
		int mode3bounds = mode2bounds - 172;

		// mode 2
		if (m_ScanlineCounter >= mode2bounds)
		{
			mode = 2;
			status = set_bit(status, 1,true);
			status = set_bit(status, 0,false);
			reqInt = get_bit(status, 5);
		}
		// mode 3
		else if (m_ScanlineCounter >= mode3bounds)
		{
			mode = 3;
			status = set_bit(status, 1, false);
			status = set_bit(status, 0, false);
		}
		// mode 0
		else
		{
			mode = 0;
			status = set_bit(status, 1, false);
			status = set_bit(status, 0, false);
			reqInt = get_bit(status, 3);
		}
	}

	// just entered a new mode so request interupt
	if (reqInt && (mode != currentmode))
		set_stat_interrupt();

	// check the conincidence flag
	if (ly == addr.read(0xFF45))
	{
		status = set_bit(status, 2,true);
		if (get_bit(status, 6))
			set_vblank_interrupt();
	}
	else
	{
		status = set_bit(status, 2, false);
	}
	addr.write(0xFF41, status);

}

bool PPU2::isLCDENABLED()
{
	return get_bit(addr.read(LCDC), 7);
}



PPU2::PPU2(AddressSpace& a) : addr(a)
{

}

std::array<std::array<PIXEL, 160>, 144> PPU2::get_display()
{
	return frame_buffer;
}

void PPU2::resetBuffers(){
	scanline_buffer.fill(PIXEL::GREEN0);
	for (int i = 0; i < 144; i++) {
		frame_buffer[i].fill(PIXEL::GREEN0);
	}
}

void PPU2::step(uint8_t cycles){
	setLCDSTATUS();

	if (isLCDENABLED())
		m_ScanlineCounter -= cycles;
	else
		return;

	if (m_ScanlineCounter <= 0)
	{
		addr.incr(LY);
		uint8_t currentline = addr.read(LY);

		m_ScanlineCounter = 456;

		if (currentline == 144)
			set_vblank_interrupt();

		else if (currentline > 153)
			addr.write(LY, 0);


		else if (currentline < 144)
			renderScanline();
	}

}


void PPU2::renderScanline(){
	uint8_t control = addr.read(0xFF40);
	uint8_t ly = addr.read(LY);
	if (get_bit(control, 7)) {
		if (get_bit(control, 0))
			renderTiles(control);

		if (get_bit(control, 1))
			renderSprites(control);
	}
	std::copy(scanline_buffer.begin(), scanline_buffer.end(), frame_buffer[ly].begin());
}

void PPU2::renderTiles(uint8_t lcdControl) {
	uint16_t tileData = 0;
	uint16_t backgroundMemory = 0;
	bool unsig = true;

	// where to draw the visual area and the window
	uint8_t scrollY = addr.read(SCY);
	uint8_t scrollX = addr.read(SCX);
	uint8_t windowY = addr.read(WY);
	uint8_t windowX = addr.read(WX) - 7;

	bool usingWindow = false;

	// is the window enabled?
	if (get_bit(lcdControl, 5))
	{
		// is the current scanline we're drawing
		// within the windows Y pos?,
		if (windowY <= addr.read(0xFF44))
			usingWindow = true;
	}

	// which tile data are we using?
	if (get_bit(lcdControl, 4))
	{
		tileData = 0x8000;
	}
	else
	{
		// IMPORTANT: This memory region uses signed
		// uint8_ts as tile identifiers
		tileData = 0x8800;
		unsig = false;
	}

	// which background mem?
	if (false == usingWindow)
	{
		if (get_bit(lcdControl, 3))
			backgroundMemory = 0x9C00;
		else
			backgroundMemory = 0x9800;
	}
	else
	{
		// which window memory?
		if (get_bit(lcdControl, 6))
			backgroundMemory = 0x9C00;
		else
			backgroundMemory = 0x9800;
	}

	uint8_t yPos = 0;

	// yPos is used to calculate which of 32 vertical tiles the
	// current scanline is drawing
	if (!usingWindow)
		yPos = scrollY + addr.read(0xFF44);
	else
		yPos = addr.read(0xFF44) - windowY;

	// which of the 8 vertical pixels of the current
	// tile is the scanline on?
	uint16_t tileRow = (((uint8_t)(yPos / 8)) * 32);

	// time to start drawing the 160 horizontal pixels
	// for this scanline
	for (int pixel = 0; pixel < 160; pixel++)
	{
		uint8_t xPos = pixel + scrollX;

		// translate the current x pos to window space if necessary
		if (usingWindow)
		{
			if (pixel >= windowX)
			{
				xPos = pixel - windowX;
			}
		}

		// which of the 32 horizontal tiles does this xPos fall within?
		uint16_t tileCol = (xPos / 8);
		int16_t tileNum;

		// get the tile identity number. Remember it can be signed
		// or unsigned
		uint16_t tileAddrss = backgroundMemory + tileRow + tileCol;
		if (unsig)
			tileNum = (uint8_t)addr.read(tileAddrss);
		else
			tileNum = (int16_t)addr.read(tileAddrss);

		// deduce where this tile identifier is in memory. Remember i
		// shown this algorithm earlier
		uint16_t tileLocation = tileData;

		if (unsig)
			tileLocation += (tileNum * 16);
		else
			tileLocation += ((tileNum + 128) * 16);

		// find the correct vertical line we're on of the
		// tile to get the tile data
		//from in memory
		uint8_t line = yPos % 8;
		line *= 2; // each vertical line takes up two uint8_ts of memory
		uint8_t data1 = addr.read(tileLocation + line);
		uint8_t data2 = addr.read(tileLocation + line + 1);

		// pixel 0 in the tile is it 7 of data 1 and data2.
		// Pixel 1 is bit 6 etc..
		int colourBit = xPos % 8;
		colourBit -= 7;
		colourBit *= -1;

		// combine data 2 and data 1 to get the colour id for this pixel
		// in the tile
		int colourNum = get_bit(data2, colourBit);
		colourNum <<= 1;
		colourNum |= get_bit(data1, colourBit);

		// now we have the colour id get the actual
		// colour from palette 0xFF47
		PIXEL col = int_to_pixel(colourNum, 0xFF47);

		int finaly = addr.read(0xFF44);

		// safety check to make sure what im about
		// to set is int the 160x144 bounds
		if ((finaly < 0) || (finaly > 143) || (pixel < 0) || (pixel > 159))
		{
			continue;
		}

		scanline_buffer[pixel] = col;
	}
}

void PPU2::renderSprites(uint8_t lcdControl){
	bool use8x16 = false;
	if (get_bit(lcdControl, 2))
		use8x16 = true;

	for (int sprite = 0; sprite < 40; sprite++)
	{
		uint8_t index = sprite * 4;
		uint8_t yPos = addr.read(0xFE00 + index) - 16;
		uint8_t xPos = addr.read(0xFE00 + index + 1) - 8;
		uint8_t tileLocation = addr.read(0xFE00 + index + 2);
		uint8_t attributes = addr.read(0xFE00 + index + 3);

		bool yFlip = get_bit(attributes, 6);
		bool xFlip = get_bit(attributes, 5);

		int scanline = addr.read(0xFF44);

		int ysize = 8;

		if (use8x16)
			ysize = 16;

		if ((scanline >= yPos) && (scanline < (yPos + ysize)))
		{
			int line = scanline - yPos;

			if (yFlip)
			{
				line -= ysize;
				line *= -1;
			}

			line *= 2;
			uint8_t data1 = addr.read((0x8000 + (tileLocation * 16)) + line);
			uint8_t data2 = addr.read((0x8000 + (tileLocation * 16)) + line + 1);



			for (int tilePixel = 7; tilePixel >= 0; tilePixel--)
			{
				int colourbit = tilePixel;
				if (xFlip)
				{
					colourbit -= 7;
					colourbit *= -1;
				}
				int colourNum = get_bit(data2, colourbit);
				colourNum <<= 1;
				colourNum |= get_bit(data1, colourbit);

				PIXEL col = int_to_pixel(colourNum, get_bit(attributes, 4) ? 0xFF49 : 0xFF48);

				// white is transparent for sprites.
				if (col == GREEN0)
					continue;


				int xPix = 0 - tilePixel;
				xPix += 7;

				int pixel = xPos + xPix;

				if ((scanline < 0) || (scanline > 143) || (pixel < 0) || (pixel > 159))
				{
					//	assert(false) ;
					continue;
				}

				// check if pixel is hidden behind background
				if (get_bit(attributes, 7) == 1)
				{
					if ((scanline_buffer[pixel] != GREEN0) || (scanline_buffer[pixel] != GREEN0) || (scanline_buffer[pixel] != GREEN0))
						continue;
				}

				scanline_buffer[pixel] = col;

			}
		}
	}
}
