//==========================================================================
// Name:            thread_audio.h
//
// Purpose:         Declares background thread classes to handle audio IO.
// Created:         May 11, 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include <wx/thread.h>
#include "fdmdv2_main.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class AudioThread
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class AudioThread: public wxThread
{
    public:
        AudioThread(MainFrame *handler) : wxThread(wxTHREAD_DETACHED){ m_pHandler = handler; }
        ~AudioThread();

    protected:
        virtual ExitCode Entry();
        MainFrame *m_pHandler;
};

