//==========================================================================
// Name:            fdmdv2_plot_scatter.h
// Purpose:         Defines a waterfall plot derivative of fdmdv2_plot.
// Created:         June 24, 2012
// Authors:         David Rowe, David Witten
// 
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#ifndef __FDMDV2_PLOT_SCATTER__
#define __FDMDV2_PLOT_SCATTER__

#include "comp.h"
#include "fdmdv2_plot.h"
#include "fdmdv2_defines.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotScatter
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class PlotScatter : public PlotPanel
{
    public:
        PlotScatter(wxFrame* parent);
        ~PlotScatter(){};
	void add_new_samples(COMP samples[]);

    protected:
        COMP m_mem[SCATTER_MEM_SYMS];
        COMP m_new_samples[FDMDV_NSYM];

        void draw(wxAutoBufferedPaintDC&  dc);
        void OnPaint(wxPaintEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnShow(wxShowEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif //__FDMDV2_PLOT_SCATTER__
