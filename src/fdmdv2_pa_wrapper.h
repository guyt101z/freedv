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

#define PA_SAMPLE_TYPE      paInt16         //paFloat32
#define FRAMES_PER_BUFFER   (64)

typedef float SAMPLE;

class PortAudioWrap
{
    public:
        PortAudioWrap();
        ~PortAudioWrap();

//        float                           m_av_mag[FDMDV_NSPEC];

    private:
        PaStream                        *m_pStream;
        void                            *m_pUserData;
        PaStreamCallback                *m_pStreamCallback;
        PaStreamFinishedCallback        *m_pStreamFinishedCallback;
        const PaStreamCallbackTimeInfo  *m_pTimeInfo;
        struct FDMDV                    *m_pFDMDV_state;
        PaStreamParameters              m_inputBuffer;
        PaStreamParameters              m_outputBuffer;
        int                             m_samplerate;
        unsigned long                   m_framesPerBuffer;
        PaStreamCallbackFlags           m_statusFlags;
        bool                            m_newdata;

    public:

        void                averageData(float mag_dB[]);

        PaDeviceIndex       getDefaultInputDevice();
        PaDeviceIndex       getDefaultOutputDevice();
        PaStreamParameters  *getDeviceInfo(PaDeviceIndex idx);

        PaError             setFramesPerBuffer(unsigned long size);
        PaError             setSampleRate(unsigned long size);

        PaError             setStreamFlags(PaStreamFlags flags);
        PaError             setCallback(PaStreamCallback *m_pStreamCallback);
        PaError             setStreamCallback(PaStream *stream, PaStreamCallback* callback) { m_pStreamCallback = callback; return 0;}
        PaError             setStreamFinishedCallback(PaStream *stream, PaStreamFinishedCallback* m_pStreamFinishedCallback);

        void                setInputBuffer(const PaStreamParameters& inputBuffer)   {this->m_inputBuffer = inputBuffer;}
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
        void                setOutputHostApiStreamInfo(void *info = NULL);
        PaTime              getOutputDefaultLowLatency();

        void                setFdmdvState(FDMDV* fdmdv_state)                       {this->m_pFDMDV_state = fdmdv_state;}
        void                setOutputBuffer(const PaStreamParameters& outputBuffer) {this->m_outputBuffer = outputBuffer;}
        void                setTimeInfo(PaStreamCallbackTimeInfo* timeInfo)         {this->m_pTimeInfo = timeInfo;}
        void                setUserData(void* userData)                             {this->m_pUserData = userData;}
        unsigned long       getFramesPerBuffer() const                              {return m_framesPerBuffer;}
        const               PaStreamParameters& getInputBuffer() const              {return m_inputBuffer;}
        const               PaStreamParameters& getOutputBuffer() const             {return m_outputBuffer;}
        const               PaStreamCallbackFlags& getStatusFlags() const           {return m_statusFlags;}

        FDMDV*              getFdmdvState()                                         {return m_pFDMDV_state;}
        int                 getSamplerate() const                                   {return m_samplerate;}
        PaStream*           getStream()                                             {return m_pStream;}
        void                *getUserData()                                          {return m_pUserData;}
        bool                getDataAvail()                                          {return m_newdata;}
        PaError             streamStart();
        PaError             streamClose();
        PaError             streamOpen();
        void                terminate();
        void                stop();
        void                abort();
        bool                isOpen() const;
        bool                isStopped() const;
        bool                isActive() const;
//        void                loadData();
};
