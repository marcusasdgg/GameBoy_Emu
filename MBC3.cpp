#include "MBC3.h"



MBC3::MBC3(std::string a, std::string savePath) : MBC(a) {
	//printf("length of mbc is %lld\n", bytes.size());
	std::fill(ram.begin(), ram.end(), 0x00);
}

uint8_t MBC3::read(uint16_t address)
{
	//check if its ram or rom or whatever.
	if (address <= 0x3FFF) {
		return bytes[address]; //rom bank 0
	}
	if (address >= 0x4000 && address <= 0x7FFF) {
		address = address - 0x4000;
		return bytes[address + romOffset];
	}
	else if (address >= 0xA000 && address <= 0xBFFF && writeable) {
		int add = address - 0xA000;
		return ram[ramOffset + add];
	}
	else {
		return 0x00;
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
		//val &= 0x7F; // MBC3 uses 7 bits to select ROM banks (0x01–0x7F)

		if (val == 0)
			val = 1; // Bank 0 is not valid here; map to bank 1

		//printf("selecting rom bank %d\n", val);
		romOffset = ((uint64_t)val) * 0x4000;
		return;
	}
	if (address >= 0x4000 && address <= 0x5FFF) {
		//change RAM bank number
		//printf("selecting ram bank %d\n", val);
		if (val > 7)
			return;
		ramOffset = ((uint64_t)val) * 0x2000;
		return;
	}

	//check not in rom and if ram its writeable as well

	if (address >= 0xA000 && address <= 0xBFFF ) {
		if (!writeable) {
			return;
		}
		uint64_t add = (uint64_t)address - 0xA000;
		ram[ramOffset + add] = val;
	}

	return;
}

void MBC3::incr(uint16_t address){
	uint8_t byt = read(address);
	write(address, byt + 1);
}

std::vector<uint8_t> MBC3::saveBytes()
{
	auto butes = MBC::saveBytes();
	butes.push_back(writeable);
	butes.push_back(ramOffset >> 8);
	butes.push_back(ramOffset);

	butes.push_back(romOffset >> 24);
	butes.push_back(romOffset >> 16);
	butes.push_back(romOffset >> 8);
	butes.push_back(romOffset);
	butes.insert(butes.end(), ram.begin(), ram.end());
	return butes;
}

void MBC3::loadSave(std::string romPath){
	uint64_t startIndex = (uint64_t) 23397 + 0xFFFF + 1;
	std::ifstream inputFile(romPath, std::ios::binary);
	std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());
	writeable = buffer[startIndex++];
	ramOffset = buffer[startIndex++] << 8  | buffer[startIndex++];
	romOffset = buffer[startIndex++] << 24 | buffer[startIndex++] << 16 | buffer[startIndex++] << 8 | buffer[startIndex++];
}
