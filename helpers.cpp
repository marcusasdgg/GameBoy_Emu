#include "helpers.h"
#include "CPU.h"
#include "PPU.h"


uint8_t get_bit_range(uint8_t value, uint8_t start, uint8_t end){
	uint8_t n = end - start + 1;
	uint8_t bitmask = (1 << n) - 1;
	return (value >> start) & bitmask;
}

uint8_t get_bit(uint8_t byte, uint8_t bit) {
	return (byte & (1 << bit)) >> bit;
}

uint16_t join_bytes(uint8_t byte1, uint8_t byte2) {
	uint16_t byte3 = (uint16_t)byte1 << 8 | byte2;
	return byte3;
}

uint8_t set_bit(uint8_t byte, uint8_t bit, bool val) {
    if (val) {
        return byte | (1 << bit);
    }
    else {
        return byte & ~(1 << bit);
    }
}
const char* to_string(registerCalls reg) {
    switch (reg) {
    case A:   return "A";
    case B:   return "B";
    case C:   return "C";
    case D:   return "D";
    case E:   return "E";
    case H:   return "H";
    case L:   return "L";
    case AF:  return "AF";
    case BC:  return "BC";
    case DE:  return "DE";
    case HL:  return "HL";
    case SP:  return "SP";
    case PC:  return "PC";
    case HLI: return "HLI";
    case HLD: return "HLD";
    default:  return "Unknown";
    }
}

const char* to_string(Cond a) {
    switch (a)
    {
    case nz:
        return "NZ";
    case z:
        return "Z";
    case nc:
        return "NC";
    case c:
        return "NC";
    default:
        return "WTF";
    }
}

const char* to_string(uint8_t val) {
    static char buffer[5]; 
    snprintf(buffer, sizeof(buffer), "0x%02X", val);
    return buffer;
}

const char* to_string(uint16_t val){
    static char buffer[7]; // "0x" + 4 hex digits + null terminator
    snprintf(buffer, sizeof(buffer), "0x%04X", val);
    return buffer;
}

const char* to_string(PPUSTATE state) {
    switch (state)
    {
    case HBLANK:
        return "HBLANK";
    case VLANK:
        return "VBLANK";
    case OAM:
        return "OAM";
    case DRAW:
        return "DRAW";
    default:
        return "WTF";
    }
    
}const char* to_string(PIXEL state) {
    switch (state)
    {
    case GREEN0:
        return "BLACK";
    case GREEN1:
        return "Gray";
    case GREEN2:
        return "Green";
    case GREEN3:
        return "White";
    default:
       return "wtf";
    }
}

const char* to_string(Interrupt interrupt) {
    switch (interrupt)
    {
    case JOYPAD:
        return "JOYPAD";
    case Serial:
        return "SERIAL";
    case Timer:
        return "TIMER";
    case LCD:
        return "STAT";
    case VBLANK:
        return "VBLANK";
    }
}