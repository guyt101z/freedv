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
BEGIN_EVENT_TABLE(PlotWaterfall, PlotPanel)
    EVT_PAINT           (PlotWaterfall::OnPaint)
    EVT_MOTION          (PlotWaterfall::OnMouseMove)
    EVT_LEFT_DOWN       (PlotWaterfall::OnMouseDown)
    EVT_LEFT_UP         (PlotWaterfall::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotWaterfall::OnMouseWheelMoved)
    EVT_SIZE            (PlotWaterfall::OnSize)
    EVT_SHOW            (PlotWaterfall::OnShow)
//    EVT_ERASE_BACKGROUND(PlotWaterfall::OnErase)
END_EVENT_TABLE()

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class WaterfallPlot
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
PlotWaterfall::PlotWaterfall(wxFrame* parent): PlotPanel(parent)
{
    int   i;

    m_bmp = new wxBitmap(MAX_BMP_X, MAX_BMP_Y, wxBITMAP_SCREEN_DEPTH);
    for(i = 0; i < 255; i++)
    {
        m_heatmap_lut[i] = heatmap((float)i, 0.0, 255.0);
    }
    m_greyscale = 0;
    SetLabelSize(10.0);
    m_Bufsz = GetMaxClientSize();
};

//----------------------------------------------------------------
// ~PlotWaterfall()
//----------------------------------------------------------------
PlotWaterfall::~PlotWaterfall()
{
    if(!m_bmp->IsNull())
    {
        delete m_bmp;
    }
}

/*
//----------------------------------------------------------------
// new_pixel_buf()
//----------------------------------------------------------------
void PlotWaterfall::new_pixel_buf(int w, int h)
{
//    int i;

    m_prev_w = w;
    m_prev_h = h;

//    wxMemoryDC m_memDC = new wxMemoryDC();

    if(m_bmp->IsOk())
    {
        delete m_bmp;
        //m_Bufsz = h * w;

        //m_img = new wxImage(sz, pdata, alpha,false);
        m_bmp = new wxBitmap(w, h, wxBITMAP_SCREEN_DEPTH);
        //m_pBmp = m_bmp->GetBitmapData();
    }
//    m_pixel_buf = new unsigned[buf_sz];
//    for(i = 0; i < buf_sz; i++)
//    {
//        m_pixel_buf[i] = 0;
//    }
}

//----------------------------------------------------------------
//  handle()
//----------------------------------------------------------------
int PlotWaterfall::handle(int event)
{
    //  detect a left mouse down if inside the window
    if ((event == FL_NO_EVENT) && (Fl::event_button() == 1))
    {
        if ((Fl::event_x() > x()) && (Fl::event_x() < (x() + w())) && (Fl::event_y() > y()) && (Fl::event_y() < (y() + h())))
        {
            // show zoomed spectrum window
            zoomWaterfallWindow->show();
        }
    }
    return 0;
}
*/

//----------------------------------------------------------------
// heatmap()
// map val to a rgb colour
// from http://eddiema.ca/2011/01/21/c-sharp-heatmaps/
//----------------------------------------------------------------
unsigned PlotWaterfall::heatmap(float val, float min, float max)
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

//-------------------------------------------------------------------------
// drawGraticule()
//-------------------------------------------------------------------------
void PlotWaterfall::drawGraticule(wxAutoBufferedPaintDC&  dc)
{
    int p;
    char buf[15];
    wxString s;

    // Vertical gridlines
    dc.SetPen(m_penShortDash);
    for(p = (PLOT_BORDER + XLEFT_OFFSET + GRID_INCREMENT); p < m_w; p += GRID_INCREMENT)
    {
        dc.DrawLine(p, (m_h + PLOT_BORDER), p, PLOT_BORDER);
    }
    // Horizontal gridlines
    dc.SetPen(m_penDotDash);
    for(p = (m_h - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (p + PLOT_BORDER), (m_w + PLOT_BORDER + XLEFT_OFFSET), (p + PLOT_BORDER));
    }
    // Label the X-Axis
    dc.SetPen(wxPen(GREY_COLOR, 1));
    for(p = GRID_INCREMENT; p < (m_w - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    {
        sprintf(buf, "%1.1f Hz",(double)(p / 10));
        dc.DrawText(buf, p - PLOT_BORDER + XLEFT_OFFSET, m_h + YBOTTOM_OFFSET/2);
    }
    // Label the Y-Axis
    for(p = (m_h - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        sprintf(buf, "%1.0f", (double)((m_h - p) * -10));
        dc.DrawText(buf, XLEFT_TEXT_OFFSET, p);
    }
}

//----------------------------------------------------------------
// draw()
//----------------------------------------------------------------
void PlotWaterfall::draw(wxAutoBufferedPaintDC&  dc)
{
    float       spec_index_per_px;
    float       intensity_per_dB;
    int         px_per_sec;
    int         index;
    int         dy;
    int         dy_blocks;
    int         bytes_in_row_of_blocks;
    int         b;
    int         px;
    int         py;
    int         intensity;
    unsigned    *last_row;
    unsigned    *pdest;
    unsigned    *psrc;
    //int p;
    //char buf[15];
    //wxString s;

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
    wxBrush ltBlueBrush = wxBrush(LIGHT_RED_COLOR);
    dc.SetBrush(ltBlueBrush);
    dc.SetPen(wxPen(BLACK_COLOR, 1));
    dc.DrawRectangle(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_w, m_h);

    drawGraticule(dc);

    /* detect resizing of window */
/*
    if ((m_h != m_prev_h) || (m_w != m_prev_w))
    {
        //delete m_pBmp;
        new_pixel_buf(m_w, m_h);
    }
*/

    // determine dy, the height of one "block"
    px_per_sec = (float)m_h / WATERFALL_SECS_Y;
    dy = DT * px_per_sec;
    // number of dy high blocks in spectrogram
    dy_blocks = m_h / dy;
    // shift previous bit map
    bytes_in_row_of_blocks = dy * m_w * sizeof(unsigned);
    for(b = 0; b < dy_blocks - 1; b++)
    {
        pdest = m_pixel_buf + b * m_w * dy;
        psrc  = m_pixel_buf + (b + 1) * m_w * dy;
        memcpy(pdest, psrc, bytes_in_row_of_blocks);
    }
    // create a new row of blocks at bottom
    spec_index_per_px = (float)FDMDV_NSPEC / (float) m_w;
    intensity_per_dB = (float)256 /(MAX_DB - MIN_DB);
    last_row = m_pixel_buf + dy *(dy_blocks - 1)* m_w;
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
        if (m_greyscale)
        {
            for(py = 0; py < dy; py++)
            {
                last_row[px + py * m_w] = intensity << 8;
            }
        }
        else
        {
            for(py = 0; py < dy; py++)
            {
                last_row[px + py * m_w] = m_heatmap_lut[intensity];
            }
        }
    }

    // update bit map
    //fl_draw_image((unsigned char*)m_pixel_buf, m_x, m_y, m_w, m_h, 4, 0);
    //dc.DrawLines(4, m_pBmp, 0, 0 );
}

//----------------------------------------------------------------
// paintEvent()
//
// Called by the system of by wxWidgets when the panel needs
// to be redrawn. You can also trigger this call by calling
// Refresh()/Update().
//----------------------------------------------------------------
void PlotWaterfall::OnPaint(wxPaintEvent & evt)
{
    wxAutoBufferedPaintDC dc(this);
    draw(dc);
}

//----------------------------------------------------------------
// OnSize()
//----------------------------------------------------------------
void PlotWaterfall::OnSize(wxSizeEvent& event)
{
    if(m_use_bitmap)
    {
        this->Refresh();
    }
}

//----------------------------------------------------------------
// OnShow()
//----------------------------------------------------------------
void PlotWaterfall::OnShow(wxShowEvent& event)
{
//    wxAutoBufferedPaintDC dc(this);
//    draw();
}
