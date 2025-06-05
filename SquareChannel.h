#pragma once
#include <cstdint>
#include <array>
class SquareChannel
{
private:
	uint8_t initial_length_timer;
	uint8_t waveduty;
	uint8_t initialvolume = 8000;
	bool envdir;
	uint8_t sweeppace;

	//sweep stuff
	uint8_t indivdualstep;
	bool direction;
	uint8_t pace;

	bool lengthEnable;
	uint16_t frequency;
	
	//when this flag is true then the channel is enabled otherwise disabled.
	bool enabled = false;


	// points to current value of the duty cycle wave.
	uint8_t sequencePointer;

	//volume value
	int16_t Volume;

	//duty cycle table
	std::array<std::array<bool, 8>, 4> dutyTable;

	// timer
	uint16_t timer = 0;

	//frequency 
	uint16_t frequencyTimer = 0;

public:
	SquareChannel();
	void step(uint8_t cycles);

	//triggers the channel to start this function should be called when the write writes to the trigger bit.
	void trigger();

	//read and write to connect to address space, apu calls these functions and from there it assigns channel 1 -4 based on address, the uint8_t specifies the local reg.

	uint8_t getPeriodLow();
	uint8_t getPeriodHighControl();
	uint8_t getInitialVolume();
	uint8_t getLengthTimer();

	void setInitialVolume(uint8_t val);
	void setPeriodHighControl(uint8_t val);
	void setPeriodLow(uint8_t val);
	void setLengthTimer(uint8_t val);

	void lengthclck();
	int16_t getVolume();
};

