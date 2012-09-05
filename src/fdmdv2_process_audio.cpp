//==========================================================================
// Name:            fdmdv2_process_audio.cpp
// Purpose:         Implements processing of data received from audio interfaces.
// Created:         August 12, 2012
// Initial author:  David Rowe
// Derived from:    code trivially converted for integration with C++ code
//                  by Dave Witten
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
/*

  FUNCTION: per_frame_rx_processing()
  AUTHOR..: David Rowe
  DATE....: July 2012

  Called every rx frame to take a buffer of input modem samples and
  convert them to a buffer of output speech samples.

  The sample source could be a sound card or file.  The sample source
  supplies a fixed number of samples with each call.  However
  fdmdv_demod requires a variable number of samples for each call.
  This function will buffer as appropriate and call fdmdv_demod with
  the correct number of samples.

  The processing sequence is:

  collect demod input samples from sound card 1 A/D
  while we have enough samples:
        demod samples into bits
        decode bits into speech samples
        output a buffer of speech samples to sound card 2 D/A

  Note that sound card 1 and sound card 2 will have slightly different
  sample rates, as their sample clocks are not syncronised.  We
  effectively lock the system to the demod A/D (sound card 1) sample
  rate. This ensures the demod gets a continuous sequence of samples,
  maintaining sync. Sample underflow or overflow will instead occur on
  the sound card 2 D/A.  This is acceptable as a buffer of lost or
  extra speech samples is unlikely to be noticed.

  The situation is actually a little more complex than that.  Through
  the demod timing estimation the buffers supplied to sound card D/A 2
  are effectively clocked at the remote modulator sound card D/A clock
  rate.  We slip/gain buffers supplied to sound card 2 to compensate.

  The current demod handles varying clock rates by having a variable
  number of input samples, e.g. 120 160 (nominal) or 200.  However the
  A/D always delivers a fixed number of samples.

  So we currently need some logic between the A/D and the demod:
        + A/D delivers fixed number of samples
        + demod processes a variable number of samples
        + this means we run demod 0,1 or 2 times, depending
            on number of buffered A/D samples
        + demod always outputs 1 frame of bits
        + so run demod and speech decoder 0, 1 or 2 times

  The ouput of the demod is codec voice data so it's OK if we miss or
  repeat a frame every now and again.

*/
#include "fdmdv2_main.h"
#include "portaudio.h"

//#include "fdmdv.h"
#include "codec2.h"

//-- Globals: Eliminate!! --------------------------------
//char         *fin_name = NULL;
//char         *fout_name = NULL;
//char         *sound_dev_name = NULL;
//FILE         *fin = NULL;
//FILE         *fout = NULL;
struct FDMDV  *fdmdv_state;
struct CODEC2 *codec2;
//float         av_mag[FDMDV_NSPEC];                  // shared between a few classes
extern float *av_mag;
//--------------------------------------------------------

void new_data(float *);
float  Ts = 0.0;
short  input_buf[2*FDMDV_NOM_SAMPLES_PER_FRAME];
int    n_input_buf = 0;
int    nin = FDMDV_NOM_SAMPLES_PER_FRAME;
short  *output_buf;
int    n_output_buf = 0;
int    codec_bits[2*FDMDV_BITS_PER_FRAME];
int    state = 0;

// Portaudio states -----------------------------

PaStream *stream = NULL;
PaError err;

typedef struct
{
    float               in48k[FDMDV_OS_TAPS + N48];
    float               in8k[MEM8 + N8];
} paCallBackData;

//----------------------------------------------------------------
// per_frame_rx_processing()
//----------------------------------------------------------------
void per_frame_rx_processing(
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
        new_data(rx_spec);
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
void new_data(float mag_dB[])
{
    int i;

    for(i = 0; i < FDMDV_NSPEC; i++)
    {
        av_mag[i] = (1.0 - BETA) * av_mag[i] + BETA * mag_dB[i];
    }
}

//----------------------------------------------------------------
// Redraw windows every DT seconds.
//----------------------------------------------------------------
void update_gui(int nin, float *Ts)
{
    *Ts += (float)nin/FS;
    *Ts += (float)nin/FS;
    if(*Ts >= DT)
    {
/*
        *Ts -= DT;
        if(!zoomSpectrumWindow->shown() && !zoomWaterfallWindow->shown())
        {
            aSpectrum->redraw();
            aWaterfall->redraw();
            aScatter->redraw();
            aTimingEst->redraw();
            aFreqEst->redraw();
            aSNR->redraw();
        }
        if(zoomSpectrumWindow->shown())
        {
            aZoomedSpectrum->redraw();
        }
        if(zoomWaterfallWindow->shown())
        {
            aZoomedWaterfall->redraw();
        }
*/
    }
}

//----------------------------------------------------------------
//  idle() is the FLTK function that gets continusouly called when FLTK
//  is not doing GUI work.  We use this function for providing file
//  input to update the GUI when simulating real time operation.
//----------------------------------------------------------------
/*
void idle(void*)
{
    int ret, i;
    if(fin_name != NULL)
    {
        ret = fread(&input_buf[n_input_buf], sizeof(short), FDMDV_NOM_SAMPLES_PER_FRAME, fin);
        n_input_buf += FDMDV_NOM_SAMPLES_PER_FRAME;
        per_frame_rx_processing(output_buf, &n_output_buf, codec_bits, input_buf, &n_input_buf, &nin, &state, codec2);
        if(fout_name != NULL)
        {
            if(n_output_buf >= N8)
            {
                ret = fwrite(output_buf, sizeof(short), N8, fout);
                n_output_buf -= N8;
                assert(n_output_buf >= 0);
                // shift speech sample output buffer
                for(i=0; i<n_output_buf; i++)
                {
                    output_buf[i] = output_buf[i+N8];
                }
            }
        }
    }
    update_gui(nin, &Ts);
    // simulate time delay from real world A/D input, and pause betwen
    // screen updates
    usleep(20000);
}
*/

/*
//----------------------------------------------------------------
// This routine will be called by the PortAudio engine when audio
//  is available.
//----------------------------------------------------------------
static int callback(
                        const void *inputBuffer,
                        void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData
                    )
{
    paCallBackData *cbData = (paCallBackData*)userData;
    unsigned int    i;
    short           *rptr = (short*)inputBuffer;
    short           *wptr = (short*)outputBuffer;
    float           *in8k = cbData->in8k;
    float           *in48k = cbData->in48k;
    float           out8k[N8];
    float           out48k[N48];
    short           out48k_short[N48];

    (void) timeInfo;
    (void) statusFlags;

    assert(inputBuffer != NULL);
    // Convert input model samples from 48 to 8 kHz
    // just use left channel
    for(i = 0; i < framesPerBuffer; i++, rptr += 2)
    {
        in48k[i + FDMDV_OS_TAPS] = *rptr;
    }
    // downsample and update filter memory
    fdmdv_48_to_8(out8k, &in48k[FDMDV_OS_TAPS], N8);
    for(i = 0; i < FDMDV_OS_TAPS; i++)
    {
        in48k[i] = in48k[i+framesPerBuffer];
    }
    // run demod, decoder and update GUI info
    for(i=0; i<N8; i++)
    {
        input_buf[n_input_buf + i] = (short)out8k[i];
    }
    n_input_buf += FDMDV_NOM_SAMPLES_PER_FRAME;
    per_frame_rx_processing(output_buf, &n_output_buf, codec_bits, input_buf, &n_input_buf, &nin, &state, codec2);

    // if demod out of sync copy input audio from A/D to aid in tuning
    if(n_output_buf >= N8)
    {
        if(state == 0)
        {
            for(i=0; i<N8; i++)
            {
                in8k[MEM8 + i] = out8k[i];       // A/D signal
            }
        }
        else
        {
            for(i=0; i < N8; i++)
            {
                in8k[MEM8+i] = output_buf[i];   // decoded speech
            }
        }
        n_output_buf -= N8;
    }
    assert(n_output_buf >= 0);
    // shift speech samples in output buffer
    for(i = 0; i < (unsigned int)n_output_buf; i++)
    {
        output_buf[i] = output_buf[i+N8];
    }
    // Convert output speech to 48 kHz sample rate
    // upsample and update filter memory
    fdmdv_8_to_48(out48k, &in8k[MEM8], N8);
    for(i = 0; i < MEM8; i++)
    {
        in8k[i] = in8k[i+N8];
    }
    assert(outputBuffer != NULL);
    // write signal to both channels
    for(i = 0; i < N48; i++)
    {
        out48k_short[i] = (short)out48k[i];
    }
    for(i=0; i<framesPerBuffer; i++, wptr+=2)
    {
        wptr[0] = out48k_short[i];
        wptr[1] = out48k_short[i];
    }
    return paContinue;
}
*/
/*
//----------------------------------------------------------------
// arg_callback()
//----------------------------------------------------------------
int arg_callback(int argc, char **argv, int &i)
{
    if(argv[i][1] == 'i')
    {
        if((i+1) >= argc)
        {
            return 0;
        }
        fin_name = argv[i+1];
        i += 2;
        return 2;
    }
    if(argv[i][1] == 'o')
    {
        if((i+1) >= argc)
        {
            return 0;
        }
        fout_name = argv[i+1];
        i += 2;
        return 2;
    }
    if(argv[i][1] == 's')
    {
        if((i+1) >= argc)
        {
            return 0;
        }
        sound_dev_name = argv[i+1];
        i += 2;
        return 2;
    }
    return 0;
}
*/
