#include "MBC.h"
#include <string>
#include <fstream>
#include <iostream>

MBC::MBC(std::string romPath){
	std::ifstream file(romPath, std::ios::binary); 
	bytes = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
}
