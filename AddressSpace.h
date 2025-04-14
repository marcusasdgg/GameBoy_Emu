#pragma once
#include <cstdint>
#include<string>
#include<array>

const int SIZE = 0x10000;

class AddressSpace{
public:
	uint8_t read(uint16_t address);
	//rename to address space...
	void readRom(std::string file_path);
	void saveRom(std::string file_path);
	void write(uint16_t address, uint8_t value);
	AddressSpace();
	void setCpuWriteable(bool);
	std::array<uint8_t,160> getOAM();
	std::array<uint8_t, 6144> getVRAM();
	void incr(uint16_t add);

private:
	uint8_t memory[SIZE];
	bool cpuWriteable = false;
};


//0000 - 3FFF for rom bank 0
// 4000 - 7FFF for rom bank 1 - NN
// 8000 - 9FFF for video ram
// A000 - bFFF for external ram
// C000 - CFFF wram
// D000 - DFFF for wram 
// E000 - FDFF echoram
// Fe00 - Fe9F OAM
// FEA0 FEFF not usable
//FF00 FF7F high ram
// FFFF FFFF IE enable register

// ff42 scy register
//ff43 scx regiuster
// ff4a wy register
// ff4b wx register
// ff47 bgp: bg palette data
// ff48 obp0 pallette
// ff49 obp1 pallette