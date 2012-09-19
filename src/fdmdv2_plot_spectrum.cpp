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
// draw()
//----------------------------------------------------------------
void PlotSpectrum::draw(wxAutoBufferedPaintDC& pDC)
{
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
#ifdef USE_TIMER
        int t = m_rPlot.GetTop();
        int l = m_rPlot.GetLeft();
        int h = m_rPlot.GetHeight();
        int w = m_rPlot.GetWidth();
        //double stride = w / FDMDV_NSPEC;

        wxPen pen;
        pen.SetColour(DARK_GREEN_COLOR);
        pen.SetWidth(1);
        m_mDC.SetPen(pen);

        float *pData = m_pTopFrame->m_rxPa->m_av_mag;
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
}

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
    for(p = (PLOT_BORDER + XLEFT_OFFSET + GRID_INCREMENT); p < ((m_rGrid.GetWidth() - XLEFT_OFFSET) + GRID_INCREMENT); p += GRID_INCREMENT)
    {
        dc.DrawLine(p, (m_rGrid.GetHeight() + PLOT_BORDER), p, PLOT_BORDER);
    }

    int y_zero = (m_rGrid.GetHeight() - m_top) / 2 ;
    dc.SetPen(m_penSolid);
    dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, y_zero, (m_rGrid.GetWidth() + PLOT_BORDER + XLEFT_OFFSET), y_zero);
    sprintf(buf, "%6.0f", 0.0);
    dc.DrawText(buf, XLEFT_TEXT_OFFSET, y_zero + TEXT_BASELINE_OFFSET_Y);

    // Horizontal gridlines
    dc.SetPen(m_penDotDash);
    for(p = 0; (y_zero + p) < m_rGrid.GetHeight() ; p += GRID_INCREMENT)
    {
        if(p > 0)
        {
            dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (y_zero + p), (m_rGrid.GetWidth() + PLOT_BORDER + XLEFT_OFFSET), (y_zero + p));
            sprintf(buf, "%6.0f", (double)(p) * -10);
            dc.DrawText(buf, XLEFT_TEXT_OFFSET, (y_zero + p + TEXT_BASELINE_OFFSET_Y));

            dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (y_zero - p), (m_rGrid.GetWidth() + PLOT_BORDER + XLEFT_OFFSET), (y_zero - p));
            sprintf(buf, "%6.0f", (double)(p) * 10);
            dc.DrawText(buf, XLEFT_TEXT_OFFSET, (y_zero - p + TEXT_BASELINE_OFFSET_Y));
        }
    }

    // Label the X-Axis
    dc.SetPen(wxPen(GREY_COLOR, 1));
    for(p = GRID_INCREMENT; p < (m_rCtrl.GetWidth() - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    {
        sprintf(buf, "%1.1f Hz",(double)(p / 10));
        dc.DrawText(buf, p - PLOT_BORDER + XLEFT_OFFSET, m_rCtrl.GetHeight() + YBOTTOM_OFFSET/2);
    }
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
// OnShow()
//----------------------------------------------------------------
void PlotSpectrum::OnShow(wxShowEvent& event)
{
//   wxAutoBufferedPaintDC dc(this);
//   draw(dc);
}
