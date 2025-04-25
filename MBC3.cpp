#include "MBC3.h"



MBC3::MBC3(std::string a) : MBC(a) {
	std::fill(ram.begin(), ram.end(), 0);
}

uint8_t MBC3::read(uint16_t address)
{
	//check if its ram or rom or whatever.
	if (address >= 0x4000 && address <= 0x7FFF) {
		return bytes[address + romOffset];
	}
	else if (address >= 0xA000 && address < 0xBFFF) {
		uint16_t add = address - 0xA000;
		return ram[ramOffset + add];
	}
	else if (address <= 0x3FFF) {
		return bytes[address];
	}
	else {
		return 0xFF;
	}
}

void MBC3::write(uint16_t address, uint8_t val)
{
	//register writes
	if (address <= 0x1FFF) {
		if (val == 0x0A)
			writeable = true;
		else
			writeable = false;
		return;
	}
	if (address >= 0x2000 && address <= 0x3FFF) {
		if (val == 0x00) {
			romOffset = 16384;
		}
		else {
			romOffset = val * 16384;
		}
		return;
	}
	if (address >= 0x4000 && address <= 0x5FFF) {
		//change RAM bank number
		ramOffset = val * 8192;
		return;
	}

	//check not in rom and if ram its writeable as well

	if (address >= 0xA000 && address <= 0xBFFF && writeable && ramOffset < 32768) {
		uint16_t add = address - 0xA000;
		ram[add + ramOffset] = val;
	}

	return;
}
