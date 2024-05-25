#include "AudioBuffer.h"
#include "producer.h"
#include "paCallback.cpp"
#include <portaudio.h>
#include <iostream>
#include <thread>

#define FRAMES_PER_BUFFER 512

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_audiofile>" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    AudioBuffer buffer;

    // Start the producer thread
    std::thread producerThread(producer, std::ref(buffer), inputFilename);

    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Open the PortAudio stream
    PaStream* stream;
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, FRAMES_PER_BUFFER, paCallback, &buffer);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return 1;
    }

    // Start the stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

    // Wait for the stream to finish
    while (Pa_IsStreamActive(stream) == 1) {
        Pa_Sleep(100);
    }

    // Stop and close the stream
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    // Join the producer thread
    producerThread.join();

    std::cout << "Audio playback finished." << std::endl;
    return 0;
}
