//==========================================================================
// Name:            fdmdv2_plot.h
// Purpose:         Declares simple wxWidgets application with GUI
// Created:         Apr. 10, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __FDMDV2_PLOT__
#define __FDMDV2_PLOT__
//#include "codec2.h"
//#include "fdmdv.h"
#include <wx/rawbmp.h>
#include <wx/image.h>
#include <wx/dcbuffer.h>

#define wxUSE_FILEDLG       1
#define wxUSE_LIBPNG        1
#define wxUSE_LIBJPEG       1
#define wxUSE_GIF           1
#define wxUSE_PCX           1
#define wxUSE_LIBTIFF       1

#define PLOT_BORDER         3
#define XLEFT_OFFSET        30
#define XLEFT_TEXT_OFFSET   7
#define YBOTTOM_OFFSET      25
#define GRID_INCREMENT      50
#define GREY_COLOR          wxColor(0x80, 0x80, 0x80)
#define BLACK_COLOR         wxColor(0x00, 0x00, 0x00)

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

/*
    wxNullBrush
    wxBLACK_BRUSH
    wxBLUE_BRUSH
    wxCYAN_BRUSH
    wxGREEN_BRUSH
    wxYELLOW_BRUSH
    wxGREY_BRUSH
    wxLIGHT_GREY_BRUSH
    wxMEDIUM_GREY_BRUSH
    wxRED_BRUSH
    wxTRANSPARENT_BRUSH
    wxWHITE_BRUSH
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotPanel
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class PlotPanel : public wxPanel
{
    public:
        PlotPanel(wxFrame* parent);
        void                paintEvent(wxPaintEvent & evt);
        void                draw(wxAutoBufferedPaintDC& dc);
        void                drawGraticule(wxAutoBufferedPaintDC& dc);
        wxPen               m_penShortDash;
        wxPen               m_penDotDash;
        wxRect              m_rectCtrl;
        wxRect              m_rectGrid;
        wxRect              m_rectPlot;
//        int                 m_gridLeftOffset;
//        int                 m_gridRightOffset;
//        int                 m_gridTopOffset;
//        int                 m_gridBottomOffset;
        double              m_label_size;
        wxSize              m_Bufsz;
        wxBitmap            *m_bmp;
        wxImagePixelData    *m_pBmp;
        //wxNativePixelData     *m_pBmp;
        //wxAlphaPixelData      *m_pBmp;

        // some useful events
        void    OnMouseMove(wxMouseEvent& event);
        void    OnMouseDown(wxMouseEvent& event);
        void    OnMouseUp(wxMouseEvent& event);
        void    OnMouseWheelMoved(wxMouseEvent& event);
        void    OnClose(wxCloseEvent& event ){ event.Skip(); }
        void    OnSize( wxSizeEvent& event );
        void    OnErase(wxEraseEvent& event);
        void    OnPaint(wxPaintEvent& event);

        double  SetZoomFactor(double zf);
        double  GetZoomFactor(double zf);
        //void OnUpdateUI( wxUpdateUIEvent& event ){ event.Skip(); }
        void    OnShow(wxShowEvent& event);
        double  GetLabelSize();
        void    SetLabelSize(double size);

    protected:
        int     m_x;
        int     m_y;
        int     m_w;
        int     m_h;
        int     m_prev_w;
        int     m_prev_h;
        int     m_prev_x;
        int     m_prev_y;
        bool    m_use_bitmap;
        bool    m_clip;
        bool    m_rubberBand;
        bool    m_mouseDown;
        double  m_zoomFactor;
    DECLARE_EVENT_TABLE()
};
#endif //__FDMDV2_PLOT__
