#pragma once
#include "MBC.h"
class NOMBC :
    public MBC
{
public:
	NOMBC(std::string a, std::string savePath = "");
	uint8_t read(uint16_t address) override;
	void write(uint16_t address, uint8_t val) override;
	void incr(uint16_t address) override;
	std::vector<uint8_t> saveBytes() override;
	void loadSave(std::string savePath) override;
};

