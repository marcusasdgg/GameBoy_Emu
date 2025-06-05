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
	uint64_t cycle_start;
	uint16_t divcounter = 0;

	void request_timer_interrupt();
public:
	Clock(double hz, AddressSpace& a);
	void tick();
	uint64_t get_cycle();
	std::vector<uint8_t> saveBytes();
	void loadSave(std::string savePath);
};

