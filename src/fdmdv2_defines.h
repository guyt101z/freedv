//==========================================================================
// Name:            fdmdv2_defines.h
// Purpose:         Definitions used by plots derived from fdmdv2_plot class.
// Created:         August 27, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
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
#ifndef __FDMDV2_DEFINES__
#define __FDMDV2_DEFINES__

#include "wx/wx.h"
#include "fdmdv.h"

// Spectrogram and Waterfall

#define FDMDV_NSPEC         512

#define MIN_MAG_DB        -40.0     // min of spectrogram/waterfall magnitude axis
#define MAX_MAG_DB          0.0     // max of spectrogram/waterfall magnitude axis
#define STEP_MAG_DB         5.0     // magnitude axis step
#define BETA                0.1     // constant for time averageing spectrum data
#define MIN_F_HZ            0       // min freq on Waterfall and Spectrum
#define MAX_F_HZ            4000    // max freq on Waterfall and Spectrum
#define STEP_F_HZ           500     // freq step on Waterfall and Spectrum graticule
#define WATERFALL_SECS_Y    5       // number of seconds respresented by y axis of waterfall
#define DT                  0.1     // time between real time graphing updates
#define FS                  8000    // FDMDV modem sample rate

// Scatter diagram 

#define SCATTER_MEM_SECS    2
// (symbols/frame)/(graphics update period) = symbols/s sent to scatter memory
// memory (symbols) = secs of memory * symbols/sec
#define SCATTER_MEM_SYMS    ((int)(SCATTER_MEM_SECS*(FDMDV_NSYM/DT)))
#define SCATTER_X_MAX       4.0
#define SCATTER_Y_MAX       4.0

// Waveform plotting constants

#define WAVEFORM_PLOT_FS    100                            // sample rate (points/s) of waveform plotted to screen
#define WAVEFORM_PLOT_TIME  5                              // length or entire waveform on screen
#define WAVEFORM_PLOT_BUF   ((int)(DT*WAVEFORM_PLOT_FS))   // number of sample we add per update

// sample rate I/O & conversion constants

#define MAX_FPB             2048                           // maximum value of portAudio framesPerBuffer
#define PA_FPB              1024                           // nominal value of portAudio framesPerBuffer
#define SAMPLE_RATE         48000                          // 48 kHz sampling rate rec. as we can trust accuracy of sound card
#define N8                  FDMDV_NOM_SAMPLES_PER_FRAME    // processing buffer size at 8 kHz
#define MEM8                (FDMDV_OS_TAPS/FDMDV_OS)
#define N48                 (N8*FDMDV_OS)                  // processing buffer size at 48 kHz
#define NUM_CHANNELS        2                              // I think most sound cards prefer stereo we will convert to mono

#define BITS_PER_CODEC_FRAME  (2 * FDMDV_BITS_PER_FRAME)
#define BYTES_PER_CODEC_FRAME (BITS_PER_CODEC_FRAME / 8)

enum
{
    ID_ROTATE_LEFT = wxID_HIGHEST + 1,
    ID_ROTATE_RIGHT,
    ID_RESIZE,
    ID_PAINT_BG
};

#endif  //__FDMDV2_DEFINES__
