#include "SquareChannel.h"
#include "helpers.h"

SquareChannel::SquareChannel()
{
	//initialise all values or don't?

	dutyTable[0] = std::array<bool, 8>({ false,false,false, false, false, false,false,true });
	dutyTable[1] = std::array<bool, 8>({ {true, false, false, false, false, false, false, true} });
	dutyTable[2] = std::array<bool, 8>({ {true, false, false, false, false, true, true, true} });
	dutyTable[3] = std::array<bool, 8>({ {false, true, true, true, true, true, true, false} });
}


void SquareChannel::lengthclck()
{
	if (enabled && timer > 0) {
		timer--;
		if (timer == 0)
		{
			enabled = false;
		}
	}
}

int16_t SquareChannel::getVolume()
{
	if (!enabled)
		return 0;
	return Volume;
}

void SquareChannel::step(uint8_t cycles) {
	// each step we do we more or less update the volume value
	// and advance sequence pointer
	if (enabled) {
		frequencyTimer -= cycles;
		if (frequencyTimer <= 0) {
			sequencePointer = (sequencePointer + 1) & 0x7;
			Volume = (dutyTable[waveduty][sequencePointer] * 2 - 1) * 4000;
			frequencyTimer = (2048 - frequency) * 4 + frequencyTimer;
		}
	}
}

void SquareChannel::trigger()
{
	enabled = true;
	sequencePointer = 0;
	timer = initial_length_timer;
	if (lengthEnable && timer == 0) {
		timer = 64; // or 256 for wave channel
	}
}

uint8_t SquareChannel::getPeriodLow()
{
	return 0xFF; // not readable
}

uint8_t SquareChannel::getPeriodHighControl()
{
	 //only length enable is readablle
	return 0 | (lengthEnable << 6);
}

uint8_t SquareChannel::getInitialVolume()
{
	return 0xFF; // again noop for now
}

uint8_t SquareChannel::getLengthTimer()
{
	 // only waveduty is visible
	return 0 | (waveduty << 6);
}

void SquareChannel::setInitialVolume(uint8_t val)
{
	//effective noop for now (envelope stuff). Todo!
}

void SquareChannel::setPeriodHighControl(uint8_t val)
{

	if (get_bit(val, 7)) {
		printf("triggered channel\n");
		trigger();
	}
		
	lengthEnable = get_bit(val, 6);
	frequency = (frequency & 0xFF) | (get_bit_range(val, 0, 2) << 8); // high bits
}

void SquareChannel::setPeriodLow(uint8_t val)
{
	frequency = (frequency & 0x700) | val; // low bits
}

void SquareChannel::setLengthTimer(uint8_t val)
{
	waveduty = get_bit_range(val, 6, 7);
	initial_length_timer = get_bit_range(val, 0, 5);
	printf("setted length timer to %d and waveduty to %d", initial_length_timer, waveduty);
}
