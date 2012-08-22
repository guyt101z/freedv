//==========================================================================
// Name:            fdmdv2_plot_waterfall.h
// Purpose:         Defines a waterfall plot derivative of fdmdv2_plot.
// Created:         June 22, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __FDMDV2_PLOT_WATERFALL__
#define __FDMDV2_PLOT_WATERFALL__

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class Waterfall
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class Waterfall : public DrawPanel
{
public:
    //Waterfall(int x, int y, int w, int h);
    Waterfall(wxFrame* parent, int x, int y, int w, int h, const char name[]);
    ~Waterfall();

protected:
    int       prev_w, prev_h;
    unsigned *pixel_buf;
    unsigned  heatmap_lut[256];
    int       greyscale;

    void new_pixel_buf(int w, int h);
    int handle(int event);
    unsigned heatmap(float val, float min, float max);
    void draw();

};

#endif //__FDMDV2_PLOT_WATERFALL__
