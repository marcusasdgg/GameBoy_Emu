#pragma once
#include <SFML/Audio.hpp>
#include <mutex>
class CustomStream : public sf::SoundStream
{
public:
	void addSample(int16_t sample);
	void start();
private:
	bool onGetData(Chunk& data) override;
	void onSeek(sf::Time timeOffset) override;
	const unsigned int sampleRate = 44100;
	const unsigned int channelCount = 1;
	std::vector<int16_t> sampleBuffer;
	std::vector<int16_t> playbackBuffer;
	std::mutex bufferMutex;
};

