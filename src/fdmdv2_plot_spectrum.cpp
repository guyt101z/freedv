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
}

//----------------------------------------------------------------
// draw()
//----------------------------------------------------------------
void PlotSpectrum::draw(wxAutoBufferedPaintDC& dc)
{
    m_rCtrl  = GetClientRect();

    // m_rGrid is coords of inner window we actually plot to.  We deflate it a bit
    // to leave room for axis labels.  We need to work this out every time we draw
    // as OnSize() may not be called before OnPaint(), for example when a new tab
    // is selected

    m_rGrid  = m_rCtrl;
    m_rGrid = m_rGrid.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));

    dc.Clear();

    // black background

    m_rPlot = wxRect(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_rGrid.GetWidth(), m_rGrid.GetHeight());
    wxBrush ltGraphBkgBrush = wxBrush(BLACK_COLOR);
    dc.SetBrush(ltGraphBkgBrush);
    dc.SetPen(wxPen(BLACK_COLOR, 0));
    dc.DrawRectangle(m_rPlot);

    // draw spectrum

    if(m_newdata)
    {
	int   x, y, prev_x, prev_y, index;
	float index_to_px, mag_dB_to_py, mag;

        m_newdata = false;

        wxPen pen;
        pen.SetColour(DARK_GREEN_COLOR);
        pen.SetWidth(1);
        dc.SetPen(pen);

	index_to_px = (float)m_rGrid.GetWidth()/FDMDV_NSPEC;
	mag_dB_to_py = (float)m_rGrid.GetHeight()/(MAX_MAG_DB-MIN_MAG_DB);

	prev_x = PLOT_BORDER + XLEFT_OFFSET;
	prev_y = PLOT_BORDER;
        for(index = 0; index < FDMDV_NSPEC; index++)
        {
            x = index*index_to_px;
	    mag = g_avmag[index];
	    if (mag > MAX_MAG_DB) mag = MAX_MAG_DB;
	    if (mag < MIN_MAG_DB) mag = MIN_MAG_DB;
	    y = -mag * mag_dB_to_py;

	    x += PLOT_BORDER + XLEFT_OFFSET;
	    y += PLOT_BORDER;

	    dc.DrawLine(x, y, prev_x, prev_y);
	    prev_x = x; prev_y = y;
        }
    }

    // and finally draw Graticule

    drawGraticule(dc);

}

//-------------------------------------------------------------------------
// drawGraticule()
//-------------------------------------------------------------------------
void PlotSpectrum::drawGraticule(wxAutoBufferedPaintDC&  dc)
{
    int      x, y, text_w, text_h;
    char     buf[15];
    wxString s;
    float    f, mag, freq_hz_to_px, mag_dB_to_py;

    wxBrush ltGraphBkgBrush;
    ltGraphBkgBrush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    ltGraphBkgBrush.SetColour(*wxBLACK);
    dc.SetBrush(ltGraphBkgBrush);
    dc.SetPen(wxPen(BLACK_COLOR, 1));

    freq_hz_to_px = (float)m_rGrid.GetWidth()/(MAX_F_HZ-MIN_F_HZ);
    mag_dB_to_py = (float)m_rGrid.GetHeight()/(MAX_MAG_DB-MIN_MAG_DB);

    // upper LH coords of plot area are (PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER)
    // lower RH coords of plot area are (PLOT_BORDER + XLEFT_OFFSET + m_rGrid.GetWidth(), 
    //                                   PLOT_BORDER + m_rGrid.GetHeight())

    // Vertical gridlines

    dc.SetPen(m_penShortDash);
    for(f=STEP_F_HZ; f<MAX_F_HZ; f+=STEP_F_HZ) {
	x = f*freq_hz_to_px;
	x += PLOT_BORDER + XLEFT_OFFSET;
        dc.DrawLine(x, m_rGrid.GetHeight() + PLOT_BORDER, x, PLOT_BORDER);
        sprintf(buf, "%4.0fHz", f);
	GetTextExtent(buf, &text_w, &text_h);
        dc.DrawText(buf, x - text_w/2, m_rGrid.GetHeight() + PLOT_BORDER + YBOTTOM_TEXT_OFFSET);
    }

    // Horizontal gridlines

    dc.SetPen(m_penDotDash);
    for(mag=MIN_MAG_DB; mag<=MAX_MAG_DB; mag+=STEP_MAG_DB) {
	y = -mag*mag_dB_to_py;
	y += PLOT_BORDER;
	dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, y, 
		    (m_rGrid.GetWidth() + PLOT_BORDER + XLEFT_OFFSET), y);
        sprintf(buf, "%3.0fdB", mag);
	GetTextExtent(buf, &text_w, &text_h);
        dc.DrawText(buf, PLOT_BORDER + XLEFT_OFFSET - text_w - XLEFT_TEXT_OFFSET, y-text_h/2);
   }


}

