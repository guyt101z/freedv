//==========================================================================
// Name:            fdmdv2_main.h
// Purpose:         Declares simple wxWidgets application with GUI.
// Created:         Apr. 9, 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __FDMDV2_MAIN__
#define __FDMDV2_MAIN__

#include <wx/wx.h>
#include "wx/image.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/graphics.h"
#include "wx/mstream.h"
#include "wx/wfstream.h"
#include "wx/quantize.h"
#include "wx/scopedptr.h"
#include "wx/stopwatch.h"
#include "wx/versioninfo.h"
#include <wx/sound.h>

#include "topFrame.h"
#include "dlg_about.h"
#include "dlg_audio.h"
#include "dlg_options.h"
#include "dlg_comports.h"
#include "fdmdv2_plot.h"


#define WAV_FILE wxT("doggrowl.wav")
enum
{
    ID_ROTATE_LEFT = wxID_HIGHEST + 1,
    ID_ROTATE_RIGHT,
    ID_RESIZE,
    ID_PAINT_BG
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class MainApp
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class MainApp : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit()
        {
            //DeleteBitmaps();
            return 0;
        }

    protected:
        //void DeleteBitmaps();
        //bool LoadImages();
};

// declare global static function wxGetApp()
DECLARE_APP(MainApp)

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class MainApp
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class MainFrame : public TopFrame
{
    public:
        MainFrame(wxWindow *parent);
        virtual ~MainFrame();
        DrawPanel*  m_panelSpectrum;
        DrawPanel*  m_panelWaterfall;
        DrawPanel*  m_panelExtra1;
        DrawPanel*  m_panelExtra2;
        bool        m_radioRunning;
        // virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
        // virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
        // virtual void OnEraseBackground( wxEraseEvent& event ) { event.Skip(); }
        // virtual void OnMouseDown( wxMouseEvent& event );
        // virtual void OnMouseUp( wxMouseEvent& event );
        // virtual void OnMouseMove( wxMouseEvent& event );
        // virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
        // virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
        // virtual void OnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }

    protected:
        // protected event handlers
        virtual void OnCloseFrame(wxCloseEvent& event);
        virtual void OnExitClick(wxCommandEvent& event);

        void OnOpen( wxCommandEvent& event );
        void OnOpenUpdateUI( wxUpdateUIEvent& event );
        void OnSave( wxCommandEvent& event );
        void OnSaveUpdateUI( wxUpdateUIEvent& event );
        void OnClose( wxCommandEvent& event );
        void OnCloseUpdateUI( wxUpdateUIEvent& event );
        void OnExit( wxCommandEvent& event );
        void OnCopy( wxCommandEvent& event );
        void OnCopyUpdateUI( wxUpdateUIEvent& event );
        void OnCut( wxCommandEvent& event );
        void OnCutUpdateUI( wxUpdateUIEvent& event );
        void OnPaste( wxCommandEvent& event );
        void OnPasteUpdateUI( wxUpdateUIEvent& event );
        void OnToolsAudio( wxCommandEvent& event );
        void OnToolsAudioUI( wxUpdateUIEvent& event );
        void OnToolsComCfg( wxCommandEvent& event );
        void OnToolsComCfgUI( wxUpdateUIEvent& event );
        void OnToolsOptions( wxCommandEvent& event );
        void OnToolsOptionsUI( wxUpdateUIEvent& event );
        void OnHelpCheckUpdates( wxCommandEvent& event );
        void OnHelpCheckUpdatesUI( wxUpdateUIEvent& event );
        void OnHelpAbout( wxCommandEvent& event );
        void OnCmdSliderScroll( wxScrollEvent& event );
        void OnSliderScrollBottom( wxScrollEvent& event );
        void OnCmdSliderScrollChanged( wxScrollEvent& event );
        void OnSliderScrollTop( wxScrollEvent& event );
        void OnCheckSQClick( wxCommandEvent& event );
        // Toggle Buttons
        void OnTogBtnSplitClick(wxCommandEvent& event);
        void OnTogBtnAnalogClick(wxCommandEvent& event);
        void OnTogBtnALCClick(wxCommandEvent& event);
        void OnTogBtnRxID( wxCommandEvent& event );
        void OnTogBtnTxID( wxCommandEvent& event );
        void OnTogBtnTXClick( wxCommandEvent& event );

        void OnPaint(wxPaintEvent& event);
        void OnClose( wxCloseEvent& event );
        void OnSize( wxSizeEvent& event );
        void OnUpdateUI( wxUpdateUIEvent& event );

        wxString LoadUserImage(wxImage& image);

//        ScrollCanvas*   m_scrolledSpectrum;
//        ScrollCanvas*   m_scrolledWaterfall;

    private:
        bool CreateSound(wxSound& snd) const;

        wxSound*    m_sound;
        wxString    m_soundFile;
#ifdef __WXMSW__
        wxString    m_soundRes;
#endif // __WXMSW__
        bool        m_useMemory;

        wxTextCtrl* m_tc;
        wxBitmap    m_bitmap;
        wxBitmap    m_alphaBitmap;
        int         m_zoom;

        // any class wishing to process wxWidgets events must use this macro
//    DECLARE_EVENT_TABLE()
};


#endif //__FDMDV2_MAIN__
