//==========================================================================
// Name:            fdmdv2_plot_scatter.cpp
// Purpose:         Implements a scatter plot derivative of fdmdv2_plot.
// Created:         June 24, 2012
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
#include "fdmdv2_plot_scatter.h"

BEGIN_EVENT_TABLE(PlotScatter, PlotPanel)
    EVT_PAINT           (PlotScatter::OnPaint)
    EVT_MOTION          (PlotScatter::OnMouseMove)
    EVT_LEFT_DOWN       (PlotScatter::OnMouseDown)
    EVT_LEFT_UP         (PlotScatter::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotScatter::OnMouseWheelMoved)
    EVT_SIZE            (PlotScatter::OnSize)
    EVT_SHOW            (PlotScatter::OnShow)
//    EVT_ERASE_BACKGROUND(PlotScatter::OnErase)
END_EVENT_TABLE()

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// PlotScatter
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
PlotScatter::PlotScatter(wxFrame* parent) : PlotPanel(parent)
{
    int i;
    printf("PlotScatter\n");
    for(i=0; i < SCATTER_MEM_SYMS; i++)
    {
        m_mem[i].real = 0.0;
        m_mem[i].imag = 0.0;
    }
}

//----------------------------------------------------------------
// draw()
//----------------------------------------------------------------
void PlotScatter::draw(wxAutoBufferedPaintDC& dc)
{
    float x_scale;
    float y_scale;
    int   i,j;
    int   x;
    int   y;

    m_rCtrl = GetClientRect();
    m_rGrid = m_rCtrl;
    m_rGrid = m_rGrid.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));

    // black background

    dc.Clear();
    m_rPlot = wxRect(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_rGrid.GetWidth(), m_rGrid.GetHeight());
    wxBrush ltGraphBkgBrush = wxBrush(BLACK_COLOR);
    dc.SetBrush(ltGraphBkgBrush);
    dc.SetPen(wxPen(BLACK_COLOR, 0));
    dc.DrawRectangle(m_rPlot);
    x_scale = (float)m_rGrid.GetWidth()/SCATTER_X_MAX;
    y_scale = (float)m_rGrid.GetHeight()/SCATTER_Y_MAX;

    wxPen pen;
    pen.SetColour(LIGHT_GREEN_COLOR);
    pen.SetWidth(1);
    dc.SetPen(pen);

    // shift memory

    for(i = 0; i < SCATTER_MEM_SYMS-FDMDV_NSYM; i++)
    {
        m_mem[i] = m_mem[i+FDMDV_NSYM];
    }

    // new samples

    for(j=0; i < SCATTER_MEM_SYMS; i++,j++)
    {
        m_mem[i] = m_new_samples[j];
    }

    // draw all samples

    for(i = 0; i < SCATTER_MEM_SYMS; i++)
    {
        x = x_scale * m_mem[i].real + m_rGrid.GetWidth()/2;
        y = y_scale * m_mem[i].imag + m_rGrid.GetHeight()/2;
	x += PLOT_BORDER + XLEFT_OFFSET;
	y += PLOT_BORDER;
	dc.DrawPoint(x, y);
    }
}

//----------------------------------------------------------------
// add_new_samples()
//----------------------------------------------------------------
void PlotScatter::add_new_samples(COMP samples[])
{
    int i;

    for(i = 0; i < FDMDV_NSYM; i++)
    {
        m_new_samples[i] = samples[i];
    }
}

//----------------------------------------------------------------
// OnPaint()
//----------------------------------------------------------------
void PlotScatter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    draw(dc);
}

//----------------------------------------------------------------
// OnSize()
//----------------------------------------------------------------
void PlotScatter::OnSize(wxSizeEvent& event)
{
    // todo: clear screen
}

//----------------------------------------------------------------
// OnShow()
//----------------------------------------------------------------
void PlotScatter::OnShow(wxShowEvent& event)
{
}

