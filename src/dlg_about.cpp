//==========================================================================
// Name:            dlg_about.h
//
// Purpose:         Creates simple wxWidgets dialog GUI for the
//                  help/about page.
//
// Date:            May 05 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include "dlg_about.h"

AboutDlg::AboutDlg( wxWindow* parent ) : DlgAbout( parent )
{
}

void AboutDlg::OnInitDialog( wxInitDialogEvent& event )
{
// TODO: Implement OnInitDialog
}

//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void AboutDlg::ExchangeData(int inout)
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
// OnOK()
//-------------------------------------------------------------------------
void AboutDlg::OnOK( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}

