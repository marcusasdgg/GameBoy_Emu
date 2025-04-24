#include "Clock.h"
#include <thread>
#include <chrono>
#include<iostream>
#include "helpers.h"
void Clock::tick(){
    uint16_t olddiv = divcounter;
    cycles++;
    divcounter++;

    addr.write(DIV, divcounter >> 8, false);
    uint8_t tac = addr.read(TAC);
    bool timer_enabled = tac & 0x04;
    
    if (timer_enabled) {
        uint8_t freq = tac & 0x03;

        int bit_to_watch;

        switch (freq) {
            case 0: bit_to_watch = 9; break;      // 4096 Hz
            case 1: bit_to_watch = 3; break;      // 262144 Hz
            case 2: bit_to_watch = 5; break;      // 65536 Hz
            case 3: bit_to_watch = 7; break;      // 16384 Hz
        }

        bool old_bit = (olddiv >> bit_to_watch) & 1;
        bool new_bit = (divcounter >> bit_to_watch) & 1;

        if (old_bit && !new_bit) {
            uint8_t tima = addr.read(TIMA);
            if (tima == 0xFF) {
                addr.write(TIMA, addr.read(TMA), false);
                request_timer_interrupt();
            }
            else {
                addr.write(TIMA, tima + 1, false);
            }
        }
    }
}

void Clock::request_timer_interrupt(){
    auto isf = addr.read(IF);

    addr.write(IF, isf | 0b00000100, false);
}

Clock::Clock(double hz, AddressSpace& a) : addr(a) {
    // Calculate time per cycle in nanoseconds (avoiding floating-point precision issues)
    cycles = 0;
}


uint64_t Clock::get_cycle(){
    return cycles;
}
