#pragma once
#include <cstdint>
#include <thread>
#include <chrono>
#include <atomic>
#include "AddressSpace.h"
class Clock{
private:
	// number of cycles
	uint64_t cycles;
	AddressSpace& addr;
public:
	Clock(double hz, AddressSpace& a);
	void tick();
	uint64_t get_cycle();
};

