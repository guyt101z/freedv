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

    PlotScalar(wxFrame* parent,
	       float t_secs, 
	       float sample_period_secs,
	       float a_min,
	       float a_max,
	       float graticule_t_step,   
	       float graticule_a_step,
	       const char  a_fmt[],
               int   mini
	       );
        ~PlotScalar();
         void add_new_sample(float sample);
         void add_new_short_samples(short samples[], int length, float scale_factor);

    protected:

	 float    m_t_secs;
	 float    m_sample_period_secs;
	 float    m_a_min;
	 float    m_a_max;
	 float    m_graticule_t_step;   
	 float    m_graticule_a_step;
	 char     m_a_fmt[15];
         int      m_mini;
	 int      m_samples;
	 float   *m_mem;              

        void draw(wxAutoBufferedPaintDC&  dc);
	void drawGraticule(wxAutoBufferedPaintDC&  dc);
        void OnPaint(wxPaintEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnShow(wxShowEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif // __FDMDV2_PLOT_SCALAR__

