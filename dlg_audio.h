//==========================================================================
// Name:              dlg_audio.h
//
// Purpose:           Subclasses dialog GUI for audio devices/options.
//                  (DlgAudio from topFrame.h)
//
// Date:            May 05 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __AUDIO_DIALOG__
#define __AUDIO_DIALOG__

#include "topFrame.h"

class AudioDlg : public DlgAudio
{
    protected:
        // Handlers for DlgAudio events.
        void OnCancel( wxCommandEvent& event );
        void OnOK( wxCommandEvent& event );
        void OnClose( wxCloseEvent& event );
    public:
        /** Constructor */
        AudioDlg( wxWindow* parent );

};

#endif // __AUDIO_DIALOG__
