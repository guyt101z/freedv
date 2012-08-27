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

//----------------------------------------------------------------
//
//----------------------------------------------------------------
PlotScatter::PlotScatter(wxFrame* parent) : PlotPanel(parent)
{
    int i;

    //align(FL_ALIGN_TOP);
    //labelsize(10);

    for(i=0; i<SCATTER_MEM; i++)
    {
        m_mem[i].real = 0.0;
        m_mem[i].imag = 0.0;
    }
    m_prev_w = 0;
    m_prev_h = 0;
    m_prev_x = 0;
    m_prev_y = 0;
}

//----------------------------------------------------------------
//
//----------------------------------------------------------------
void PlotScatter::draw()
{
    float x_scale;
    float y_scale;
    int   i;
    int   j;
    int   x1;
    int   y1;

    //Fl_Box::draw();

    /* detect resizing of window */
    if((m_h != m_prev_h) || (m_w != m_prev_w) || (m_x != m_prev_x) || (m_y != m_prev_y))
    {
        //fl_color(FL_BLACK);
        //fl_rectf(x(),y(),w(),h());
        m_prev_h = m_h;
        m_prev_w = m_w;
        m_prev_x = m_x;
        m_prev_y = m_y;
    }

    //fl_push_clip(x(),y(),w(),h());

    x_scale = m_w/SCATTER_X_MAX;
    y_scale = m_h/SCATTER_Y_MAX;

    // erase last samples
    //fl_color(FL_BLACK);

    for(i=0; i<FDMDV_NSYM; i++)
    {
        x1 = x_scale * m_mem[i].real + m_x + m_w/2;
        y1 = y_scale * m_mem[i].imag + m_y + m_h/2;
        //fl_point(x1, y1);
        m_mem[i] = m_mem[i+FDMDV_NSYM];
    }

    // shift memory
    for(i = FDMDV_NSYM; i < SCATTER_MEM-FDMDV_NSYM; i++)
    {
        m_mem[i] = m_mem[i+FDMDV_NSYM];
    }

    // draw new samples
    //fl_color(FL_GREEN);
    for(i = SCATTER_MEM-FDMDV_NSYM, j = 0; i < SCATTER_MEM; i++,j++)
    {
        x1 = x_scale * m_new_samples[j].real + m_x + m_w/2;
        y1 = y_scale * m_new_samples[j].imag + m_y + m_h/2;
        //fl_point(x1, y1);
        m_mem[i] = m_new_samples[j];
    }
   // fl_pop_clip();
}

//----------------------------------------------------------------
//
//----------------------------------------------------------------
void PlotScatter::add_new_samples(COMP samples[])
{
    int i;

    for(i=0; i<FDMDV_NSYM; i++)
    {
        m_new_samples[i] = samples[i];
    }
}
