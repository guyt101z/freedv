//==========================================================================
// Name:            fdmdv2_main.h
//
// Purpose:         Declares simple wxWidgets application with GUI.
// Created:         Apr. 9, 2012
// Authors:         David Rowe, David Witten
// 
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#ifndef __FDMDV2_MAIN__
#define __FDMDV2_MAIN__

#include "version.h"
#ifndef _NO_AUTOTOOLS_
#include "../config.h"
#endif
#include <wx/wx.h>

#include <wx/tglbtn.h>
#include <wx/app.h>
//#include <wx/aboutdlg.h>
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
#include <wx/url.h>
#include <wx/sstream.h>
#include <wx/listbox.h>
#include <wx/textdlg.h>

#include <samplerate.h>

#include <hamlib.h> 
#include "ctb-0.16/ctb.h"
#include "ctb-0.16/portscan.h"
#include "ctb-0.16/serportx.h"
#include "ctb-0.16/serport.h"

#include "codec2.h"
#include "codec2_fdmdv.h"
#include "codec2_fifo.h"
#include "golay23.h"

#include "topFrame.h"
#include "dlg_ptt.h"
#include "fdmdv2_plot.h"
#include "fdmdv2_plot_scalar.h"
#include "fdmdv2_plot_scatter.h"
#include "fdmdv2_plot_waterfall.h"
#include "fdmdv2_plot_spectrum.h"
#include "fdmdv2_pa_wrapper.h"
#include "sndfile.h"
#include "portaudio.h"
#include "dlg_audiooptions.h"
#include "dlg_filter.h"
#include "dlg_about.h"
#include "dlg_options.h"
#include "varicode.h"
#include "sox_biquad.h"

#define _USE_TIMER              1
#define _USE_ONIDLE             1
#define _DUMMY_DATA             1
//#define _AUDIO_PASSTHROUGH    1
#define _REFRESH_TIMER_PERIOD   (DT*1000)
//#define _USE_ABOUT_DIALOG       1

enum {
        ID_START = wxID_HIGHEST,
        ID_TIMER_WATERFALL,
        ID_TIMER_SPECTRUM,
        ID_TIMER_SCATTER,
        ID_TIMER_SCALAR
     };

#define EXCHANGE_DATA_IN    0
#define EXCHANGE_DATA_OUT   1

extern int                 g_nSoundCards;
extern int                 g_soundCard1InDeviceNum;
extern int                 g_soundCard1OutDeviceNum;
extern int                 g_soundCard1SampleRate;
extern int                 g_soundCard2InDeviceNum;
extern int                 g_soundCard2OutDeviceNum;
extern int                 g_soundCard2SampleRate;

class MainFrame;

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

        wxString            m_textNumChOut;
        wxString            m_textNumChIn;

        wxString            m_strRxInAudio;
        wxString            m_strRxOutAudio;
        wxString            m_textVoiceInput;
        wxString            m_textVoiceOutput;
        wxString            m_strSampleRate;
        wxString            m_strBitrate;

        // PTT -----------------------------------

        bool                m_boolHalfDuplex;

        bool                m_boolHamlibUseForPTT;
        unsigned int        m_intHamlibRig;
        wxString            m_strHamlibSerialPort;
        Hamlib              *m_hamlib;

        bool                m_boolUseSerialPTT;
        wxString            m_strRigCtrlPort;
        bool                m_boolUseRTS;
        bool                m_boolRTSPos;
        bool                m_boolUseDTR;
        bool                m_boolDTRPos;

        // Play/Rec files

        wxString            m_playFileToMicInPath;
        wxString            m_recFileFromRadioPath;
        unsigned int        m_recFileFromRadioSecs;
        wxString            m_playFileFromRadioPath;

        wxString            m_callSign;

        bool                m_snrSlow;

        // LPC Post Filter
        bool                m_codec2LPCPostFilterEnable;
        bool                m_codec2LPCPostFilterBassBoost;
        float               m_codec2LPCPostFilterGamma;
        float               m_codec2LPCPostFilterBeta;
        
        // Mic In Equaliser
        float               m_MicInBassFreqHz;
        float               m_MicInBassGaindB;
        float               m_MicInTrebleFreqHz;
        float               m_MicInTrebleGaindB;
        float               m_MicInMidFreqHz;
        float               m_MicInMidGaindB;
        float               m_MicInMidQ;
        bool                m_MicInEQEnable;

        // Spk Out Equaliser
        float               m_SpkOutBassFreqHz;
        float               m_SpkOutBassGaindB;
        float               m_SpkOutTrebleFreqHz;
        float               m_SpkOutTrebleGaindB;
        float               m_SpkOutMidFreqHz;
        float               m_SpkOutMidGaindB;
        float               m_SpkOutMidQ;
        bool                m_SpkOutEQEnable;

        // Flags for displaying windows
        int                 m_show_wf;
        int                 m_show_spect;
        int                 m_show_scatter;
        int                 m_show_timing;
        int                 m_show_freq;
        int                 m_show_speech_in;
        int                 m_show_speech_out;
        int                 m_show_demod_in;
        int                 m_show_test_frame_errors;

        // notebook display after tx->rxtransition
        int                 m_rxNbookCtrl;

        wxRect              m_rTopWindow;

        int                 m_framesPerBuffer;

        bool                loadConfig();
        bool                saveConfig();

        // misc

        bool        m_testFrames;

        int        FilterEvent(wxEvent& event);
        MainFrame *frame;
    protected:
};

// declare global static function wxGetApp()
DECLARE_APP(MainApp)

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// paCallBackData
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
typedef struct
{
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

    int             inputChannels1, inputChannels2;

    // EQ filter states
    void           *sbqMicInBass;
    void           *sbqMicInTreble;
    void           *sbqMicInMid;
    void           *sbqSpkOutBass;
    void           *sbqSpkOutTreble;
    void           *sbqSpkOutMid;

    bool            micInEQEnable;
    bool            spkOutEQEnable;

} paCallBackData;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// panel with custom loop checkbox for play file dialog
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class MyExtraPlayFilePanel : public wxPanel
{
public:
    MyExtraPlayFilePanel(wxWindow *parent);
    void setLoopPlayFileToMicIn(bool checked) { m_cb->SetValue(checked); }
    bool getLoopPlayFileToMicIn(void) { return m_cb->GetValue(); }
private:
    wxCheckBox *m_cb;
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// panel with custom Seconds-to-record control for record file dialog
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class MyExtraRecFilePanel : public wxPanel
{
public:
    MyExtraRecFilePanel(wxWindow *parent);
    ~MyExtraRecFilePanel() 
    {
        wxLogDebug("Destructor\n");
    }
    void setSecondsToRecord(wxString value) { m_secondsToRecord->SetValue(value); }
    wxString getSecondsToRecord(void) 
    { 
        wxLogDebug("getSecondsToRecord: %s\n",m_secondsToRecord->GetValue());
        return m_secondsToRecord->GetValue(); 
    }
private:
    wxTextCtrl *m_secondsToRecord;
};

class txRxThread;

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
        PlotScalar*             m_panelTestFrameErrors;

        bool                    m_RxRunning;

        PortAudioWrap           *m_rxPa;
        PortAudioWrap           *m_txPa;

        PaError                 m_rxErr;
        PaError                 m_txErr;
        
        txRxThread*             m_txRxThread;

        bool                    OpenHamlibRig();
        void                    SetupSerialPort(void);
        void                    CloseSerialPort(void);
        void                    SerialPTTRx(void);

        bool                    m_modal;

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


    void initPortAudioDevice(PortAudioWrap *pa, int inDevice, int outDevice, 
                             int soundCard, int sampleRate, int inputChannels);

    void togglePTT(void);

    protected:

        ctb::IOBase*            m_device;
        ctb::SerialPort*        m_serialPort;

        void setsnrBeta(bool snrSlow);

        // protected event handlers
        virtual void OnCloseFrame(wxCloseEvent& event);
        void OnExitClick(wxCommandEvent& event);

        void startTxStream();
        void startRxStream();
        void stopTxStream();
        void stopRxStream();
        void abortTxStream();
        void abortRxStream();
        
        void OnTop(wxCommandEvent& event);
        void OnExit( wxCommandEvent& event );

        void OnToolsAudio( wxCommandEvent& event );
        void OnToolsAudioUI( wxUpdateUIEvent& event );
        void OnToolsComCfg( wxCommandEvent& event );
        void OnToolsComCfgUI( wxUpdateUIEvent& event );
        void OnToolsFilter( wxCommandEvent& event );
        void OnToolsOptions(wxCommandEvent& event);
        void OnToolsOptionsUI(wxUpdateUIEvent& event);

        void OnPlayFileToMicIn( wxCommandEvent& event );
        void OnRecFileFromRadio( wxCommandEvent& event );
        void OnPlayFileFromRadio( wxCommandEvent& event );

        void OnHelpCheckUpdates( wxCommandEvent& event );
        void OnHelpCheckUpdatesUI( wxUpdateUIEvent& event );
        void OnHelpAbout( wxCommandEvent& event );
        void OnCmdSliderScroll( wxScrollEvent& event );
//        void OnSliderScrollBottom( wxScrollEvent& event );
//        void OnCmdSliderScrollChanged( wxScrollEvent& event );
//        void OnSliderScrollTop( wxScrollEvent& event );
        void OnCheckSQClick( wxCommandEvent& event );
        void OnCheckSNRClick( wxCommandEvent& event );

        // Toggle Buttons
        void OnTogBtnSplitClick(wxCommandEvent& event);
        void OnTogBtnAnalogClick(wxCommandEvent& event);
        void OnTogBtnRxID( wxCommandEvent& event );
        void OnTogBtnTxID( wxCommandEvent& event );
        void OnTogBtnPTT( wxCommandEvent& event );
        void OnTogBtnOnOff( wxCommandEvent& event );

        void OnCallSignReset( wxCommandEvent& event );
        void OnBerReset( wxCommandEvent& event );

        //System Events
        void OnPaint(wxPaintEvent& event);
        void OnSize( wxSizeEvent& event );
        void OnUpdateUI( wxUpdateUIEvent& event );
        void OnDeleteConfig(wxCommandEvent&);
#ifdef _USE_TIMER
        void OnTimer(wxTimerEvent &evt);
#endif
#ifdef _USE_ONIDLE
        void OnIdle(wxIdleEvent &evt);
#endif
    private:
        bool        m_useMemory;
        wxTextCtrl* m_tc;
        int         m_zoom;
        float       m_snrBeta;
        
        // Callsign
        char        m_callsign[MAX_CALLSIGN];
        char       *m_pcallsign;

        // level Gauge
        float       m_maxLevel;
 
        // flags to indicate when new EQ filters need to be designed

        bool        m_newMicInFilter;
        bool        m_newSpkOutFilter;

        void*       designAnEQFilter(const char filterType[], float freqHz, float gaindB, float Q = 0.0);
        void        designEQFilters(paCallBackData *cb);
        void        deleteEQFilters(paCallBackData *cb);
};

void txRxProcessing();

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// class txRxThread - experimental tx/rx processing thread
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class txRxThread : public wxThread
{
public:
    txRxThread(void) : wxThread(wxTHREAD_JOINABLE) { m_run = 1; }

    // thread execution starts here
    void *Entry() 
    {
        while (m_run) 
        {
            txRxProcessing();        
            wxThread::Sleep(20);
        }
        return NULL;
    }

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    void OnExit() { }

public:
    bool  m_run;
};

void resample_for_plot(struct FIFO *plotFifo, short buf[], int length);

int resample(SRC_STATE *src,
             short      output_short[],
             short      input_short[],
             int        output_sample_rate,
             int        input_sample_rate,
             int        length_output_short, // maximum output array length in samples
             int        length_input_short
             );
void txRxProcessing();
void per_frame_rx_processing(
                                        FIFO    *output_fifo,   // decoded speech samples
                                        int     codec_bits[],  // current frame of bits for decoder
                                        FIFO    *input_fifo,   // modem samples input to demod
                                        int     *nin,          // amount of samples demod needs for next call
                                        int     *state,        // used to collect codec_bits[] halves
                                        struct  CODEC2 *c2     // Codec 2 states
                                    );

void per_frame_tx_processing(
                                            short   tx_fdm_scaled[], // ouput modulated samples
                                            short   input_buf[],     // speech sample input
                                            CODEC2  *c2              // Codec 2 states
                        );


#endif //__FDMDV2_MAIN__
