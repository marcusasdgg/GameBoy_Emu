#pragma once
#include "MBC.h"
class NOMBC :
    public MBC
{
public:
	NOMBC(std::string a);
	uint8_t read(uint16_t address) override;
	void write(uint16_t address, uint8_t val) override;
	void incr(uint16_t address) override;
};

