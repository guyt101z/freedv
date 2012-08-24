//==========================================================================
// Name:            fdmdv2_plot_waterfall.cpp
// Purpose:         Implements a waterfall plot derivative of fdmdv2_plot.
// Created:         June 22, 2012
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
#include <string.h>
#include "wx/wx.h"
#include "fdmdv2_main.h"
#include "fdmdv2_plot.h"
#include "fdmdv2_plot_waterfall.h"

extern float *av_mag;
/*

  Notes:

  The height h() pixels represents WATERFALL_SECS_Y of data.  Every DT
  seconds we get a vector of FDMDV_NSPEC spectrum samples which we use
  to update the last row.  The height of each row is dy pixels, which
  maps to DT seconds.  We call each dy high rectangle of pixels a
  block.

*/
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class WaterfallPlot
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
//Waterfall::Waterfall(int x, int y, int w, int h): Fl_Box(x, y, w, h, "Waterfall")
Waterfall::Waterfall(wxFrame* parent, int x, int y, int w, int h, const char name[]): DrawPanel(parent)
{
    int   i;

    for(i=0; i<255; i++)
    {
        heatmap_lut[i] = heatmap((float)i, 0.0, 255.0);
    }
    greyscale = 0;
    //align(FL_ALIGN_TOP);
    //labelsize(10);
    new_pixel_buf(w,h);
};

Waterfall::~Waterfall()
{
    delete pixel_buf;
}

void Waterfall::new_pixel_buf(int w, int h)
{
    int buf_sz, i;

    prev_w = w; prev_h = h;
    buf_sz = h*w;
    pixel_buf = new unsigned[buf_sz];
    for(i=0; i<buf_sz; i++)
    {
        pixel_buf[i] = 0;
    }
}

int Waterfall::handle(int event)
{
    //  detect a left mouse down if inside the window
/*
    if ((event == FL_NO_EVENT) && (Fl::event_button() == 1))
    {
        if ((Fl::event_x() > x()) && (Fl::event_x() < (x() + w())) && (Fl::event_y() > y()) && (Fl::event_y() < (y() + h())))
        {
            // show zoomed spectrum window
            zoomWaterfallWindow->show();
        }
    }
*/
    return 0;
}

// map val to a rgb colour
// from http://eddiema.ca/2011/01/21/c-sharp-heatmaps/
unsigned Waterfall::heatmap(float val, float min, float max)
{
    unsigned r = 0;
    unsigned g = 0;
    unsigned b = 0;

    val = (val - min) / (max - min);
    if(val <= 0.2)
    {
        b = (unsigned)((val / 0.2) * 255);
    }
    else if(val >  0.2 &&  val <= 0.7)
    {
        b = (unsigned)((1.0 - ((val - 0.2) / 0.5)) * 255);
    }
    if(val >= 0.2 &&  val <= 0.6)
    {
        g = (unsigned)(((val - 0.2) / 0.4) * 255);
    }
    else if(val >  0.6 &&  val <= 0.9)
    {
        g = (unsigned)((1.0 - ((val - 0.6) / 0.3)) * 255);
    }
    if(val >= 0.5)
    {
        r = (unsigned)(((val - 0.5) / 0.5) * 255);
    }
    //printf("%f %x %x %x\n", val, r, g, b);
    return  (b << 16) + (g << 8) + r;
}

void Waterfall::draw()
{
    float  spec_index_per_px;
    float  intensity_per_dB;
    int    px_per_sec;
    int    index;
    int    dy;
    int    dy_blocks;
    int    bytes_in_row_of_blocks;
    int    b;
    int    px;
    int    py;
    int    intensity;
    unsigned *last_row;
    unsigned *pdest;
    unsigned *psrc;

    /* detect resizing of window */
    if ((m_h != m_prev_h) || (m_w != m_prev_w))
    {
        delete pixel_buf;
        new_pixel_buf(m_w, m_h);
    }
//    Fl_Box::draw();
    // determine dy, the height of one "block"
    px_per_sec = (float)m_h / WATERFALL_SECS_Y;
    dy = DT * px_per_sec;
    // number of dy high blocks in spectrogram
    dy_blocks = m_h / dy;
    // shift previous bit map
    bytes_in_row_of_blocks = dy * m_w * sizeof(unsigned);
    for(b = 0; b < dy_blocks - 1; b++)
    {
        pdest = pixel_buf + b * m_w * dy;
        psrc  = pixel_buf + (b + 1) * m_w * dy;
        memcpy(pdest, psrc, bytes_in_row_of_blocks);
    }
    // create a new row of blocks at bottom
    spec_index_per_px = (float)FDMDV_NSPEC / (float) m_w;
    intensity_per_dB = (float)256 /(MAX_DB - MIN_DB);
    last_row = pixel_buf + dy *(dy_blocks - 1)* m_w;
    for(px = 0; px < m_w; px++)
    {
        index = px * spec_index_per_px;
        intensity = intensity_per_dB * (av_mag[index] - MIN_DB);
        if (intensity > 255)
        {
            intensity = 255;
        }
        if (intensity < 0)
        {
            intensity = 0;
        }
        if (greyscale)
        {
            for(py = 0; py < dy; py++)
            {
                last_row[px + py * m_w] = intensity<<8;
            }
        }
        else
        {
            for(py = 0; py < dy; py++)
            {
                last_row[px + py * m_w] = heatmap_lut[intensity];
            }
        }
    }
    // update bit map
    //fl_draw_image((unsigned char*)pixel_buf, m_x, m_y, m_w, m_h, 4, 0);
}


