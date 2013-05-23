//==========================================================================
// Name:            dlg_options.cpp
// Purpose:         Dialog for controlling misc FreeDV options
// Date:            May 24 2013
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

#include "dlg_options.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class OptionsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
OptionsDlg::OptionsDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{

    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer30;
    bSizer30 = new wxBoxSizer(wxVERTICAL);

    //------------------------------
    // Call Sign Text Box
    //------------------------------

    wxStaticBoxSizer* sbSizer_callSign;
    sbSizer_callSign = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Call Sign")), wxVERTICAL);

    m_txtCtrlCallSign = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_txtCtrlCallSign->SetToolTip(_("Call Sign of your station"));
    sbSizer_callSign->Add(m_txtCtrlCallSign, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);

    bSizer30->Add(sbSizer_callSign,0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 3);

    //------------------------------
    // Test Frames check box
    //------------------------------

    wxStaticBoxSizer* sbSizer_testFrames;
    sbSizer_testFrames = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Test Frames")), wxVERTICAL);

    m_ckboxTestFrame = new wxCheckBox(this, wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    sbSizer_testFrames->Add(m_ckboxTestFrame, 0, wxALIGN_LEFT, 0);

    bSizer30->Add(sbSizer_testFrames,0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 3);

    //------------------------------
    // Cancel - OK Buttons 
    //------------------------------

    wxBoxSizer* bSizer31 = new wxBoxSizer(wxHORIZONTAL);
    m_sdbSizer5Cancel = new wxButton(this, wxID_CANCEL);
    bSizer31->Add(m_sdbSizer5Cancel, 0, wxALL, 2);
    m_sdbSizer5OK = new wxButton(this, wxID_OK);
    bSizer31->Add(m_sdbSizer5OK, 0, wxALL, 2);

    bSizer30->Add(bSizer31, 0, wxALIGN_RIGHT|wxALL, 0);

    this->SetSizer(bSizer30);
    this->Layout();

    this->Centre(wxBOTH);
 
    // Connect Events -------------------------------------------------------

    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(OptionsDlg::OnInitDialog));

    m_sdbSizer5Cancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDlg::OnCancel), NULL, this);
    m_sdbSizer5OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDlg::OnOK), NULL, this);
}

//-------------------------------------------------------------------------
// ~OptionsDlg()
//-------------------------------------------------------------------------
OptionsDlg::~OptionsDlg()
{
    // Disconnect Events

    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(OptionsDlg::OnInitDialog));

    m_sdbSizer5Cancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDlg::OnCancel), NULL, this);
    m_sdbSizer5OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDlg::OnOK), NULL, this);
}


//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void OptionsDlg::ExchangeData(int inout, bool storePersistent)
{
    wxConfigBase *pConfig = wxConfigBase::Get();

    if(inout == EXCHANGE_DATA_IN)
    {
        m_txtCtrlCallSign->SetValue(wxGetApp().m_callSign);
        m_ckboxTestFrame->SetValue(wxGetApp().m_testFrames);
    }

    if(inout == EXCHANGE_DATA_OUT)
    {
        // LPC Post filter

        wxGetApp().m_callSign   = m_txtCtrlCallSign->GetValue();
        wxGetApp().m_testFrames = m_ckboxTestFrame->GetValue();

        if (storePersistent) {
            pConfig->Write(wxT("/Data/CallSign"), wxGetApp().m_callSign);
            pConfig->Flush();
        }
    }
    delete wxConfigBase::Set((wxConfigBase *) NULL);
}

//-------------------------------------------------------------------------
// OnCancel()
//-------------------------------------------------------------------------
void OptionsDlg::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

//-------------------------------------------------------------------------
// OnOK()
//-------------------------------------------------------------------------
void OptionsDlg::OnOK(wxCommandEvent& event)
{
    ExchangeData(EXCHANGE_DATA_OUT, true);
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void OptionsDlg::OnClose(wxCloseEvent& event)
{
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnInitDialog()
//-------------------------------------------------------------------------
void OptionsDlg::OnInitDialog(wxInitDialogEvent& event)
{
    ExchangeData(EXCHANGE_DATA_IN, false);
}
