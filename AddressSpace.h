#pragma once
#include <cstdint>
#include<string>
#include<array>
#include<vector>
#include "MBC.h"
#include <memory>
#include "APU.h"

#define testMode false
#define debug false

//registers
#define P1JOYP 0xFF00
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
#define NR21 0xFF16
#define NR22 0xFF17
#define NR23 0xFF18
#define NR24 0xFF19
#define NR30 0xFF1A
#define NR31 0xFF1B
#define NR32 0xFF1C
#define NR33 0xFF1D
#define NR34 0xFF1E
#define NR41 0xFF20
#define NR42 0xFF21
#define NR43 0xFF22
#define NR44 0xFF23
#define NR50 0xFF24
#define NR51 0xFF25
#define NR52 0xFF26
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
#define WY 0xFF4A
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


//TODO : fix all functions in address space to resolve from old memory system (64 byte array) to compartementalised.
// TODO: for performance purposes get the non user controlled parts of the cpu and ppu to read address using specialised functions.

const int SIZE = 0x10000;

class AddressSpace{
public:
	
	uint8_t read(uint16_t address);
	void loadRom(std::string filePat);
	//rename to address space...
	void readRom(std::string file_path);
	void saveRom(std::string file_path);
	void write(uint16_t address, uint8_t value, bool iscpu = true);
	AddressSpace(std::string bootupPath, std::string romPath, std::string savePath = "");
	void setCpuWriteable(bool);
	std::array<uint8_t,160> getOAM();
	std::array<uint8_t, 6144> getVRAM();
	void incr(uint16_t add);
	

	std::vector<uint8_t> get_range(uint16_t start, uint16_t end);

	void mapbuttons(std::array<bool, 8>& state);
	~AddressSpace();

	void writeSTAT(uint8_t stat);

	void incrLY();

	void setIF(uint8_t iff);
	void setSTAT(uint8_t stat);

	
	uint8_t getVRAMADD(uint16_t address);

	std::vector<uint8_t> saveBytes();
	void loadSave(std::string savePath);
	void tickAPU(uint8_t cycles);
private:
	MBC* mbc;
	APU apu;
	std::array<bool, 8> buttonstate;
	std::array<uint8_t, 256> bootupRom;
	std::array<uint8_t, 8192> vram;
	std::array<uint8_t, 4096> fixedRam;
	std::array<uint8_t, 4096> switchableRam;
	std::array<uint8_t, 160> oam;
	std::array<uint8_t, 16> wram;
	
	//lcd/graphic registers
	uint8_t lcdc;
	uint8_t stat;
	uint8_t scx;
	uint8_t scy;
	uint8_t ly;
	uint8_t lyc;
	uint8_t dma;
	uint8_t bgp;
	uint8_t obp0;
	uint8_t obp1;
	uint8_t wy;
	uint8_t wx;

	// sound registers
	uint8_t nr11;
	uint8_t nr12;
	uint8_t nr13;
	uint8_t nr14;
	uint8_t nr21;
	uint8_t nr22;
	uint8_t nr23;
	uint8_t nr24;
	uint8_t nr30;
	uint8_t nr31;
	uint8_t nr32;
	uint8_t nr33;
	uint8_t nr34;
	uint8_t nr41;
	uint8_t nr42;
	uint8_t nr43;
	uint8_t nr44;
	uint8_t nr50;
	uint8_t nr51;
	uint8_t nr52;
	
	//interrupt register
	uint8_t ifr;
	uint8_t ie;

	//serial stuff
	uint8_t sb;
	uint8_t sc;

	//timer registers
	uint8_t div;
	uint8_t tima;
	uint8_t tma;
	uint8_t tac;

	//joypad register
	uint8_t joyp;

	
	//unincluded vram DMA registers, BG/OBJ p[allettes, WRAM bank select (mainly just CGB stufF).

	bool inStartup = true;
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



// registers implemented:
// disable bootup rom
// timer
//

//registers to implement:
// dma