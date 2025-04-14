#include "helpers.h"

uint8_t get_bit_range(uint8_t value, uint8_t start, uint8_t end){
	uint8_t n = end - start + 1;
	uint8_t bitmask = (1 << n) - 1;
	return (value >> start) & bitmask;
}

uint8_t get_bit(uint8_t byte, uint8_t bit) {
	return (byte & (1 << bit)) >> bit;
}