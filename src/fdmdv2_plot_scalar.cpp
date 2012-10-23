//==========================================================================
// Name:            fdmdv2_plot_scalar.cpp
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
#include "fdmdv2_plot_scalar.h"

BEGIN_EVENT_TABLE(PlotScalar, PlotPanel)
    EVT_PAINT           (PlotScalar::OnPaint)
    EVT_MOTION          (PlotScalar::OnMouseMove)
    EVT_LEFT_DOWN       (PlotScalar::OnMouseDown)
    EVT_LEFT_UP         (PlotScalar::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotScalar::OnMouseWheelMoved)
    EVT_SIZE            (PlotScalar::OnSize)
    EVT_SHOW            (PlotScalar::OnShow)
//    EVT_ERASE_BACKGROUND(PlotScalar::OnErase)
END_EVENT_TABLE()

//----------------------------------------------------------------
// PlotScalar()
//----------------------------------------------------------------
//PlotScalar::PlotScalar(wxFrame* parent, int x_max_, int y_max_): PlotPanel(parent)
PlotScalar::PlotScalar(wxFrame* parent): PlotPanel(parent)
{
    int i;

    //align(FL_ALIGN_TOP);
    //labelsize(10);

//    m_x_max = x_max_;
//    m_y_max = y_max_;
    m_mem = new float[m_x_max];

    for(i = 0; i < m_x_max; i++)
    {
        m_mem[i] = 0.0;
    }
    m_prev_w = 0;
    m_prev_h = 0;
    m_prev_x = 0;
    m_prev_y = 0;
    m_index = 0;
}

//----------------------------------------------------------------
// ~PlotScalar()
//----------------------------------------------------------------
PlotScalar::~PlotScalar()
{
    delete m_mem;
}

//----------------------------------------------------------------
// add_new_sample()
//----------------------------------------------------------------
void PlotScalar::add_new_sample(float sample)
{
    m_new_sample = sample;
}

//----------------------------------------------------------------
// clip()
//----------------------------------------------------------------
int PlotScalar::clip(int y1)
{
    if(y1 > (m_rCtrl.GetHeight()/2 - 10))
    {
        y1 = m_rCtrl.GetHeight()/2 - 10;
    }
    if(y1 < -(m_rCtrl.GetHeight()/2 - 10))
    {
        y1 = -(m_rCtrl.GetHeight()/2 - 10);
    }
    return y1;
}

//----------------------------------------------------------------
// draw()
//----------------------------------------------------------------
void PlotScalar::draw(wxAutoBufferedPaintDC&  dc)
{
    float x_scale;
    float y_scale;
    int   i;
    int   x1;
    int   y1;
    int   x2;
    int   y2;
    char  label[100];
    wxPen pen;

    /* detect resizing of window */
    if((m_rCtrl.GetHeight() != m_prev_h) || (m_rCtrl.GetWidth() != m_prev_w) || (m_x != m_prev_x) || (m_y != m_prev_y))
    {
        dc.SetPen(BLACK_COLOR);
        dc.DrawRectangle(m_x, m_y, m_rCtrl.GetWidth(), m_rCtrl.GetHeight());
        m_prev_h = m_rCtrl.GetHeight();
        m_prev_w = m_rCtrl.GetWidth();
        m_prev_x = m_x;
        m_prev_y = m_y;
    }

    //fl_push_clip(m_x, m_y, m_w, m_h);
    x_scale = (float)m_x_max;
    y_scale = (float)m_rCtrl.GetHeight() /(2.0 * m_y_max);

    // erase last sample
    dc.SetPen(BLACK_COLOR);
    x1 = x_scale * m_index + m_x;
    y1 = y_scale * m_mem[m_index];
    y1 = clip(y1);
    y1 = m_y + m_rCtrl.GetHeight()/2 - y1;
    dc.DrawPoint(x1, y1);

    // draw new sample
    dc.SetPen(GREEN_COLOR);
    x1 = x_scale * m_index + m_x;
    y1 = y_scale * m_new_sample;
    y1 = clip(y1);
    y1 = m_y + m_rCtrl.GetHeight()/2 - y1;
    dc.DrawPoint(x1, y1);
    m_mem[m_index] = m_new_sample;
    m_index++;
    if(m_index >=  m_x_max)
    {
        m_index = 0;
    }

    // y axis graticule
    m_step = 10;
    while((2.0 * m_y_max/m_step) > 10)
    {
        m_step *= 2.0;
    }
    while((2.0 * m_y_max/m_step) < 4)
    {
        m_step /= 2.0;
    }
    pen = dc.GetPen();
    pen.SetColour(DARK_GREEN_COLOR);
    pen.SetStyle(wxPENSTYLE_DOT);
    dc.SetPen(pen);
    for(i =- m_y_max; i < m_y_max; i += m_step)
    {
        x1 = m_x;
        y1 = m_y + m_rCtrl.GetHeight()/2 - i * y_scale;
        x2 = m_x + m_rCtrl.GetWidth();
        y2 = y1;
        dc.DrawLine(x1, y1, x2, y2);
    }

    // y axis graticule labels
    pen = dc.GetPen();
    pen.SetColour(GREEN_COLOR);
    pen.SetStyle(wxPENSTYLE_DOT);
    dc.SetPen(pen);
    for(i =- m_y_max; i < m_y_max; i += m_step)
    {
        x1 = m_x;
        y1 = m_y + m_rCtrl.GetHeight()/2 - i * y_scale;
        sprintf(label, "%d", i);
        wxSize sz = dc.GetTextExtent(label);
        dc.DrawLabel(label,  wxRect(x1, y1, sz.GetWidth(), sz.GetHeight()), wxALIGN_LEFT);
    }
    //fl_pop_clip();
}

//----------------------------------------------------------------
// OnPaint()
//----------------------------------------------------------------
void PlotScalar::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    draw(dc);
}

//----------------------------------------------------------------
// OnSize()
//----------------------------------------------------------------
void PlotScalar::OnSize(wxSizeEvent& event)
{
//    wxAutoBufferedPaintDC dc(this);
//    draw(dc);
}

//----------------------------------------------------------------
// OnShow()
//----------------------------------------------------------------
void PlotScalar::OnShow(wxShowEvent& event)
{
//    wxAutoBufferedPaintDC dc(this);
 //   draw(dc);
}
