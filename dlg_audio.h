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

#include <wx/wx.h>
#include "topFrame.h"
#include "extern/include/portaudio.h"
#include "extern/include/portaudiocpp/PortAudioCpp.hxx"
#ifdef WIN32
#include "extern/include/portaudiocpp/AsioDeviceAdapter.hxx"
#endif

class AudioDlg : public DlgAudio
{
    protected:
        // Handlers for DlgAudio events.
        void OnCancel( wxCommandEvent& event );
        void OnOK( wxCommandEvent& event );
        void OnClose( wxCloseEvent& event );
        void OnInitDialog( wxInitDialogEvent& event );
        void OnApply( wxCommandEvent& event );
        void populateStandardSampleRates(
                wxComboBox* target,
                const portaudio::DirectionSpecificStreamParameters &inputParameters,
                const portaudio::DirectionSpecificStreamParameters &outputParameters);
        int populateAudioInfo();
    public:
        /** Constructor */
        AudioDlg( wxWindow* parent );
        void OnRxInputSelect( wxCommandEvent& event );
        void OnTxOutputSelect( wxCommandEvent& event );
        void OnVoiceInputSelect( wxCommandEvent& event );
        void OnVoiceOutputSelect( wxCommandEvent& event );
};

#endif // __AUDIO_DIALOG__
