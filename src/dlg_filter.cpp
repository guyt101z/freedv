//==========================================================================
// Name:            dlg_filter.cpp
// Purpose:         Dialog for controlling Codec audio filtering
// Date:            Nov 25 2012
// Authors:         David Rowe, David Witten
// 
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#include "dlg_filter.h"

#define SLIDER_MAX 100

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class FilterDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
FilterDlg::FilterDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer30;
    bSizer30 = new wxBoxSizer(wxVERTICAL);

    wxGridSizer* gSizer3;
    gSizer3 = new wxGridSizer(6, 3, 0, 0);

    gSizer3->Add(new wxStaticText(this, wxID_ANY, _("")));
    
    m_codec2LPCPostFilterEnable = new wxCheckBox(this, wxID_ANY, _("Enable LPC Post Filter"), wxDefaultPosition,wxDefaultSize , wxCHK_2STATE);
    gSizer3->Add(m_codec2LPCPostFilterEnable, 2);

    gSizer3->Add(new wxStaticText(this, wxID_ANY, _("")));
    gSizer3->Add(new wxStaticText(this, wxID_ANY, _("")));

    m_codec2LPCPostFilterBassBoost = new wxCheckBox(this, wxID_ANY, _("0-1 kHz 3dB Boost"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    gSizer3->Add(m_codec2LPCPostFilterBassBoost, 2);
 
    gSizer3->Add(new wxStaticText(this, wxID_ANY, _("")));

    m_staticText91 = new wxStaticText(this, wxID_ANY, _("Beta:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText91->Wrap(-1);
    gSizer3->Add(m_staticText91, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_codec2LPCPostFilterBeta = new wxSlider(this, wxID_ANY, 0, 0, SLIDER_MAX, wxDefaultPosition, wxSize(155,wxDefaultCoord));
    gSizer3->Add(m_codec2LPCPostFilterBeta, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_staticTextBeta = new wxStaticText(this, wxID_ANY, _("0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticTextBeta->Wrap(-1);
    //gSizer3->Add(m_staticTextBeta, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 2);
    gSizer3->Add(m_staticTextBeta, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL);

    m_staticText911 = new wxStaticText(this, wxID_ANY, _("Gamma:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText911->Wrap(-1);
    gSizer3->Add(m_staticText911, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_codec2LPCPostFilterGamma = new wxSlider(this, wxID_ANY, 0, 0, SLIDER_MAX, wxDefaultPosition, wxSize(155,wxDefaultCoord));
    gSizer3->Add(m_codec2LPCPostFilterGamma, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_staticTextGamma = new wxStaticText(this, wxID_ANY, _("0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticTextGamma->Wrap(-1);
    gSizer3->Add(m_staticTextGamma, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL);

    bSizer30->Add(gSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5);

    m_sdbSizer5 = new wxStdDialogButtonSizer();
    m_sdbSizer5OK = new wxButton(this, wxID_OK);
    m_sdbSizer5->AddButton(m_sdbSizer5OK);
    m_sdbSizer5Apply = new wxButton(this, wxID_APPLY);
    m_sdbSizer5->AddButton(m_sdbSizer5Apply);
    m_sdbSizer5Cancel = new wxButton(this, wxID_CANCEL);
    m_sdbSizer5->AddButton(m_sdbSizer5Cancel);
    m_sdbSizer5->Realize();

    bSizer30->Add(m_sdbSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    this->SetSizer(bSizer30);
    this->Layout();

    this->Centre(wxBOTH);
    this->Centre(wxBOTH);

    // Connect Events
    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(FilterDlg::OnInitDialog));
    m_codec2LPCPostFilterBeta->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(FilterDlg::OnBetaScroll), NULL, this);
    m_codec2LPCPostFilterGamma->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(FilterDlg::OnGammaScroll), NULL, this);
    m_sdbSizer5Cancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnCancel), NULL, this);
    m_sdbSizer5OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnOK), NULL, this);
}

//-------------------------------------------------------------------------
// ~FilterDlg()
//-------------------------------------------------------------------------
FilterDlg::~FilterDlg()
{
    // Disconnect Events
    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(FilterDlg::OnInitDialog));
    m_codec2LPCPostFilterBeta->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(FilterDlg::OnBetaScroll), NULL, this);
    m_codec2LPCPostFilterGamma->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(FilterDlg::OnGammaScroll), NULL, this);
    m_sdbSizer5Cancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnCancel), NULL, this);
    m_sdbSizer5OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnOK), NULL, this);
}

//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void FilterDlg::ExchangeData(int inout)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    if(inout == EXCHANGE_DATA_IN)
    {
        m_codec2LPCPostFilterEnable->SetValue(wxGetApp().m_codec2LPCPostFilterEnable);
        m_codec2LPCPostFilterBassBoost->SetValue(wxGetApp().m_codec2LPCPostFilterBassBoost);
        //m_codec2LPCPostFilterBeta->SetValue(wxGetApp().m_codec2LPCPostFilterBeta);
        //m_codec2LPCPostFilterGamma->SetValue(wxGetApp().m_codec2LPCPostFilterGamma);
    }
    if(inout == EXCHANGE_DATA_OUT)
    {
        wxGetApp().m_codec2LPCPostFilterEnable     = m_codec2LPCPostFilterEnable->GetValue();
        wxGetApp().m_codec2LPCPostFilterBassBoost  = m_codec2LPCPostFilterBassBoost->GetValue();
        //wxGetApp().m_codec2LPCPostFilterBeta       = m_codec2LPCPostFilterBeta->GetValue();
        //wxGetApp().m_codec2LPCPostFilterGamma      = m_codec2LPCPostFilterGamma->GetValue();

        pConfig->Write(wxT("/Filter/codec2LPCPostFilterEnable"),     wxGetApp().m_codec2LPCPostFilterEnable);
        pConfig->Write(wxT("/Filter/codec2LPCPostFilterBassBoost"),  wxGetApp().m_codec2LPCPostFilterBassBoost);
        //pConfig->Write(wxT("/Filter/codec2LPCPostFilterBeta"),       wxGetApp().m_codec2LPCPostFilterBeta);
        //pConfig->Write(wxT("/Filter/codec2LPCPostFilterGamma"),      wxGetApp().m_codec2LPCPostFilterGamma);

        pConfig->Flush();
    }
    delete wxConfigBase::Set((wxConfigBase *) NULL);
}

//-------------------------------------------------------------------------
// OnCancel()
//-------------------------------------------------------------------------
void FilterDlg::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

//-------------------------------------------------------------------------
// OnOK()
//-------------------------------------------------------------------------
void FilterDlg::OnOK(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void FilterDlg::OnClose(wxCloseEvent& event)
{
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnInitDialog()
//-------------------------------------------------------------------------
void FilterDlg::OnInitDialog(wxInitDialogEvent& event)
{
    ExchangeData(EXCHANGE_DATA_IN);
}

void FilterDlg::OnBetaScroll(wxScrollEvent& event) {
    float beta = (float)m_codec2LPCPostFilterBeta->GetValue()/SLIDER_MAX;
    wxString buf;
    buf.Printf(wxT("%3.2f"), beta);
    m_staticTextBeta->SetLabel(buf);
}

void FilterDlg::OnGammaScroll(wxScrollEvent& event) {
    float gamma = (float)m_codec2LPCPostFilterGamma->GetValue()/SLIDER_MAX;
    wxString buf;
    buf.Printf(wxT("%3.2f"), gamma);
    m_staticTextGamma->SetLabel(buf);
}

