//==========================================================================
// Name:            fdmdv2_plot_scalar.h
// Purpose:         Defines a scalar plot derivative of fdmdv2_plot.
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
#ifndef __FDMDV2_PLOT_SCALAR__
#define __FDMDV2_PLOT_SCALAR__

#include "fdmdv2_plot.h"
#include "fdmdv2_defines.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotScalar
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class PlotScalar: public PlotPanel
{
    public:

        //PlotScalar(wxFrame* parent, int x, int y, int w, int h, int x_max_, int y_max_, const char name[]);
        PlotScalar(wxFrame* parent, int x_max_, int y_max_);
        ~PlotScalar();
         void add_new_sample(float sample);

    protected:

        int    m_x_max;
        int    m_y_max;
        float  *m_mem;              /* array of x_max samples */
        float  m_new_sample;
        int    m_index;
        int    m_step;

        int clip(int y1);
        void draw(wxAutoBufferedPaintDC&  dc);
        void OnPaint(wxPaintEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnShow(wxShowEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif // __FDMDV2_PLOT_SCALAR__

