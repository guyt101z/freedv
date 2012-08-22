//==========================================================================
// Name:            fdmdv2_plot_scatter.h
// Purpose:         Defines a waterfall plot derivative of fdmdv2_plot.
// Created:         June 24, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __FDMDV2_PLOT_SCATTER__
#define __FDMDV2_PLOT_SCATTER__


#define SCATTER_MEM       (FDMDV_NSYM)*50
#define SCATTER_X_MAX        3.0
#define SCATTER_Y_MAX        3.0

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class ScattterPlot
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class ScatterPlot : public DrawPanel
{
public:
   ScatterPlot(wxFrame* parent);
    ~ScatterPlot(){};

protected:
    COMP m_mem[SCATTER_MEM];
    COMP m_new_samples[FDMDV_NSYM];

    void draw();
    void add_new_samples(COMP samples[]);
    DECLARE_EVENT_TABLE()
};

#endif //__FDMDV2_PLOT_SCATTER__
