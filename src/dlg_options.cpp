//==========================================================================
// Name:            dlg_options.cpp
//
// Purpose:         Creates simple wxWidgets dialog GUI general program
//                  devices/options.
//
// Date:            May 06 2012
// Authors:         David Rowe, David Witten
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#include "dlg_options.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class OptionsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
OptionsDlg::OptionsDlg( wxWindow* parent ) : DlgOptions( parent )
{
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void OptionsDlg::OnClose( wxCloseEvent& event )
{
}

//-------------------------------------------------------------------------
// OnInitDialog()
//-------------------------------------------------------------------------
void OptionsDlg::OnInitDialog( wxInitDialogEvent& event )
{
    ExchangeData(EXCHANGE_DATA_IN);
}

//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void OptionsDlg::ExchangeData(int inout)
{
//    wxConfigBase *pConfig = wxConfigBase::Get();
    if(inout == EXCHANGE_DATA_IN)
    {
//        m_textRigCtrlPort->SetValue(wxGetApp().m_strRigCtrlPort);
//        m_textRigCtrlBaud->SetValue(wxGetApp().m_strRigCtrlBaud);
//        m_textRigCtrlDatabits->SetValue(wxGetApp().m_strRigCtrlDatabits);
//        m_textRigCtrlStopbits->SetValue(wxGetApp().m_strRigCtrlStopbits);
//        m_textRigCtrlParity->SetValue(wxGetApp().m_strRigCtrlParity);
    }
    if(inout == EXCHANGE_DATA_OUT)
    {
//        wxGetApp().m_strRigCtrlPort             = m_textRigCtrlPort->GetValue();
//        wxGetApp().m_strRigCtrlBaud             = m_textRigCtrlBaud->GetValue();
//        wxGetApp().m_strRigCtrlDatabits         = m_textRigCtrlDatabits->GetValue();
//        wxGetApp().m_strRigCtrlStopbits         = m_textRigCtrlStopbits->GetValue();
//        wxGetApp().m_strRigCtrlParity           = m_textRigCtrlParity->GetValue();

//        pConfig->Write(wxT("/Rig/Port"),        wxGetApp().m_strRigCtrlPort);
//        pConfig->Write(wxT("/Rig/Baud"),        wxGetApp().m_strRigCtrlBaud);
//        pConfig->Write(wxT("/Rig/DataBits"),    wxGetApp().m_strRigCtrlDatabits);
//        pConfig->Write(wxT("/Rig/StopBits"),    wxGetApp().m_strRigCtrlStopbits);
//        pConfig->Write(wxT("/Rig/Parity"),      wxGetApp().m_strRigCtrlParity);

//        pConfig->Flush();
    }
//    delete wxConfigBase::Set((wxConfigBase *) NULL);
}

//-------------------------------------------------------------------------
// OnCancel()
//-------------------------------------------------------------------------
void OptionsDlg::OnCancel( wxCommandEvent& event )
{
    this->EndModal(wxID_CANCEL);
}

//-------------------------------------------------------------------------
// OnOK()
//-------------------------------------------------------------------------
void OptionsDlg::OnOK( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnApply()
//-------------------------------------------------------------------------
void OptionsDlg::OnApply( wxCommandEvent& event )
{
    ExchangeData(EXCHANGE_DATA_OUT);
}
