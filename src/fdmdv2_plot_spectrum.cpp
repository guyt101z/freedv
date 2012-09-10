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
    EVT_MOTION          (PlotSpectrum::OnMouseMove)
    EVT_LEFT_DOWN       (PlotSpectrum::OnMouseDown)
    EVT_LEFT_UP         (PlotSpectrum::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotSpectrum::OnMouseWheelMoved)
    EVT_PAINT           (PlotSpectrum::OnPaint)
    EVT_SIZE            (PlotSpectrum::OnSize)
    EVT_SHOW            (PlotSpectrum::OnShow)
//    EVT_ERASE_BACKGROUND(PlotSpectrum::OnErase)
END_EVENT_TABLE()

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotSpectrum
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
PlotSpectrum::PlotSpectrum(wxFrame* parent): PlotPanel(parent)
{
    SetLabelSize(10.0);
}

#define TEXT_BASELINE_OFFSET_Y  -5

//-------------------------------------------------------------------------
// drawGraticule()
//-------------------------------------------------------------------------
void PlotSpectrum::drawGraticule(wxAutoBufferedPaintDC&  dc)
{
    int p;
    char buf[15];
    wxString s;
    //int h_mod_inc = 0;

    // Vertical gridlines
    dc.SetPen(m_penShortDash);
    for(p = (PLOT_BORDER + XLEFT_OFFSET + GRID_INCREMENT); p < ((m_w - XLEFT_OFFSET) + GRID_INCREMENT); p += GRID_INCREMENT)
    {
        dc.DrawLine(p, (m_h + PLOT_BORDER), p, PLOT_BORDER);
    }

    int y_zero = (m_h - m_top) / 2 ;
    dc.SetPen(m_penSolid);
    dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, y_zero, (m_w + PLOT_BORDER + XLEFT_OFFSET), y_zero);
    sprintf(buf, "%6.0f", 0.0);
    dc.DrawText(buf, XLEFT_TEXT_OFFSET, y_zero + TEXT_BASELINE_OFFSET_Y);

    // Horizontal gridlines
    dc.SetPen(m_penDotDash);
    for(p = 0; (y_zero + p) < m_h ; p += GRID_INCREMENT)
    {
        if(p > 0)
        {
            dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (y_zero + p), (m_w + PLOT_BORDER + XLEFT_OFFSET), (y_zero + p));
            sprintf(buf, "%6.0f", (double)(p) * -10);
            dc.DrawText(buf, XLEFT_TEXT_OFFSET, (y_zero + p + TEXT_BASELINE_OFFSET_Y));

            dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (y_zero - p), (m_w + PLOT_BORDER + XLEFT_OFFSET), (y_zero - p));
            sprintf(buf, "%6.0f", (double)(p) * 10);
            dc.DrawText(buf, XLEFT_TEXT_OFFSET, (y_zero - p + TEXT_BASELINE_OFFSET_Y));
        }
    }

    // Label the X-Axis
    dc.SetPen(wxPen(GREY_COLOR, 1));
    for(p = GRID_INCREMENT; p < (m_w - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    {
        sprintf(buf, "%1.1f Hz",(double)(p / 10));
        dc.DrawText(buf, p - PLOT_BORDER + XLEFT_OFFSET, m_h + YBOTTOM_OFFSET/2);
    }
}

//----------------------------------------------------------------
// draw()
//----------------------------------------------------------------
void PlotSpectrum::draw(wxAutoBufferedPaintDC&  dc)
{
/*
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
*/
    m_rectCtrl  = GetClientRect();
    m_rectGrid  = m_rectCtrl;

    m_rectGrid.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));
    m_rectGrid.Offset(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER);

//    m_rectGrid.Deflate(PLOT_BORDER, (PLOT_BORDER + (YBOTTOM_OFFSET/2)));
//    m_rectGrid.Offset(PLOT_BORDER, PLOT_BORDER);

    m_h = m_rectGrid.GetHeight();
    m_w = m_rectGrid.GetWidth();

    dc.Clear();

    // Draw a filled rectangle with aborder
    wxBrush ltYellowBrush = wxBrush(LIGHT_YELLOW_COLOR);
    dc.SetBrush(ltYellowBrush);
//    dc.SetBrush(BLACK_COLOR);
//    dc.SetPen(wxPen(BLACK_COLOR, 1));
    m_top = PLOT_BORDER;
    m_left = PLOT_BORDER + XLEFT_OFFSET;
    dc.DrawRectangle(m_left, m_top, m_w, m_h);

    drawGraticule(dc);
/*
    Fl_Box::draw();
    fl_color(FL_BLACK);
    fl_rectf(x(),y(),w(),h());
    fl_color(FL_GREEN);
    fl_line_style(FL_SOLID);

    fl_push_clip(x(),y(),w(),h());
    //printf("%d %d\n", w(), h());
*/
/*
    dc.SetBrush(*wxBLUE_BRUSH);
    dc.SetPen(wxPen(YELLOW_COLOR, 2));
    dc.DrawRectangle(PLOT_BORDER, PLOT_BORDER, m_w, m_h);

    x_px_per_point = (float)m_w / FDMDV_NSPEC;
    y_px_per_dB = (float)m_h / (MAX_DB - MIN_DB);


    // plot spectrum
    for(i = 0; i < FDMDV_NSPEC - 1; i++)
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
 */
}

//----------------------------------------------------------------
// OnPaint()
//----------------------------------------------------------------
void PlotSpectrum::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    draw(dc);
}

//----------------------------------------------------------------
// OnSize()
//----------------------------------------------------------------
void PlotSpectrum::OnSize(wxSizeEvent& event)
{
    if(m_use_bitmap)
    {
        this->Refresh();
    }
}

//----------------------------------------------------------------
// OnShow()
//----------------------------------------------------------------
void PlotSpectrum::OnShow(wxShowEvent& event)
{
//   wxAutoBufferedPaintDC dc(this);
//   draw(dc);
}
