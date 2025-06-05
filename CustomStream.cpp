#include "CustomStream.h"



bool CustomStream::onGetData(Chunk& data)
{
    //we flush the vector into the chunk
    std::lock_guard<std::mutex> lock(bufferMutex);


    if (sampleBuffer.empty()) {
        static std::vector<int16_t> silence(512, 0); // 512 samples of silence
        data.samples = silence.data();
        data.sampleCount = silence.size();
        return true;
    }

    playbackBuffer = std::move(sampleBuffer);
    sampleBuffer.clear();

    data.samples = playbackBuffer.data();
    data.sampleCount = static_cast<std::size_t>(playbackBuffer.size());

    return true;
}

void CustomStream::onSeek(sf::Time timeOffset)
{
    return; // since this is an active dynamic playback stream no seeking allowed.
}

void CustomStream::addSample(int16_t sample)
{
    //if (sample != 0)
    //    printf("found non 0 sample %d\n", sample);
    std::lock_guard<std::mutex> lock(bufferMutex);
    sampleBuffer.push_back(sample);
}

void CustomStream::start()
{
    initialize(channelCount,
        sampleRate,
        {sf::SoundChannel::Mono});
}

