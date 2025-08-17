#pragma once
#include "SquareChannel.h"
#include "CustomStream.h"
#include <array>
#include "WaveChannel.h"
//apu runs at 44.1 khz audio
//contains 4 channels

#define samplesize 4096


// new idea:

// address space combines with APU for some funny reason, similar to how MBC works right now.


class APU
{
private:

	CustomStream stream;
	int tickCount = 0;
	int frameSequencerStep = 0;

	bool enabled = false;


	void stepFrameSequencer();
	 
	// frame sequencer associated functions
	void clockLengthCounters();
	void clockVolumeEnvelopes();
	void clockSweepUnit();

	//channels
	SquareChannel channel1;
	SquareChannel channel2;
	WaveChannel channel3;


public:
	APU(std::array<uint8_t, 16>& fatberg);
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	void tick(uint8_t cycles);
};

