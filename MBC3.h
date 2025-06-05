#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "MBC.h"
#include <array>

class MBC3: public MBC {
public:
	MBC3(std::string a, std::string savePath = "");
	uint8_t read(uint16_t address) override;
	void write(uint16_t address, uint8_t val) override;
	void incr(uint16_t address) override;
	std::vector<uint8_t> saveBytes() override;
	void loadSave(std::string romPath) override;
private:
	std::array<uint8_t, 65536> ram;
	bool writeable = false;
	uint32_t romOffset = 32768;
	uint16_t ramOffset = 0;
	//clock
};

