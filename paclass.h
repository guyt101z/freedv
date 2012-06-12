#include <stdio.h>
#include <math.h>
#include "portaudio.h"

// Note that many of the older ISA sound cards on PCs do NOT support
// full duplex audio (simultaneous record and playback).
// And some only support full duplex at lower sample rates.
#define SAMPLE_RATE         (44100)
#define PA_SAMPLE_TYPE      paFloat32       //paInt16
#define FRAMES_PER_BUFFER   (64)

typedef float SAMPLE;

float CubicAmplifier(float input);

class PortAudioWrap
{
    public:
        PortAudioWrap();
        ~PortAudioWrap();

    private:
        PaStream                        *stream;
        PaStreamParameters              inputBuffer;
        PaStreamParameters              outputBuffer;
        void                            *userData;
        int                             samplerate;
        unsigned long                   framesPerBuffer;
        PaStreamCallbackFlags           statusFlags;
        PaStreamCallback                *streamCallback;
        PaStreamFinishedCallback        *streamFinishedCallback;
        const PaStreamCallbackTimeInfo  *timeInfo;
        PaDeviceIndex                   inputDevice;
        PaDeviceIndex                   outputDevice;

    public:

        PaError streamOpen();

        PaDeviceIndex getDefaultInputDevice();
        PaDeviceIndex getDefaultOutputDevice();

        PaStreamParameters *getDeviceInfo(PaDeviceIndex idx);
        PaError     setFramesPerBuffer(unsigned long size);
        PaError     setSampleRate(unsigned long size);
        PaError     setStreamFlags(PaStreamFlags flags);
        PaError     setCallback(PaStreamCallback *streamCallback);
        PaError     setStreamCallback(PaStream *stream, PaStreamCallback* callback ) { streamCallback = callback; return 0;}
        PaError     setStreamFinishedCallback(PaStream *stream, PaStreamFinishedCallback* streamFinishedCallback );

        PaError     setInputDevice(PaDeviceIndex dev);
        PaError     setInputChannelCount(int count);
        int         getInputChannelCount();
        PaError     setInputSampleFormat(PaSampleFormat format);
        PaError     setInputSampleRate(PaSampleFormat format);
        PaError     setInputLatency(PaTime latency);
        void        setInputHostApiStreamInfo(void *info = NULL);
        PaTime      getInputDefaultLowLatency();
        const char  *getDeviceName(PaDeviceIndex dev);

        PaError     setOutputDevice(PaDeviceIndex dev);
        PaError     setOutputChannelCount(int count);
        int         getOutputChannelCount();
        PaError     setOutputSampleFormat(PaSampleFormat format);
        PaError     setOutputLatency(PaTime latency);
        void        setOutputHostApiStreamInfo(void *info = NULL);
        PaTime      getOutputDefaultLowLatency();

        PaError     init();
        PaError     streamStart();
        PaError     streamClose();
        void        terminate();
        bool        isOpen() const;
        void        stop();
        void        abort();
        bool        isStopped() const;
        bool        isActive() const;
};
