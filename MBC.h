#pragma once
#include <vector>
#include <string>
class MBC {
public:
	virtual uint8_t read(uint16_t address);
	virtual void write(uint16_t address, uint8_t value);
	MBC(std::string romPath);
protected:
	std::vector<uint8_t> bytes;
};