//==========================================================================
// Name:            fdmdv2_scalar.h
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

