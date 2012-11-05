//==========================================================================
// Name:            fdmdv2_main.h
//
// Purpose:         Declares simple wxWidgets application with GUI.
// Created:         Apr. 9, 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#ifndef __FDMDV2_MAIN__
#define __FDMDV2_MAIN__

#include <wx/wx.h>

#include <wx/tglbtn.h>
#include <wx/app.h>
#include "wx/rawbmp.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/config.h"
#include <wx/fileconf.h>
#include "wx/graphics.h"
#include "wx/mstream.h"
#include "wx/wfstream.h"
#include "wx/quantize.h"
#include "wx/scopedptr.h"
#include "wx/stopwatch.h"
#include "wx/versioninfo.h"
#include <wx/sound.h>

#include <samplerate.h>

#include "codec2.h"
#include "fdmdv.h"
#include "fifo.h"

#include "topFrame.h"
#include "dlg_about.h"
#include "dlg_audio.h"
#include "dlg_options.h"
#include "dlg_comports.h"
#include "fdmdv2_plot.h"
#include "fdmdv2_plot_scalar.h"
#include "fdmdv2_plot_scatter.h"
#include "fdmdv2_plot_waterfall_linux.h"
#include "fdmdv2_plot_spectrum.h"
#include "fdmdv2_pa_wrapper.h"
#include "sndfile.h"
#include "portaudio.h"

#define _USE_TIMER              1
//#define _USE_ONIDLE             1
#define _DUMMY_DATA             1
//#define _AUDIO_PASSTHROUGH    1
#define _REFRESH_TIMER_PERIOD   (DT*1000)
#define _READ_WITH_SNDFILE      1

enum {
        ID_START = wxID_HIGHEST,
        ID_TIMER_WATERFALL,
        ID_TIMER_SPECTRUM,
        ID_TIMER_SCATTER,
        ID_TIMER_SCALAR
     };

#define EXCHANGE_DATA_IN    0
#define EXCHANGE_DATA_OUT   1

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class MainApp
//
// @class $(Name)
// @author $(User)
// @date $(Date)
// @file $(CurrentFileName).$(CurrentFileExt)
// @brief
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class MainApp : public wxApp
{
    public:
        virtual bool        OnInit();
        virtual int         OnExit();

        wxString            m_strVendName;
        wxString            m_StrAppName;

//        wxRadioButton       m_radioBtnPortAudio;
//        wxRadioButton       m_radioBtnFileOnly;

        wxString            m_textNumChOut;
        wxString            m_textNumChIn;

        wxString            m_strRxInAudio;
        wxString            m_strRxOutAudio;
        wxString            m_textVoiceInput;
        wxString            m_textVoiceOutput;
        wxString            m_strSampleRate;
        wxString            m_strBitrate;

        wxString            m_strRigCtrlPort;
        wxString            m_strRigCtrlBaud;
        wxString            m_strRigCtrlDatabits;
        wxString            m_strRigCtrlStopbits;
        wxString            m_strRigCtrlParity;
        int                 m_show_wf;
        int                 m_show_spect;
        int                 m_show_scatter;
        int                 m_show_timing;
        int                 m_show_freq;
        int                 m_show_speech_in;
        int                 m_show_speech_out;
        int                 m_show_demod_in;

        wxRect              m_rTopWindow;

        bool                loadConfig();
        bool                saveConfig();

    protected:
};

// declare global static function wxGetApp()
DECLARE_APP(MainApp)

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// paCallBackData
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
typedef struct
{
    PlotSpectrum    *pSPPanel;
    PlotWaterfall   *pWFPanel;

    // libresample states for 48 to 8 kHz conversions

    SRC_STATE      *insrc1;
    SRC_STATE      *outsrc1;
    SRC_STATE      *insrc2;
    SRC_STATE      *outsrc2;

    // FIFOs attached to first sound card

    struct FIFO    *infifo1;
    struct FIFO    *outfifo1;

    // FIFOs attached to second sound card

    struct FIFO    *infifo2;
    struct FIFO    *outfifo2;

    // FIFOs for rx process

    struct FIFO    *rxinfifo;
    struct FIFO    *rxoutfifo;

} paCallBackData;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class MainFrame
//
// @class $(Name)
// @author $(User)
// @date $(Date)
// @file $(CurrentFileName).$(CurrentFileExt)
// @brief
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class MainFrame : public TopFrame
{
    public:
        MainFrame(wxWindow *parent);
        virtual ~MainFrame();

        PlotSpectrum*           m_panelSpectrum;
        PlotWaterfall*          m_panelWaterfall;
        PlotScatter*            m_panelScatter;
        PlotScalar*             m_panelTimeOffset;
        PlotScalar*             m_panelFreqOffset;
        PlotScalar*             m_panelSpeechIn;
        PlotScalar*             m_panelSpeechOut;
        PlotScalar*             m_panelDemodIn;
        bool                    m_SquelchActive;
        bool                    m_RxRunning;
        bool                    m_TxRunning;
        PortAudioWrap           *m_rxPa;
        PortAudioWrap           *m_txPa;
        paCallBackData          *m_rxUserdata;
        paCallBackData          *m_txUserdata;
        PaDeviceIndex           m_rxDevIn;
        PaDeviceIndex           m_rxDevOut;
        PaDeviceIndex           m_txDevIn;
        PaDeviceIndex           m_txDevOut;
        PaError                 m_rxErr;
        PaError                 m_txErr;
#ifdef _USE_TIMER
        wxTimer                 m_plotTimer;
#endif

    void destroy_fifos(void);
    void destroy_src(void);
    void autoDetectSoundCards(PortAudioWrap *pa);

        static int rxCallback(
                                const void *inBuffer,
                                void *outBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *outTime,
                                PaStreamCallbackFlags statusFlags,
                                void *userData
                             );

        static int txCallback(
                                const void *inBuffer,
                                void *outBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *outTime,
                                PaStreamCallbackFlags statusFlags,
                                void *userData
                             );

        static void per_frame_rx_processing(
                                        FIFO    *output_fifo,   // decoded speech samples
                                        int     codec_bits[],  // current frame of bits for decoder
                                        FIFO    *input_fifo,   // modem samples input to demod
                                        int     *nin,          // amount of samples demod needs for next call
                                        int     *state,        // used to collect codec_bits[] halves
                                        struct  CODEC2 *c2     // Codec 2 states
                                    );

        static void per_frame_tx_processing(
                                            short   tx_fdm_scaled[], // ouput modulated samples
                                            short   input_buf[],     // speech sample input
                                            CODEC2  *c2              // Codec 2 states
                        );

    int initPortAudioDevice(PortAudioWrap *pa, int inDevice, int outDevice, int soundCard, int sampleRate);

 protected:
        // protected event handlers
        virtual void OnCloseFrame(wxCloseEvent& event);
        virtual void OnExitClick(wxCommandEvent& event);
        //void OnQuit(wxCommandEvent& event);

        void averageData(float mag_dB[]);
        void startTxStream();
        void startRxStream();
        void stopTxStream();
        void stopRxStream();
        void abortTxStream();
        void abortRxStream();

        void OnOpen( wxCommandEvent& event );
//        void OnOpenUpdateUI( wxUpdateUIEvent& event );
        void OnSave( wxCommandEvent& event );
//        void OnSaveUpdateUI( wxUpdateUIEvent& event );
        void OnClose( wxCommandEvent& event );
//        void OnCloseUpdateUI( wxUpdateUIEvent& event );
        void OnExit( wxCommandEvent& event );
        void OnCopy( wxCommandEvent& event );
//        void OnCopyUpdateUI( wxUpdateUIEvent& event );
        void OnCut( wxCommandEvent& event );
//        void OnCutUpdateUI( wxUpdateUIEvent& event );
        void OnPaste( wxCommandEvent& event );
//        void OnPasteUpdateUI( wxUpdateUIEvent& event );
        void OnToolsAudio( wxCommandEvent& event );
        void OnToolsAudioUI( wxUpdateUIEvent& event );
        void OnToolsComCfg( wxCommandEvent& event );
        void OnToolsComCfgUI( wxUpdateUIEvent& event );
        void OnToolsOptions( wxCommandEvent& event );
        void OnToolsOptionsUI( wxUpdateUIEvent& event );
        //void OnCaptureRxStream( wxCommandEvent& event );
       // void OnCaptureTxStream( wxCommandEvent& event );
        void OnPlayAudioFile( wxCommandEvent& event );
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
        void OnTogBtnOnOff( wxCommandEvent& event );
        void OnTogBtnLoopRx( wxCommandEvent& event );
        void OnTogBtnLoopTx( wxCommandEvent& event );

//        void OnTogBtnSplitClickUI(wxUpdateUIEvent& event);
//        void OnTogBtnAnalogClickUI(wxUpdateUIEvent& event);
//        void OnTogBtnALCClickUI(wxUpdateUIEvent& event);
//        void OnTogBtnRxIDUI(wxUpdateUIEvent& event );
//        void OnTogBtnTxIDUI(wxUpdateUIEvent& event );
//        void OnTogBtnTXClickUI(wxUpdateUIEvent& event );
//        void OnTogBtnOnOffUI(wxUpdateUIEvent& event );

        //System Events
        void OnPaint(wxPaintEvent& event);
        void OnClose( wxCloseEvent& event );
        void OnSize( wxSizeEvent& event );
        void OnUpdateUI( wxUpdateUIEvent& event );
        void OnDeleteConfig(wxCommandEvent&);
#ifdef _USE_TIMER
        void OnTimer(wxTimerEvent &evt);
#endif
#ifdef _USE_ONIDLE
        void OnIdlge(wxIdleEvent &evt);
#endif
//        wxString LoadUserImage(wxImage& image);
    private:
        bool CreateSound(wxSound& snd) const;
        wxString    m_soundFile;
        SF_INFO     m_sfInfo;
        SNDFILE     *m_sfFile;
#ifdef __WXMSW__
//        wxString    m_soundRes;
#endif // __WXMSW__
        bool        m_useMemory;
        wxTextCtrl* m_tc;
        int         m_zoom;
};

#endif //__FDMDV2_MAIN__
