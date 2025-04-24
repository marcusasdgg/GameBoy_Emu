#pragma once
#include <cstdint>
#include<string>
#include<array>
#include<vector>

#define debug true
//registers
#define P1JOYP = 0xFF00;
#define SB 0xFF01
#define SC 0xFF02
#define DIV 0xFF04
#define TIMA 0xFF05
#define TMA 0xFF06
#define TAC 0xFF07
#define IF 0xFF0F
#define NR10 0xFF10
#define NR11 0xFF11
#define NR12 0xFF112
#define NR13 0xFF13
#define NR14 0xFF14
#define NR21 0xFF21
#define NR22 0xFF22
#define NR23 0xFF23
#define NR24 0xFF24
#define NR30 0xFF30
#define NR31 0xFF31
#define NR32 0xFF32
#define NR33 0xFF33
#define NR34 0xFF34
#define NR41 0xFF41
#define NR42 0xFF42
#define NR43 0xFF43
#define NR44 0xFF44
#define NR50 0xFF50
#define NR51 0xFF51
#define NR52 0xFF52
#define LCDC 0xFF40
#define STAT 0xFF41
#define SCY 0xFF42
#define SCX 0xFF43
#define LY 0xFF44
#define LYC 0xFF45
#define DMA 0xFF46
#define BGP 0xFF47
#define OBP0 0xFF48
#define OBP1 0xFF49
#define WY 0xFF41
#define WX 0xFF4B
#define KEY1 0xFF4D
#define VBK 0xFF4F
#define HDMA1 0xFF51
#define HDMA2 0xFF52
#define HDMA3 0xFF53
#define HDMA4 0xFF54
#define HDMA5 0xFF55
#define RP 0xFF56
#define BCPSBGPI 0xFF68
#define BCPDBGPD 0xFF69
#define OCPSOBPI 0xFF6A
#define OCPDOBPD 0xFF6B
#define OPRI 0xFF6C
#define SVBK 0xFF70
#define PCM12 0xFF76
#define PCM34 0xFF77
#define IE 0xFFFF



const int SIZE = 0x10000;

class AddressSpace{
public:
	
	uint8_t read(uint16_t address);
	void loadRom(std::string filePat);
	//rename to address space...
	void readRom(std::string file_path);
	void saveRom(std::string file_path);
	void write(uint16_t address, uint8_t value);
	AddressSpace(std::string bootupPath, std::string romPath);
	void setCpuWriteable(bool);
	std::array<uint8_t,160> getOAM();
	std::array<uint8_t, 6144> getVRAM();
	void incr(uint16_t add);

	std::vector<uint8_t> get_range(uint16_t start, uint16_t end);

private:
	std::array<uint8_t, 256> bootupRom;
	bool inStartup = true;
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