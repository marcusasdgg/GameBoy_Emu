#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "MBC.h"
#include <array>

class MBC3: public MBC {
public:
	MBC3(std::string a);
	uint8_t read(uint16_t address) override;
	void write(uint16_t address, uint8_t val) override;
	void incr(uint16_t address) override;
private:
	std::array<uint8_t, 32768> ram;
	bool writeable = false;
	uint64_t romOffset = 32768;
	uint64_t ramOffset = 8192;
	//clock
};

