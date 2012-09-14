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
#include "fdmdv2_plot_waterfall.h"

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
//
// @class $(Name)
// @author $(User)
// @date $(Date)
// @file $(CurrentFileName).$(CurrentFileExt)
// @brief
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
PlotWaterfall::PlotWaterfall(wxFrame* parent): PlotPanel(parent)
{
    for(int i = 0; i < 255; i++)
    {
        m_heatmap_lut[i] = heatmap((float)i, 0.0, 255.0);
    }
    m_greyscale     = 0;
    m_Bufsz         = GetMaxClientSize();
    m_newdata       = false;
    m_firstPass     = true;
    m_line_color    = 0;
    SetLabelSize(10.0);
}

//----------------------------------------------------------------
// ~PlotWaterfall()
//----------------------------------------------------------------
PlotWaterfall::~PlotWaterfall()
{
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

        //m_img = new wxImage(sz, pdata, alpha, false);
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

#define DATA_LINE_WIDTH  25

//----------------------------------------------------------------
// draw()
//----------------------------------------------------------------
void PlotWaterfall::draw(wxAutoBufferedPaintDC& pDC)
{
    bool rc;
    wxMemoryDC m_mDC;
    m_mDC.SelectObject(*m_bmp);
//   m_mDC.SetMapMode(pDC.GetMapMode());
    m_rCtrl  = GetClientRect();
    m_rGrid  = m_rCtrl;

    m_rGrid = m_rGrid.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));
    m_rGrid.Offset(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER);

    pDC.Clear();
    m_rPlot = wxRect(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_rGrid.GetWidth(), m_rGrid.GetHeight());
//    m_rPlot =  m_rPlot.Deflate(1, 1);
    if(m_firstPass)
    {
        m_firstPass = false;
        m_mDC.FloodFill(0, 0, VERY_LTGREY_COLOR);
//        m_rPlot.Offset(1, 1);

        // Draw a filled rectangle with aborder
        wxBrush ltGraphBkgBrush = wxBrush(LIGHT_RED_COLOR);
        m_mDC.SetBrush(ltGraphBkgBrush);
        m_mDC.SetPen(wxPen(BLACK_COLOR, 0));
        m_mDC.DrawRectangle(m_rPlot);
    }
    if(m_newdata)
    {
        m_newdata = false;
//        m_rPlot =  m_rPlot.Deflate(2, 2);
//        m_rPlot.Offset(1, 1);

//        plotPixelData(dc);
#ifdef USE_TIMER
        int t = m_rPlot.GetTop();
        int l = m_rPlot.GetLeft();
        int b = m_rPlot.GetBottom();
        int r = m_rPlot.GetRight();
        int h = m_rPlot.GetHeight();
        int w = m_rPlot.GetWidth();

        wxDateTime dt;
        char buf[15];
        sprintf(buf, "%9X", (unsigned int)dt.GetTimeNow());
        wxPen pen;
        pen.SetCap(wxCAP_BUTT);
        pen.SetStyle(wxPENSTYLE_SOLID);
        pen.SetWidth(1);
        pen.SetColour(BLACK_COLOR);
        m_mDC.SetPen(pen);
        m_mDC.DrawText(buf, l + 100, h - 38);
//        bool rc = pDC.StretchBlit(l, t + DATA_LINE_WIDTH, r, b , &m_mDC, l, t, r, b - DATA_LINE_WIDTH);
//        bool rc = pDC.StretchBlit(l, t, w, h - DATA_LINE_WIDTH, &m_mDC, l, t + DATA_LINE_WIDTH, w, h - DATA_LINE_WIDTH);
//        bool rc = pDC.StretchBlit(l, t, w, h, &m_mDC, l, t, w, h);
        int t2 = t + 1;
        int w2 = w - 1;
        rc = m_mDC.StretchBlit(l, t2, w2, h - (DATA_LINE_WIDTH), &m_mDC, l, t2 + DATA_LINE_WIDTH, w2, h - DATA_LINE_WIDTH);
        switch(m_line_color)
        {
            case 0:
                pen.SetColour(RED_COLOR);
                m_line_color = 1;
                break;

            case 1:
                pen.SetColour(YELLOW_COLOR);
                m_line_color = 2;
                break;

            case 2:
                pen.SetColour(BLUE_COLOR);
                m_line_color = 0;
                break;
        }
        pen.SetWidth(DATA_LINE_WIDTH);
        m_mDC.SetPen(pen);
        m_mDC.DrawLine(l + 1, h - (DATA_LINE_WIDTH/2) + 1, r, h - (DATA_LINE_WIDTH/2) + 1);
        rc = pDC.Blit(l, t, w, h, &m_mDC, l, t);
#endif
    }
    drawGraticule(pDC);
    m_mDC.SetBrush(wxNullBrush);
    m_mDC.SelectObject(wxNullBitmap);
}

//-------------------------------------------------------------------------
// drawGraticule()
//-------------------------------------------------------------------------
void PlotWaterfall::drawGraticule(wxAutoBufferedPaintDC&  pDC)
{
    int p;
    char buf[15];
    wxString s;

    //wxBrush ltGraphBkgBrush = wxBrush(LIGHT_RED_COLOR);
    wxBrush ltGraphBkgBrush;
    ltGraphBkgBrush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    pDC.SetBrush(ltGraphBkgBrush);
    pDC.SetPen(wxPen(BLACK_COLOR, 1));
//    pDC.DrawRectangle(m_rPlot);

    // Vertical gridlines
    pDC.SetPen(m_penShortDash);
    for(p = (PLOT_BORDER + XLEFT_OFFSET + GRID_INCREMENT); p < ((m_rGrid.GetWidth() - XLEFT_OFFSET) + GRID_INCREMENT); p += GRID_INCREMENT)
    {
        pDC.DrawLine(p, (m_rGrid.GetHeight() + PLOT_BORDER), p, PLOT_BORDER);
    }
    // Horizontal gridlines
    pDC.SetPen(m_penDotDash);
    for(p = (m_rGrid.GetHeight() - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        pDC.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (p + PLOT_BORDER), (m_rGrid.GetWidth() + PLOT_BORDER + XLEFT_OFFSET), (p + PLOT_BORDER));
    }
    // Label the X-Axis
    pDC.SetPen(wxPen(GREY_COLOR, 1));
    for(p = GRID_INCREMENT; p < (m_rGrid.GetWidth() - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    {
        sprintf(buf, "%1.1f Hz",(double)(p / 10));
        pDC.DrawText(buf, p - PLOT_BORDER + XLEFT_OFFSET, m_rGrid.GetHeight() + YBOTTOM_OFFSET/2);
    }
    // Label the Y-Axis
    for(p = (m_rGrid.GetHeight() - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        sprintf(buf, "%1.0f", (double)((m_rGrid.GetHeight() - p) * -10));
        pDC.DrawText(buf, XLEFT_TEXT_OFFSET, p);
    }
}

//-------------------------------------------------------------------------
// plotPixelData()
//-------------------------------------------------------------------------
void PlotWaterfall::plotPixelData(wxAutoBufferedPaintDC&  dc)
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

    // determine dy, the height of one "block"
    px_per_sec = (float)m_rCtrl.GetHeight() / WATERFALL_SECS_Y;
    dy = DT * px_per_sec;
    // number of dy high blocks in spectrogram
    dy_blocks = m_rCtrl.GetHeight()/ dy;
    // shift previous bit map
    bytes_in_row_of_blocks = dy * m_rCtrl.GetWidth() * sizeof(unsigned);
    for(b = 0; b < dy_blocks - 1; b++)
    {
        pdest = (unsigned int *)m_pBmp + b * m_rCtrl.GetWidth() * dy;
        psrc  = (unsigned int *)m_pBmp + (b + 1) * m_rCtrl.GetWidth() * dy;
        memcpy(pdest, psrc, bytes_in_row_of_blocks);
    }
    // create a new row of blocks at bottom
    spec_index_per_px = (float)FDMDV_NSPEC / (float) m_rCtrl.GetWidth();
    intensity_per_dB  = (float)256 /(MAX_DB - MIN_DB);
    last_row = (unsigned int *)m_pBmp + dy *(dy_blocks - 1)* m_rCtrl.GetWidth();

    wxNativePixelData data(*m_bmp);
    if(!data)
    {
        // ... raw access to bitmap data unavailable, do something else ...
        return;
    }
    if(data.GetWidth() < 20 || data.GetHeight() < 20)
    {
        // ... complain: the bitmap it too small ...
        return;
    }
    wxNativePixelData::Iterator p(data);
    // we draw a (10, 10)-(20, 20) rect manually using the given r, g, b
    p.Offset(data, 10, 10);
    for(px = 0; px < m_rCtrl.GetWidth(); px++)
    {
        index = px * spec_index_per_px;
        // intensity = intensity_per_dB * (m_av_mag[index] - MIN_DB);
        intensity = intensity_per_dB * (((MainFrame *)GetParent())->m_rxPa->m_av_mag[index] - MIN_DB);
//        intensity = intensity_per_dB * (((MainFrame *)GetParent())->m_av_mag[index] - MIN_DB);
        if(intensity > 255)
        {
            intensity = 255;
        }
        if(intensity > 255)
        {
            intensity = 255;
        }
        if (intensity < 0)
        {
            intensity = 0;
        }
        if(m_greyscale)
        {
            for(py = 0; py < dy; py++)
            {
                last_row[px + py * m_rCtrl.GetWidth()] = intensity << 8;
            }
        }
        else
        {
            for(py = 0; py < dy; py++)
            {
                last_row[px + py * m_rCtrl.GetWidth()] = m_heatmap_lut[intensity];
            }
        }
    }
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
    wxAutoBufferedPaintDC pdc(this);
    draw(pdc);
}

//----------------------------------------------------------------
// OnSize()
//----------------------------------------------------------------
//void PlotWaterfall::OnSize(wxSizeEvent& event)
//{
//    m_rCtrlPrev = m_rCtrl;
//    m_rCtrl     = GetClientRect();
//    if(m_use_bitmap)
//    {
//        m_firstPass = true;
//        m_bmp = new wxBitmap(m_rCtrl.GetWidth(), m_rCtrl.GetHeight(), wxBITMAP_SCREEN_DEPTH);
//        this->Refresh();
//    }
//}

//----------------------------------------------------------------
// OnShow()
//----------------------------------------------------------------
void PlotWaterfall::OnShow(wxShowEvent& event)
{
//    wxAutoBufferedPaintDC dc(this);
//    draw();
}
