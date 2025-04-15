#include "Clock.h"
#include <thread>
#include <chrono>
#include<iostream>
void Clock::tick(){
    cycles.fetch_add(1, std::memory_order_relaxed);
    addr.incr(0xFF41);
}

Clock::Clock(double hz, AddressSpace& a) : addr(a) {
    // Calculate time per cycle in nanoseconds (avoiding floating-point precision issues)
    active = true;
    cycles = 0;
    time_per_cycle = std::chrono::nanoseconds(static_cast<long long>(1'000'000'000 / hz));
}

void Clock::start_clock(){
    clock_thread = std::thread([this]() { clock_loop(); });
}

void Clock::stop_clock(){
    active = false;
    if (clock_thread.joinable()){
        clock_thread.join();
    }
    else {
        printf("clock thread not joimnable()???\n");
    }

}

void Clock::clock_loop() {
    auto start = std::chrono::high_resolution_clock::now();
    auto next_tick = start;

    while (active) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<long long, std::nano> duration = now - start;

        if (duration >= time_per_cycle) {
            tick();
            start = now;
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(30));
    }
}

uint64_t Clock::get_cycle(){
    return cycles.load(std::memory_order_relaxed);
}
