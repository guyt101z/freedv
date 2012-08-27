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

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotPanel
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class PlotPanel : public wxPanel
{
    public:
        PlotPanel(wxFrame* parent);
        void paintEvent(wxPaintEvent & evt);
        void paintNow();
        void render(wxDC& dc);
        wxPen m_penShortDash;
        wxPen m_penDotDash;
        wxRect m_rectCtrl;
        wxRect m_rectGrid;
        wxRect m_rectPlot;
        int m_gridLeftOffset;
        int m_gridRightOffset;
        int m_gridTopOffset;
        int m_gridBottomOffset;
        double m_label_size;
        wxBitmap            *m_bmp;
        //wxNativePixelData   *m_pBmp;
        //wxAlphaPixelData    *m_pBmp;
        wxImagePixelData    *m_pBmp;

        // some useful events
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseDown(wxMouseEvent& event);
        void OnMouseUp(wxMouseEvent& event);
        void OnMouseWheelMoved(wxMouseEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnClose( wxCloseEvent& event ){ event.Skip(); }
        void OnSize( wxSizeEvent& event );
        void OnErase(wxEraseEvent& event);
        double SetZoomFactor(double zf);
        double GetZoomFactor(double zf);
        //void OnUpdateUI( wxUpdateUIEvent& event ){ event.Skip(); }
        void OnShow(wxShowEvent& event);
        double GetLabelSize();
        void SetLabelSize(double size);

    protected:
        int  m_x;
        int  m_y;
        int  m_w;
        int  m_h;
        int  m_prev_w;
        int  m_prev_h;
        int  m_prev_x;
        int  m_prev_y;
        bool m_bitmap;
        bool m_clip;
        bool m_rubberBand;
        bool m_mouseDown;
        double m_zoomFactor;
    DECLARE_EVENT_TABLE()
};
#endif //__FDMDV2_PLOT__
