#include "AddressSpace.h"
#include <algorithm>
#include <fstream>
#include <vector>





AddressSpace::AddressSpace(std::string bootPath, std::string romPath) {
	std::fill(memory, memory + SIZE, 0);
    std::ifstream inputFile(bootPath, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    std::copy(buffer.begin(), buffer.end(), bootupRom.begin());

    loadRom(romPath);
}

void AddressSpace::setCpuWriteable(bool cond){
    cpuWriteable = cond;
}


std::array<uint8_t,160> AddressSpace::getOAM(){
    std::array<uint8_t, 160> spriteData;
    std::copy(memory + 0xFE00, memory + 0xFE9F + 1, spriteData.begin());
    return spriteData;
}

std::array<uint8_t, 6144> AddressSpace::getVRAM()
{
    std::array<uint8_t, 160> vram;
    std::copy(memory + 0x8000, memory + 0x97FF + 1, vram.begin());
    return std::array<uint8_t, 6144>();
}

void AddressSpace::incr(uint16_t add){
    memory[add] += 1;
}

uint8_t AddressSpace::read(uint16_t address) {
    if (inStartup && address < 256) {
        return bootupRom[address];
    } 
    if (address >= 0xFF00 && address <= 0xFF7F) {
        // Handle I/O access
    }

    return memory[address];
}

void AddressSpace::loadRom(std::string file_path){
    std::ifstream inputFile(file_path, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    if (buffer.size() > SIZE) {
        printf("too big");
        return;
    }
    std::copy(buffer.begin(), buffer.end(), memory);
    inputFile.close();
}



void AddressSpace::readRom(std::string file_path) {
    std::ifstream inputFile(file_path, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    if (buffer.size() > SIZE) {
        printf("too big");
        return;
    }
    std::copy(buffer.begin(), buffer.end(), memory);
    inputFile.close();
}

void AddressSpace::saveRom(std::string file_path) {
    std::ofstream outputFile(file_path, std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(memory), SIZE);
    printf("saving");
    outputFile.close();
}

void AddressSpace::write(uint16_t address, uint8_t value) {
    if (address == 0xFF50 && value != 0) {
        inStartup = false;
    }
    memory[address] = value;
}