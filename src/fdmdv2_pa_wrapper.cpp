#include "fdmdv2_pa_wrapper.h"

PortAudioWrap::PortAudioWrap()
{
    stream                  = NULL;
    userData                = NULL;
    samplerate              = 0;
    framesPerBuffer         = 0;
    statusFlags             = 0;
    streamCallback          = NULL;
    streamFinishedCallback  = NULL;
    timeInfo                = 0;
}

PortAudioWrap::~PortAudioWrap()
{
}

//PaError PortAudioWrap::init()
//{
//    return Pa_Initialize();
//}

PaError PortAudioWrap::streamOpen()
{
    return Pa_OpenStream(
                            &stream,
                            &inputBuffer,
                            &outputBuffer,
                            samplerate,
                            framesPerBuffer,
                            statusFlags,
                            *streamCallback,
                            userData
                        );
}

PaError PortAudioWrap::streamStart()
{
    return Pa_StartStream(stream);
}

PaError PortAudioWrap::streamClose()
{
    if(isOpen())
    {
        PaError rv = Pa_CloseStream(stream);
//        stream = NULL;
        return rv;
    }
    else
    {
        return paNoError;
    }
}

void PortAudioWrap::terminate()
{
    if(Pa_IsStreamStopped(stream) != paNoError)
    {
        Pa_StopStream(stream);
    }
    Pa_Terminate();
}

void PortAudioWrap::stop()
{
    Pa_StopStream(stream);
}

void PortAudioWrap::abort()
{
    Pa_AbortStream(stream);
}

bool PortAudioWrap::isStopped() const
{
    PaError ret = Pa_IsStreamStopped(stream);
    return ret;
}

bool PortAudioWrap::isActive() const
{
    PaError ret = Pa_IsStreamActive(stream);
    return ret;
}

bool PortAudioWrap::isOpen() const
{
    return (stream != NULL);
}

PaDeviceIndex PortAudioWrap::getDefaultInputDevice()
{
    return Pa_GetDefaultInputDevice();
}

PaDeviceIndex PortAudioWrap::getDefaultOutputDevice()
{
    return Pa_GetDefaultOutputDevice();
}

PaError PortAudioWrap::setInputChannelCount(int count)
{
    inputBuffer.channelCount = count;
    return paNoError;
}

PaError PortAudioWrap::getInputChannelCount()
{
    return inputBuffer.channelCount;
}

PaError PortAudioWrap::setInputSampleFormat(PaSampleFormat format)
{
    inputBuffer.sampleFormat = format;
    return paNoError;
}

PaError PortAudioWrap::setInputLatency(PaTime latency)
{
    inputBuffer.suggestedLatency = latency;
    return paNoError;
}

void PortAudioWrap::setInputHostApiStreamInfo(void *info)
{
    inputBuffer.hostApiSpecificStreamInfo = info;
}

PaTime  PortAudioWrap::getInputDefaultLowLatency()
{
    return Pa_GetDeviceInfo(inputBuffer.device)->defaultLowInputLatency;
}

PaError PortAudioWrap::setOutputChannelCount(int count)
{
    outputBuffer.channelCount = count;
    return paNoError;
}

const int PortAudioWrap::getOutputChannelCount()
{
    return outputBuffer.channelCount;
}

const char *PortAudioWrap::getDeviceName(PaDeviceIndex dev)
{
    const PaDeviceInfo *info;
    info = Pa_GetDeviceInfo(dev);
    return info->name;
}

PaError PortAudioWrap::setOutputSampleFormat(PaSampleFormat format)
{
    outputBuffer.sampleFormat = format;
    return paNoError;
}

PaError PortAudioWrap::setOutputLatency(PaTime latency)
{
    outputBuffer.suggestedLatency = latency;
    return paNoError;
}

void PortAudioWrap::setOutputHostApiStreamInfo(void *info)
{
    outputBuffer.hostApiSpecificStreamInfo = info;
}

PaTime  PortAudioWrap::getOutputDefaultLowLatency()
{
    return Pa_GetDeviceInfo(outputBuffer.device)->defaultLowOutputLatency;
}

PaError PortAudioWrap::setFramesPerBuffer(unsigned long size)
{
    framesPerBuffer = size;
    return paNoError;
}

PaError PortAudioWrap::setSampleRate(unsigned long rate)
{
    samplerate = rate;
    return paNoError;
}

PaError PortAudioWrap::setStreamFlags(PaStreamFlags flags)
{
    statusFlags = flags;
    return paNoError;
}

PaError PortAudioWrap::setInputDevice(PaDeviceIndex index)
{
//    inputDevice = index;
    inputBuffer.device = index;
    return paNoError;
}

PaError PortAudioWrap::setOutputDevice(PaDeviceIndex index)
{
//    outputDevice = index;
    outputBuffer.device = index;
    return paNoError;
}

PaError PortAudioWrap::setCallback(PaStreamCallback *callback)
{
    streamCallback = callback;
    return paNoError;
}
