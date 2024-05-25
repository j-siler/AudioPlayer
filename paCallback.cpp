#include "AudioBuffer.h"
#include <portaudio.h>
#include <vector>

static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
    AudioBuffer* buffer = (AudioBuffer*)userData;
    float* out = (float*)outputBuffer;
    std::vector<float> chunk;

    if (buffer->pop(chunk)) {
        for (size_t i = 0; i < chunk.size(); ++i) {
            *out++ = chunk[i];
        }
    } else {
        // Fill the remaining buffer with silence if no more data
        for (unsigned long i = 0; i < framesPerBuffer * 2; ++i) { // Assume 2 channels
            *out++ = 0.0f;
        }
        return paComplete; // Signal that playback is complete
    }

    return paContinue;
}
