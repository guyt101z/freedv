//==========================================================================
// Name:            dlg_ptt.cpp
// Purpose:         Subclasses dialog GUI for PTT Config. Creates simple 
//                  wxWidgets dialog GUI to select real/virtual Comm ports.
// Date:            May 11 2012
// Authors:         David Rowe, David Witten, Joel Stanley
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
#include "dlg_ptt.h"
#include "fdmdv2_main.h"

#ifdef __WIN32__
#include <wx/msw/registry.h>
#endif

#include <sstream>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class ComPortsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
ComPortsDlg::ComPortsDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);
    
    wxStaticBoxSizer* staticBoxSizer28 = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("VOX PTT Settings")), wxVERTICAL);
    m_ckHalfDuplex = new wxCheckBox(this, wxID_ANY, _("Half Duplex"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_ckHalfDuplex->SetToolTip(_("Should be checked for VOX operated Tx/Rx switching"));
    staticBoxSizer28->Add(m_ckHalfDuplex, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    mainSizer->Add(staticBoxSizer28, 0, wxEXPAND, 5);

    wxStaticBoxSizer* staticBoxSizer18 = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("Hamlib Settings")), wxVERTICAL);

    //wxGridSizer* gridSizer100 = new wxGridSizer(1, 5, 0, 0);
    wxBoxSizer* gridSizer100 = new wxBoxSizer(wxHORIZONTAL);

    /* Use Hamlib for PTT checkbox. */
    m_ckUseHamlibPTT = new wxCheckBox(this, wxID_ANY, _("Use Hamlib PTT"), wxDefaultPosition, wxSize(-1, -1), 0);
    m_ckUseHamlibPTT->SetValue(false);
    gridSizer100->Add(m_ckUseHamlibPTT, 0, wxALIGN_CENTER_VERTICAL, 0);

    /* Hamlib Rig Type combobox. */
    gridSizer100->Add(new wxStaticText(this, wxID_ANY, _("Rig Model:"), wxDefaultPosition, wxDefaultSize, 0), 
                      0, wxALIGN_CENTER_VERTICAL | wxLEFT, 20);
    m_cbRigName = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250, -1), 0, NULL, wxCB_DROPDOWN);
    /* TODO(Joel): this is a hack. At the least, need to gurantee that m_hamLib
     * exists. */
    wxGetApp().m_hamlib->populateComboBox(m_cbRigName);
    m_cbRigName->SetSelection(wxGetApp().m_intHamlibRig);
    gridSizer100->Add(m_cbRigName, 0, wxALIGN_CENTER_VERTICAL, 0);

    /* Hamlib Serial Port combobox. */
    gridSizer100->Add(new wxStaticText(this, wxID_ANY, _("Serial Device:"), wxDefaultPosition, wxDefaultSize, 0), 
                      0, wxALIGN_CENTER_VERTICAL | wxLEFT, 20);
    m_cbSerialPort = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(140, -1), 0, NULL, wxCB_DROPDOWN);
    gridSizer100->Add(m_cbSerialPort, 0, wxALIGN_CENTER_VERTICAL, 0);

    staticBoxSizer18->Add(gridSizer100, 1);
    mainSizer->Add(staticBoxSizer18, 1);

    wxBoxSizer* boxSizer12 = new wxBoxSizer(wxHORIZONTAL);

    m_buttonOK = new wxButton(this, wxID_OK, _("OK"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_buttonOK->SetDefault();
    boxSizer12->Add(m_buttonOK, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    m_buttonCancel = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxSize(-1,-1), 0);
    boxSizer12->Add(m_buttonCancel, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    m_buttonApply = new wxButton(this, wxID_APPLY, _("Apply"), wxDefaultPosition, wxSize(-1,-1), 0);
    boxSizer12->Add(m_buttonApply, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    mainSizer->Add(boxSizer12, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
    //SetSizeHints(450,300);

    if ( GetSizer() ) 
    {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);

    // Connect events
    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(ComPortsDlg::OnInitDialog), NULL, this);
    m_buttonOK->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnOK), NULL, this);
    m_buttonCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnCancel), NULL, this);
    m_buttonApply->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnApply), NULL, this);
}

//-------------------------------------------------------------------------
// ~ComPortsDlg()
//-------------------------------------------------------------------------
ComPortsDlg::~ComPortsDlg()
{
    // Disconnect Events
    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(ComPortsDlg::OnInitDialog), NULL, this);
    m_buttonOK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnOK), NULL, this);
    m_buttonCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnCancel), NULL, this);
    m_buttonApply->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComPortsDlg::OnApply), NULL, this);
}

//-------------------------------------------------------------------------
// OnInitDialog()
//-------------------------------------------------------------------------
void ComPortsDlg::OnInitDialog(wxInitDialogEvent& event)
{
    populatePortList();
    ExchangeData(EXCHANGE_DATA_IN);
}

//-------------------------------------------------------------------------
// populatePortList()
//-------------------------------------------------------------------------
void ComPortsDlg::populatePortList()
{
#ifdef __WXMSW__
    m_listCtrlPorts->Clear();
    wxArrayString aStr;
    wxRegKey key(wxRegKey::HKLM, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"));
    if(!key.Exists())
    {
        return;
    }
    else
    {
        // Get the number of subkeys and enumerate them.
        if(!key.Open(wxRegKey::Read))
        {
            return;
        }    
        size_t subkeys;
        size_t values;
        if(!key.GetKeyInfo(&subkeys, NULL, &values, NULL))
        {
            return;
        }
        if(!key.HasValues())
        {
            return;
        }
        wxString key_name;
        long el = 1;
        key.GetFirstValue(key_name, el);
        wxString valType;
        wxString key_data;
        for(unsigned int i = 0; i < values; i++)
        {
            key.QueryValue(key_name, key_data);
//            wxPrintf("Value:  %s Data: %s\n", key_name, key_data);
            aStr.Add(key_data, 1);
            key.GetNextValue(key_name, el);
        }
    }
    m_cbSerialPort->Append(aStr);
#endif
#ifdef __WXGTK__
    /* TODO(Joel): http://stackoverflow.com/questions/2530096/how-to-find-all-serial-devices-ttys-ttyusb-on-linux-without-opening-them */
    m_cbSerialPort->Clear();
    m_cbSerialPort->Append("/dev/ttyUSB0");
    m_cbSerialPort->Append("/dev/ttyS0");
#endif
}

//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void ComPortsDlg::ExchangeData(int inout)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString str;
    
    if(inout == EXCHANGE_DATA_IN)
    {
        m_ckHalfDuplex->SetValue(wxGetApp().m_boolHalfDuplex);
        m_ckUseHamlibPTT->SetValue(wxGetApp().m_boolHamlibUseForPTT);
        m_cbRigName->SetSelection(wxGetApp().m_intHamlibRig);
        m_cbSerialPort->SetValue(wxGetApp().m_strHamlibSerialPort);
    }
    if(inout == EXCHANGE_DATA_OUT)
    {
        wxGetApp().m_boolHalfDuplex = m_ckHalfDuplex->GetValue();
        pConfig->Write(wxT("/Rig/HalfDuplex"), wxGetApp().m_boolHalfDuplex);

        /* Hamlib settings. */
        wxGetApp().m_boolHamlibUseForPTT = m_ckUseHamlibPTT->GetValue();
        wxGetApp().m_intHamlibRig = m_cbRigName->GetSelection();
        wxGetApp().m_strHamlibSerialPort = m_cbSerialPort->GetValue();

        pConfig->Write(wxT("/Hamlib/UseForPTT"), wxGetApp().m_boolHamlibUseForPTT);
        pConfig->Write(wxT("/Hamlib/RigName"), wxGetApp().m_intHamlibRig);
        pConfig->Write(wxT("/Hamlib/SerialPort"), wxGetApp().m_strHamlibSerialPort);
        pConfig->Flush();
    }
    delete wxConfigBase::Set((wxConfigBase *) NULL);
}

//-------------------------------------------------------------------------
// OnApply()
//-------------------------------------------------------------------------
void ComPortsDlg::OnApply(wxCommandEvent& event)
{
    ExchangeData(EXCHANGE_DATA_OUT);
}

//-------------------------------------------------------------------------
// OnCancel()
//-------------------------------------------------------------------------
void ComPortsDlg::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void ComPortsDlg::OnOK(wxCommandEvent& event)
{
    ExchangeData(EXCHANGE_DATA_OUT);
    this->EndModal(wxID_OK);
}
