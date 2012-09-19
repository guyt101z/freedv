//==========================================================================
// Name:            dlg_comports.cpp
// Purpose:         Creates simple wxWidgets dialog GUI to select
//                  real/virtual Comm ports.
// Date:            May 11 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include "dlg_comports.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class ComPortsDlg
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
ComPortsDlg::ComPortsDlg( wxWindow* parent ) : DlgComPorts( parent )
{
}

//-------------------------------------------------------------------------
// OnListItemSelected()
//-------------------------------------------------------------------------
void ComPortsDlg::OnListItemSelected( wxListEvent& event )
{
    // TODO: Implement OnListItemSelected
}

//-------------------------------------------------------------------------
// ExchangeData()
//-------------------------------------------------------------------------
void ComPortsDlg::ExchangeData(int inout)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    if(inout == EXCHANGE_DATA_IN)
    {
        m_textRigCtrlPort->SetValue(wxGetApp().m_strRigCtrlPort);
        m_textRigCtrlBaud->SetValue(wxGetApp().m_strRigCtrlBaud);
        m_textRigCtrlDatabits->SetValue(wxGetApp().m_strRigCtrlDatabits);
        m_textRigCtrlStopbits->SetValue(wxGetApp().m_strRigCtrlStopbits);
        m_textRigCtrlParity->SetValue(wxGetApp().m_strRigCtrlParity);
    }
    if(inout == EXCHANGE_DATA_OUT)
    {
        wxGetApp().m_strRigCtrlPort             = m_textRigCtrlPort->GetValue();
        wxGetApp().m_strRigCtrlBaud             = m_textRigCtrlBaud->GetValue();
        wxGetApp().m_strRigCtrlDatabits         = m_textRigCtrlDatabits->GetValue();
        wxGetApp().m_strRigCtrlStopbits         = m_textRigCtrlStopbits->GetValue();
        wxGetApp().m_strRigCtrlParity           = m_textRigCtrlParity->GetValue();

        pConfig->Write(wxT("/Rig/Port"),        wxGetApp().m_strRigCtrlPort);
        pConfig->Write(wxT("/Rig/Baud"),        wxGetApp().m_strRigCtrlBaud);
        pConfig->Write(wxT("/Rig/DataBits"),    wxGetApp().m_strRigCtrlDatabits);
        pConfig->Write(wxT("/Rig/StopBits"),    wxGetApp().m_strRigCtrlStopbits);
        pConfig->Write(wxT("/Rig/Parity"),      wxGetApp().m_strRigCtrlParity);
        //m_textRigCtrlFlowControl

        pConfig->Flush();
    }
    delete wxConfigBase::Set((wxConfigBase *) NULL);
}

//-------------------------------------------------------------------------
// OnCancel()
//-------------------------------------------------------------------------
void ComPortsDlg::OnCancel( wxCommandEvent& event )
{
    this->EndModal(wxID_CANCEL);
}

//-------------------------------------------------------------------------
// OnOK()
//-------------------------------------------------------------------------
void ComPortsDlg::OnOK( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnApply()
//-------------------------------------------------------------------------
void ComPortsDlg::OnApply( wxCommandEvent& event )
{
    ExchangeData(EXCHANGE_DATA_OUT);
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void ComPortsDlg::OnClose( wxCloseEvent& event )
{
    this->EndModal(wxID_OK);
}

//-------------------------------------------------------------------------
// OnInitDialog()
//-------------------------------------------------------------------------
void ComPortsDlg::OnInitDialog( wxInitDialogEvent& event )
{
    ExchangeData(EXCHANGE_DATA_IN);
//    populateAudioInfo();
}

