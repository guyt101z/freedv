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

#define SLIDER_MAX 20

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
    gSizer3 = new wxGridSizer(6, 2, 0, 0);

    m_staticText8 = new wxStaticText(this, wxID_ANY, _("Enable LPC Post Filter:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText8->Wrap(-1);
    gSizer3->Add(m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_codec2LPCPostFilterEnable = new wxCheckBox(this, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    gSizer3->Add(m_codec2LPCPostFilterEnable, 2, wxALL|wxEXPAND, 2);

    m_staticText9 = new wxStaticText(this, wxID_ANY, _("0-1 kHz 3dB Boost"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText9->Wrap(-1);
    gSizer3->Add(m_staticText9, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_codec2LPCPostFilterBassBoost = new wxCheckBox(this, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    gSizer3->Add(m_codec2LPCPostFilterBassBoost, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_staticText91 = new wxStaticText(this, wxID_ANY, _("Beta:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText91->Wrap(-1);
    gSizer3->Add(m_staticText91, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_codec2LPCPostFilterBeta = new wxSlider(this, wxID_ANY, 0, 0, SLIDER_MAX);
    gSizer3->Add(m_codec2LPCPostFilterBeta, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_staticText911 = new wxStaticText(this, wxID_ANY, _("Gamma:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    m_staticText911->Wrap(-1);
    gSizer3->Add(m_staticText911, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 2);

    m_codec2LPCPostFilterGamma = new wxSlider(this, wxID_ANY, 0, 0, SLIDER_MAX);
    gSizer3->Add(m_codec2LPCPostFilterGamma, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    bSizer30->Add(gSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5);

    m_sdbSizer5 = new wxStdDialogButtonSizer();
    m_sdbSizer5OK = new wxButton(this, wxID_OK);
    m_sdbSizer5->AddButton(m_sdbSizer5OK);
    m_sdbSizer5Apply = new wxButton(this, wxID_APPLY);
    m_sdbSizer5->AddButton(m_sdbSizer5Apply);
    m_sdbSizer5Cancel = new wxButton(this, wxID_CANCEL);
    m_sdbSizer5->AddButton(m_sdbSizer5Cancel);
    m_sdbSizer5->Realize();

    bSizer30->Add(m_sdbSizer5, 0, wxEXPAND, 5);


    this->SetSizer(bSizer30);
    this->Layout();

    this->Centre(wxBOTH);
    this->Centre(wxBOTH);

    // Connect Events
    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(FilterDlg::OnInitDialog));
    //m_sdbSizer5Apply->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnApply), NULL, this);
    m_sdbSizer5Cancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnCancel), NULL, this);
    m_sdbSizer5OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnOK), NULL, this);
    //m_listCtrlPortSelect->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(FilterDlg::OnListItemSelected), NULL, this);
}

//-------------------------------------------------------------------------
// ~FilterDlg()
//-------------------------------------------------------------------------
FilterDlg::~FilterDlg()
{
    // Disconnect Events
    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(FilterDlg::OnInitDialog));
    //m_listCtrlPortSelect->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(FilterDlg::OnListItemSelected), NULL, this);
    //m_sdbSizer5Apply->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnApply), NULL, this);
    m_sdbSizer5Cancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnCancel), NULL, this);
    m_sdbSizer5OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FilterDlg::OnOK), NULL, this);
}

//-------------------------------------------------------------------------
// OnListItemSelected()
//-------------------------------------------------------------------------
void FilterDlg::OnListItemSelected(wxListEvent& event)
{
    // TODO: Implement OnListItemSelected
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
// OnApply()
//-------------------------------------------------------------------------
void FilterDlg::OnApply(wxCommandEvent& event)
{
    ExchangeData(EXCHANGE_DATA_OUT);
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

