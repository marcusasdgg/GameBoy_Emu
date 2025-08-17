#pragma once
#include <cstdint>
#include <vector>
class WaveChannel
{
private:
	bool enabled = false;
	uint8_t initial_length_timer = 0;
	uint8_t outputScale = 0;
	uint16_t frequency = 0;
	bool lengthEnable = false;
	uint16_t timer = 0;

	uint16_t frequencyTimer = 0;
	int16_t Volume = 0;

	std::array<uint8_t, 0x10000>& waveRam;
	const int wavePatternRamStart = 0xFF30;
	uint16_t samplePtr = 0;

public:
	WaveChannel(std::array<uint8_t, 0x10000>& patternRam) : waveRam(patternRam) {};
	void step();
	void trigger();
	void lengthclck();

	int16_t getVolume();

	//helper functions for read and writes
	uint8_t getPeriodHighControl();
	void setPeriodHighControl(uint8_t data);

	uint8_t getPeriodLow();
	void setPeriodLow(uint8_t data);

	uint8_t getOutputLevel();
	void setOutputLevel(uint8_t data);

	uint8_t getLengthTimer();
	void setLengthTimer(uint8_t data);

	uint8_t getDacEnable();
	void setDacEnable(uint8_t data);
};

