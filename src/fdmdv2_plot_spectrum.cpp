//==========================================================================
// Name:            fdmdv2_plot_waterfall.cpp
// Purpose:         Implements a waterfall plot derivative of fdmdv2_plot.
// Created:         June 23, 2012
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
#include "fdmdv2_plot_spectrum.h"

BEGIN_EVENT_TABLE(PlotSpectrum, PlotPanel)
    EVT_PAINT           (PlotSpectrum::OnPaint)
    EVT_MOTION          (PlotSpectrum::OnMouseMove)
    EVT_LEFT_DOWN       (PlotSpectrum::OnMouseDown)
    EVT_LEFT_UP         (PlotSpectrum::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotSpectrum::OnMouseWheelMoved)
    EVT_SIZE            (PlotSpectrum::OnSize)
    EVT_SHOW            (PlotSpectrum::OnShow)
//    EVT_ERASE_BACKGROUND(PlotSpectrum::OnErase)
END_EVENT_TABLE()

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotSpectrum
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
//PlotSpectrum::PlotSpectrum(wxFrame* parent, int x, int y, int w, int h): PlotPanel(parent)
PlotSpectrum::PlotSpectrum(wxFrame* parent): PlotPanel(parent)
{
//    align(FL_ALIGN_TOP);
//    labelsize(10);
}

//----------------------------------------------------------------
//
//----------------------------------------------------------------
int PlotSpectrum::handle(int event)
{
/*
    //  detect a left mouse down if inside the spectrum window
    if((event == FL_NO_EVENT) && (Fl::event_button() == 1))
    {
        if((Fl::event_x() > x()) && (Fl::event_x() < (x() + w())) && (Fl::event_y() > y()) && (Fl::event_y() < (y() + h())))
        {
            // show zoomed spectrum window
            zoomSpectrumWindow->show();
        }
    }
*/
    return 0;
}

//----------------------------------------------------------------
//
//----------------------------------------------------------------
void PlotSpectrum::draw()
{
    float x_px_per_point = 0.0;
    float y_px_per_dB = 0.0;
    int   i;
    int   x1;
    int   y1;
    int   x2;
    int   y2;
    float mag1, mag2;
    char  label[20];
    float px_per_hz;
/*
    Fl_Box::draw();
    fl_color(FL_BLACK);
    fl_rectf(x(),y(),w(),h());
    fl_color(FL_GREEN);
    fl_line_style(FL_SOLID);

    fl_push_clip(x(),y(),w(),h());
    //printf("%d %d\n", w(), h());
*/
    x_px_per_point = (float)m_w / FDMDV_NSPEC;
    y_px_per_dB = (float)m_h / (MAX_DB - MIN_DB);

    // plot spectrum

    for(i=0; i<FDMDV_NSPEC-1; i++)
    {
//        mag1 = av_mag[i];
//        mag2 = av_mag[i+1];

        x1 = m_x + i * x_px_per_point;
        y1 = m_y + -mag1 * y_px_per_dB;
        x2 = m_x + (i+1) * x_px_per_point;
        y2 = m_y + -mag2 * y_px_per_dB;
//        fl_line(x1,y1,x2,y2);
    }

    // y axis graticule
//    fl_line_style(FL_DOT);
    for(i=MIN_DB; i<MAX_DB; i+=10)
    {
        x1 = m_x;
        y1 = m_y + -i * y_px_per_dB;
        x2 = m_x + m_w;
        y2 = y1;
        //printf("%d %d %d %d\n", x1, y1, x2, y2);
//        fl_line(x1,y1,x2,y2);
        sprintf(label, "%d", i);
//        fl_draw(label, x1, y1);
    }

    // x axis graticule
    px_per_hz = (float)m_w/(MAX_HZ-MIN_HZ);
//    fl_line_style(FL_DOT);
    for(i = 500; i < MAX_HZ; i += 500)
    {
        x1 = m_x + i * px_per_hz;
        y1 = m_y;
        x2 = x1;
        y2 = m_y + m_h;
        //printf("i=%d %d %d %d %d\n", i, x1, y1, x2, y2);
//        fl_line(x1,y1,x2,y2);
        sprintf(label, "%d", i);
//        fl_draw(label, x1, y2);
    }
//    fl_pop_clip();
}
