#include "Clock.h"
#include <thread>
#include <chrono>
#include<iostream>
void Clock::tick(){
    cycles++;
    addr.incr(0xFF41);
    //add logic to check if the addr is now 0;
}

Clock::Clock(double hz, AddressSpace& a) : addr(a) {
    // Calculate time per cycle in nanoseconds (avoiding floating-point precision issues)
    cycles = 0;
}


uint64_t Clock::get_cycle(){
    return cycles;
}
