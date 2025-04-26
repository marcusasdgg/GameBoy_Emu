#include "MBC3.h"



MBC3::MBC3(std::string a) : MBC(a) {
	//printf("length of mbc is %lld\n", bytes.size());
	std::fill(ram.begin(), ram.end(), 0);
}

uint8_t MBC3::read(uint16_t address)
{
	//check if its ram or rom or whatever.
	if (address >= 0x4000 && address <= 0x7FFF) {
		address = address - 0x4000;
		return bytes[address + romOffset];
	}
	else if (address >= 0xA000 && address < 0xBFFF && writeable) {
		uint16_t add = address - 0xA000;
		return ram[ramOffset + add];
	}
	else if (address <= 0x3FFF) {
		return bytes[address]; //rom bank 0
	}
	else {
		return 0xFF;
	}
}

void MBC3::write(uint16_t address, uint8_t val)
{
	//register writes
	if (address <= 0x1FFF) {
		if (val & 0xA)
			writeable = true;
		else
			writeable = false;
		return;
	}
	if (address >= 0x2000 && address <= 0x3FFF) {
		val &= 0x7F; // MBC3 uses 7 bits to select ROM banks (0x01–0x7F)

		if (val == 0)
			val = 1; // Bank 0 is not valid here; map to bank 1

		//printf("selecting rom bank %d\n", val);
		romOffset = val * 0x4000;
		return;
	}
	if (address >= 0x4000 && address <= 0x5FFF) {
		//change RAM bank number
		//printf("selecting ram bank %d\n", val);
		if (val > 3)
			return;

		ramOffset = val * 8192;
		return;
	}

	//check not in rom and if ram its writeable as well

	if (address >= 0xA000 && address <= 0xBFFF && writeable && ramOffset < 32768) {
		uint16_t add = address - 0xA000;
		//printf("writing to ram bank %d", ramOffset / 8192);
		ram[add + ramOffset] = val;
	}

	return;
}

void MBC3::incr(uint16_t address){
	uint8_t byt = read(address);
	write(address, byt + 1);
}
