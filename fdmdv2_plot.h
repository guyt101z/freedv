//==========================================================================
// Name:            fdmdv2_plot.h
// Purpose:         Declares simple wxWidgets application with GUI
// Created:         Apr. 10, 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __FDMDV2_PLOT__
#define __FDMDV2_PLOT__

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class DrawPanel
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class DrawPanel : public wxPanel
{
    public:
        DrawPanel(wxFrame* parent);
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

        // some useful events
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseDown(wxMouseEvent& event);
        void OnMouseUp(wxMouseEvent& event);
        void OnMouseWheelMoved(wxMouseEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnClose( wxCloseEvent& event ){ event.Skip(); }
        void OnSize( wxSizeEvent& event );
        void OnErase(wxEraseEvent& event);
        //void OnUpdateUI( wxUpdateUIEvent& event ){ event.Skip(); }
        void OnShow(wxShowEvent& event);

    protected:
        bool m_bitmap;
        bool m_clip;
        bool m_rubberBand;
        bool m_mouseDown;
    DECLARE_EVENT_TABLE()
};
#endif //__FDMDV2_PLOT__
