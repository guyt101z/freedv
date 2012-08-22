//==========================================================================
// Name:            fdmdv2_scalar.h
// Purpose:         Defines a scalar plot derivative of fdmdv2_plot.
// Created:         June 22, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __FDMDV2_PLOT_SCALAR__
#define __FDMDV2_PLOT_SCALAR__


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class Scalar
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class Scalar: public DrawPanel
{
public:

    Scalar(wxFrame* parent, int x, int y, int w, int h, int x_max_, int y_max_, const char name[]);
    ~Scalar();
    void add_new_sample(float sample);
    
protected:

    int    m_x_max;
    int    m_y_max;
    float  *m_mem;              /* array of x_max samples */
    float  m_new_sample;
    int    m_index;
    int    m_step;

    int clip(int y1);
    void draw();
};

//// update average of each spectrum point
//void new_data(float mag_dB[])
//{
//    int i;
//
//    for(i=0; i<FDMDV_NSPEC; i++)
//    {
//        av_mag[i] = (1.0 - BETA)*av_mag[i] + BETA*mag_dB[i];
//    }
//}

#endif // __FDMDV2_PLOT_SCALAR__

