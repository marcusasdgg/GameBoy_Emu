#pragma once
#include <vector>
#include <string>
#include <fstream>
class MBC {
public:
	virtual uint8_t read(uint16_t address) = 0;
	virtual void write(uint16_t address, uint8_t value) = 0;
	virtual void incr(uint16_t address) = 0;
	virtual std::vector<uint8_t> saveBytes();
	virtual void loadSave(std::string romPath) = 0;
	MBC(std::string romPath);
protected:
	std::vector<uint8_t> bytes;
};