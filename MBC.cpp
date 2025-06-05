#include "MBC.h"
#include <string>
#include <fstream>
#include <iostream>

std::vector<uint8_t> MBC::saveBytes()
{
	auto butes = std::vector<uint8_t>(bytes);
	return butes;
}

MBC::MBC(std::string romPath){
	std::ifstream file(romPath, std::ios::binary); 
	
	bytes = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	printf("size of rom is %lld\n", bytes.size());
}
