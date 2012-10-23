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

extern float g_avmag[];

BEGIN_EVENT_TABLE(PlotSpectrum, PlotPanel)
    EVT_MOTION          (PlotSpectrum::OnMouseMove)
    EVT_LEFT_DOWN       (PlotSpectrum::OnMouseDown)
    EVT_LEFT_UP         (PlotSpectrum::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotSpectrum::OnMouseWheelMoved)
    EVT_PAINT           (PlotSpectrum::OnPaint)
    EVT_SHOW            (PlotSpectrum::OnShow)
END_EVENT_TABLE()

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotSpectrum
//
// @class $(Name)
// @author $(User)
// @date $(Date)
// @file $(CurrentFileName).$(CurrentFileExt)
// @brief
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
PlotSpectrum::PlotSpectrum(wxFrame* parent): PlotPanel(parent)
{
    m_greyscale     = 0;
    m_Bufsz         = GetMaxClientSize();
    m_newdata       = false;
    m_firstPass     = true;
    m_line_color    = 0;
    SetLabelSize(10.0);
}

//----------------------------------------------------------------
// ~PlotSpectrum()
//----------------------------------------------------------------
PlotSpectrum::~PlotSpectrum()
{
}

//----------------------------------------------------------------
// OnSize()
//----------------------------------------------------------------
void PlotSpectrum::OnSize(wxSizeEvent& event) {
    printf("PlotSpectrum::OnSize\n");
}

//----------------------------------------------------------------
// OnPaint()
//----------------------------------------------------------------
void PlotSpectrum::OnPaint(wxPaintEvent& event)
{
    printf("PlotSpectrum::OnPaint\n");
    wxAutoBufferedPaintDC dc(this);
    draw(dc);
}

//----------------------------------------------------------------
// OnShow()
//----------------------------------------------------------------
void PlotSpectrum::OnShow(wxShowEvent& event)
{
}

//----------------------------------------------------------------
// draw()
//----------------------------------------------------------------
void PlotSpectrum::draw(wxAutoBufferedPaintDC& dc)
{
    m_rCtrl  = GetClientRect();

    // m_rGrid is coords of inner window we actually plot to.  We deflate it a bit
    // to leave room for axis labels.

    m_rGrid  = m_rCtrl;
    m_rGrid = m_rGrid.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));

    // black background

    m_rPlot = wxRect(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_rGrid.GetWidth(), m_rGrid.GetHeight());
    wxBrush ltGraphBkgBrush = wxBrush(BLACK_COLOR);
    dc.SetBrush(ltGraphBkgBrush);
    dc.SetPen(wxPen(BLACK_COLOR, 0));
    dc.DrawRectangle(m_rPlot);

    // graticule

    drawGraticule(dc);

#ifdef OLD

    wxMemoryDC m_mDC;
    m_mDC.SelectObject(*m_pBmp);
    m_rCtrl  = GetClientRect();
    m_rGrid  = m_rCtrl;

    m_rGrid = m_rGrid.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));
    m_rGrid.Offset(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER);

    pDC.Clear();
    m_rPlot = wxRect(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_rGrid.GetWidth(), m_rGrid.GetHeight());
    if(m_firstPass)
    {
        m_firstPass = false;
        m_mDC.Clear();
        m_mDC.FloodFill(0, 0, VERY_LTGREY_COLOR);

        // Draw a filled rectangle with aborder
//        wxBrush ltGraphBkgBrush = wxBrush(LIGHT_YELLOW_COLOR);
        wxBrush ltGraphBkgBrush = wxBrush(DARK_GREY_COLOR);
        m_mDC.SetBrush(ltGraphBkgBrush);
        m_mDC.SetPen(wxPen(BLACK_COLOR, 0));
        m_mDC.DrawRectangle(m_rPlot);
    }
    if(m_newdata)
    {
        m_newdata = false;
//        plotPixelData(dc);
#ifdef _USE_TIMER
        int t = m_rPlot.GetTop();
        int l = m_rPlot.GetLeft();
        int h = m_rPlot.GetHeight();
        int w = m_rPlot.GetWidth();
        //double stride = w / FDMDV_NSPEC;

        wxPen pen;
        pen.SetColour(DARK_GREEN_COLOR);
        pen.SetWidth(1);
        m_mDC.SetPen(pen);

//        float *pData = m_pTopFrame->m_rxPa->m_av_mag;
        float *pData = g_avmag;
        for(int x = 1; x < w; x++)
        {
//            m_mDC.DrawPoint(x, (int)pData[x]);
            m_mDC.DrawLine((x - 1), (int)pData[(x - 1)] + (h / 2), x, (int)pData[x] + (h / 2));
        }
        pDC.Blit(l, t, w, h, &m_mDC, l, t);
#endif
        drawGraticule(pDC);
    }
    m_mDC.SetBrush(wxNullBrush);
    m_mDC.SelectObject(wxNullBitmap);
#endif
}

//-------------------------------------------------------------------------
// drawGraticule()
//-------------------------------------------------------------------------
void PlotSpectrum::drawGraticule(wxAutoBufferedPaintDC&  dc)
{
    int      x, y, text_w, text_h;
    char     buf[15];
    wxString s;
    float    f, dB, freq_hz_to_px, ampl_dB_to_px;

    wxBrush ltGraphBkgBrush;
    ltGraphBkgBrush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    ltGraphBkgBrush.SetColour(*wxBLACK);
    dc.SetBrush(ltGraphBkgBrush);
    dc.SetPen(wxPen(BLACK_COLOR, 1));

    freq_hz_to_px = (float)m_rGrid.GetWidth()/(MAX_HZ-MIN_HZ);
    ampl_dB_to_px = (float)m_rGrid.GetHeight()/(MAX_DB-MIN_DB);

    // upper LH coords of plot area are (PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER)
    // lower RH coords of plot area are (PLOT_BORDER + XLEFT_OFFSET + m_rGrid.GetWidth(), 
    //                                   PLOT_BORDER + m_rGrid.GetHeight())

    // Vertical gridlines

    dc.SetPen(m_penShortDash);

    for(f=STEP_HZ; f<MAX_HZ; f+=STEP_HZ) {
	x = f*freq_hz_to_px;
	x += PLOT_BORDER + XLEFT_OFFSET;
        dc.DrawLine(x, m_rGrid.GetHeight() + PLOT_BORDER, x, PLOT_BORDER);
        sprintf(buf, "%4.0fHz", f);
	GetTextExtent(buf, &text_w, &text_h);
        dc.DrawText(buf, x - text_w/2, m_rGrid.GetHeight() + PLOT_BORDER + YBOTTOM_TEXT_OFFSET);
    }

    // Horizontal gridlines

    for(dB=MIN_DB; dB<MAX_DB; dB+=STEP_DB) {
	y = m_rGrid.GetHeight() + dB*ampl_dB_to_px;
	y += PLOT_BORDER;
	dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, y, 
		    (m_rGrid.GetWidth() + PLOT_BORDER + XLEFT_OFFSET), y);
        sprintf(buf, "%3.0fdB", dB);
	GetTextExtent(buf, &text_w, &text_h);
        dc.DrawText(buf, PLOT_BORDER + XLEFT_OFFSET - text_w - XLEFT_TEXT_OFFSET, y-text_h/2);
   }


}

