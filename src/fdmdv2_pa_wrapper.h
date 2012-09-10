//==========================================================================
// Name:            fdmdv2_pa_wrapper.h
// Purpose:         Defines a wrapper class around PortAudio
// Created:         August 12, 2012
// Initial author:  David Witten
// License:
//
//  Copyright (C) 2012 David Witten
//
//  All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#include <stdio.h>
#include <math.h>
#include "fdmdv2_defines.h"
#include "fdmdv.h"
#include "codec2.h"
#include "portaudio.h"

// Note that many of the older ISA sound cards on PCs do NOT support
// full duplex audio (simultaneous record and playback).
// And some only support full duplex at lower sample rates.
//#define SAMPLE_RATE         (44100)
//
#define PA_SAMPLE_TYPE      paFloat32       //paInt16
#define FRAMES_PER_BUFFER   (64)

typedef float SAMPLE;

class PortAudioWrap
{
    public:
        PortAudioWrap();
        ~PortAudioWrap();

        float                           m_av_mag[FDMDV_NSPEC];

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
        struct FDMDV                    *fdmdv_state;

    public:
        void per_frame_rx_processing(
            short   output_buf[],  /* output buf of decoded speech samples          */
            int     *n_output_buf, /* how many samples currently in output_buf[]    */
            int     codec_bits[],  /* current frame of bits for decoder             */
            short   input_buf[],   /* input buf of modem samples input to demod     */
            int     *n_input_buf,  /* how many samples currently in input_buf[]     */
            int     *nin,          /* amount of samples demod needs for next call   */
            int     *state,        /* used to collect codec_bits[] halves           */
            struct  CODEC2 *c2     /* Codec 2 states                                */
        );

        /*
                static int Callback(
                                        const void *inBuffer,
                                        void *outBuffer,
                                        unsigned long framesPerBuffer,
                                        const PaStreamCallbackTimeInfo *outTime,
                                        PaStreamCallbackFlags statusFlags,
                                        void *userData
                                   );
                static int txCallback(
                                        const void *inBuffer,
                                        void *outBuffer,
                                        unsigned long framesPerBuffer,
                                        const PaStreamCallbackTimeInfo *outTime,
                                        PaStreamCallbackFlags statusFlags,
                                        void *userData
                                     );
        */

        PaDeviceIndex       getDefaultInputDevice();
        PaDeviceIndex       getDefaultOutputDevice();
        PaStreamParameters  *getDeviceInfo(PaDeviceIndex idx);

        PaError             setFramesPerBuffer(unsigned long size);
        PaError             setSampleRate(unsigned long size);
        PaError             setStreamFlags(PaStreamFlags flags);
        PaError             setCallback(PaStreamCallback *streamCallback);
        PaError             setStreamCallback(PaStream *stream, PaStreamCallback* callback) { streamCallback = callback; return 0;}
        PaError             setStreamFinishedCallback(PaStream *stream, PaStreamFinishedCallback* streamFinishedCallback);
        PaError             streamOpen();

        PaError             setInputDevice(PaDeviceIndex dev);
        PaError             setInputChannelCount(int count);
        int                 getInputChannelCount();
        PaError             setInputSampleFormat(PaSampleFormat format);
        PaError             setInputSampleRate(PaSampleFormat format);
        PaError             setInputLatency(PaTime latency);
        void                setInputHostApiStreamInfo(void *info = NULL);
        PaTime              getInputDefaultLowLatency();
        const char          *getDeviceName(PaDeviceIndex dev);

        PaError             setOutputDevice(PaDeviceIndex dev);
        PaError             setOutputChannelCount(int count);
        const int           getOutputChannelCount();
        PaError             setOutputSampleFormat(PaSampleFormat format);
        PaError             setOutputLatency(PaTime latency);
        PaError             streamStart();
        PaError             streamClose();
        void                setOutputHostApiStreamInfo(void *info = NULL);
        PaTime              getOutputDefaultLowLatency();
        void                averageData(float mag_dB[]);

        void                setFdmdvState(FDMDV* fdmdv_state)                       {this->fdmdv_state = fdmdv_state;}
//        void                setFramesPerBuffer(unsigned long framesPerBuffer)       {this->framesPerBuffer = framesPerBuffer;}
        void                setInputBuffer(const PaStreamParameters& inputBuffer)   {this->inputBuffer = inputBuffer;}
//        void                setAvMag(float av_mag)                                  {this->m_av_mag = av_mag;}
        void                setOutputBuffer(const PaStreamParameters& outputBuffer) {this->outputBuffer = outputBuffer;}
        void                setSamplerate(int samplerate)                           {this->samplerate = samplerate;}
        void                setStatusFlags(const PaStreamCallbackFlags& statusFlags) {this->statusFlags = statusFlags;}
        void                setStream(PaStream* stream)                             {this->stream = stream;}
//        void                setStreamCallback(PaStreamCallback* streamCallback)     {this->streamCallback = streamCallback;}
        void                setStreamFinishedCallback(PaStreamFinishedCallback* streamFinishedCallback) {this->streamFinishedCallback = streamFinishedCallback;}
        void                setTimeInfo(PaStreamCallbackTimeInfo* timeInfo)         {this->timeInfo = timeInfo;}
        void                setUserData(void* userData)                             {this->userData = userData;}
        FDMDV*              getFdmdvState()                                         {return fdmdv_state;}
        unsigned long       getFramesPerBuffer() const                              {return framesPerBuffer;}
        const               PaStreamParameters& getInputBuffer() const              {return inputBuffer;}
//        const float         *getAvMag(int idx) const                                 {return (float *)&m_av_mag[idx];}
        const               PaStreamParameters& getOutputBuffer() const             {return outputBuffer;}
        int                 getSamplerate() const                                   {return samplerate;}
        const PaStreamCallbackFlags& getStatusFlags() const                         {return statusFlags;}
        PaStream*           getStream()                                             {return stream;}
//        PaStreamCallback*  getStreamCallback()                                       {return streamCallback;}
//        PaStreamFinishedCallback* getStreamFinishedCallback()                       {return streamFinishedCallback;}
//        PaStreamCallbackTimeInfo* getTimeInfo()                                     {return timeInfo;}
        void                *getUserData()                                          {return userData;}

        void                terminate();
        void                stop();
        void                abort();
        bool                isOpen() const;
        bool                isStopped() const;
        bool                isActive() const;
        void                loadData();
};
