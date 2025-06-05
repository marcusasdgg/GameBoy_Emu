#include "APU.h"
#include "helpers.h"

#include <cstdlib>  // for rand()
void APU::stepFrameSequencer()
{

	if (frameSequencerStep % 2 == 0) {
		clockLengthCounters();
	}

	//if (frameSequencerStep == 7) {
	//	clockVolumeEnvelopes();
	//}


	//if (frameSequencerStep == 0 || frameSequencerStep == 4) {
	//	clockSweepUnit(); 
	//}

	frameSequencerStep++;
	frameSequencerStep %= 8;
}

void APU::clockLengthCounters()
{
	// this function ticks down the remaining length for the tone?

	channel1.lengthclck();

	channel2.lengthclck();

	//if (!disable3 && timer3 > 0) {
	//	timer3--;
	//	if (timer3 == 0)
	//		disable3 = true;
	//}

	//if (!disable4 && timer4 > 0) {
	//	timer4--;
	//	if (timer4 == 0)
	//		disable1 = true;
	//}
}

APU::APU()
{
	stream.start();
	stream.play();
}

uint8_t APU::read(uint16_t address)
{
	switch (address) {

	case NR10:
		//sweep effective noop for now.
	case NR11:
		return channel1.getLengthTimer();
	case NR12:
		return channel1.getInitialVolume();
	case NR13:
		return channel1.getPeriodLow();
	case NR14:
		return channel1.getPeriodHighControl();

		//channel 1: Square
	case NR21:
		return channel2.getLengthTimer();
	case NR22:
		return channel2.getInitialVolume();
	case NR23:
		return channel2.getPeriodLow();
	case NR24:
		return channel2.getPeriodHighControl();
		

		//channel 3: wave
	case NR30:
	case NR31:
	case NR32:
	case NR33:
	case NR34:
		return 0xFF;

		//channel 4: noise
	case NR41:
	case NR42:
	case NR43:
	case NR44:
		return 0xFF;

		//master controls
	case NR50:
	case NR51:
	case NR52:
		return 0xFF;

		//this shouldn't happen?
	default:
		return 0xFF;
	}
}

void APU::write(uint16_t address, uint8_t data)
{
	switch (address) {

		case NR10:
			//sweep
			break;
		case NR11:
			channel1.setLengthTimer(data);
			return;
		case NR12:
			channel1.setInitialVolume(data);
			return;
		case NR13:
			channel1.setPeriodLow(data);
			return;
		case NR14:
			channel1.setPeriodHighControl(data);
			return;
	
		//channel 1: Square
		case NR21:
			channel2.setLengthTimer(data);
			return;
		case NR22:
			channel2.setInitialVolume(data);
			return;
		case NR23:
			channel2.setPeriodLow(data);
			return;
		case NR24:
			channel2.setPeriodHighControl(data);
			return;

		
		//channel 3: wave todo! later
		case NR30:
		case NR31:
		case NR32:
		case NR33:
		case NR34:
			return;

		//channel 4: noise todo! later
		case NR41:
		case NR42:
		case NR43:
		case NR44:
			return;

		//master controls todo! immediately
		case NR50:
		case NR51:
			return;
		case NR52:
			enabled = !(get_bit(data, 7) == 0);
			printf("write to NR52 to set enabled to %d\n", enabled);
			return;

		//this shouldn't happen?
		default:
			return;
	}
}

// lets make this just advance 1 tick instead
void APU::tick(uint8_t cycles)
{
	if (!enabled)
		return;
	
	tickCount++;

	if(tickCount % 8192 == 0) {
		stepFrameSequencer();
	}


	channel1.step(1);
	channel2.step(1);

	//stepChannel3();
	//stepChannel4();

	// now play channel?
	if (tickCount % 95 == 0) {
		// this is 1 sample of 44.1khz
		int16_t sample1 = channel1.getVolume();
		int16_t sample2 = channel2.getVolume();
		int mixed = sample1 + sample2;

		if (mixed > 32767) mixed = 32767;
		if (mixed < -32768) mixed = -32768;
		stream.addSample((int16_t)mixed);
	}

}

