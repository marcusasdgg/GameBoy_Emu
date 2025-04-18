#include "CPU.h"
//bitmasks
static const uint8_t bitmask[] = {
    0b00000001, 
    0b00000010, 
    0b00000100,
    0b00001000,
    0b00010000, 
    0b00100000, 
    0b01000000, 
    0b10000000 
};

void CPU::bitu3r8(uint8_t bit, registerCalls a) {
    if (debug)
        printf("bit u3 r8\n");
    uint8_t temp = retrieve_register_8(a);
    if ((temp & bitmask[bit]) == 0)
        set_zero(true);
    else
        set_zero(false);

    set_n(false);
    set_half_carry(true);
}

void CPU::bitu3hl(uint8_t bit){
    if (debug)
        printf("bit u3 hl\n");
    uint16_t add = retrieve_register_16(HL);
    uint8_t temp = address_space.read(add);
    if ((temp & bitmask[bit]) == 0) set_zero(true);
    set_n(false);
    set_half_carry(true);
}

void CPU::resu3r8(uint8_t bit, registerCalls a){
    if (debug)
        printf("res u3 r8\n");
    uint8_t temp = retrieve_register_8(a);
    temp &= ~bitmask[bit];
    store_in_register(a, temp);
}

void CPU::resu3hl(uint8_t bit){
    if (debug)
        printf("res u3 hl\n");
    uint16_t add = retrieve_register_16(HL);
    uint8_t temp = address_space.read(add);
    temp &= ~bitmask[bit];
    address_space.write(add, temp);
}

void CPU::setu3r8(uint8_t bit, registerCalls a){
    if (debug)
        printf("set u3 r8\n");
    uint8_t temp = retrieve_register_8(a);
    temp |= bitmask[bit];
    store_in_register(a, temp);
}

void CPU::setu3hl(uint8_t bit){
    if (debug)
        printf("set u3 hl\n");
    uint16_t add = retrieve_register_16(HL);
    uint8_t temp = address_space.read(add);
    temp |= bitmask[bit];
    address_space.write(add, temp);
}


