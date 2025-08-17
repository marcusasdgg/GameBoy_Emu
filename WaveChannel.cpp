#include "WaveChannel.h"
#include "helpers.h"

void WaveChannel::step()
{
	if (enabled) {
		frequencyTimer --;
		if (frequencyTimer <= 0) {
			uint8_t data = get_bit_range(waveRam[samplePtr / 2], (samplePtr % 2) * 4, (samplePtr % 2 + 1) * 4 - 1);
			printf("volume is %d\n", data);
			Volume = (data * 2 - 1) * 2000;
			frequencyTimer = (2048 - frequency) * 4 + frequencyTimer;
			samplePtr++;
			if (samplePtr == wavePatternRamStart + 32)
				samplePtr = wavePatternRamStart;
		}
	}
}

void WaveChannel::trigger()
{
	enabled = true;
	timer = initial_length_timer;
	if (lengthEnable && timer == 0) {
		timer = 256;
	}
}

void WaveChannel::lengthclck()
{
	if (enabled && timer > 0) {
		timer--;
		if (timer == 0)
		{
			enabled = false;
		}
	}
}

int16_t WaveChannel::getVolume()
{
	return Volume;
}

uint8_t WaveChannel::getPeriodHighControl()
{
	return lengthEnable << 6;
}

void WaveChannel::setPeriodHighControl(uint8_t data)
{
	if (get_bit(data, 7)) {
		printf("triggered channel 3\n");
		trigger();
	}

	lengthEnable = get_bit(data, 6);
	frequency = (frequency & 0xFF) | (get_bit_range(data, 0, 2) << 8); // high bits
}

uint8_t WaveChannel::getPeriodLow()
{
	return 0xFF;
}

void WaveChannel::setPeriodLow(uint8_t data)
{
	frequency |= data;
}

uint8_t WaveChannel::getOutputLevel()
{
	return outputScale << 5;
}

void WaveChannel::setOutputLevel(uint8_t data)
{
	outputScale = get_bit_range(data, 5, 6);
}

uint8_t WaveChannel::getLengthTimer()
{
	return 0;
}

void WaveChannel::setLengthTimer(uint8_t data)
{
	initial_length_timer = data;
}

uint8_t WaveChannel::getDacEnable()
{
	return 0;
}

void WaveChannel::setDacEnable(uint8_t data)
{
	enabled = get_bit(data, 7);
}
