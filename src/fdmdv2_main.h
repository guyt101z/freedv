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

#include <wx/wx.h>

#include <wx/tglbtn.h>
#include <wx/app.h>
#include <wx/aboutdlg.h>
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

#include <samplerate.h>

#include "codec2.h"
#include "fdmdv.h"
#include "fifo.h"

#include "topFrame.h"
#include "dlg_comports.h"
#include "fdmdv2_plot.h"
#include "fdmdv2_plot_scalar.h"
#include "fdmdv2_plot_scatter.h"
#include "fdmdv2_plot_waterfall_linux.h"
#include "fdmdv2_plot_spectrum.h"
#include "fdmdv2_pa_wrapper.h"
#include "sndfile.h"
#include "portaudio.h"
#include "dlg_audiooptions.h"
#include "dlg_filter.h"
#include "varicode.h"

#define _USE_TIMER              1
#define _USE_ONIDLE             1
#define _DUMMY_DATA             1
//#define _AUDIO_PASSTHROUGH    1
#define _REFRESH_TIMER_PERIOD   (DT*1000)

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

        wxString            m_strRigCtrlPort;
        wxString            m_strRigCtrlBaud;
        wxString            m_strRigCtrlDatabits;
        wxString            m_strRigCtrlStopbits;
        wxString            m_strRigCtrlParity;
        bool                m_boolUseRTS;
        bool                m_boolRTSPos;
        bool                m_boolUseDTR;
        bool                m_boolDTRPos;

        wxString            m_playFileToMicInPath;
        wxString            m_recFileFromRadioPath;
        unsigned int        m_recFileFromRadioSecs;

        bool                m_snrSlow;

        // Codec 2 Post Filter
        bool                m_codec2LPCPostFilterEnable;
        bool                m_codec2LPCPostFilterBassBoost;
        float               m_codec2LPCPostFilterGamma;
        float               m_codec2LPCPostFilterBeta;
        
        int                 m_show_wf;
        int                 m_show_spect;
        int                 m_show_scatter;
        int                 m_show_timing;
        int                 m_show_freq;
        int                 m_show_speech_in;
        int                 m_show_speech_out;
        int                 m_show_demod_in;

        wxRect              m_rTopWindow;

        int                 m_framesPerBuffer;

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
} paCallBackData;

// panel with custom loop checkbox for play file dialog
class MyExtraPlayFilePanel : public wxPanel
{
public:
    MyExtraPlayFilePanel(wxWindow *parent);
    void setLoopPlayFileToMicIn(bool checked) { m_cb->SetValue(checked); }
    bool getLoopPlayFileToMicIn(void) { return m_cb->GetValue(); }
private:
    wxCheckBox *m_cb;
};

// panel with custom Seconds-to-record control for record file dialog
class MyExtraRecFilePanel : public wxPanel
{
public:
    MyExtraRecFilePanel(wxWindow *parent);
    void setSecondsToRecord(wxString value) { m_secondsToRecord->SetValue(value); }
    wxString getSecondsToRecord(void) { return m_secondsToRecord->GetValue(); }
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

        bool                    m_RxRunning;

        PortAudioWrap           *m_rxPa;
        PortAudioWrap           *m_txPa;

        PaError                 m_rxErr;
        PaError                 m_txErr;
        
        txRxThread*             m_txRxThread;

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

 protected:

        void setsnrBeta(bool snrSlow);
        void sendTxID(void);

        // protected event handlers
        virtual void OnCloseFrame(wxCloseEvent& event);
        virtual void OnExitClick(wxCommandEvent& event);

        void startTxStream();
        void startRxStream();
        void stopTxStream();
        void stopRxStream();
        void abortTxStream();
        void abortRxStream();

        void OnExit( wxCommandEvent& event );
        void OnToolsAudio( wxCommandEvent& event );
        void OnToolsAudioUI( wxUpdateUIEvent& event );
        void OnToolsComCfg( wxCommandEvent& event );
        void OnToolsComCfgUI( wxUpdateUIEvent& event );
        void OnToolsFilter( wxCommandEvent& event );

        void OnPlayFileToMicIn( wxCommandEvent& event );
        void OnRecFileFromRadio( wxCommandEvent& event );

        void OnHelpCheckUpdates( wxCommandEvent& event );
        void OnHelpCheckUpdatesUI( wxUpdateUIEvent& event );
        void OnHelpAbout( wxCommandEvent& event );
        void OnCmdSliderScroll( wxScrollEvent& event );
        void OnSliderScrollBottom( wxScrollEvent& event );
        void OnCmdSliderScrollChanged( wxScrollEvent& event );
        void OnSliderScrollTop( wxScrollEvent& event );
        void OnCheckSQClick( wxCommandEvent& event );
        void OnCheckSNRClick( wxCommandEvent& event );

        // Toggle Buttons
        void OnTogBtnSplitClick(wxCommandEvent& event);
        void OnTogBtnAnalogClick(wxCommandEvent& event);
        void OnTogBtnRxID( wxCommandEvent& event );
        void OnTogBtnTxID( wxCommandEvent& event );
        void OnTogBtnTXClick( wxCommandEvent& event );
        void OnTogBtnOnOff( wxCommandEvent& event );

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
        float       m_txIDTimerTics;
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
