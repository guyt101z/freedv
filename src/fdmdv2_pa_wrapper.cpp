//==========================================================================
// Name:            fdmdv2_pa_wrapper.cpp
// Purpose:         Implements a wrapper class around the PortAudio library.
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
    loadData();
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
    inputBuffer.device = index;
    return paNoError;
}

PaError PortAudioWrap::setOutputDevice(PaDeviceIndex index)
{
    outputBuffer.device = index;
    return paNoError;
}

PaError PortAudioWrap::setCallback(PaStreamCallback *callback)
{
    streamCallback = callback;
    return paNoError;
}

typedef struct
{
    float               in48k[FDMDV_OS_TAPS + N48];
    float               in8k[MEM8 + N8];
} paCallBackData;


//----------------------------------------------------------------
// per_frame_rx_processing()
//----------------------------------------------------------------
void  PortAudioWrap::per_frame_rx_processing(
                                short   output_buf[],  /* output buf of decoded speech samples          */
                                int     *n_output_buf, /* how many samples currently in output_buf[]    */
                                int     codec_bits[],  /* current frame of bits for decoder             */
                                short   input_buf[],   /* input buf of modem samples input to demod     */
                                int     *n_input_buf,  /* how many samples currently in input_buf[]     */
                                int     *nin,          /* amount of samples demod needs for next call   */
                                int     *state,        /* used to collect codec_bits[] halves           */
                                struct  CODEC2 *c2     /* Codec 2 states                                */
                            )
{
    struct FDMDV_STATS  stats;
    int                 sync_bit;
    float               rx_fdm[FDMDV_MAX_SAMPLES_PER_FRAME];
    int                 rx_bits[FDMDV_BITS_PER_FRAME];
    unsigned char       packed_bits[BYTES_PER_CODEC_FRAME];
    float               rx_spec[FDMDV_NSPEC];
    int                 i;
    int                 nin_prev;
    int                 bit;
    int                 byte;
    int                 next_state;

    assert(*n_input_buf <= (2 * FDMDV_NOM_SAMPLES_PER_FRAME));

    /*
      This while loop will run the demod 0, 1 (nominal) or 2 times:

      0: when tx sample clock runs faster than rx, occasionally we
         will run out of samples

      1: normal, run decoder once, every 2nd frame output a frame of
         speech samples to D/A

      2: when tx sample clock runs slower than rx, occasionally we will
         have enough samples to run demod twice.

      With a +/- 10 Hz sample clock difference at FS=8000Hz (+/- 1250
      ppm), case 0 or 1 occured about once every 30 seconds.  This is
      no problem for the decoded audio.
    */
    while(*n_input_buf >= *nin)
    {
        // demod per frame processing
        for(i = 0; i < *nin; i++)
        {
            rx_fdm[i] = (float)input_buf[i]/FDMDV_SCALE;
        }
        nin_prev = *nin;
        fdmdv_demod(fdmdv_state, rx_bits, &sync_bit, rx_fdm, nin);
        *n_input_buf -= nin_prev;
        assert(*n_input_buf >= 0);

        // shift input buffer
        for(i=0; i<*n_input_buf; i++)
        {
            input_buf[i] = input_buf[i+nin_prev];
        }

        // compute rx spectrum & get demod stats, and update GUI plot data
        fdmdv_get_rx_spectrum(fdmdv_state, rx_spec, rx_fdm, nin_prev);
        fdmdv_get_demod_stats(fdmdv_state, &stats);
        averageData(rx_spec);
//        aScatter->add_new_samples(stats.rx_symbols);
//        aTimingEst->add_new_sample(stats.rx_timing);
//        aFreqEst->add_new_sample(stats.foff);
//        aSNR->add_new_sample(stats.snr_est);
        /*
           State machine to:

           + Mute decoded audio when out of sync.  The demod is synced
             when we are using the fine freq estimate and SNR is above
             a thresh.

           + Decode codec bits only if we have a 0,1 sync bit
             sequence.  Collects two frames of demod bits to decode
             one frame of codec bits.
        */
        next_state = *state;
        switch(*state)
        {
            case 0:
                // mute output audio when out of sync
                if(*n_output_buf < 2 * codec2_samples_per_frame(c2) - N8)
                {
                    for(i=0; i<N8; i++)
                    {
                        output_buf[*n_output_buf + i] = 0;
                    }
                    *n_output_buf += N8;
                }
                assert(*n_output_buf <= (2 * codec2_samples_per_frame(c2)));
                if((stats.fest_coarse_fine == 1) && (stats.snr_est > 3.0))
                {
                    next_state = 1;
                }
                break;

            case 1:
                if(sync_bit == 0)
                {
                    next_state = 2;
                    // first half of frame of codec bits
                    memcpy(codec_bits, rx_bits, FDMDV_BITS_PER_FRAME * sizeof(int));
                }
                else
                {
                    next_state = 1;
                }
                if(stats.fest_coarse_fine == 0)
                {
                    next_state = 0;
                }
                break;

            case 2:
                next_state = 1;
                if(stats.fest_coarse_fine == 0)
                {
                    next_state = 0;
                }
                if(sync_bit == 1)
                {
                    // second half of frame of codec bits
                    memcpy(&codec_bits[FDMDV_BITS_PER_FRAME], rx_bits, FDMDV_BITS_PER_FRAME*sizeof(int));
                    // pack bits, MSB received first
                    bit  = 7;
                    byte = 0;
                    memset(packed_bits, 0, BYTES_PER_CODEC_FRAME);
                    for(i = 0; i < BITS_PER_CODEC_FRAME; i++)
                    {
                        packed_bits[byte] |= (codec_bits[i] << bit);
                        bit--;
                        if(bit < 0)
                        {
                            bit = 7;
                            byte++;
                        }
                    }
                    assert(byte == BYTES_PER_CODEC_FRAME);
                    // add decoded speech to end of output buffer
                    if(*n_output_buf <= codec2_samples_per_frame(c2))
                    {
                        codec2_decode(c2, &output_buf[*n_output_buf], packed_bits);
                        *n_output_buf += codec2_samples_per_frame(c2);
                    }
                    assert(*n_output_buf <= (2 * codec2_samples_per_frame(c2)));
                }
                break;
        }
        *state = next_state;
    }
}

//----------------------------------------------------------------
// update average of each spectrum point
//----------------------------------------------------------------
void PortAudioWrap::averageData(float mag_dB[])
{
    int i;

    for(i = 0; i < FDMDV_NSPEC; i++)
    {
        m_av_mag[i] = (1.0 - BETA) * m_av_mag[i] + BETA * mag_dB[i];
    }
}

//----------------------------------------------------------------
// create Dummy Data
//----------------------------------------------------------------
void PortAudioWrap::loadData()
{
    int i;
    for(i = 0; i < FDMDV_NSPEC; i++)
    {
        m_av_mag[i] = i;
    }
}
