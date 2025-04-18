#pragma once
#include <cstdint>
#include "CPU.h"


uint8_t get_bit_range(uint8_t value, uint8_t start, uint8_t end);


uint8_t get_bit(uint8_t byte, uint8_t bit);

uint16_t join_bytes(uint8_t byte1, uint8_t byte2);

const char* to_string(registerCalls);
const char* to_string(Cond);

const char* to_string(uint8_t);
const char* to_string(uint16_t);