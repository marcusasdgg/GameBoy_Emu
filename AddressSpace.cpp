#include "AddressSpace.h"
#include <algorithm>


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

void AddressSpace::write(uint16_t address, uint8_t value) {
    memory[address] = value;
}