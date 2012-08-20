//============================================================
// AudioStream.h
//
//
//============================================================
#include <cmath>
#include <cassert>
#include <cstddef>
#include "portaudiocpp/PortAudioCpp.hxx"
#include <wx/wx.h>
#include <wx/thread.h>
#include "libsndfile/include/sndfile.h"
#include "extern/include/portaudio.h"
#include "extern/include/portaudiocpp/PortAudioCpp.hxx"

#ifndef __AudioStream__
#define __AudioStream__

const int NUM_SECONDS = 5;
const double SAMPLE_RATE = 44100.0;
const int FRAMES_PER_BUFFER = 64;
const int TABLE_SIZE = 200;

class AudioStream
{
public:
//    AudioStream(int tableSize) : tableSize_(tableSize), leftPhase_(0), rightPhase_(0);
    AudioStream(int tableSize);
    ~AudioStream();

    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);

private:
    float *table_;
    int tableSize_;
    int leftPhase_;
    int rightPhase_;

    void Open();

protected:

private:
};

#endif // __AudioStream__
