#pragma once
#include <cstdint>

const int SIZE = 0x10000;
class AddressSpace{
public:
	uint8_t read(uint16_t address);
	void readRom(std::string file_path);
	void write(uint16_t address, uint8_t value);
	AddressSpace();

private:
	uint8_t memory[SIZE];
};

