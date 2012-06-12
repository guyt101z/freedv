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

#ifndef __THREAD_AUDIO__
#define __THREAD_AUDIO__

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class AudioThread
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class AudioThread: public wxThread
{
    public:
        AudioThread(MainFrame *handler) : wxThread(wxTHREAD_DETACHED){ m_pHandler = handler; }
        ~AudioThread();
        void *Entry();
        void OnDelete();
        void OnExit();
        void OnKill();
        bool TestDestroy();

    protected:
        //ExitCode Entry();
        MainFrame *m_pHandler;
};

    // declare a new type of event, to be used by our MyThread class:
    wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_COMPLETED, wxThreadEvent);
    wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent);
    class MyFrame;

    class MyThread : public wxThread
    {
    public:
        MyThread(MyFrame *handler)
            : wxThread(wxTHREAD_DETACHED)
            { m_pHandler = handler }
        ~MyThread();

    protected:
        virtual ExitCode Entry();
        MyFrame *m_pHandler;
    };

    class MyFrame : public wxFrame
    {
    public:
        ...
        ~MyFrame()
        {
            // it's better to do any thread cleanup in the OnClose()
            // event handler, rather than in the destructor.
            // This is because the event loop for a top-level window is not
            // active anymore when its destructor is called and if the thread
            // sends events when ending, they won't be processed unless
            // you ended the thread from OnClose.
            // See @ref overview_windowdeletion for more info.
        }
        ...
        void DoStartThread();
        void DoPauseThread();

        // a resume routine would be nearly identic to DoPauseThread()
        void DoResumeThread() { ... }

        void OnThreadUpdate(wxThreadEvent&);
        void OnThreadCompletion(wxThreadEvent&);
        void OnClose(wxCloseEvent&);

    protected:
        MyThread *m_pThread;
        wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer

        wxDECLARE_EVENT_TABLE();
    };

    wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
        EVT_CLOSE(MyFrame::OnClose)
        EVT_MENU(Minimal_Start,  MyFrame::DoStartThread)
        EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MYTHREAD_UPDATE, MyFrame::OnThreadUpdate)
        EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MYTHREAD_COMPLETED, MyFrame::OnThreadCompletion)
    wxEND_EVENT_TABLE()

    wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_COMPLETED, wxThreadEvent)
    wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent)

    void MyFrame::DoStartThread()
    {
        m_pThread = new MyThread(this);

        if ( m_pThread->Create() != wxTHREAD_NO_ERROR )
        {
            wxLogError("Can't create the thread!");
            delete m_pThread;
            m_pThread = NULL;
        }
        else
        {
            if (m_pThread->Run() != wxTHREAD_NO_ERROR )
            {
                wxLogError("Can't create the thread!");
                delete m_pThread;
                m_pThread = NULL;
            }

            // after the call to wxThread::Run(), the m_pThread pointer is "unsafe":
            // at any moment the thread may cease to exist (because it completes its work).
            // To avoid dangling pointers OnThreadExit() will set m_pThread
            // to NULL when the thread dies.
        }
    }

    wxThread::ExitCode MyThread::Entry()
    {
        while (!TestDestroy())
        {
            // ... do a bit of work...

            wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_MYTHREAD_UPDATE));
        }

        // signal the event handler that this thread is going to be destroyed
        // NOTE: here we assume that using the m_pHandler pointer is safe,
        //       (in this case this is assured by the MyFrame destructor)
        wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_MYTHREAD_COMPLETED));

        return (wxThread::ExitCode)0;     // success
    }

    MyThread::~MyThread()
    {
        wxCriticalSectionLocker enter(m_pHandler->m_pThreadCS);

        // the thread is being destroyed; make sure not to leave dangling pointers around
        m_pHandler->m_pThread = NULL;
    }

    void MyFrame::OnThreadCompletion(wxThreadEvent&)
    {
        wxMessageOutputDebug().Printf("MYFRAME: MyThread exited!\n");
    }

    void MyFrame::OnThreadUpdate(wxThreadEvent&)
    {
        wxMessageOutputDebug().Printf("MYFRAME: MyThread update...\n");
    }

    void MyFrame::DoPauseThread()
    {
        // anytime we access the m_pThread pointer we must ensure that it won't
        // be modified in the meanwhile; since only a single thread may be
        // inside a given critical section at a given time, the following code
        // is safe:
        wxCriticalSectionLocker enter(m_pThreadCS);

        if (m_pThread)         // does the thread still exist?
        {
            // without a critical section, once reached this point it may happen
            // that the OS scheduler gives control to the MyThread::Entry() function,
            // which in turn may return (because it completes its work) making
            // invalid the m_pThread pointer

            if (m_pThread->Pause() != wxTHREAD_NO_ERROR )
                wxLogError("Can't pause the thread!");
        }
    }

    void MyFrame::OnClose(wxCloseEvent&)
    {
        {
            wxCriticalSectionLocker enter(m_pThreadCS);

            if (m_pThread)         // does the thread still exist?
            {
                wxMessageOutputDebug().Printf("MYFRAME: deleting thread");

                if (m_pThread->Delete() != wxTHREAD_NO_ERROR )
                    wxLogError("Can't delete the thread!");
            }
        }       // exit from the critical section to give the thread
                // the possibility to enter its destructor
                // (which is guarded with m_pThreadCS critical section!)

        while (1)
        {
            { // was the ~MyThread() function executed?
                wxCriticalSectionLocker enter(m_pThreadCS);
                if (!m_pThread) break;
            }

            // wait for thread completion
            wxThread::This()->Sleep(1);
        }

        Destroy();
    }


#endif // __THREAD_AUDIO__
