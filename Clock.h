#pragma once
#include <cstdint>
#include <thread>
#include <chrono>
#include <atomic>
class Clock{
private:
	// number of cycles
	std::atomic<uint64_t> cycles;
	bool active;
	std::chrono::nanoseconds time_per_cycle;
	std::thread clock_thread;

	// increment the cycle count
	void tick();
	void clock_loop();


public:
	Clock(const Clock&) = delete;
	Clock& operator=(const Clock&) = delete;
	Clock(double hz);
	void start_clock();

	void stop_clock();

	uint64_t get_cycle();

};

