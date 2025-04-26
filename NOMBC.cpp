#include "NOMBC.h"

NOMBC::NOMBC(std::string a) : MBC(a){

}

uint8_t NOMBC::read(uint16_t address)
{
	return bytes[address];
}

void NOMBC::write(uint16_t address, uint8_t val)
{
	return; // no mbc is all rom
}

void NOMBC::incr(uint16_t address){
	return;
}
