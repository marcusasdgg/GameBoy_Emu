#include "NOMBC.h"

NOMBC::NOMBC(std::string a, std::string savePath) : MBC(a){

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

std::vector<uint8_t> NOMBC::saveBytes()
{
	return MBC::saveBytes();
}

void NOMBC::loadSave(std::string savePath)
{
}
