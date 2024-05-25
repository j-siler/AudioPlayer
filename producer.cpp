#include "producer.h"
#include <sndfile.h>
#include <iostream>

#define FRAMES_PER_BUFFER 512

void producer(AudioBuffer& buffer, const std::string& filename) {
    SF_INFO sfinfo;
    SNDFILE* infile = sf_open(filename.c_str(), SFM_READ, &sfinfo);

    if (!infile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        std::cerr << sf_strerror(NULL) << std::endl;
        buffer.setFinished();
        return;
    }

    std::vector<float> chunk(FRAMES_PER_BUFFER * sfinfo.channels);
    while (true) {
        sf_count_t framesRead = sf_readf_float(infile, chunk.data(), FRAMES_PER_BUFFER);
        if (framesRead <= 0) break;
        chunk.resize(framesRead * sfinfo.channels); // Resize to actual frames read
        buffer.push(chunk);
    }

    sf_close(infile);
    buffer.setFinished();
}
