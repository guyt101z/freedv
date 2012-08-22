//==========================================================================
// Name:            fdmdv2_plot_waterfall.cpp
// Purpose:         Implements a waterfall plot derivative of fdmdv2_plot.
// Created:         June 22, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include <string.h>
#include "wx/wx.h"
#include "fdmdv2_main.h"
#include "fdmdv2_plot.h"
#include "fdmdv2_scalar.h"

Scalar::Scalar(wxFrame* parent, int x, int y, int w, int h, int x_max_, int y_max_, const char name[]): DrawPanel(parent)
{
    int i;

    //align(FL_ALIGN_TOP);
    //labelsize(10);

    m_x_max = x_max_;
    m_y_max = y_max_;
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

Scalar::~Scalar()
{
    delete m_mem;
}

void Scalar::add_new_sample(float sample)
{
    m_new_sample = sample;
}

int Scalar::clip(int y1)
{
    if(y1 > (m_h/2 - 10))
    {
        y1 = m_h/2 - 10;
    }
    if(y1 < -(m_h/2 - 10))
    {
        y1 = -(m_h/2 - 10);
    }
    return y1;
}

void Scalar::draw()
{
    float x_scale;
    float y_scale;
    int   i;
    int   x1;
    int   y1;
    int   x2;
    int   y2;
    char  label[100];

//    DrawPanel::draw();

    /* detect resizing of window */
    if((m_h != m_prev_h) || (m_w != m_prev_w) || (m_x != m_prev_x) || (m_y != m_prev_y))
    {
        //fl_color(FL_BLACK);
        //fl_rectf(m_x, m_y, m_w, m_h);
        m_prev_h = m_h;
        m_prev_w = m_w;
        m_prev_x = m_x;
        m_prev_y = m_y;
    }

    //fl_push_clip(m_x, m_y, m_w, m_h);
    x_scale = (float)m_x_max;
    y_scale = (float)m_h /(2.0 * m_y_max);

    // erase last sample
    //fl_color(FL_BLACK);
    x1 = x_scale * m_index + m_x;
    y1 = y_scale * m_mem[m_index];
    y1 = clip(y1);
    y1 = m_y + m_h/2 - y1;
    //fl_point(x1, y1);

    // draw new sample
    //fl_color(FL_GREEN);
    x1 = x_scale * m_index + m_x;
    y1 = y_scale * m_new_sample;
    y1 = clip(y1);
    y1 = m_y + m_h/2 - y1;
//    fl_point(x1, y1);
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
   // fl_color(FL_DARK_GREEN);
   // fl_line_style(FL_DOT);
    for(i =- m_y_max; i < m_y_max; i += m_step)
    {
        x1 = m_x;
        y1 = m_y + m_h/2 - i * y_scale;
        x2 = m_x + m_w;
        y2 = y1;
        //fl_line(x1, y1, x2, y2);
    }

    // y axis graticule labels
   // fl_color(FL_GREEN);
   // fl_line_style(FL_SOLID);
    for(i =- m_y_max; i < m_y_max; i += m_step)
    {
        x1 = m_x;
        y1 = m_y + m_h/2 - i * y_scale;
        sprintf(label, "%d", i);
        //fl_draw(label, x1, y1);
    }
    //fl_pop_clip();
}

// // update average of each spectrum point
// void new_data(float mag_dB[])
// {
//     int i;
//
//     for(i=0; i<FDMDV_NSPEC; i++)
//     {
//         av_mag[i] = (1.0 - BETA)*av_mag[i] + BETA*mag_dB[i];
//     }
// }
