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
//    m_bmp               = new wxBitmap(MAX_BMP_X, MAX_BMP_Y, wxBITMAP_SCREEN_DEPTH);
    m_zoomFactor        = 1.0;
    m_newdata           = false;
    m_clip              = false;
    m_use_bitmap        = true;
    m_rubberBand        = false;
    m_mouseDown         = false;
    m_penShortDash      = wxPen(wxColor(0xA0, 0xA0, 0xA0), 1, wxPENSTYLE_SHORT_DASH);
    m_penDotDash        = wxPen(wxColor(0xD0, 0xD0, 0xD0), 1, wxPENSTYLE_DOT_DASH);
    m_penSolid          = wxPen(wxColor(0x00, 0x00, 0x00), 1, wxPENSTYLE_SOLID);
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetLabelSize(10.0);
/*
    wxNativePixelData m_pBmp(*m_bmp);
    if(!m_pBmp)
    {
        // ... raw access to bitmap data unavailable, do something else ...
        return;
    }
    if(m_pBmp.GetWidth() < 20 || m_pBmp.GetHeight() < 20)
    {
        // ... complain: the bitmap it too small ...
        return;
    }
*/
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
    m_rCtrlPrev = m_rCtrl;
    m_rCtrl     = GetClientRect();
    if(m_use_bitmap)
    {
        m_bmp = new wxBitmap(m_rCtrl.GetWidth(), m_rCtrl.GetHeight(), wxBITMAP_SCREEN_DEPTH);
        m_firstPass = true;
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
void PlotPanel::drawGraticule(wxAutoBufferedPaintDC& pdc)
{
    int p;
    char buf[15];
    wxString s;

    // Vertical gridlines
    pdc.SetPen(m_penShortDash);
    for(p = (PLOT_BORDER + XLEFT_OFFSET + GRID_INCREMENT); p < ((m_rCtrl.GetWidth() - XLEFT_OFFSET) + GRID_INCREMENT); p += GRID_INCREMENT)
    {
        pdc.DrawLine(p, (m_rGrid.GetHeight() + PLOT_BORDER), p, PLOT_BORDER);
    }
    // Horizontal gridlines
    pdc.SetPen(m_penDotDash);
    for(p = (m_rGrid.GetHeight() - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        pdc.DrawLine(PLOT_BORDER + XLEFT_OFFSET, (p + PLOT_BORDER), (m_rCtrl.GetWidth() + PLOT_BORDER + XLEFT_OFFSET), (p + PLOT_BORDER));
    }
    // Label the X-Axis
    pdc.SetPen(wxPen(GREY_COLOR, 1));
    for(p = GRID_INCREMENT; p < (m_rCtrl.GetWidth() - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    {
        sprintf(buf, "%1.1f Hz",(double)(p / 10));
        pdc.DrawText(buf, p - PLOT_BORDER + XLEFT_OFFSET, m_rGrid.GetHeight() + YBOTTOM_OFFSET/2);
    }
    // Label the Y-Axis
    //for(p = GRID_INCREMENT; p < (h - YBOTTOM_OFFSET); p += GRID_INCREMENT)
    for(p = (m_rGrid.GetHeight() - GRID_INCREMENT); p > PLOT_BORDER; p -= GRID_INCREMENT)
    {
        sprintf(buf, "%1.0f", (double)((m_rGrid.GetHeight() - p) * -10));
        pdc.DrawText(buf, XLEFT_TEXT_OFFSET, p);
    }
}

//-------------------------------------------------------------------------
// draw()
//-------------------------------------------------------------------------
void PlotPanel::draw(wxAutoBufferedPaintDC& pdc)
{
//    wxAutoBufferedPaintDC dc(this);
//    wxMemoryDC memdc(&m_bmp);

    m_rCtrl  = GetClientRect();
    m_rGrid  = m_rCtrl;

    m_rCtrl.Deflate(PLOT_BORDER + (XLEFT_OFFSET/2), (PLOT_BORDER + (YBOTTOM_OFFSET/2)));
    m_rCtrl.Offset(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER);

    //m_h = m_rCtrl.GetHeight();
    //m_w = m_rCtrl.GetWidth();

    pdc.Clear();

    // Draw a filled rectangle with aborder
    wxBrush ltBlueBrush = wxBrush(LIGHT_BLUE_COLOR);
    pdc.SetBrush(ltBlueBrush);
    pdc.SetPen(wxPen(BLACK_COLOR, 1));
    pdc.DrawRectangle(PLOT_BORDER + XLEFT_OFFSET, PLOT_BORDER, m_rCtrl.GetWidth(), m_rCtrl.GetHeight());
    drawGraticule(pdc);
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
    wxAutoBufferedPaintDC pdc(this);
    draw(pdc);
}

