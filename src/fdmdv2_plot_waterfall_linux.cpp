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

extern float g_avmag[]; // magnitude spectrum passed in to draw() 

BEGIN_EVENT_TABLE(PlotWaterfall, PlotPanel)
    EVT_PAINT           (PlotWaterfall::OnPaint)
    EVT_MOTION          (PlotWaterfall::OnMouseMove)
    EVT_LEFT_DOWN       (PlotWaterfall::OnMouseDown)
    EVT_LEFT_UP         (PlotWaterfall::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotWaterfall::OnMouseWheelMoved)
    EVT_SIZE            (PlotWaterfall::OnSize)
    EVT_SHOW            (PlotWaterfall::OnShow)
END_EVENT_TABLE()

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class WaterfallPlot
//
// @class   WaterfallPlot
// @author  David Witten
// @date    $(Date)
// @file    $(CurrentFileName).$(CurrentFileExt)
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

// When the window size gets set we can work outthe size of the window
// we plot in and allocate a bit map of the correct size

void PlotWaterfall::OnSize(wxSizeEvent& event) {

    m_rCtrl  = GetClientRect();

    // m_rGrid is coords of inner window we actually plot to.  We deflate it a bit
    // to leave room for axis labels.

    m_rGrid  = m_rCtrl;
    m_rGrid = m_rGrid.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));

    // we want a bit map the size of m_rGrid

    m_pBmp = new wxBitmap(m_rGrid.GetWidth(), m_rGrid.GetHeight(), 24);
}

//----------------------------------------------------------------
// paintEvent()
//
// @class $(Name)
// @author $(User)
// @date $(Date)
// @file $(CurrentFileName).$(CurrentFileExt)
// @brief
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
// OnShow()
//----------------------------------------------------------------
void PlotWaterfall::OnShow(wxShowEvent& event)
{
}

//----------------------------------------------------------------
// ~PlotWaterfall()
//----------------------------------------------------------------
PlotWaterfall::~PlotWaterfall()
{
}

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

//----------------------------------------------------------------
// draw()
//----------------------------------------------------------------
void PlotWaterfall::draw(wxAutoBufferedPaintDC& dc)
{

    if(m_newdata)
    {
        m_newdata = false;
        plotPixelData();
	dc.DrawBitmap(*m_pBmp, PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER);
    }
    else {
	
	// no data to plot so just erase to black.  Blue looks nicer
	// but is same colour as low amplitude signal

	// Bug: When Stop is pressed this code doesn't erase the lower 
	// 25% of the Waterfall Window

	m_rPlot = wxRect(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_rGrid.GetWidth(), m_rGrid.GetHeight());
	wxBrush ltGraphBkgBrush = wxBrush(BLACK_COLOR);
	dc.SetBrush(ltGraphBkgBrush);
	dc.SetPen(wxPen(BLACK_COLOR, 0));
	dc.DrawRectangle(m_rPlot);
    }
    
    drawGraticule(dc);

}


//-------------------------------------------------------------------------
// drawGraticule()
//-------------------------------------------------------------------------
void PlotWaterfall::drawGraticule(wxAutoBufferedPaintDC& dc)
{
    int p;
    char buf[15];

    wxString s;

    wxBrush ltGraphBkgBrush;
    ltGraphBkgBrush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    ltGraphBkgBrush.SetColour(*wxBLACK);
    dc.SetBrush(ltGraphBkgBrush);
    dc.SetPen(wxPen(BLACK_COLOR, 1));

    // Vertical gridlines
    dc.SetPen(m_penShortDash);
    for(p = (PLOT_BORDER + XLEFT_OFFSET + GRID_INCREMENT); p < ((m_rGrid.GetWidth() - XLEFT_OFFSET) + GRID_INCREMENT); p += GRID_INCREMENT)
    {
        dc.DrawLine(p, (m_rGrid.GetHeight() + PLOT_BORDER), p, PLOT_BORDER);
    }
    // Horizontal gridlines
    dc.SetPen(m_penDotDash);
    for(p = (m_rGrid.GetHeight() - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (p + PLOT_BORDER), (m_rGrid.GetWidth() + PLOT_BORDER + XLEFT_OFFSET), (p + PLOT_BORDER));
    }
    // Label the X-Axis
    dc.SetPen(wxPen(GREY_COLOR, 1));
    for(p = GRID_INCREMENT; p < (m_rGrid.GetWidth() - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    {
        sprintf(buf, "%1.1f Hz",(double)(p / 10));
        dc.DrawText(buf, p - PLOT_BORDER + XLEFT_OFFSET, m_rGrid.GetHeight() + YBOTTOM_OFFSET/3);
    }
    // Label the Y-Axis
    for(p = (m_rGrid.GetHeight() - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        sprintf(buf, "%1.0f", (double)((m_rGrid.GetHeight() - p) * 10));
        dc.DrawText(buf, XLEFT_TEXT_OFFSET, p);
    }
}

//-------------------------------------------------------------------------
// plotPixelData()
//-------------------------------------------------------------------------
void PlotWaterfall::plotPixelData()
{
    float       spec_index_per_px;
    float       intensity_per_dB;
    int         px_per_sec;
    int         index;
    int         dy;
    int         dy_blocks;
    int         b;
    int         px;
    int         py;
    int         intensity;

    /*
      Design Notes:

      The height in pixels represents WATERFALL_SECS_Y of data.  Every DT
      seconds we get a vector of FDMDV_NSPEC spectrum samples which we use
      to update the last row.  The height of each row is dy pixels, which
      maps to DT seconds.  We call each dy high rectangle of pixels a
      block.

    */

    // determine dy, the height of one "block"
    px_per_sec = (float)m_rGrid.GetHeight() / WATERFALL_SECS_Y;
    dy = DT * px_per_sec;

    // number of dy high blocks in spectrogram
    dy_blocks = m_rGrid.GetHeight()/ dy;

    intensity_per_dB  = (float)256 /(MAX_DB - MIN_DB);
    spec_index_per_px = (float)FDMDV_NSPEC / (float) m_rGrid.GetWidth();
    
    /*
    printf("h %d w %d px_per_sec %d dy %d dy_blocks %d spec_index_per_px: %f\n", 
	   m_rGrid.GetHeight(), m_rGrid.GetWidth(), px_per_sec, 
	   dy, dy_blocks, spec_index_per_px);
    */

    // Shift previous bit map up one row of blocks ----------------------------

    wxNativePixelData data(*m_pBmp);
    assert(data != NULL);
    wxNativePixelData::Iterator bitMapStart(data);
    wxNativePixelData::Iterator p = bitMapStart;

    for(b = 0; b < dy_blocks - 1; b++) {
	wxNativePixelData::Iterator psrc = bitMapStart;
	wxNativePixelData::Iterator pdest = bitMapStart;
	pdest.OffsetY(data, dy * b);
	psrc.OffsetY(data, dy * (b+1));

	// copy one line of blocks

	for(py = 0; py < dy; py++) {
	    wxNativePixelData::Iterator pdestRowStart = pdest;
	    wxNativePixelData::Iterator psrcRowStart = psrc;

	    for(px = 0; px < m_rGrid.GetWidth(); px++) {
		pdest.Red() = psrc.Red();
		pdest.Green() = psrc.Green();
		pdest.Blue() = psrc.Blue();
		pdest++;
		psrc++;
	    }

	    pdest = pdestRowStart;
	    pdest.OffsetY(data, 1);
	    psrc = psrcRowStart;
	    psrc.OffsetY(data, 1);	    
        }

    }

    // Draw last line of blocks using latest amplitude data ------------------

    p = bitMapStart;
    p.OffsetY(data, dy *(dy_blocks - 1));

    for(py = 0; py < dy; py++)
    {
	wxNativePixelData::Iterator rowStart = p;

	for(px = 0; px < m_rGrid.GetWidth(); px++)
        {
	    index = px * spec_index_per_px;
	    assert(index < FDMDV_NSPEC);

	    intensity = intensity_per_dB * (g_avmag[index] - MIN_DB);
	    if(intensity > 255) intensity = 255;
	    if (intensity < 0) intensity = 0;
	    //printf("%d %f %d \n", index, g_avmag[index], intensity);

	    p.Red() = m_heatmap_lut[intensity] & 0xff;
	    p.Green() = (m_heatmap_lut[intensity] >> 8) & 0xff;
	    p.Blue() = (m_heatmap_lut[intensity] >> 16) & 0xff;
	    ++p;
        }

	p = rowStart;
	p.OffsetY(data, 1);
    }
}
