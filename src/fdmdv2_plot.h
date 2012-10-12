//==========================================================================
// Name:            fdmdv2_plot.h
// Purpose:         Declares simple wxWidgets application with GUI
// Created:         Apr. 10, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
//#include "fdmdv2_main.h"
#ifndef __FDMDV2_PLOT__
#define __FDMDV2_PLOT__
#include <wx/wx.h>
#include <wx/aui/auibook.h>
#include <wx/rawbmp.h>
#include <wx/image.h>
#include <wx/dcbuffer.h>

#define MAX_ZOOM            7
#define MAX_BMP_X           (400 * MAX_ZOOM)
#define MAX_BMP_Y           (400 * MAX_ZOOM)
#define DATA_LINE_HEIGHT    10
#define TEXT_BASELINE_OFFSET_Y  -5


#define wxUSE_FILEDLG       1
#define wxUSE_LIBPNG        1
#define wxUSE_LIBJPEG       1
#define wxUSE_GIF           1
#define wxUSE_PCX           1
#define wxUSE_LIBTIFF       1

#define PLOT_BORDER         3
#define XLEFT_OFFSET        30
#define XLEFT_TEXT_OFFSET   8
#define YBOTTOM_OFFSET      25
#define GRID_INCREMENT      50

#define BLACK_COLOR         wxColor(0x00, 0x00, 0x00)
#define GREY_COLOR          wxColor(0x80, 0x80, 0x80)
#define DARK_GREY_COLOR     wxColor(0x40, 0x40, 0x40)
#define MEDIUM_GREY_COLOR   wxColor(0xC0, 0xC0, 0xC0)
#define LIGHT_GREY_COLOR    wxColor(0xE0, 0xE0, 0xE0)
#define VERY_LTGREY_COLOR   wxColor(0xF8, 0xF8, 0xF8)
#define WHITE_COLOR         wxColor(0xFF, 0xFF, 0xFF)

#define DARK_BLUE_COLOR     wxColor(0x00, 0x00, 0x60)
#define BLUE_COLOR          wxColor(0x00, 0x00, 0xFF)
#define LIGHT_BLUE_COLOR    wxColor(0x80, 0x80, 0xFF)

#define RED_COLOR           wxColor(0xFF, 0x5E, 0x5E)
#define LIGHT_RED_COLOR     wxColor(0xFF, 0xE0, 0xE0)
#define DARK_RED_COLOR      wxColor(0xFF, 0x00, 0x00)
#define PINK_COLOR          wxColor(0xFF, 0x80, 0xFF)

#define LIGHT_GREEN_COLOR   wxColor(0xE3, 0xFF, 0xE0)
#define GREEN_COLOR         wxColor(0x95, 0xFF, 0x8A)
#define DARK_GREEN_COLOR    wxColor(0x20, 0xFF, 0x08)
#define VERY_GREEN_COLOR    wxColor(0x00, 0xFF, 0x00)

#define YELLOW_COLOR        wxColor(0xFF, 0xFF, 0x5E)
#define LIGHT_YELLOW_COLOR  wxColor(0xFF, 0xFF, 0xB5)
#define DARK_YELLOW_COLOR   wxColor(0xFF, 0xFF, 0x08)

class MainFrame;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotPanel
//
// @class $(Name)
// @author $(User)
// @date $(Date)
// @file $(CurrentFileName).$(CurrentFileExt)
// @brief
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class PlotPanel : public wxPanel
{
    public:
        PlotPanel(wxFrame* parent);
        ~PlotPanel();
        wxPen               m_penShortDash;
        wxPen               m_penDotDash;
        wxPen               m_penSolid;
        wxRect              m_rCtrlPrev;
        wxRect              m_rCtrl;
        wxRect              m_rGrid;
        wxRect              m_rPlot;
        MainFrame           *m_pTopFrame;
        wxAuiNotebook       *m_pNoteBook;
        double              m_label_size;
        wxSize              m_Bufsz;
        bool                m_newdata;
        wxImage             m_oImage;
        wxBitmap            *m_pBmp;
        wxNativePixelData   *m_pPix;

        // some useful events
        void            OnMouseMove(wxMouseEvent& event);
        void            OnMouseDown(wxMouseEvent& event);
        void            OnMouseUp(wxMouseEvent& event);
        void            OnMouseWheelMoved(wxMouseEvent& event);
        void            OnClose(wxCloseEvent& event ){ event.Skip(); }
        void            OnSize( wxSizeEvent& event );
        void            OnErase(wxEraseEvent& event);
        void            OnPaint(wxPaintEvent& event);
        //void OnUpdateUI( wxUpdateUIEvent& event ){ event.Skip(); }

        void            paintEvent(wxPaintEvent & evt);
        virtual void    draw(wxAutoBufferedPaintDC&  pdc);
        virtual void    drawGraticule(wxAutoBufferedPaintDC&  pdc);
        virtual double  SetZoomFactor(double zf);
        virtual double  GetZoomFactor(double zf);
        virtual void    OnShow(wxShowEvent& event);
        virtual double  GetLabelSize();
        virtual void    SetLabelSize(double size);

    protected:
        int             m_x;
        int             m_y;
        int             m_left;
        int             m_top;
        int             m_prev_w;
        int             m_prev_h;
        int             m_prev_x;
        int             m_prev_y;
        bool            m_use_bitmap;
        bool            m_clip;
        bool            m_rubberBand;
        bool            m_mouseDown;
        bool            m_firstPass;
        double          m_zoomFactor;
        int             m_greyscale;
        int             m_line_color;
    DECLARE_EVENT_TABLE()
};
#endif //__FDMDV2_PLOT__
