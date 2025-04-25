#pragma once
#include "AddressSpace.h"
class APU
{
private:
	//channels?
	AddressSpace& addr;
public:
	APU(AddressSpace& a);
	void tick(uint8_t cycles);
};

