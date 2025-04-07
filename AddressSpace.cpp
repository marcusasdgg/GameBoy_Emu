#include "AddressSpace.h"
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>


AddressSpace::AddressSpace() {
	std::fill(memory, memory + SIZE, 0);
}

uint8_t AddressSpace::read(uint16_t address) {
    // You can add special handling for different memory regions here
    if (address >= 0xFF00 && address <= 0xFF7F) {
        // Handle I/O access
    }
    return memory[address];
}

void AddressSpace::readRom(std::string file_path) {
    std::ifstream inputFile(file_path, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    if (buffer.size() > 64000) {
        printf("too big");
        return;
    }
    std::copy(buffer.begin(), buffer.end(), memory);
    inputFile.close();
}

void AddressSpace::write(uint16_t address, uint8_t value) {
    memory[address] = value;
}