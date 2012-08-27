//==========================================================================
// Name:            fdmdv2_plot.cpp
// Purpose:         Implements simple wxWidgets application with GUI.
// Created:         Apr. 9, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include <string.h>
#include "wx/wx.h"
#include "fdmdv2_main.h"
#include "fdmdv2_plot.h"

#define wxUSE_FILEDLG   1
#define wxUSE_LIBPNG    1
#define wxUSE_LIBJPEG   1
#define wxUSE_GIF       1
#define wxUSE_PCX       1
#define wxUSE_LIBTIFF   1

BEGIN_EVENT_TABLE(PlotPanel, wxPanel)
    EVT_PAINT           (PlotPanel::OnPaint)
    EVT_MOTION          (PlotPanel::OnMouseMove)
    EVT_LEFT_DOWN       (PlotPanel::OnMouseDown)
    EVT_LEFT_UP         (PlotPanel::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotPanel::OnMouseWheelMoved)
    EVT_SIZE            (PlotPanel::OnSize)
    EVT_SHOW            (PlotPanel::OnShow)
//    EVT_ERASE_BACKGROUND(PlotPanel::OnErase)
END_EVENT_TABLE()

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotPanel(wxFrame* parent) : wxPanel(parent)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
PlotPanel::PlotPanel(wxFrame* parent) : wxPanel(parent)
{
    m_clip              = false;
    m_bitmap            = true;
    m_zoomFactor        = 1.0;
    m_rubberBand        = false;
    m_mouseDown         = false;
    m_penShortDash      = wxPen(wxColor(0xA0, 0xA0, 0xA0), 1, wxPENSTYLE_SHORT_DASH);
    m_penDotDash        = wxPen(wxColor(0xD0, 0xD0, 0xD0), 1, wxPENSTYLE_DOT_DASH);
    m_gridLeftOffset    = 10;
    m_gridRightOffset   = 10;
    m_gridTopOffset     = 10;
    m_gridBottomOffset  = 10;
    SetLabelSize(10.0);
    paintNow();
}

//-------------------------------------------------------------------------
// GetLabelSize()
//-------------------------------------------------------------------------
double PlotPanel::GetLabelSize()
{
    return m_label_size;
}

//-------------------------------------------------------------------------
// SetLabelSize()
//-------------------------------------------------------------------------
void PlotPanel::SetLabelSize(double size)
{
    m_label_size = size;
}

//-------------------------------------------------------------------------
// OnActivate()
//-------------------------------------------------------------------------
void PlotPanel::OnShow(wxShowEvent& event)
{
    paintNow();
}

//-------------------------------------------------------------------------
// OnErase()
//-------------------------------------------------------------------------
void PlotPanel::OnErase(wxEraseEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnSize()
//-------------------------------------------------------------------------
void PlotPanel::OnSize(wxSizeEvent& event)
{
    if(m_bitmap)
    {
        paintNow();
    }
}

//-------------------------------------------------------------------------
// OnMouseMove()
//-------------------------------------------------------------------------
void PlotPanel::OnMouseMove(wxMouseEvent& event)
{
//    if(m_mouseDown)
//    {
//        paintNow();
//    }
}

//-------------------------------------------------------------------------
// OnMouseDown()
//-------------------------------------------------------------------------
void PlotPanel::OnMouseDown(wxMouseEvent& event)
{
    m_mouseDown = true;
}

//-------------------------------------------------------------------------
// OnMouseWheelMoved()
//-------------------------------------------------------------------------
void PlotPanel::OnMouseWheelMoved(wxMouseEvent& event)
{
}

//-------------------------------------------------------------------------
// OnMouseUp()
//-------------------------------------------------------------------------
void PlotPanel::OnMouseUp(wxMouseEvent& event)
{
    m_mouseDown = false;
}

//-------------------------------------------------------------------------
// SetZoomFactor()
//-------------------------------------------------------------------------
double PlotPanel::SetZoomFactor(double zf)
{
    if((zf > 0) && (zf < 5.0))
    {
        m_zoomFactor = zf;
    }
    return zf;
}

//-------------------------------------------------------------------------
// GetZoomFactor()
//-------------------------------------------------------------------------
double PlotPanel::GetZoomFactor(double zf)
{
    return m_zoomFactor;
}

#define PLOT_BORDER         10
#define XLEFT_OFFSET        0
#define YBOTTOM_OFFSET      25
#define GRID_INCREMENT      50
#define GREY_COLOR          wxColor(0x80, 0x80, 0x80)
#define BLACK_COLOR         wxColor(0x00, 0x00, 0x00)

//-------------------------------------------------------------------------
// render() Temporary. Subclass for each view, overide this as needed.
//-------------------------------------------------------------------------
void PlotPanel::render(wxDC&  dc)
{
    m_rectCtrl  = GetClientRect();
    m_rectGrid  = m_rectCtrl;

    m_rectGrid.Deflate(PLOT_BORDER, (PLOT_BORDER + (YBOTTOM_OFFSET/2)));
    m_rectGrid.Offset(PLOT_BORDER, PLOT_BORDER);

    int h = m_rectGrid.GetHeight();
    int w = m_rectGrid.GetWidth();

    int p;
    char buf[15];
    wxString s;

    dc.Clear();

    // Draw a filled rectangle with aborder
    dc.SetBrush(*wxBLUE_BRUSH);
    dc.SetPen(wxPen(BLACK_COLOR, 2));
    dc.DrawRectangle(PLOT_BORDER, PLOT_BORDER, w, h);

    // Vertical gridlines
    dc.SetPen(m_penShortDash);
    for(p = (PLOT_BORDER + GRID_INCREMENT); p < w; p += GRID_INCREMENT)
    {
        dc.DrawLine(p, (h + PLOT_BORDER), p, PLOT_BORDER);
    }
    // Horizontal gridlines
    dc.SetPen(m_penDotDash);
    for(p = (h - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        dc.DrawLine(PLOT_BORDER, (p + PLOT_BORDER), (w + PLOT_BORDER), (p + PLOT_BORDER));
    }
    // Label the X-Axis
    dc.SetPen(wxPen(GREY_COLOR, 1));
    for(p = GRID_INCREMENT; p < (w - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    {
        sprintf(buf, "%1.1f Hz",(double)(p / 10));
        dc.DrawText(buf, p - PLOT_BORDER, h + YBOTTOM_OFFSET/2);
    }
}

//-------------------------------------------------------------------------
// paintEvent()
//
// Called by the system of by wxWidgets when the panel needs
// to be redrawn. You can also trigger this call by calling
// Refresh()/Update().
//-------------------------------------------------------------------------
void PlotPanel::OnPaint(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

//-------------------------------------------------------------------------
// paintNow()
//
// Alternatively, you can use a clientDC to paint on the panel
// at any time. Using this generally does not free you from
// catching paint events, since it is possible that e.g. the window
// manager throws away your drawing when the window comes to the
// background, and expects you will redraw it when the window comes
// back (by sending a paint event).
//
// In most cases, this will not be needed at all; simply handling
// paint events and calling Refresh() when a refresh is needed
// will do the job.
//-------------------------------------------------------------------------
void PlotPanel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

// general purpose way of plotting scalar values that are
// updated once per frame

