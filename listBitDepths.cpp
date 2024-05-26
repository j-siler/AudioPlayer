#include <iostream>
#include <portaudio.h>

void checkDeviceCapabilities(int deviceIndex, const char* deviceName, int maxChannels, bool isInput) {
    PaError err;
    PaSampleFormat sampleFormats[] = { paInt8, paInt16, paInt24, paInt32, paFloat32 };
    const char* formatNames[] = { "8-bit", "16-bit", "24-bit", "32-bit", "32-bit float" };

    for (int channelCount = 1; channelCount <= maxChannels; ++channelCount) {
        bool hasSupportedFormat = false;
        std::cout << (isInput ? "Input" : "Output") << " channels: " << channelCount << " - Supported bit depths: ";
        
        for (size_t j = 0; j < sizeof(sampleFormats) / sizeof(sampleFormats[0]); ++j) {
            PaStreamParameters parameters;
            parameters.device = deviceIndex;
            parameters.channelCount = channelCount;
            parameters.sampleFormat = sampleFormats[j];
            parameters.suggestedLatency = (isInput ? Pa_GetDeviceInfo(deviceIndex)->defaultLowInputLatency : Pa_GetDeviceInfo(deviceIndex)->defaultLowOutputLatency);
            parameters.hostApiSpecificStreamInfo = nullptr;

            err = Pa_IsFormatSupported(isInput ? &parameters : nullptr, isInput ? nullptr : &parameters, 44100.0);
            if (err == paFormatIsSupported) {
                if (hasSupportedFormat) {
                    std::cout << ", ";
                }
                std::cout << formatNames[j];
                hasSupportedFormat = true;
            }
        }

        if (!hasSupportedFormat) {
            std::cout << "None";
        }
        std::cout << std::endl;
    }
}

void listBitDepths() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    int numDevices = Pa_GetDeviceCount();
    if (numDevices < 0) {
        std::cerr << "ERROR: Pa_CountDevices returned " << numDevices << std::endl;
        err = Pa_Terminate();
        return;
    }

    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo) {
            std::cout << "Device " << i << ": " << deviceInfo->name << std::endl;
            const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            if (hostApiInfo) {
                std::cout << "Host API: " << hostApiInfo->name << std::endl;
            }

            if (deviceInfo->maxInputChannels > 0) {
                checkDeviceCapabilities(i, deviceInfo->name, deviceInfo->maxInputChannels, true);
            }

            if (deviceInfo->maxOutputChannels > 0) {
                checkDeviceCapabilities(i, deviceInfo->name, deviceInfo->maxOutputChannels, false);
            }

            std::cout << std::endl;
        }
    }

    err = Pa_Terminate();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    }
}

int main() {
    listBitDepths();
    return 0;
}
