//==========================================================================
// Name:            fdmdv2_plot_spectrum.h
// Purpose:         Defines a spectrum plot derived from fdmdv2_plot class.
// Created:         June 22, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __FDMDV2_PLOT_SPECTRUM__
#define __FDMDV2_PLOT_SPECTRUM__

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class Waterfall
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class SpectrumPlot : public DrawPanel
{
    public:
        SpectrumPlot(wxFrame* parent, int x, int y, int w, int h);
        ~SpectrumPlot() {};
    protected:
        int handle(int event);
        void draw();
};


#endif //__FDMDV2_PLOT_SPECTRUM__
