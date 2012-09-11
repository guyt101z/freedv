//==========================================================================
// Name:            fdmdv2_plot.cpp
// Purpose:         Implements simple wxWidgets application with GUI.
// Created:         Apr. 9, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//
// @class $(Name)
// @author $(User)
// @date $(Date)
// @file $(CurrentFileName).$(CurrentFileExt)
// @brief
//
//==========================================================================
#include <string.h>
#include "fdmdv2_plot.h"

BEGIN_EVENT_TABLE(PlotPanel, wxPanel)
    EVT_PAINT           (PlotPanel::OnPaint)
    EVT_MOTION          (PlotPanel::OnMouseMove)
    EVT_LEFT_DOWN       (PlotPanel::OnMouseDown)
    EVT_LEFT_UP         (PlotPanel::OnMouseUp)
    EVT_MOUSEWHEEL      (PlotPanel::OnMouseWheelMoved)
    EVT_SIZE            (PlotPanel::OnSize)
    EVT_SHOW            (PlotPanel::OnShow)
END_EVENT_TABLE()

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PlotPanel(wxFrame* parent) : wxPanel(parent)
//
// @class $(Name)
// @author $(User)
// @date $(Date)
// @file $(CurrentFileName).$(CurrentFileExt)
// @brief
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
PlotPanel::PlotPanel(wxFrame* parent) : wxPanel(parent)
{
    m_pNoteBook         = (wxAuiNotebook *) parent;
    m_pTopFrame         = (MainFrame *)m_pNoteBook->GetParent();
    m_bmp               = new wxBitmap(MAX_BMP_X, MAX_BMP_Y, wxBITMAP_SCREEN_DEPTH);
    wxNativePixelData m_pBmp(*m_bmp);
    if ( !m_pBmp )
    {
        // ... raw access to bitmap data unavailable, do something else ...
        return;
    }

    if ( m_pBmp.GetWidth() < 20 || m_pBmp.GetHeight() < 20 )
    {
        // ... complain: the bitmap it too small ...
        return;
    }

    m_zoomFactor        = 1.0;
    m_clip              = false;
    m_use_bitmap        = true;
    m_newdata           = false;
    m_rubberBand        = false;
    m_mouseDown         = false;
    m_penShortDash      = wxPen(wxColor(0xA0, 0xA0, 0xA0), 1, wxPENSTYLE_SHORT_DASH);
    m_penDotDash        = wxPen(wxColor(0xD0, 0xD0, 0xD0), 1, wxPENSTYLE_DOT_DASH);
    m_penSolid          = wxPen(wxColor(0x00, 0x00, 0x00), 1, wxPENSTYLE_SOLID);
//    m_gridLeftOffset    = 20;
//    m_gridRightOffset   = 5;
//    m_gridTopOffset     = 5;
//    m_gridBottomOffset  = 10;
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetLabelSize(10.0);
}

//-------------------------------------------------------------------------
// ~PlotPanel()
//-------------------------------------------------------------------------
PlotPanel::~PlotPanel()
{
    if(!m_bmp->IsNull())
    {
        delete m_bmp;
    }
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
// OnShow()
//-------------------------------------------------------------------------
void PlotPanel::OnShow(wxShowEvent& event)
{
    this->Refresh();
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
    if(m_use_bitmap)
    {
        this->Refresh();
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

//-------------------------------------------------------------------------
// drawGraticule()
//-------------------------------------------------------------------------
void PlotPanel::drawGraticule(wxAutoBufferedPaintDC&  dc)
{
    int p;
    char buf[15];
    wxString s;

    // Vertical gridlines
    dc.SetPen(m_penShortDash);
    for(p = (PLOT_BORDER + XLEFT_OFFSET + GRID_INCREMENT); p < ((m_w - XLEFT_OFFSET) + GRID_INCREMENT); p += GRID_INCREMENT)
    {
        dc.DrawLine(p, (m_h + PLOT_BORDER), p, PLOT_BORDER);
    }
    // Horizontal gridlines
    dc.SetPen(m_penDotDash);
    for(p = (m_h - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        dc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (p + PLOT_BORDER), (m_w + PLOT_BORDER + XLEFT_OFFSET), (p + PLOT_BORDER));
    }
    // Label the X-Axis
    dc.SetPen(wxPen(GREY_COLOR, 1));
    for(p = GRID_INCREMENT; p < (m_w - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    {
        sprintf(buf, "%1.1f Hz",(double)(p / 10));
        dc.DrawText(buf, p - PLOT_BORDER + XLEFT_OFFSET, m_h + YBOTTOM_OFFSET/2);
    }
    // Label the Y-Axis
    //for(p = GRID_INCREMENT; p < (h - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    for(p = (m_h - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        sprintf(buf, "%1.0f", (double)((m_h - p) * -10));
        dc.DrawText(buf, XLEFT_TEXT_OFFSET, p);
    }
}

//-------------------------------------------------------------------------
// draw()
//-------------------------------------------------------------------------
void PlotPanel::draw(wxAutoBufferedPaintDC&  dc)
{
    m_rectCtrl  = GetClientRect();
    m_rectGrid  = m_rectCtrl;

    m_rectGrid.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));
    m_rectGrid.Offset(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER);

    m_h = m_rectGrid.GetHeight();
    m_w = m_rectGrid.GetWidth();

    dc.Clear();

    // Draw a filled rectangle with aborder
    wxBrush ltBlueBrush = wxBrush(LIGHT_BLUE_COLOR);
    dc.SetBrush(ltBlueBrush);
    dc.SetPen(wxPen(BLACK_COLOR, 1));
    dc.DrawRectangle(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_w, m_h);
    drawGraticule(dc);
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
    wxAutoBufferedPaintDC dc(this);
    draw(dc);
}

