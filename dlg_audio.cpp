//==========================================================================
// Name:            dlg_audio.cpp
//
// Purpose:         Creates simple wxWidgets dialog GUI for audio
//                  devices/options.
//
// Date:            May 05 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include "dlg_audio.h"

AudioDlg::AudioDlg( wxWindow* parent ) : DlgAudio( parent )
{

}

void AudioDlg::OnCancel( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}

void AudioDlg::OnOK( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}

void AudioDlg::OnClose( wxCloseEvent& event )
{
    this->EndModal(wxID_OK);
}
