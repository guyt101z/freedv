//==========================================================================
// Name:            fdmdv2_main.cpp
//
// Purpose:         FreeDV main()
// Created:         Apr. 9, 2012
// Authors:         David Rowe, David Witten
//
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#include "fdmdv2_main.h"

#define wxUSE_FILEDLG   1
#define wxUSE_LIBPNG    1
#define wxUSE_LIBJPEG   1
#define wxUSE_GIF       1
#define wxUSE_PCX       1
#define wxUSE_LIBTIFF   1

//-------------------------------------------------------------------
// Bunch of globals used for communication with sound card call
// back functions
// ------------------------------------------------------------------

// Global Codec2 & modem states - just one reqd for tx & rx
int                 g_Nc;
int                 g_mode;
struct CODEC2      *g_pCodec2;
struct FDMDV       *g_pFDMDV;
struct FDMDV_STATS  g_stats;
float               g_pwr_scale;
int                 g_clip;

// test Frames
int                 g_testFrames;
int                 g_test_frame_sync_state;
int                 g_test_frame_count;
int                 g_total_bits;
int                 g_total_bit_errors;
int                 g_sz_error_pattern;
short              *g_error_pattern;
struct FIFO        *g_errorFifo;

// time averaged magnitude spectrum used for waterfall and spectrum display
float               g_avmag[FDMDV_NSPEC];

// GUI controls that affect rx and tx processes
int   g_SquelchActive;
float g_SquelchLevel;
int   g_analog;
int   g_split;
int   g_tx;
float g_snr;
bool  g_half_duplex;

// sending and receiving Call Sign data
struct FIFO         *g_txDataInFifo;
struct VARICODE_DEC  g_varicode_dec_states;
struct FIFO         *g_rxDataOutFifo;

// tx/rx processing states
int                 g_nRxIn = FDMDV_NOM_SAMPLES_PER_FRAME;
int                 g_CodecBits[2*MAX_BITS_PER_FDMDV_FRAME];
int                 g_State;
paCallBackData     *g_rxUserdata;

// FIFOs used for plotting waveforms
struct FIFO        *g_plotDemodInFifo;
struct FIFO        *g_plotSpeechOutFifo;
struct FIFO        *g_plotSpeechInFifo;

// Soundcard config
int                 g_nSoundCards;
int                 g_soundCard1InDeviceNum;
int                 g_soundCard1OutDeviceNum;
int                 g_soundCard1SampleRate;
int                 g_soundCard2InDeviceNum;
int                 g_soundCard2OutDeviceNum;
int                 g_soundCard2SampleRate;

// playing and recording from sound files

SNDFILE            *g_sfPlayFile;
bool                g_playFileToMicIn;
bool                g_loopPlayFileToMicIn;
int                 g_playFileToMicInEventId;

SNDFILE            *g_sfRecFile;
bool                g_recFileFromRadio;
unsigned int        g_recFromRadioSamples;
int                 g_recFileFromRadioEventId;

SNDFILE            *g_sfPlayFileFromRadio;
bool                g_playFileFromRadio;
bool                g_loopPlayFileFromRadio;
int                 g_playFileFromRadioEventId;

wxWindow           *g_parent;

// Click to tune rx and tx frequency offset states
float               g_RxFreqOffsetHz;
COMP                g_RxFreqOffsetPhaseRect;
COMP                g_RxFreqOffsetFreqRect;
float               g_TxFreqOffsetHz;
COMP                g_TxFreqOffsetPhaseRect;
COMP                g_TxFreqOffsetFreqRect;

// experimental mutex to make sound card callbacks mutually exclusive
// TODO: review code and see if we need this any more, as fifos should
// now be thread safe

wxMutex g_mutexProtectingCallbackData;

// WxWidgets - initialize the application
IMPLEMENT_APP(MainApp);

//-------------------------------------------------------------------------
// OnInit()
//-------------------------------------------------------------------------
bool MainApp::OnInit()
{
    if(!wxApp::OnInit())
    {
        return false;
    }
    SetVendorName(wxT("CODEC2-Project"));
    SetAppName(wxT("FreeDV"));      // not needed, it's the default value

#ifdef _WXMSW_
    // Force use of file-based configuration persistance on Windows platforma
    wxConfig *pConfig = new wxConfig();
    wxFileConfig *pFConfig = new wxFileConfig(wxT("FreeDV"), wxT("CODEC2-Project"), wxT("freedv.conf"), wxT("freedv.conf"),  wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    pConfig->Set(pFConfig);
    pConfig->SetRecordDefaults();
#else
    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->SetRecordDefaults();
#endif

    m_rTopWindow = wxRect(0, 0, 0, 0);
    m_strRxInAudio.Empty();
    m_strRxOutAudio.Empty();
    m_textVoiceInput.Empty();
    m_textVoiceOutput.Empty();
    m_strSampleRate.Empty();
    m_strBitrate.Empty();
    m_boolUseTonePTT   = false;
    m_boolUseSerialPTT = true;
    // Create the main application window
    MainFrame *frame = new MainFrame(NULL);
    SetTopWindow(frame);
    // Should guarantee that the first plot tab defined is the one
    // displayed. But it doesn't when built from command line.  Why?
    frame->m_auiNbookCtrl->ChangeSelection(0);
    frame->Layout();
    frame->Show();
    g_parent =frame;
    return true;
}

//-------------------------------------------------------------------------
// OnExit()
//-------------------------------------------------------------------------
int MainApp::OnExit()
{
    return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class MainFrame(wxFrame* pa->ent) : TopFrame(parent)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
MainFrame::MainFrame(wxWindow *parent) : TopFrame(parent)
{
    m_zoom              = 1.;
    m_serialPort        = NULL;
    m_device            = NULL;

    tools->AppendSeparator();
    wxMenuItem* m_menuItemToolsConfigDelete;
    m_menuItemToolsConfigDelete = new wxMenuItem(tools, wxID_ANY, wxString(_("&Restore defaults")) , wxT("Delete config file/keys and restore defaults"), wxITEM_NORMAL);
    this->Connect(m_menuItemToolsConfigDelete->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnDeleteConfig));

    tools->Append(m_menuItemToolsConfigDelete);

    wxConfigBase *pConfig = wxConfigBase::Get();

    // restore frame position and size
    int x = pConfig->Read(wxT("/MainFrame/top"),       20);
    int y = pConfig->Read(wxT("/MainFrame/left"),      20);
    int w = pConfig->Read(wxT("/MainFrame/width"),     800);
    int h = pConfig->Read(wxT("/MainFrame/height"),    550);

    // note: run DebugView program to see this message under windows
    //wxLogDebug("x = %d y = %d w = %d h = %d\n", x,y,w,h);
    wxGetApp().m_show_wf            = pConfig->Read(wxT("/MainFrame/show_wf"),           1);
    wxGetApp().m_show_spect         = pConfig->Read(wxT("/MainFrame/show_spect"),        1);
    wxGetApp().m_show_scatter       = pConfig->Read(wxT("/MainFrame/show_scatter"),      1);
    wxGetApp().m_show_timing        = pConfig->Read(wxT("/MainFrame/show_timing"),       1);
    wxGetApp().m_show_freq          = pConfig->Read(wxT("/MainFrame/show_freq"),         1);
    wxGetApp().m_show_speech_in     = pConfig->Read(wxT("/MainFrame/show_speech_in"),    1);
    wxGetApp().m_show_speech_out    = pConfig->Read(wxT("/MainFrame/show_speech_out"),   1);
    wxGetApp().m_show_demod_in      = pConfig->Read(wxT("/MainFrame/show_demod_in"),     1);
    wxGetApp().m_show_test_frame_errors = pConfig->Read(wxT("/MainFrame/show_test_frame_errors"),     1);

    wxGetApp().m_rxNbookCtrl        = pConfig->Read(wxT("/MainFrame/rxNbookCtrl"),    (long)0);

    g_SquelchActive = pConfig->Read(wxT("/Audio/SquelchActive"), 1);
    g_SquelchLevel = pConfig->Read(wxT("/Audio/SquelchLevel"), (int)(SQ_DEFAULT_SNR*2));
    g_SquelchLevel /= 2.0;

    Move(x, y);
    SetClientSize(w, h);

    if(wxGetApp().m_show_wf)
    {
        // Add Waterfall Plot window
        m_panelWaterfall = new PlotWaterfall((wxFrame*) m_auiNbookCtrl, false, 0);
        m_panelWaterfall->SetToolTip(_("Left click to tune, Right click to toggle mono/colour"));
        m_auiNbookCtrl->AddPage(m_panelWaterfall, _("Waterfall"), true, wxNullBitmap);
    }
    if(wxGetApp().m_show_spect)
    {
        // Add Spectrum Plot window
        m_panelSpectrum = new PlotSpectrum((wxFrame*) m_auiNbookCtrl, g_avmag,
                                           FDMDV_NSPEC*((float)MAX_F_HZ/FDMDV_MAX_F_HZ));
        m_panelSpectrum->SetToolTip(_("Left click to tune"));
        m_auiNbookCtrl->AddPage(m_panelSpectrum, _("Spectrum"), true, wxNullBitmap);
    }
    if(wxGetApp().m_show_scatter)
    {
        // Add Scatter Plot window
        m_panelScatter = new PlotScatter((wxFrame*) m_auiNbookCtrl);
        m_auiNbookCtrl->AddPage(m_panelScatter, _("Scatter"), true, wxNullBitmap);
    }
    if(wxGetApp().m_show_demod_in)
    {
        // Add Demod Input window
        m_panelDemodIn = new PlotScalar((wxFrame*) m_auiNbookCtrl, 1, WAVEFORM_PLOT_TIME, 1.0/WAVEFORM_PLOT_FS, -1, 1, 1, 0.2, "%2.1f", 0);
        m_auiNbookCtrl->AddPage(m_panelDemodIn, _("Frm Radio"), true, wxNullBitmap);
        g_plotDemodInFifo = fifo_create(2*WAVEFORM_PLOT_BUF);
    }

    if(wxGetApp().m_show_speech_in)
    {
        // Add Speech Input window
        m_panelSpeechIn = new PlotScalar((wxFrame*) m_auiNbookCtrl, 1, WAVEFORM_PLOT_TIME, 1.0/WAVEFORM_PLOT_FS, -1, 1, 1, 0.2, "%2.1f", 0);
        m_auiNbookCtrl->AddPage(m_panelSpeechIn, _("Frm Mic"), true, wxNullBitmap);
        g_plotSpeechInFifo = fifo_create(2*WAVEFORM_PLOT_BUF);
    }

    if(wxGetApp().m_show_speech_out)
    {
        // Add Speech Output window
        m_panelSpeechOut = new PlotScalar((wxFrame*) m_auiNbookCtrl, 1, WAVEFORM_PLOT_TIME, 1.0/WAVEFORM_PLOT_FS, -1, 1, 1, 0.2, "%2.1f", 0);
        m_auiNbookCtrl->AddPage(m_panelSpeechOut, _("To Spkr/Hdphns"), true, wxNullBitmap);
        g_plotSpeechOutFifo = fifo_create(2*WAVEFORM_PLOT_BUF);
    }

    if(wxGetApp().m_show_timing)
    {
        // Add Timing Offset window
        m_panelTimeOffset = new PlotScalar((wxFrame*) m_auiNbookCtrl, 1, 5.0, DT, -0.5, 0.5, 1, 0.1, "%2.1f", 0);
        m_auiNbookCtrl->AddPage(m_panelTimeOffset, L"Timing \u0394", true, wxNullBitmap);
    }
    if(wxGetApp().m_show_freq)
    {
        // Add Frequency Offset window
        m_panelFreqOffset = new PlotScalar((wxFrame*) m_auiNbookCtrl, 1, 5.0, DT, -200, 200, 1, 50, "%3.0fHz", 0);
        m_auiNbookCtrl->AddPage(m_panelFreqOffset, L"Frequency \u0394", true, wxNullBitmap);
    }

    if(wxGetApp().m_show_test_frame_errors)
    {
        // Add Test Frame Errors window
        m_panelTestFrameErrors = new PlotScalar((wxFrame*) m_auiNbookCtrl, 2*FDMDV_NC_MAX, 30.0, DT, 0, 2*FDMDV_NC_MAX+2, 1, 1, "", 1);
        m_auiNbookCtrl->AddPage(m_panelTestFrameErrors, L"Test Frame Errors", true, wxNullBitmap);
    }

    wxGetApp().m_framesPerBuffer = pConfig->Read(wxT("/Audio/framesPerBuffer"), PA_FPB);

    g_soundCard1InDeviceNum  = pConfig->Read(wxT("/Audio/soundCard1InDeviceNum"),         -1);
    g_soundCard1OutDeviceNum = pConfig->Read(wxT("/Audio/soundCard1OutDeviceNum"),        -1);
    g_soundCard1SampleRate   = pConfig->Read(wxT("/Audio/soundCard1SampleRate"),          -1);

    g_soundCard2InDeviceNum  = pConfig->Read(wxT("/Audio/soundCard2InDeviceNum"),         -1);
    g_soundCard2OutDeviceNum = pConfig->Read(wxT("/Audio/soundCard2OutDeviceNum"),        -1);
    g_soundCard2SampleRate   = pConfig->Read(wxT("/Audio/soundCard2SampleRate"),          -1);

    g_nSoundCards = 0;
    if ((g_soundCard1InDeviceNum > -1) && (g_soundCard1OutDeviceNum > -1)) {
        g_nSoundCards = 1;
        if ((g_soundCard2InDeviceNum > -1) && (g_soundCard2OutDeviceNum > -1))
            g_nSoundCards = 2;
    }

    wxGetApp().m_strRigCtrlPort     = pConfig->Read(wxT("/Rig/Port"),               wxT("COM3"));
    wxGetApp().m_strRigCtrlBaud     = pConfig->Read(wxT("/Rig/Baud"),               wxT("9600"));
    wxGetApp().m_strRigCtrlDatabits = pConfig->Read(wxT("/Rig/DataBits"),           wxT("8"));
    wxGetApp().m_strRigCtrlStopbits = pConfig->Read(wxT("/Rig/StopBits"),           wxT("1"));
    wxGetApp().m_strRigCtrlParity   = pConfig->Read(wxT("/Rig/Parity"),             wxT("n"));
    wxGetApp().m_boolUseTonePTT     = pConfig->ReadBool(wxT("/Rig/UseTonePTT"),     false);
    wxGetApp().m_boolHalfDuplex     = pConfig->ReadBool(wxT("/Rig/HalfDuplex"),     true);
    wxGetApp().m_boolUseSerialPTT   = pConfig->ReadBool(wxT("/Rig/UseSerialPTT"),   false);
    wxGetApp().m_boolUseRTS         = pConfig->ReadBool(wxT("/Rig/UseRTS"),         true);
    wxGetApp().m_boolRTSPos         = pConfig->ReadBool(wxT("/Rig/RTSPolarity"),    false);
    wxGetApp().m_boolUseDTR         = pConfig->ReadBool(wxT("/Rig/UseDTR"),         false);
    wxGetApp().m_boolDTRPos         = pConfig->ReadBool(wxT("/Rig/DTRPolarity"),    false);
    wxGetApp().m_playFileToMicInPath = pConfig->Read("/File/playFileToMicInPath",   wxT(""));
    wxGetApp().m_recFileFromRadioPath = pConfig->Read("/File/recFileFromRadioPath", wxT(""));
    wxGetApp().m_recFileFromRadioSecs = pConfig->Read("/File/recFileFromRadioSecs", 30);
    wxGetApp().m_playFileFromRadioPath = pConfig->Read("/File/playFileFromRadioPath", wxT(""));

    bool slow = false; // prevents compile error when using default bool
    wxGetApp().m_snrSlow = pConfig->Read("/Audio/snrSlow", slow);

    bool t = true;     // prevents compile error when using default bool
    wxGetApp().m_codec2LPCPostFilterEnable     = pConfig->Read(wxT("/Filter/codec2LPCPostFilterEnable"),    t);
    wxGetApp().m_codec2LPCPostFilterBassBoost  = pConfig->Read(wxT("/Filter/codec2LPCPostFilterBassBoost"), t);
    wxGetApp().m_codec2LPCPostFilterGamma      = (float)pConfig->Read(wxT("/Filter/codec2LPCPostFilterGamma"),     CODEC2_LPC_PF_GAMMA*100)/100.0;
    wxGetApp().m_codec2LPCPostFilterBeta       = (float)pConfig->Read(wxT("/Filter/codec2LPCPostFilterBeta"),      CODEC2_LPC_PF_BETA*100)/100.0;
    //printf("main(): m_codec2LPCPostFilterBeta: %f\n", wxGetApp().m_codec2LPCPostFilterBeta);

    wxGetApp().m_MicInBassFreqHz = (float)pConfig->Read(wxT("/Filter/MicInBassFreqHz"),    1);
    wxGetApp().m_MicInBassGaindB = (float)pConfig->Read(wxT("/Filter/MicInBassGaindB"),    (long)0)/10.0;
    wxGetApp().m_MicInTrebleFreqHz = (float)pConfig->Read(wxT("/Filter/MicInTrebleFreqHz"),    1);
    wxGetApp().m_MicInTrebleGaindB = (float)pConfig->Read(wxT("/Filter/MicInTrebleGaindB"),    (long)0)/10.0;
    wxGetApp().m_MicInMidFreqHz = (float)pConfig->Read(wxT("/Filter/MicInMidFreqHz"),    1);
    wxGetApp().m_MicInMidGaindB = (float)pConfig->Read(wxT("/Filter/MicInMidGaindB"),    (long)0)/10.0;
    wxGetApp().m_MicInMidQ = (float)pConfig->Read(wxT("/Filter/MicInMidQ"),              (long)100)/100.0;

    bool f = false;
    wxGetApp().m_MicInEQEnable = (float)pConfig->Read(wxT("/Filter/MicInEQEnable"), f);

    wxGetApp().m_SpkOutBassFreqHz = (float)pConfig->Read(wxT("/Filter/SpkOutBassFreqHz"),    1);
    wxGetApp().m_SpkOutBassGaindB = (float)pConfig->Read(wxT("/Filter/SpkOutBassGaindB"),    (long)0)/10.0;
    wxGetApp().m_SpkOutTrebleFreqHz = (float)pConfig->Read(wxT("/Filter/SpkOutTrebleFreqHz"),    1);
    wxGetApp().m_SpkOutTrebleGaindB = (float)pConfig->Read(wxT("/Filter/SpkOutTrebleGaindB"),    (long)0)/10.0;
    wxGetApp().m_SpkOutMidFreqHz = (float)pConfig->Read(wxT("/Filter/SpkOutMidFreqHz"),    1);
    wxGetApp().m_SpkOutMidGaindB = (float)pConfig->Read(wxT("/Filter/SpkOutMidGaindB"),    (long)0)/10.0;
    wxGetApp().m_SpkOutMidQ = (float)pConfig->Read(wxT("/Filter/SpkOutMidQ"),                (long)100)/100.0;

    wxGetApp().m_SpkOutEQEnable = (float)pConfig->Read(wxT("/Filter/SpkOutEQEnable"), f);

    wxGetApp().m_callSign = pConfig->Read("/Data/CallSign", wxT(""));

    pConfig->SetPath(wxT("/"));

//    this->Connect(m_menuItemHelpUpdates->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnHelpCheckUpdatesUI));
    //m_togRxID->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnRxIDUI), NULL, this);
    //m_togTxID->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTxIDUI), NULL, this);
    m_togBtnOnOff->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnOnOffUI), NULL, this);
    m_togBtnSplit->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnSplitClickUI), NULL, this);
    m_togBtnAnalog->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnAnalogClickUI), NULL, this);
    //m_togBtnALC->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnALCClickUI), NULL, this);
   // m_btnTogPTT->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnPTT_UI), NULL, this);

    m_togBtnSplit->Disable();
    //m_togRxID->Disable();
    //m_togTxID->Disable();
    m_togBtnAnalog->Disable();
    //m_togBtnALC->Disable();

    SetupSerialPort();

    // squelch settings
    char sqsnr[15];
    m_sliderSQ->SetValue((int)(g_SquelchLevel*2.0));
    sprintf(sqsnr, "%4.1f", g_SquelchLevel);
    wxString sqsnr_string(sqsnr);
    m_textSQ->SetLabel(sqsnr_string);
    m_ckboxSQ->SetValue(g_SquelchActive);

    // SNR settings

    m_ckboxSNR->SetValue(wxGetApp().m_snrSlow);
    setsnrBeta(wxGetApp().m_snrSlow);

#ifdef _USE_TIMER
    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);       // ID_MY_WINDOW);
    m_plotTimer.SetOwner(this, ID_TIMER_WATERFALL);
    //m_panelWaterfall->Refresh();
#endif

    m_RxRunning = false;

#ifdef _USE_ONIDLE
    Connect(wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle), NULL, this);
#endif //_USE_TIMER

    g_sfPlayFile = NULL;
    g_playFileToMicIn = false;
    g_loopPlayFileToMicIn = false;

    g_sfRecFile = NULL;
    g_recFileFromRadio = false;

    g_sfPlayFileFromRadio = NULL;
    g_playFileFromRadio = false;
    g_loopPlayFileFromRadio = false;

    // init click-tune states

    g_RxFreqOffsetHz = 0.0;
    g_RxFreqOffsetFreqRect.real = cos(g_RxFreqOffsetHz);
    g_RxFreqOffsetFreqRect.imag = sin(g_RxFreqOffsetHz);
    g_RxFreqOffsetPhaseRect.real = cos(0.0);
    g_RxFreqOffsetPhaseRect.imag = sin(0.0);
    m_panelWaterfall->setRxFreq(FDMDV_FCENTRE - g_RxFreqOffsetHz);
    m_panelSpectrum->setRxFreq(FDMDV_FCENTRE - g_RxFreqOffsetHz);

    g_TxFreqOffsetHz = 0.0;
    g_TxFreqOffsetFreqRect.real = cos(g_TxFreqOffsetHz);
    g_TxFreqOffsetFreqRect.imag = sin(g_TxFreqOffsetHz);
    g_TxFreqOffsetPhaseRect.real = cos(0.0);
    g_TxFreqOffsetPhaseRect.imag = sin(0.0);

    g_tx = 0;
    g_split = 0;

    // data states
    g_txDataInFifo = fifo_create(MAX_CALLSIGN*VARICODE_MAX_BITS);
    g_rxDataOutFifo = fifo_create(MAX_CALLSIGN*VARICODE_MAX_BITS);
    varicode_decode_init(&g_varicode_dec_states);

    sox_biquad_start();
    golay23_init();

    g_testFrames = 0;
    g_test_frame_sync_state = 0;
    g_total_bit_errors = 0;
    g_total_bits = 0;
}

//-------------------------------------------------------------------------
// ~MainFrame()
//-------------------------------------------------------------------------
MainFrame::~MainFrame()
{
    int x;
    int y;
    int w;
    int h;

    //MainApp *pApp = wxGetApp();
    wxConfigBase *pConfig = wxConfigBase::Get();
    if(pConfig)
    {
        GetClientSize(&w, &h);
        GetPosition(&x, &y);
        //wxLogDebug("x = %d y = %d w = %d h = %d\n", x,y,w,h);
        pConfig->Write(wxT("/MainFrame/top"),               (long) x);
        pConfig->Write(wxT("/MainFrame/left"),              (long) y);
        pConfig->Write(wxT("/MainFrame/width"),             (long) w);
        pConfig->Write(wxT("/MainFrame/height"),            (long) h);
        pConfig->Write(wxT("/MainFrame/show_wf"),           wxGetApp().m_show_wf);
        pConfig->Write(wxT("/MainFrame/show_spect"),        wxGetApp().m_show_spect);
        pConfig->Write(wxT("/MainFrame/show_scatter"),      wxGetApp().m_show_scatter);
        pConfig->Write(wxT("/MainFrame/show_timing"),       wxGetApp().m_show_timing);
        pConfig->Write(wxT("/MainFrame/show_freq"),         wxGetApp().m_show_freq);
        pConfig->Write(wxT("/MainFrame/show_speech_in"),    wxGetApp().m_show_speech_in);
        pConfig->Write(wxT("/MainFrame/show_speech_out"),   wxGetApp().m_show_speech_out);
        pConfig->Write(wxT("/MainFrame/show_demod_in"),     wxGetApp().m_show_demod_in);
        pConfig->Write(wxT("/MainFrame/show_test_frame_errors"), wxGetApp().m_show_test_frame_errors);

        pConfig->Write(wxT("/MainFrame/rxNbookCtrl"), wxGetApp().m_rxNbookCtrl);

        pConfig->Write(wxT("/Audio/SquelchActive"),         g_SquelchActive);
        pConfig->Write(wxT("/Audio/SquelchLevel"),          (int)(g_SquelchLevel*2.0));

        pConfig->Write(wxT("/Audio/framesPerBuffer"),       wxGetApp().m_framesPerBuffer);

        pConfig->Write(wxT("/Audio/soundCard1InDeviceNum"),   g_soundCard1InDeviceNum);
        pConfig->Write(wxT("/Audio/soundCard1OutDeviceNum"),  g_soundCard1OutDeviceNum);
        pConfig->Write(wxT("/Audio/soundCard1SampleRate"),    g_soundCard1SampleRate );

        pConfig->Write(wxT("/Audio/soundCard2InDeviceNum"),   g_soundCard2InDeviceNum);
        pConfig->Write(wxT("/Audio/soundCard2OutDeviceNum"),  g_soundCard2OutDeviceNum);
        pConfig->Write(wxT("/Audio/soundCard2SampleRate"),    g_soundCard2SampleRate );

        pConfig->Write(wxT("/Rig/Port"),                    wxGetApp().m_strRigCtrlPort);
        pConfig->Write(wxT("/Rig/Baud"),                    wxGetApp().m_strRigCtrlBaud);
        pConfig->Write(wxT("/Rig/DataBits"),                wxGetApp().m_strRigCtrlDatabits);
        pConfig->Write(wxT("/Rig/StopBits"),                wxGetApp().m_strRigCtrlStopbits);
        pConfig->Write(wxT("/Rig/Parity"),                  wxGetApp().m_strRigCtrlParity);
        pConfig->Write(wxT("/Rig/UseRTS"),                  wxGetApp().m_boolUseRTS);
        pConfig->Write(wxT("/Rig/RTSPolarity"),             wxGetApp().m_boolRTSPos);
        pConfig->Write(wxT("/Rig/UseDTR"),                  wxGetApp().m_boolUseDTR);
        pConfig->Write(wxT("/Rig/DTRPolarity"),             wxGetApp().m_boolDTRPos);
        pConfig->Write(wxT("/Rig/UseTonePTT"),              wxGetApp().m_boolUseTonePTT);
        pConfig->Write(wxT("/Rig/UseSerialPTT"),            wxGetApp().m_boolUseSerialPTT);
        pConfig->Write(wxT("/Rig/HalfDuplex"),              wxGetApp().m_boolHalfDuplex);

        pConfig->Write(wxT("/File/playFileToMicInPath"),    wxGetApp().m_playFileToMicInPath);
        pConfig->Write(wxT("/File/recFileFromRadioPath"),   wxGetApp().m_recFileFromRadioPath);
        pConfig->Write(wxT("/File/recFileFromRadioSecs"),   wxGetApp().m_recFileFromRadioSecs);
        pConfig->Write(wxT("/File/playFileFromRadioPath"),  wxGetApp().m_playFileFromRadioPath);

        pConfig->Write(wxT("/Audio/snrSlow"), wxGetApp().m_snrSlow);

        pConfig->Write(wxT("/Data/CallSign"), wxGetApp().m_callSign);

        pConfig->Write(wxT("/Filter/MicInEQEnable"), wxGetApp().m_MicInEQEnable);
        pConfig->Write(wxT("/Filter/SpkOutEQEnable"), wxGetApp().m_SpkOutEQEnable);
    }

    //m_togRxID->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnRxIDUI), NULL, this);
    //m_togTxID->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTxIDUI), NULL, this);
    m_togBtnOnOff->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnOnOffUI), NULL, this);
    m_togBtnSplit->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnSplitClickUI), NULL, this);
    m_togBtnAnalog->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnAnalogClickUI), NULL, this);
    //m_togBtnALC->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnALCClickUI), NULL, this);
    //m_btnTogPTT->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnPTT_UI), NULL, this);

    CloseSerialPort();
    sox_biquad_finish();

    if (m_RxRunning)
    {
        stopRxStream();
    }
    if (g_sfPlayFile != NULL)
    {
        sf_close(g_sfPlayFile);
        g_sfPlayFile = NULL;
    }
    if (g_sfRecFile != NULL)
    {
        sf_close(g_sfRecFile);
        g_sfRecFile = NULL;
    }
#ifdef _USE_TIMER
    if(m_plotTimer.IsRunning())
    {
        m_plotTimer.Stop();
        Unbind(wxEVT_TIMER, &MainFrame::OnTimer, this);
    }
#endif //_USE_TIMER

#ifdef _USE_ONIDLE
    Disconnect(wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle), NULL, this);
#endif // _USE_ONIDLE

    delete wxConfigBase::Set((wxConfigBase *) NULL);
}

#ifdef _USE_TIMER
//----------------------------------------------------------------
// OnTimer()
//
// when the timer fires every DT seconds we update the GUI displays.
// the tabs only the plot that is visible actually gets updated, this
// keeps CPU load reasonable
//----------------------------------------------------------------
void MainFrame::OnTimer(wxTimerEvent &evt)
{
    if (m_panelWaterfall->checkDT()) {
        m_panelWaterfall->setRxFreq(FDMDV_FCENTRE - g_RxFreqOffsetHz);
        m_panelWaterfall->m_newdata = true;
        m_panelWaterfall->Refresh();
    }

    m_panelSpectrum->setRxFreq(FDMDV_FCENTRE - g_RxFreqOffsetHz);
    m_panelSpectrum->m_newdata = true;
    m_panelSpectrum->Refresh();

    m_panelScatter->add_new_samples(g_stats.rx_symbols);
    m_panelScatter->Refresh();

    // Oscilliscope type speech plots -------------------------------------------------------

    short speechInPlotSamples[WAVEFORM_PLOT_BUF];
    if (fifo_read(g_plotSpeechInFifo, speechInPlotSamples, WAVEFORM_PLOT_BUF))
        memset(speechInPlotSamples, 0, WAVEFORM_PLOT_BUF*sizeof(short));
    m_panelSpeechIn->add_new_short_samples(0, speechInPlotSamples, WAVEFORM_PLOT_BUF, 32767);
    m_panelSpeechIn->Refresh();

    short speechOutPlotSamples[WAVEFORM_PLOT_BUF];
    if (fifo_read(g_plotSpeechOutFifo, speechOutPlotSamples, WAVEFORM_PLOT_BUF))
        memset(speechOutPlotSamples, 0, WAVEFORM_PLOT_BUF*sizeof(short));
    m_panelSpeechOut->add_new_short_samples(0, speechOutPlotSamples, WAVEFORM_PLOT_BUF, 32767);
    m_panelSpeechOut->Refresh();

    short demodInPlotSamples[WAVEFORM_PLOT_BUF];
    if (fifo_read(g_plotDemodInFifo, demodInPlotSamples, WAVEFORM_PLOT_BUF))
        memset(demodInPlotSamples, 0, WAVEFORM_PLOT_BUF*sizeof(short));
    m_panelDemodIn->add_new_short_samples(0,demodInPlotSamples, WAVEFORM_PLOT_BUF, 32767);
    m_panelDemodIn->Refresh();

    // Demod states -----------------------------------------------------------------------

    m_panelTimeOffset->add_new_sample(0, (float)g_stats.rx_timing/FDMDV_NOM_SAMPLES_PER_FRAME);
    m_panelTimeOffset->Refresh();

    m_panelFreqOffset->add_new_sample(0, g_stats.foff);
    m_panelFreqOffset->Refresh();

    // SNR text box and gauge ------------------------------------------------------------

    // LP filter g_stats.snr_est some more to stabilise the
    // display. g_stats.snr_est already has some low pass filtering
    // but we need it fairly fast to activate squelch.  So we
    // optionally perform some further filtering for the display
    // version of SNR.  The "Slow" checkbox controls the amount of
    // filtering.  The filtered snr also controls the squelch

    g_snr = m_snrBeta*g_snr + (1.0 - m_snrBeta)*g_stats.snr_est;
    float snr_limited = g_snr;

    if (snr_limited < -9.0) snr_limited = -9.0; // stop text box overflow
    char snr[15];
    if (wxGetApp().m_snrSlow)
        sprintf(snr, "%4.1f", snr_limited);
    else
        sprintf(snr, "%d", (int)(snr_limited+0.5)); // round to nearest dB
    wxString snr_string(snr);
    m_textSNR->SetLabel(snr_string);

    if (snr_limited < 0.0) snr_limited = 0;
    if (snr_limited > 20.0) snr_limited = 20.0;
    m_gaugeSNR->SetValue((int)(snr_limited));

    // Level Gauge -----------------------------------------------------------------------

    float tooHighThresh;
    if (!g_tx && m_RxRunning)
    {
        // receive mode - display From Radio peaks

        // peak from this DT sampling period
        int maxDemodIn = 0;
        for(int i=0; i<WAVEFORM_PLOT_BUF; i++)
            if (maxDemodIn < abs(demodInPlotSamples[i]))
                maxDemodIn = abs(demodInPlotSamples[i]);

        // peak from last second
        if (maxDemodIn > m_maxLevel)
            m_maxLevel = maxDemodIn;

        tooHighThresh = FROM_RADIO_MAX;
    }
    else
    {
        // transmit mode - display From Mic peaks

        // peak from this DT sampling period
        int maxSpeechIn = 0;
        for(int i=0; i<WAVEFORM_PLOT_BUF; i++)
            if (maxSpeechIn < abs(speechInPlotSamples[i]))
                maxSpeechIn = abs(speechInPlotSamples[i]);

        // peak from last second
        if (maxSpeechIn > m_maxLevel)
            m_maxLevel = maxSpeechIn;

       tooHighThresh = FROM_MIC_MAX;
    }

    // Peak Reading meter: updates peaks immediately, then slowly decays
    int maxScaled = (int)(100.0 * ((float)m_maxLevel/32767.0));
    m_gaugeLevel->SetValue(maxScaled);
    if (((float)maxScaled/100) > tooHighThresh)
        m_textLevel->SetLabel("Too High");
    else
        m_textLevel->SetLabel("");

    m_maxLevel *= LEVEL_BETA;

    // sync LED (Colours don't work on Windows) ------------------------

    if (g_State) {
        m_rbSync->SetForegroundColour( wxColour( 0, 255, 0 ) ); // green
        m_rbSync->SetValue(true);
    }
    else {
        m_rbSync->SetForegroundColour( wxColour( 255, 0, 0 ) ); // red
        m_rbSync->SetValue(false);
    }

    // send Callsign ----------------------------------------------------

    if (fifo_used(g_txDataInFifo) == 0) {
        char callsign[MAX_CALLSIGN];
        strncpy(callsign, (const char*) wxGetApp().m_callSign.mb_str(wxConvUTF8), MAX_CALLSIGN-1);
        char callsigncr[MAX_CALLSIGN+1];
        strcpy(callsigncr, callsign);
        callsigncr[strlen(callsign)] = 13;

        // varicode encode and write to tx data fifo

        short varicode[MAX_CALLSIGN*VARICODE_MAX_BITS];
        int nout = varicode_encode(varicode, callsigncr, MAX_CALLSIGN*VARICODE_MAX_BITS, strlen(callsign)+1);
        fifo_write(g_txDataInFifo, varicode, nout);
        //printf("Callsign sending: %s nout: %d\n", callsign, nout);
    }

    // See if any Callsign info received --------------------------------

    short ashort;
    while (fifo_read(g_rxDataOutFifo, &ashort, 1) == 0) {
        if ((ashort == 13) || ((m_pcallsign - m_callsign) > MAX_CALLSIGN-1)) {
            // CR completes line
            *m_pcallsign = 0;
            m_pcallsign = m_callsign;
        }
        else
        {
            *m_pcallsign++ = (char)ashort;
            wxString s;
            s.Printf("%s", m_callsign);
            m_txtCtrlCallSign->SetValue(s);
        }
    }

    // Run time update of EQ filters -----------------------------------
    if (m_newMicInFilter || m_newSpkOutFilter) {
        g_mutexProtectingCallbackData.Lock();
        deleteEQFilters(g_rxUserdata);
        designEQFilters(g_rxUserdata);
        g_mutexProtectingCallbackData.Unlock();
        m_newMicInFilter = m_newSpkOutFilter = false;
    }
    g_rxUserdata->micInEQEnable = wxGetApp().m_MicInEQEnable;
    g_rxUserdata->spkOutEQEnable = wxGetApp().m_SpkOutEQEnable;

    // Test Frame Bit Error Updates ------------------------------------

    // Toggle test frame mode at run time

    if (!g_testFrames && m_ckboxTestFrame->GetValue()) {

        // reset stats on check box off to on transition

        g_test_frame_sync_state = 0;
        g_total_bits = 0;
        g_total_bit_errors = 0;
    }
    g_testFrames = m_ckboxTestFrame->GetValue();

    if (g_State) {
        char bits[80], errors[80], ber[80];

        // update stats on main page

        sprintf(bits, "Bits...: %d", g_total_bits); wxString bits_string(bits); m_textBits->SetLabel(bits_string);
        sprintf(errors, "Errors: %d", g_total_bit_errors); wxString errors_string(errors); m_textErrors->SetLabel(errors_string);
        float b = (float)g_total_bit_errors/(1E-6+g_total_bits);
        sprintf(ber, "BER...: %4.3f", b); wxString ber_string(ber); m_textBER->SetLabel(ber_string);

        // update error plots

        short *error_pattern = new short[g_sz_error_pattern];

        if (fifo_read(g_errorFifo, error_pattern, g_sz_error_pattern) == 0) {
            int i,b;
            for(b=0; b<g_Nc*2; b++) {
                for(i=b; i<g_sz_error_pattern; i+= 2*g_Nc)
                    m_panelTestFrameErrors->add_new_sample(b, b + 0.8*error_pattern[i]);
            }

            m_panelTestFrameErrors->Refresh();
       }


        delete error_pattern;
    }

}

#endif

#ifdef _USE_ONIDLE
//----------------------------------------------------------------
// OnIdle()
//----------------------------------------------------------------
void MainFrame::OnIdle(wxIdleEvent& event)
{
}
#endif // _USE_TIMER

//-------------------------------------------------------------------------
// OnCloseFrame()
//-------------------------------------------------------------------------
void MainFrame::OnCloseFrame(wxCloseEvent& event)
{
    Pa_Terminate();
    Destroy();
}

//-------------------------------------------------------------------------
// OnTop()
//-------------------------------------------------------------------------
void MainFrame::OnTop(wxCommandEvent& event)
{
    int style = GetWindowStyle();

    if (style & wxSTAY_ON_TOP)
    {
        style &= ~wxSTAY_ON_TOP;
    }
    else
    {
        style |= wxSTAY_ON_TOP;
    }
    SetWindowStyle(style);
}

//-------------------------------------------------------------------------
// OnDeleteConfig()
//-------------------------------------------------------------------------
void MainFrame::OnDeleteConfig(wxCommandEvent&)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    if(pConfig->DeleteAll())
    {
        wxLogMessage(wxT("Config file/registry key successfully deleted."));

        delete wxConfigBase::Set(NULL);
        wxConfigBase::DontCreateOnDemand();
    }
    else
    {
        wxLogError(wxT("Deleting config file/registry key failed."));
    }
}

//-------------------------------------------------------------------------
// Paint()
//-------------------------------------------------------------------------
void MainFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    if(GetMenuBar()->IsChecked(ID_PAINT_BG))
    {
        dc.Clear();
    }
    dc.SetUserScale(m_zoom, m_zoom);
}

//-------------------------------------------------------------------------
// OnCmdSliderScroll()
//-------------------------------------------------------------------------
void MainFrame::OnCmdSliderScroll(wxScrollEvent& event)
{
    char sqsnr[15];
    g_SquelchLevel = (float)m_sliderSQ->GetValue()/2.0;
    sprintf(sqsnr, "%4.1f", g_SquelchLevel); // 0.5 dB steps
    wxString sqsnr_string(sqsnr);
    m_textSQ->SetLabel(sqsnr_string);

    event.Skip();
}

//-------------------------------------------------------------------------
// OnCheckSQClick()
//-------------------------------------------------------------------------
void MainFrame::OnCheckSQClick(wxCommandEvent& event)
{
    if(!g_SquelchActive)
    {
        g_SquelchActive = true;
    }
    else
    {
        g_SquelchActive = false;
    }
}

void MainFrame::setsnrBeta(bool snrSlow)
{
    if(snrSlow)
    {
        m_snrBeta = 0.9; // make this closer to 1.0 to smooth SNR est further
    }
    else
    {
        m_snrBeta = 0.0; // no smoothing of SNR estimate from demodulator
    }
}

//-------------------------------------------------------------------------
// OnCheckSQClick()
//-------------------------------------------------------------------------
void MainFrame::OnCheckSNRClick(wxCommandEvent& event)
{
    wxGetApp().m_snrSlow = m_ckboxSNR->GetValue();
    setsnrBeta(wxGetApp().m_snrSlow);
    //printf("m_snrSlow: %d\n", (int)wxGetApp().m_snrSlow);
}

//-------------------------------------------------------------------------
// OnTogBtnPTT ()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnPTT (wxCommandEvent& event)
{
    if (g_tx)
    {
        // tx-> rx transition, swap to the page we were on for last rx
        m_auiNbookCtrl->ChangeSelection(wxGetApp().m_rxNbookCtrl);
    }
    else
    {
        // rx-> tx transition, swap to Mic In page to monitor speech
        wxGetApp().m_rxNbookCtrl = m_auiNbookCtrl->GetSelection();
        m_auiNbookCtrl->ChangeSelection(m_auiNbookCtrl->GetPageIndex((wxWindow *)m_panelSpeechIn));
    }
    g_tx = m_btnTogPTT->GetValue();
    // Tortured and tortuous logic, it seems to me...
    if(wxGetApp().m_boolUseSerialPTT && m_serialPort != NULL)
    {
        if(event.IsChecked())
        {
            if(wxGetApp().m_boolUseRTS)     // Use RTS
            {
                if(wxGetApp().m_boolRTSPos) // RTS asserted HIGH
                {
                    m_serialPort->SetLineState(ctb::LinestateRts);
                }
                else                        // RTS asserted LOW
                {
                    m_serialPort->ClrLineState(ctb::LinestateRts);
                }
            }
            else                            // Use DTR
            {
                if(wxGetApp().m_boolDTRPos) // DTR asserted HIGH
                {
                    m_serialPort->SetLineState(ctb::LinestateDtr);
                }
                else                        // DTR asserted LOW
                {
                    m_serialPort->ClrLineState(ctb::LinestateDtr);
                }
            }
        }
        else  // !isChecked() - so Clear
        {
            if(wxGetApp().m_boolUseRTS)     // Use RTS
            {
                if(wxGetApp().m_boolRTSPos) // RTS cleared LOW
                {
                    m_serialPort->ClrLineState(ctb::LinestateRts);
                }
                else                        // RTS cleared HIGH
                {
                    m_serialPort->SetLineState(ctb::LinestateRts);
                }
            }
            else                            // Use DTR
            {
                if(wxGetApp().m_boolDTRPos) // DTR cleared LOW
                {
                    m_serialPort->ClrLineState(ctb::LinestateDtr);
                }
                else                        // DTR cleared HIGH
                {
                    m_serialPort->SetLineState(ctb::LinestateDtr);
                }
            }
        }
    }
    // reset level gauge
    m_maxLevel = 0;
    m_textLevel->SetLabel(wxT(""));
    m_gaugeLevel->SetValue(0);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnRxID()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnRxID(wxCommandEvent& event)
{
    // empty any junk in rx data FIFO
    short junk;
    while(fifo_read(g_rxDataOutFifo,&junk,1) == 0);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnTxID()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnTxID(wxCommandEvent& event)
{
    event.Skip();
}

void MainFrame::OnTogBtnSplitClick(wxCommandEvent& event) {
    if (g_split)
        g_split = 0;
    else
        g_split = 1;
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnAnalogClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnAnalogClick (wxCommandEvent& event)
{
    if (g_analog == 0)
        g_analog = 1;
    else
        g_analog = 0;

    event.Skip();
}

void MainFrame::OnCallSignReset(wxCommandEvent& event)
{
    m_pcallsign = m_callsign;
    memset(m_callsign, 0, MAX_CALLSIGN);
    wxString s;
    s.Printf("%s", m_callsign);
    m_txtCtrlCallSign->SetValue(s);
}

void MainFrame::OnBerReset(wxCommandEvent& event)
{
    g_total_bits = 0;
    g_total_bit_errors = 0;
}

#ifdef ALC
//-------------------------------------------------------------------------
// OnTogBtnALCClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnALCClick(wxCommandEvent& event)
{
    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnALCClick"), wxOK);

    event.Skip();
}
#endif

// extra panel added to file open dialog to add loop checkbox
MyExtraPlayFilePanel::MyExtraPlayFilePanel(wxWindow *parent): wxPanel(parent)
{
    m_cb = new wxCheckBox(this, -1, wxT("Loop"));
    m_cb->SetToolTip(_("When checked file will repeat forever"));
    m_cb->SetValue(g_loopPlayFileToMicIn);

    // bug: I can't this to align right.....
    wxBoxSizer *sizerTop = new wxBoxSizer(wxHORIZONTAL);
    sizerTop->Add(m_cb, 0, wxALIGN_RIGHT, 0);
    SetSizerAndFit(sizerTop);
}

static wxWindow* createMyExtraPlayFilePanel(wxWindow *parent)
{
    return new MyExtraPlayFilePanel(parent);
}

//-------------------------------------------------------------------------
// OnPlayFileToMicIn()
//-------------------------------------------------------------------------
void MainFrame::OnPlayFileToMicIn(wxCommandEvent& event)
{
    wxUnusedVar(event);

    if(g_playFileToMicIn)
    {
        g_mutexProtectingCallbackData.Lock();
        g_playFileToMicIn = false;
        sf_close(g_sfPlayFile);
        SetStatusText(wxT(""));
        g_mutexProtectingCallbackData.Unlock();
    }
    else
    {
        wxString    soundFile;
        SF_INFO     sfInfo;

        wxFileDialog openFileDialog(
                                    this,
                                    wxT("Play File to Mic In"),
                                    wxGetApp().m_playFileToMicInPath,
                                    wxEmptyString,
                                    wxT("WAV and RAW files (*.wav;*.raw)|*.wav;*.raw|")
                                    wxT("All files (*.*)|*.*"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST
                                    );

        // add the loop check box
        openFileDialog.SetExtraControlCreator(&createMyExtraPlayFilePanel);

        if(openFileDialog.ShowModal() == wxID_CANCEL)
        {
            return;     // the user changed their mind...
        }

        wxString fileName, extension;
        soundFile = openFileDialog.GetPath();
        wxFileName::SplitPath(soundFile, &wxGetApp().m_playFileToMicInPath, &fileName, &extension);
        //wxLogDebug("m_playFileToMicInPath: %s", wxGetApp().m_playFileToMicInPath);
        sfInfo.format = 0;

        if(!extension.IsEmpty())
        {
            extension.LowerCase();
            if(extension == wxT("raw"))
            {
                sfInfo.format     = SF_FORMAT_RAW | SF_FORMAT_PCM_16;
                sfInfo.channels   = 1;
                sfInfo.samplerate = FS;
            }
        }
        g_sfPlayFile = sf_open(soundFile, SFM_READ, &sfInfo);
        if(g_sfPlayFile == NULL)
        {
            wxString strErr = sf_strerror(NULL);
            wxMessageBox(strErr, wxT("Couldn't open sound file"), wxOK);
            return;
        }

        wxWindow * const ctrl = openFileDialog.GetExtraControl();

        // Huh?! I just copied wxWidgets-2.9.4/samples/dialogs ....
        g_loopPlayFileToMicIn = static_cast<MyExtraPlayFilePanel*>(ctrl)->getLoopPlayFileToMicIn();

        SetStatusText(wxT("Playing File: ") + fileName + wxT(" to Mic Input") , 0);
        g_playFileToMicIn = true;
    }
}

//-------------------------------------------------------------------------
// OnPlayFileFromRadio()
// This puppy "plays" a recorded file into the denmonulator input, allowing us
// to replay off air signals.
//-------------------------------------------------------------------------
void MainFrame::OnPlayFileFromRadio(wxCommandEvent& event)
{
    wxUnusedVar(event);

    printf("OnPlayFileFromRadio:: %d\n", (int)g_playFileFromRadio);
    if (g_playFileFromRadio)
    {
        printf("OnPlayFileFromRadio:: Stop\n");
        g_mutexProtectingCallbackData.Lock();
        g_playFileFromRadio = false;
        sf_close(g_sfPlayFileFromRadio);
        SetStatusText(wxT(""));
        g_mutexProtectingCallbackData.Unlock();
    }
    else
    {
        wxString    soundFile;
        SF_INFO     sfInfo;

        wxFileDialog openFileDialog(
                                    this,
                                    wxT("Play File - From Radio"),
                                    wxGetApp().m_playFileFromRadioPath,
                                    wxEmptyString,
                                    wxT("WAV and RAW files (*.wav;*.raw)|*.wav;*.raw|")
                                    wxT("All files (*.*)|*.*"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST
                                    );

        // add the loop check box
        openFileDialog.SetExtraControlCreator(&createMyExtraPlayFilePanel);

        if(openFileDialog.ShowModal() == wxID_CANCEL)
        {
            return;     // the user changed their mind...
        }

        wxString fileName, extension;
        soundFile = openFileDialog.GetPath();
        wxFileName::SplitPath(soundFile, &wxGetApp().m_playFileFromRadioPath, &fileName, &extension);
        //wxLogDebug("m_playFileToFromRadioPath: %s", wxGetApp().m_playFileFromRadioPath);
        sfInfo.format = 0;

        if(!extension.IsEmpty())
        {
            extension.LowerCase();
            if(extension == wxT("raw"))
            {
                sfInfo.format     = SF_FORMAT_RAW | SF_FORMAT_PCM_16;
                sfInfo.channels   = 1;
                sfInfo.samplerate = FS;
            }
        }
        g_sfPlayFileFromRadio = sf_open(soundFile, SFM_READ, &sfInfo);
        if(g_sfPlayFileFromRadio == NULL)
        {
            wxString strErr = sf_strerror(NULL);
            wxMessageBox(strErr, wxT("Couldn't open sound file"), wxOK);
            return;
        }

        wxWindow * const ctrl = openFileDialog.GetExtraControl();

        // Huh?! I just copied wxWidgets-2.9.4/samples/dialogs ....
        g_loopPlayFileFromRadio = static_cast<MyExtraPlayFilePanel*>(ctrl)->getLoopPlayFileToMicIn();

        SetStatusText(wxT("Playing File: ") + fileName + wxT(" into From Radio") , 0);
        g_playFileFromRadio = true;
    }
}

// extra panel added to file save dialog to set number of seconds to record for

MyExtraRecFilePanel::MyExtraRecFilePanel(wxWindow *parent): wxPanel(parent)
{
    wxBoxSizer *sizerTop = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, _("Seconds:"), wxDefaultPosition, wxDefaultSize, 0);
    sizerTop->Add(staticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_secondsToRecord = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_secondsToRecord->SetToolTip(_("Number of seconds to record for"));
    m_secondsToRecord->SetValue(wxString::Format(wxT("%i"), wxGetApp().m_recFileFromRadioSecs));
    sizerTop->Add(m_secondsToRecord, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    SetSizerAndFit(sizerTop);
}

static wxWindow* createMyExtraRecFilePanel(wxWindow *parent)
{
    return new MyExtraRecFilePanel(parent);
}

//-------------------------------------------------------------------------
// OnRecFileFromRadio()
//-------------------------------------------------------------------------
void MainFrame::OnRecFileFromRadio(wxCommandEvent& event)
{
    wxUnusedVar(event);

    if (g_recFileFromRadio) {
        printf("Stopping Record....\n");
        g_mutexProtectingCallbackData.Lock();
        g_recFileFromRadio = false;
        sf_close(g_sfRecFile);
        SetStatusText(wxT(""));
        g_mutexProtectingCallbackData.Unlock();
    }
    else {

        wxString    soundFile;
        SF_INFO     sfInfo;

         wxFileDialog openFileDialog(
                                    this,
                                    wxT("Record File From Radio"),
                                    wxGetApp().m_recFileFromRadioPath,
                                    wxEmptyString,
                                    wxT("WAV and RAW files (*.wav;*.raw)|*.wav;*.raw|")
                                    wxT("All files (*.*)|*.*"),
                                    wxFD_SAVE
                                    );

        // add the loop check box
        openFileDialog.SetExtraControlCreator(&createMyExtraRecFilePanel);

        if(openFileDialog.ShowModal() == wxID_CANCEL)
        {
            return;     // the user changed their mind...
        }

        wxString fileName, extension;
        soundFile = openFileDialog.GetPath();
        wxFileName::SplitPath(soundFile, &wxGetApp().m_recFileFromRadioPath, &fileName, &extension);
        wxLogDebug("m_recFileFromRadioPath: %s", wxGetApp().m_recFileFromRadioPath);
        wxLogDebug("soundFile: %s", soundFile);
        sfInfo.format = 0;

        if(!extension.IsEmpty())
        {
            extension.LowerCase();
            if(extension == wxT("raw"))
            {
                sfInfo.format     = SF_FORMAT_RAW | SF_FORMAT_PCM_16;
                sfInfo.channels   = 1;
                sfInfo.samplerate = FS;
            }
            else if(extension == wxT("wav"))
            {
                sfInfo.format     = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
                sfInfo.channels   = 1;
                sfInfo.samplerate = FS;
            } else {
                wxMessageBox(wxT("Invalid file format"), wxT("Record File From Radio"), wxOK);
                return;
            }
        }
        else {
            wxMessageBox(wxT("Invalid file format"), wxT("Record File From Radio"), wxOK);
            return;
        }

        // Bug: on Win32 I cant read m_recFileFromRadioSecs, so have hard coded it
#ifdef __WIN32__
        long secs = wxGetApp().m_recFileFromRadioSecs;
        g_recFromRadioSamples = FS*(unsigned int)secs;
#else
        // work out number of samples to record

        wxWindow * const ctrl = openFileDialog.GetExtraControl();
        wxString secsString = static_cast<MyExtraRecFilePanel*>(ctrl)->getSecondsToRecord();
        wxLogDebug("test: %s secsString: %s\n", wxT("testing 123"), secsString);

        long secs;
        if (secsString.ToLong(&secs)) {
            wxGetApp().m_recFileFromRadioSecs = (unsigned int)secs;
            //printf(" secondsToRecord: %d\n",  (unsigned int)secs);
            g_recFromRadioSamples = FS*(unsigned int)secs;
            //printf("g_recFromRadioSamples: %d\n", g_recFromRadioSamples);
        }
        else {
            wxMessageBox(wxT("Invalid number of Seconds"), wxT("Record File From Radio"), wxOK);
            return;
        }
#endif

        g_sfRecFile = sf_open(soundFile, SFM_WRITE, &sfInfo);
        if(g_sfRecFile == NULL)
        {
            wxString strErr = sf_strerror(NULL);
            wxMessageBox(strErr, wxT("Couldn't open sound file"), wxOK);
            return;
        }

        SetStatusText(wxT("Recording File: ") + fileName + wxT(" From Radio") , 0);
        g_recFileFromRadio = true;
    }

}

//-------------------------------------------------------------------------
// OnExit()
//-------------------------------------------------------------------------
void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
#ifdef _USE_TIMER
    m_plotTimer.Stop();
#endif // _USE_TIMER
    if(g_sfPlayFile != NULL)
    {
        sf_close(g_sfPlayFile);
        g_sfPlayFile = NULL;
    }
    if(g_sfRecFile != NULL)
    {
        sf_close(g_sfRecFile);
        g_sfRecFile = NULL;
    }
    if(m_RxRunning)
    {
        stopRxStream();
    }
    m_togBtnSplit->Disable();
    //m_togRxID->Disable();
    //m_togTxID->Disable();
    m_togBtnAnalog->Disable();
    //m_togBtnALC->Disable();
    //m_btnTogPTT->Disable();
    CloseSerialPort();
    Pa_Terminate();
    Destroy();
}

//-------------------------------------------------------------------------
// OnExitClick()
//-------------------------------------------------------------------------
void MainFrame::OnExitClick(wxCommandEvent& event)
{
    OnExit(event);
}

//-------------------------------------------------------------------------
// OnToolsAudio()
//-------------------------------------------------------------------------
void MainFrame::OnToolsAudio(wxCommandEvent& event)
{
    wxUnusedVar(event);
    int rv = 0;
    AudioOptsDialog *dlg = new AudioOptsDialog(NULL);
    rv = dlg->ShowModal();
    if(rv == wxID_OK)
    {
        dlg->ExchangeData(EXCHANGE_DATA_OUT);
    }
    delete dlg;
}

//-------------------------------------------------------------------------
// OnToolsFilter()
//-------------------------------------------------------------------------
void MainFrame::OnToolsFilter(wxCommandEvent& event)
{
    wxUnusedVar(event);
    FilterDlg *dlg = new FilterDlg(NULL, m_RxRunning, &m_newMicInFilter, &m_newSpkOutFilter);
    dlg->ShowModal();
    delete dlg;
}

//-------------------------------------------------------------------------
// OnToolsSetCallSign()
//-------------------------------------------------------------------------
void MainFrame::OnToolsSetCallSign(wxCommandEvent& event)
{
    wxUnusedVar(event);
    printf("MainFrame::OnToolSetCallSign\n");
    wxGetApp().m_callSign = wxGetTextFromUser(wxT("Enter Callsign"),
                                              wxT("Enter Callsign"),
                                              wxGetApp().m_callSign);
}

//-------------------------------------------------------------------------
// OnToolsAudioUI()
//-------------------------------------------------------------------------
void MainFrame::OnToolsAudioUI(wxUpdateUIEvent& event)
{
    event.Enable(!m_RxRunning);
}

//-------------------------------------------------------------------------
// OnToolsComCfg()
//-------------------------------------------------------------------------
void MainFrame::OnToolsComCfg(wxCommandEvent& event)
{
    wxUnusedVar(event);
    int rv = 0;
    int  iLineState  = 0;
    //bool bPTTEnabled = m_btnTogPTT->IsEnabled();
    //bool bPTTState   = m_btnTogPTT->GetValue();

    if(m_serialPort != NULL)
    {
        int iLineState   = m_serialPort->GetLineState();
        CloseSerialPort();
    }
    ComPortsDlg *dlg = new ComPortsDlg(NULL);
    rv = dlg->ShowModal();
    if(rv == wxID_OK)
    {
        dlg->ExchangeData(EXCHANGE_DATA_OUT);
        SetupSerialPort();
    }
    else if(rv == wxID_CANCEL)
    {
        SetupSerialPort();
        if(m_serialPort != NULL)
        {
            if(iLineState | ctb::LinestateRts)
            {
                m_serialPort->SetLineState(ctb::LinestateRts);
            }
            else
            {
                m_serialPort->ClrLineState(ctb::LinestateRts);
            }
            if(iLineState | ctb::LinestateDtr)
            {
                m_serialPort->SetLineState(ctb::LinestateDtr);
            }
            else
            {
                m_serialPort->ClrLineState(ctb::LinestateDtr);
            }
        // m_btnTogPTT->Enable(bPTTEnabled);
        // m_btnTogPTT->SetValue(bPTTState);
        }
    }
    delete dlg;
}

//-------------------------------------------------------------------------
// OnToolsComCfgUI()
//-------------------------------------------------------------------------
void MainFrame::OnToolsComCfgUI(wxUpdateUIEvent& event)
{
    event.Enable(!m_RxRunning);
}

//-------------------------------------------------------------------------
// OnHelpCheckUpdates()
//-------------------------------------------------------------------------
void MainFrame::OnHelpCheckUpdates(wxCommandEvent& event)
{
    wxMessageBox("Got Click!", "OnHelpCheckUpdates", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnHelpCheckUpdatesUI()
//-------------------------------------------------------------------------
void MainFrame::OnHelpCheckUpdatesUI(wxUpdateUIEvent& event)
{
    event.Enable(false);
}

//-------------------------------------------------------------------------
//OnHelpAbout()
//-------------------------------------------------------------------------
void MainFrame::OnHelpAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
#ifdef _USE_ABOUT_DIALOG
    int rv = 0;
    AboutDlg *dlg = new AboutDlg(NULL);
    rv = dlg->ShowModal();
    if(rv == wxID_OK)
    {
        dlg->ExchangeData(EXCHANGE_DATA_OUT);
    }
    delete dlg;
#else
    wxString svnLatestRev("Can't determine latest SVN revision.");

    // Try to determine current SVN revision from the Internet
    wxURL url(wxT("http://freetel.svn.sourceforge.net/svnroot/freetel/fdmdv2/"));

    if(url.GetError() == wxURL_NOERR)
    {
        wxString htmldata;
        wxInputStream *in = url.GetInputStream();

        if(in && in->IsOk())
        {
            //printf("In OK\n");
            wxStringOutputStream html_stream(&htmldata);
            in->Read(html_stream);
            //wxLogDebug(htmldata);

            wxString s("<h2>freetel - Revision ");
            int startIndex = htmldata.find(s) + s.Length();
            int endIndex = htmldata.find(wxT(": /fdmdv2</h2>"));
            svnLatestRev = wxT("Latest svn revision: ") + htmldata.SubString(startIndex, endIndex-1);
            //printf("startIndex: %d endIndex: %d\n", startIndex, endIndex);
       }
       delete in;
    }

    wxString msg;
    msg.Printf( wxT("FreeDV %s\n\n")
                wxT("Open Source Narrow Band Digital Voice over Radio\n\n")
                wxT("For Help and Support visit: http://freedv.org\n\n")
                wxT("GNU Public License V2.1\n\n")
                wxT("Copyright (c) David Witten KD0EAG and David Rowe VK5DGR\n\n")
                wxT("svn revision: %s\n") + svnLatestRev, FREEDV_VERSION, SVN_REVISION);

    wxMessageBox(msg, wxT("About"), wxOK | wxICON_INFORMATION, this);

#endif // _USE_ABOUT_DIALOG
#ifdef USE_SIMPLE_ABOUT_DIALOG
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("HAMLib Test"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Short description goes here"));
    ::wxAboutBox(info);
#endif // USE_SIMPLE_ABOUT_DIALOG

}

//bool wxLaunchDefaultBrowser(http:("http://freedv.org/");

//-------------------------------------------------------------------------
// OnTogBtnOnOff()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnOnOff(wxCommandEvent& event)
{
    wxString startStop = m_togBtnOnOff->GetLabel();

    // we are attempting to start

    if (startStop.IsSameAs("Start"))
    {
        //
        // Start Running -------------------------------------------------
        //

        // modify some button states when running

        m_togBtnSplit->Enable();
        //m_togRxID->Enable();
        //m_togTxID->Enable();
        m_togBtnAnalog->Enable();
/*
        if(m_serialPort != NULL)
        {
            m_btnTogPTT->Enable();
        }
*/
        m_togBtnOnOff->SetLabel(wxT("Stop"));

        m_rb1400old->Disable();
        m_rb1600->Disable();
#ifdef DISABLED_FEATURE
        m_rb1400->Disable();
        m_rb2000->Disable();
#endif
        // determine what mode we are using

        int codec2_mode;
        if (m_rb1400old->GetValue()) {
            g_mode = MODE_1400_V0_91;
            g_Nc = 14;
            codec2_mode = CODEC2_MODE_1400;
        }

        if (m_rb1600->GetValue()) {
            g_mode = MODE_1600;
            g_Nc = 16;
            codec2_mode = CODEC2_MODE_1300;
        }
        if (m_rb1600Wide->GetValue()) {
            g_mode = MODE_1600_WIDE;
            g_Nc = 16;
            codec2_mode = CODEC2_MODE_1300;
        }
#ifdef DISABLED_FEATURE
        if (m_rb1400->GetValue()) {
            g_mode = MODE_1400;
            g_Nc = 14;
            codec2_mode = CODEC2_MODE_1400;
        }
        if (m_rb2000->GetValue()) {
            g_mode = MODE_2000;
            g_Nc = 20;
            codec2_mode = CODEC2_MODE_1400;
        }
        printf("g_mode: %d  Nc: %d  codec2_mode: %d\n", g_mode, g_Nc, codec2_mode);
#endif

        // init modem and codec states

        g_pFDMDV  = fdmdv_create(g_Nc);
        g_sz_error_pattern = fdmdv_error_pattern_size(g_pFDMDV);
        g_error_pattern = (short*)malloc(g_sz_error_pattern*sizeof(short));
        g_errorFifo = fifo_create(2*g_sz_error_pattern);

        assert(g_error_pattern != NULL);

        g_pCodec2 = codec2_create(codec2_mode);

        if (g_mode == MODE_1400_V0_91)
            fdmdv_use_old_qpsk_mapping(g_pFDMDV);
        if (g_mode == MODE_1600_WIDE)
            fdmdv_set_fsep(g_pFDMDV, FSEP_WIDE);

        // adjust scatter diagram for Number of FDM carriers

        m_panelScatter->setNc(g_Nc);

        // scale factor for to normalise ouput power across modes
        // note: PAPR will still be worse for higher Nc, especially in frame test mode

        g_pwr_scale = sqrt((14.0+4.0)/(g_Nc+4.0));
        g_clip = 120;

        // init Codec 2 LPC Post Filter

        codec2_set_lpc_post_filter(g_pCodec2,
                                   wxGetApp().m_codec2LPCPostFilterEnable,
                                   wxGetApp().m_codec2LPCPostFilterBassBoost,
                                   wxGetApp().m_codec2LPCPostFilterBeta,
                                   wxGetApp().m_codec2LPCPostFilterGamma);

        g_State = 0;
        g_snr = 0.0;
        g_half_duplex = wxGetApp().m_boolHalfDuplex;

        m_pcallsign = m_callsign;

        m_maxLevel = 0;
        m_textLevel->SetLabel(wxT(""));
        m_gaugeLevel->SetValue(0);

        //printf("g_stats.snr: %f\n", g_stats.snr_est);

        // attempt to start sound cards and tx/rx processing

        startRxStream();

        if (m_RxRunning)
        {
#ifdef _USE_TIMER
            m_plotTimer.Start(_REFRESH_TIMER_PERIOD, wxTIMER_CONTINUOUS);
#endif // _USE_TIMER
        }
    }

    // Stop was pressed or start up failed
    if (startStop.IsSameAs("Stop") || !m_RxRunning ) {

        //
        // Stop Running -------------------------------------------------
        //

#ifdef _USE_TIMER
        m_plotTimer.Stop();
#endif // _USE_TIMER

        stopRxStream();

        free(g_error_pattern);
        fifo_destroy(g_errorFifo);
        fdmdv_destroy(g_pFDMDV);
        codec2_destroy(g_pCodec2);

        m_newMicInFilter = m_newSpkOutFilter = true;

        m_togBtnSplit->Disable();
        //m_togRxID->Disable();
        //m_togTxID->Disable();
        m_togBtnAnalog->Disable();
        //m_btnTogPTT->Disable();
        m_togBtnOnOff->SetLabel(wxT("Start"));
        m_rb1400old->Enable();
        m_rb1600->Enable();
#ifdef DISABLED_FEATURE
        m_rb1400->Enable();
        m_rb2000->Enable();
#endif
    }
}

//-------------------------------------------------------------------------
// stopRxStream()
//-------------------------------------------------------------------------
void MainFrame::stopRxStream()
{
    if(m_RxRunning)
    {
        m_RxRunning = false;

        wxLogDebug("waiting for thread to stop");
        m_txRxThread->m_run = 0;
        m_txRxThread->Wait();
        wxLogDebug("thread stopped");

        m_rxPa->stop();
        m_rxPa->streamClose();

        if (g_nSoundCards == 2) {
            m_txPa->stop();
            m_txPa->streamClose();
            delete m_txPa;
        }

        delete m_rxPa;
        destroy_fifos();
        destroy_src();
        deleteEQFilters(g_rxUserdata);
        delete g_rxUserdata;
    }
}

void MainFrame::destroy_fifos(void)
{
    fifo_destroy(g_rxUserdata->infifo1);
    fifo_destroy(g_rxUserdata->outfifo1);
    fifo_destroy(g_rxUserdata->infifo2);
    fifo_destroy(g_rxUserdata->outfifo2);
    fifo_destroy(g_rxUserdata->rxinfifo);
    fifo_destroy(g_rxUserdata->rxoutfifo);
}

void MainFrame::destroy_src(void)
{
    src_delete(g_rxUserdata->insrc1);
    src_delete(g_rxUserdata->outsrc1);
    src_delete(g_rxUserdata->insrc2);
    src_delete(g_rxUserdata->outsrc2);
}

void  MainFrame::initPortAudioDevice(PortAudioWrap *pa, int inDevice, int outDevice,
                                     int soundCard, int sampleRate, int inputChannels)
{
    // Note all of the wrapper functions below just set values in a
    // portaudio struct so can't return any errors. So no need to trap
    // any errors in this function.

    // init input params

    pa->setInputDevice(inDevice);
    pa->setInputChannelCount(inputChannels);           // stereo input
    pa->setInputSampleFormat(PA_SAMPLE_TYPE);
    pa->setInputLatency(pa->getInputDefaultLowLatency());
    pa->setInputHostApiStreamInfo(NULL);

    // init output params

    pa->setOutputDevice(outDevice);
    pa->setOutputChannelCount(2);                      // stereo output
    pa->setOutputSampleFormat(PA_SAMPLE_TYPE);
    pa->setOutputLatency(pa->getOutputDefaultLowLatency());
    pa->setOutputHostApiStreamInfo(NULL);

    // init params that affect input and output

    /*
       On a good day, framesPerBuffer in callback will be PA_FPB.  It
       was found that you don't always get framesPerBuffer exactly
       equal PA_PFB, for example when I set PA_FPB to 960 I got
       framesPerBuffer = 1024.
    */
    pa->setFramesPerBuffer(wxGetApp().m_framesPerBuffer);
    pa->setSampleRate(sampleRate);
    pa->setStreamFlags(paClipOff);
}

//-------------------------------------------------------------------------
// startRxStream()
//-------------------------------------------------------------------------
void MainFrame::startRxStream()
{
    int   src_error;
    const PaDeviceInfo *deviceInfo1 = NULL, *deviceInfo2 = NULL;
    int   inputChannels1, inputChannels2;

    if(!m_RxRunning) {
        m_RxRunning = true;

        if(Pa_Initialize())
        {
            wxMessageBox(wxT("Port Audio failed to initialize"), wxT("Pa_Initialize"), wxOK);
        }

        m_rxPa = new PortAudioWrap();

        if (g_nSoundCards == 0) {
            wxMessageBox(wxT("No Sound Cards configured, use Tools - Audio Config to configure"), wxT("Error"), wxOK);
            delete m_rxPa;
            m_RxRunning = false;
            return;
        }

        // Init Sound card 1 ----------------------------------------------
        // sanity check on sound card device numbers

        if ((m_rxPa->getDeviceCount() <= g_soundCard1InDeviceNum) ||
            (m_rxPa->getDeviceCount() <= g_soundCard1OutDeviceNum)) {
            wxMessageBox(wxT("Sound Card 1 not present"), wxT("Error"), wxOK);
            delete m_rxPa;
            m_RxRunning = false;
            return;
        }

        // work out how many input channels this device supports.

        deviceInfo1 = Pa_GetDeviceInfo(g_soundCard1InDeviceNum);
        if (deviceInfo1 == NULL) {
            wxMessageBox(wxT("Couldn't get device info from Port Audio for Sound Card 1"), wxT("Error"), wxOK);
            delete m_rxPa;
            m_RxRunning = false;
            return;
        }
        if (deviceInfo1->maxInputChannels == 1)
            inputChannels1 = 1;
        else
            inputChannels1 = 2;

        initPortAudioDevice(m_rxPa, g_soundCard1InDeviceNum, g_soundCard1OutDeviceNum, 1,
                            g_soundCard1SampleRate, inputChannels1);

        // Init Sound Card 2 ------------------------------------------------

        if (g_nSoundCards == 2) {

            m_txPa = new PortAudioWrap();

            // sanity check on sound card device numbers

            //printf("m_txPa->getDeviceCount(): %d\n", m_txPa->getDeviceCount());
            //printf("g_soundCard2InDeviceNum: %d\n", g_soundCard2InDeviceNum);
            //printf("g_soundCard2OutDeviceNum: %d\n", g_soundCard2OutDeviceNum);

            if ((m_txPa->getDeviceCount() <= g_soundCard2InDeviceNum) ||
                (m_txPa->getDeviceCount() <= g_soundCard2OutDeviceNum)) {
                wxMessageBox(wxT("Sound Card 2 not present"), wxT("Error"), wxOK);
                delete m_rxPa;
                delete m_txPa;
                m_RxRunning = false;
                return;
            }

            deviceInfo2 = Pa_GetDeviceInfo(g_soundCard2InDeviceNum);
            if (deviceInfo2 == NULL) {
                wxMessageBox(wxT("Couldn't get device info from Port Audio for Sound Card 1"), wxT("Error"), wxOK);
                delete m_rxPa;
                m_RxRunning = false;
                return;
            }
            if (deviceInfo2->maxInputChannels == 1)
                inputChannels2 = 1;
            else
                inputChannels2 = 2;

            initPortAudioDevice(m_txPa, g_soundCard2InDeviceNum, g_soundCard2OutDeviceNum, 2,
                                g_soundCard2SampleRate, inputChannels2);
        }

        // Init call back data structure ----------------------------------------------

        g_rxUserdata = new paCallBackData;
        g_rxUserdata->inputChannels1 = inputChannels1;
        if (deviceInfo2 != NULL)
            g_rxUserdata->inputChannels2 = inputChannels2;

        // init sample rate conversion states

        g_rxUserdata->insrc1 = src_new(SRC_SINC_FASTEST, 1, &src_error);
        assert(g_rxUserdata->insrc1 != NULL);
        g_rxUserdata->outsrc1 = src_new(SRC_SINC_FASTEST, 1, &src_error);
        assert(g_rxUserdata->outsrc1 != NULL);
        g_rxUserdata->insrc2 = src_new(SRC_SINC_FASTEST, 1, &src_error);
        assert(g_rxUserdata->insrc2 != NULL);
        g_rxUserdata->outsrc2 = src_new(SRC_SINC_FASTEST, 1, &src_error);
        assert(g_rxUserdata->outsrc2 != NULL);

        // create FIFOs used to interface between different buffer sizes

        g_rxUserdata->infifo1 = fifo_create(8*N48);
        g_rxUserdata->outfifo1 = fifo_create(8*N48);
        g_rxUserdata->outfifo2 = fifo_create(8*N48);
        g_rxUserdata->infifo2 = fifo_create(8*N48);

        g_rxUserdata->rxinfifo = fifo_create(3 * FDMDV_NOM_SAMPLES_PER_FRAME);
        g_rxUserdata->rxoutfifo = fifo_create(2 * codec2_samples_per_frame(g_pCodec2));

        // Init Equaliser Filters ------------------------------------------------------

        m_newMicInFilter = m_newSpkOutFilter = true;
        designEQFilters(g_rxUserdata);
        g_rxUserdata->micInEQEnable = wxGetApp().m_MicInEQEnable;
        g_rxUserdata->spkOutEQEnable = wxGetApp().m_SpkOutEQEnable;

        // Start sound card 1 ----------------------------------------------------------

        m_rxPa->setUserData(g_rxUserdata);
        m_rxErr = m_rxPa->setCallback(rxCallback);

        m_rxErr = m_rxPa->streamOpen();

        if(m_rxErr != paNoError) {
            wxMessageBox(wxT("Sound Card 1 Open/Setup error."), wxT("Error"), wxOK);
            delete m_rxPa;
            delete m_txPa;
            destroy_fifos();
            destroy_src();
            deleteEQFilters(g_rxUserdata);
            delete g_rxUserdata;
            m_RxRunning = false;
            return;
        }

        m_rxErr = m_rxPa->streamStart();
        if(m_rxErr != paNoError) {
            wxMessageBox(wxT("Sound Card 1 Stream Start Error."), wxT("Error"), wxOK);
            delete m_rxPa;
            delete m_txPa;
            destroy_fifos();
            destroy_src();
            deleteEQFilters(g_rxUserdata);
            delete g_rxUserdata;
            m_RxRunning = false;
            return;
        }

        // Start sound card 2 ----------------------------------------------------------

        if (g_nSoundCards == 2) {

            // question: can we use same callback data
            // (g_rxUserdata)or both sound card callbacks?  Is there a
            // chance of them both being called at the same time?  We
            // could need a mutex ...

            m_txPa->setUserData(g_rxUserdata);
            m_txErr = m_txPa->setCallback(txCallback);
            m_txErr = m_txPa->streamOpen();

            if(m_txErr != paNoError) {
                wxMessageBox(wxT("Sound Card 2 Open/Setup error."), wxT("Error"), wxOK);
                m_rxPa->stop();
                m_rxPa->streamClose();
                delete m_rxPa;
                delete m_txPa;
                destroy_fifos();
                destroy_src();
                deleteEQFilters(g_rxUserdata);
                delete g_rxUserdata;
                m_RxRunning = false;
                return;
            }
            m_txErr = m_txPa->streamStart();
            if(m_txErr != paNoError) {
                wxMessageBox(wxT("Sound Card 2 Start Error."), wxT("Error"), wxOK);
                m_rxPa->stop();
                m_rxPa->streamClose();
                delete m_rxPa;
                delete m_txPa;
                destroy_fifos();
                destroy_src();
                deleteEQFilters(g_rxUserdata);
                delete g_rxUserdata;
                m_RxRunning = false;
                return;
            }
        }

        // start tx/rx processing thread

        m_txRxThread = new txRxThread;

        if ( m_txRxThread->Create() != wxTHREAD_NO_ERROR )
        {
            wxLogError(wxT("Can't create thread!"));
        }

        m_txRxThread->SetPriority(WXTHREAD_MAX_PRIORITY);

        if ( m_txRxThread->Run() != wxTHREAD_NO_ERROR )
        {
            wxLogError(wxT("Can't start thread!"));
        }
    }
}

#define SBQ_MAX_ARGS 4

void *MainFrame::designAnEQFilter(const char filterType[], float freqHz, float gaindB, float Q)
{
    char  *arg[SBQ_MAX_ARGS];
    char   argstorage[SBQ_MAX_ARGS][80];
    void  *sbq;
    int    i, argc;

    assert((strcmp(filterType, "bass") == 0)   ||
           (strcmp(filterType, "treble") == 0) ||
           (strcmp(filterType, "equalizer") == 0));

    for(i=0; i<SBQ_MAX_ARGS; i++) {
        arg[i] = &argstorage[i][0];
        arg[i] = &argstorage[i][0];
        arg[i] = &argstorage[i][0];
    }

    argc = 0;

    if ((strcmp(filterType, "bass") == 0) || (strcmp(filterType, "treble") == 0)) {
        sprintf(arg[argc++], "%s", filterType);
        sprintf(arg[argc++], "%f", gaindB+1E-6);
        sprintf(arg[argc++], "%f", freqHz);
    }

    if (strcmp(filterType, "equalizer") == 0) {
        sprintf(arg[argc++], "%s", filterType);
        sprintf(arg[argc++], "%f", freqHz);
        sprintf(arg[argc++], "%f", Q);
        sprintf(arg[argc++], "%f", gaindB+1E-6);
    }

    assert(argc <= SBQ_MAX_ARGS);

    sbq = sox_biquad_create(argc-1, (const char **)arg);

    return sbq;
}

void  MainFrame::designEQFilters(paCallBackData *cb)
{
    // init Mic In Equaliser Filters

    if (m_newMicInFilter) {
        //printf("designing new Min In filters\n");
        cb->sbqMicInBass   = designAnEQFilter("bass", wxGetApp().m_MicInBassFreqHz, wxGetApp().m_MicInBassGaindB);
        cb->sbqMicInTreble = designAnEQFilter("treble", wxGetApp().m_MicInTrebleFreqHz, wxGetApp().m_MicInTrebleGaindB);
        cb->sbqMicInMid    = designAnEQFilter("equalizer", wxGetApp().m_MicInMidFreqHz, wxGetApp().m_MicInMidGaindB, wxGetApp().m_MicInMidQ);
    }

    // init Spk Out Equaliser Filters

    if (m_newSpkOutFilter) {
        //printf("designing new Spk Out filters\n");
        //printf("designEQFilters: wxGetApp().m_SpkOutBassFreqHz: %f\n",wxGetApp().m_SpkOutBassFreqHz);
        cb->sbqSpkOutBass   = designAnEQFilter("bass", wxGetApp().m_SpkOutBassFreqHz, wxGetApp().m_SpkOutBassGaindB);
        cb->sbqSpkOutTreble = designAnEQFilter("treble", wxGetApp().m_SpkOutTrebleFreqHz, wxGetApp().m_SpkOutTrebleGaindB);
        cb->sbqSpkOutMid    = designAnEQFilter("equalizer", wxGetApp().m_SpkOutMidFreqHz, wxGetApp().m_SpkOutMidGaindB, wxGetApp().m_SpkOutMidQ);
    }
}

void  MainFrame::deleteEQFilters(paCallBackData *cb)
{
    if (m_newMicInFilter) {
        sox_biquad_destroy(cb->sbqMicInBass);
        sox_biquad_destroy(cb->sbqMicInTreble);
        sox_biquad_destroy(cb->sbqMicInMid);
    }
    if (m_newSpkOutFilter) {
        sox_biquad_destroy(cb->sbqSpkOutBass);
        sox_biquad_destroy(cb->sbqSpkOutTreble);
        sox_biquad_destroy(cb->sbqSpkOutMid);
    }
}

// returns number of output samples generated by resampling
int resample(SRC_STATE *src,
            short      output_short[],
            short      input_short[],
            int        output_sample_rate,
            int        input_sample_rate,
            int        length_output_short, // maximum output array length in samples
            int        length_input_short
            )
{
    SRC_DATA src_data;
    float    input[N48*2];
    float    output[N48*2];

    assert(src != NULL);
    assert(length_input_short <= N48*2);
    assert(length_output_short <= N48*2);

    src_short_to_float_array(input_short, input, length_input_short);

    src_data.data_in = input;
    src_data.data_out = output;
    src_data.input_frames = length_input_short;
    src_data.output_frames = length_output_short;
    src_data.end_of_input = 0;
    src_data.src_ratio = (float)output_sample_rate/input_sample_rate;

    src_process(src, &src_data);

    assert(src_data.output_frames_gen <= length_output_short);
    src_float_to_short_array(output, output_short, src_data.output_frames_gen);

    return src_data.output_frames_gen;
}


// Decimates samples using an algorithm that produces nice plots of
// speech signals at a low sample rate.  We want a low sample rate so
// we don't hammer the graphics system too hard.  Saves decimated data
// to a fifo for plotting on screen.
void resample_for_plot(struct FIFO *plotFifo, short buf[], int length)
{
    int decimation = FS/WAVEFORM_PLOT_FS;
    int nSamples, sample;
    int i, st, en, max, min;
    short dec_samples[2*N8];

    assert(length <= 2*N8);

    nSamples = length/decimation;

    for(sample = 0; sample < nSamples; sample += 2)
    {
        st = decimation*sample;
        en = decimation*(sample+2);
        max = min = 0;
        for(i=st; i<en; i++ )
        {
            if (max < buf[i]) max = buf[i];
            if (min > buf[i]) min = buf[i];
        }
        dec_samples[sample] = max;
        dec_samples[sample+1] = min;
    }
    fifo_write(plotFifo, dec_samples, nSamples);
}

void txRxProcessing()
{
    paCallBackData  *cbData = g_rxUserdata;

    // Buffers re-used by tx and rx processing
    // signals in in48k/out48k are at a maximum sample rate of 48k, could be 44.1kHz
    // depending on sound hardware.

    short           in8k_short[2*N8];
    short           in48k_short[2*N48];
    short           out8k_short[2*N8];
    short           out48k_short[2*N48];
    int             nout;

    //wxLogDebug("start infifo1: %5d outfifo1: %5d\n", fifo_n(cbData->infifo1), fifo_n(cbData->outfifo1));

    //
    //  RX side processing --------------------------------------------
    //

    // while we have enough input samples available ...

    int nsam = g_soundCard1SampleRate * (float)N8/FS;
    assert(nsam <= N48);
    g_mutexProtectingCallbackData.Lock();
    while (fifo_read(cbData->infifo1, in48k_short, nsam) == 0)
    {
        g_mutexProtectingCallbackData.Unlock();
        unsigned int n8k;

        n8k = resample(cbData->insrc1, in8k_short, in48k_short, FS, g_soundCard1SampleRate, N8, nsam);

        // optionally save "from radio" signal (write demod input to file)
        // Really useful for testing and development as it allows us
        // to repeat tests using off air signals

        g_mutexProtectingCallbackData.Lock();
        if (g_recFileFromRadio && (g_sfRecFile != NULL)) {
            //printf("g_recFromRadioSamples: %d  n8k: %d \n", g_recFromRadioSamples);
            if (g_recFromRadioSamples < n8k) {
                sf_write_short(g_sfRecFile, in8k_short, g_recFromRadioSamples);
                wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, g_recFileFromRadioEventId );
                // call stop/start record menu item, should be thread safe
                g_parent->GetEventHandler()->AddPendingEvent( event );
                g_recFromRadioSamples = 0;
                //printf("finished recording g_recFromRadioSamples: %d n8k: %d!\n", g_recFileFromRadio, n8k);
            }
            else {
                sf_write_short(g_sfRecFile, in8k_short, n8k);
                g_recFromRadioSamples -= n8k;
            }
        }
        g_mutexProtectingCallbackData.Unlock();

        // optionally read "from radio" signal from file (read demod input from file)

        g_mutexProtectingCallbackData.Lock();
        if (g_playFileFromRadio && (g_sfPlayFileFromRadio != NULL)) {
            int n = sf_read_short(g_sfPlayFileFromRadio, in8k_short, n8k);
            if (n != N8) {
                if (g_loopPlayFileFromRadio)
                    sf_seek(g_sfPlayFileFromRadio, 0, SEEK_SET);
                else {
                    printf("playFileFromRadio fnsihed, issuing event!\n");
                    wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, g_playFileFromRadioEventId );
                    // call stop/start play menu item, should be thread safe
                    g_parent->GetEventHandler()->AddPendingEvent( event );
                }
            }
        }
        g_mutexProtectingCallbackData.Unlock();

        fifo_write(cbData->rxinfifo, in8k_short, n8k);
        resample_for_plot(g_plotDemodInFifo, in8k_short, n8k);

        per_frame_rx_processing(cbData->rxoutfifo, g_CodecBits, cbData->rxinfifo, &g_nRxIn, &g_State, g_pCodec2);

        // Get some audio to send to headphones/speaker.  If out of
        // sync or in analog mode we pass thru the "from radio" audio
        // to the headphones/speaker.  When out of sync it's useful to
        // hear the audio from the channel, e.g. as a tuning aid

        if ((g_State == 0) || g_analog) {
            memcpy(out8k_short, in8k_short, sizeof(short)*n8k);
        }
        else {
            // we are in sync so use decoded audio
            memset(out8k_short, 0, sizeof(short)*N8);
            fifo_read(cbData->rxoutfifo, out8k_short, N8);
        }

        // Opional Spk Out EQ Filtering, need mutex as filter can change at run time
        g_mutexProtectingCallbackData.Lock();
        if (cbData->spkOutEQEnable) {
            sox_biquad_filter(cbData->sbqSpkOutBass,   out8k_short, out8k_short, N8);
            sox_biquad_filter(cbData->sbqSpkOutTreble, out8k_short, out8k_short, N8);
            sox_biquad_filter(cbData->sbqSpkOutMid,    out8k_short, out8k_short, N8);
        }
        g_mutexProtectingCallbackData.Unlock();

        // note squelch automatically disabled in analog mode

        if (g_SquelchActive && (g_SquelchLevel > g_snr) && !g_analog) {
            //printf("g_SquelchLevel: %f g_snr: %f\n", g_SquelchLevel, g_snr);
            memset(out8k_short, 0, sizeof(short)*N8);
        }

        resample_for_plot(g_plotSpeechOutFifo, out8k_short, N8);

        g_mutexProtectingCallbackData.Lock();
        if (g_nSoundCards == 1) {
            nout = resample(cbData->outsrc2, out48k_short, out8k_short, g_soundCard1SampleRate, FS, N48, N8);
            fifo_write(cbData->outfifo1, out48k_short, nout);
        }
        else {
            nout = resample(cbData->outsrc2, out48k_short, out8k_short, g_soundCard2SampleRate, FS, N48, N8);
            fifo_write(cbData->outfifo2, out48k_short, nout);
        }
    }
    g_mutexProtectingCallbackData.Unlock();

    //
    //  TX side processing --------------------------------------------
    //

    if ((g_nSoundCards == 2) && ((g_half_duplex && g_tx) || !g_half_duplex)) {

        // Make sure we have at least 6 frames of modulator output
        // samples.  This also locks the modulator to the sample rate
        // of sound card 1.  We want to make sure that modulator
        // samples are uninterrupted by differences in sample rate
        // between this sound card and sound card 2.

        g_mutexProtectingCallbackData.Lock();
        while((unsigned)fifo_used(cbData->outfifo1) < 6*N48)
        {
            g_mutexProtectingCallbackData.Unlock();

            int   nsam = g_soundCard2SampleRate * (float)codec2_samples_per_frame(g_pCodec2)/FS;
            assert(nsam <= 2*N48);

            // infifo2 is written to by another sound card so it may
            // over or underflow, but we don't realy care.  It will
            // just result in a short interruption in audio being fed
            // to codec2_enc, possibly making a click every now and
            // again in the decoded audio at the other end.

            // zero speech input just in case infifo2 underflows
            memset(in48k_short, 0, nsam*sizeof(short));
            fifo_read(cbData->infifo2, in48k_short, nsam);

            nout = resample(cbData->insrc2, in8k_short, in48k_short, FS, g_soundCard2SampleRate, 2*N8, nsam);

            // optionally use file for mic input signal

            g_mutexProtectingCallbackData.Lock();
            if (g_playFileToMicIn && (g_sfPlayFile != NULL)) {
                int n = sf_read_short(g_sfPlayFile, in8k_short, nout);
                if (n != nout) {
                    if (g_loopPlayFileToMicIn)
                        sf_seek(g_sfPlayFile, 0, SEEK_SET);
                    else {
                        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, g_playFileToMicInEventId );
                        // call stop/start play menu item, should be thread safe
                        g_parent->GetEventHandler()->AddPendingEvent( event );
                    }
                }
            }
            g_mutexProtectingCallbackData.Unlock();

            // Optional Mic In EQ Filtering, need mutex as filter can change at run time
            g_mutexProtectingCallbackData.Lock();
            if (cbData->micInEQEnable) {
                sox_biquad_filter(cbData->sbqMicInBass, in8k_short, in8k_short, nout);
                sox_biquad_filter(cbData->sbqMicInTreble, in8k_short, in8k_short, nout);
                sox_biquad_filter(cbData->sbqMicInMid, in8k_short, in8k_short, nout);
            }
            g_mutexProtectingCallbackData.Unlock();

            resample_for_plot(g_plotSpeechInFifo, in8k_short, nout);

            if (g_analog) {

                // Boost the "from mic" -> "to radio" audio in analog
                // mode.  The need for the gain was found by
                // experiment - analog SSB sounded too quiet compared
                // to digital. With digital voice we generally drive
                // the "to radio" (SSB radio mic input) at about 25%
                // of the peak level for normal SSB voice. So we
                // introduce 6dB gain to make analog SSB sound the
                // same level as the digital.  Watch out for clipping.
                for(int i=0; i<2*N8; i++) {
                    float out = (float)in8k_short[i]*2.0;
                    if (out > 32767) out = 32767.0;
                    if (out < -32767) out = -32767.0;
                    out8k_short[i] = out;
                }
            }
            else
                per_frame_tx_processing(out8k_short, in8k_short, g_pCodec2);

            // output 40ms of modem tone
            nout = resample(cbData->outsrc1, out48k_short, out8k_short, g_soundCard1SampleRate, FS, 2*N48, 2*N8);
            g_mutexProtectingCallbackData.Lock();
            fifo_write(cbData->outfifo1, out48k_short, nout);
        }
        g_mutexProtectingCallbackData.Unlock();
    }
    //wxLogDebug("  end infifo1: %5d outfifo1: %5d\n", fifo_n(cbData->infifo1), fifo_n(cbData->outfifo1));
}

//-------------------------------------------------------------------------
// rxCallback()
//-------------------------------------------------------------------------
int MainFrame::rxCallback(
                            const void      *inputBuffer,
                            void            *outputBuffer,
                            unsigned long   framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void            *userData
                         )
{
    paCallBackData  *cbData = (paCallBackData*)userData;
    short           *rptr    = (short*)inputBuffer;
    short           *wptr    = (short*)outputBuffer;

    short           indata[MAX_FPB];
    short           outdata[MAX_FPB];

    unsigned int    i;

    (void) timeInfo;
    (void) statusFlags;

    assert(inputBuffer != NULL);
    assert(outputBuffer != NULL);

    wxMutexLocker lock(g_mutexProtectingCallbackData);

    //if (statusFlags)
    //    printf("cb1 statusFlags: 0x%x\n", (int)statusFlags);

    //
    //  RX side processing --------------------------------------------
    //

    // assemble a mono buffer (just use left channel if stereo) and write to FIFO

    assert(framesPerBuffer < MAX_FPB);

    for(i = 0; i < framesPerBuffer; i++, rptr += cbData->inputChannels1)
    {
        indata[i] = *rptr;
    }
    if (fifo_write(cbData->infifo1, indata, framesPerBuffer)) {
        //wxLogDebug("infifo1 full\n");
    }

    // OK now set up output samples for this callback

    if (fifo_read(cbData->outfifo1, outdata, framesPerBuffer) == 0)
    {
        // write signal to both channels */
        for(i = 0; i < framesPerBuffer; i++, wptr += 2)
        {
            wptr[0] = outdata[i];
            wptr[1] = outdata[i];
        }
    }
    else
    {
        //wxLogDebug("outfifo1 empty\n");
        // zero output if no data available
        for(i = 0; i < framesPerBuffer; i++, wptr += 2)
        {
            wptr[0] = 0;
            wptr[1] = 0;
        }
    }

    return paContinue;
}

//----------------------------------------------------------------
// per_frame_rx_processing()
//----------------------------------------------------------------
void per_frame_rx_processing(
                                            FIFO    *output_fifo,   // decoded speech samples
                                            int      codec_bits[],  // current frame of bits for decoder
                                            FIFO    *input_fifo,    // modem samples input to demod
                                            int     *nin,           // amount of samples demod needs for next call
                                            int     *state,         // used to collect codec_bits[] halves
                                            CODEC2  *c2             // Codec 2 states
                                        )
{
    int                 reliable_sync_bit;
    short               input_buf[FDMDV_MAX_SAMPLES_PER_FRAME];
    short               output_buf[N8*2];
    COMP                rx_fdm[FDMDV_MAX_SAMPLES_PER_FRAME];
    COMP                rx_fdm_offset[FDMDV_MAX_SAMPLES_PER_FRAME];
    int                 rx_bits[MAX_BITS_PER_FDMDV_FRAME];
    unsigned char       packed_bits[MAX_BYTES_PER_CODEC_FRAME];
    float               rx_spec[FDMDV_NSPEC];
    int                 i;
    int                 nin_prev;
    int                 bit;
    int                 byte;
    int                 next_state;
    int                 bits_per_fdmdv_frame, bits_per_codec_frame, bytes_per_codec_frame;

    bits_per_fdmdv_frame = fdmdv_bits_per_frame(g_pFDMDV);
    //printf(" bits_per_fdmdv_frame: %d\n", bits_per_fdmdv_frame);
    assert(bits_per_fdmdv_frame <= MAX_BITS_PER_FDMDV_FRAME);
    bits_per_codec_frame = codec2_bits_per_frame(c2);
    assert(bits_per_codec_frame <= MAX_BITS_PER_CODEC_FRAME);
    bytes_per_codec_frame = (bits_per_codec_frame+7)/8;
    assert(bytes_per_codec_frame <= MAX_BYTES_PER_CODEC_FRAME);

    //
    //  This while loop will run the demod 0, 1 (nominal) or 2 times:
    //
    //  0: when tx sample clock runs faster than rx, occasionally we
    //     will run out of samples
    //
    //  1: normal, run decoder once, every 2nd frame output a frame of
    //     speech samples to D/A
    //
    //  2: when tx sample clock runs slower than rx, occasionally we will
    //     have enough samples to run demod twice.
    //
    //  With a +/- 10 Hz sample clock difference at FS=8000Hz (+/- 1250
    //  ppm), case 0 or 2 occured about once every 30 seconds.  This is
    //  no problem for the decoded audio.
    //

    //printf("state: %d sync: %d nin %d \n", *state, sync_bit, *nin);
    while (fifo_read(input_fifo, input_buf, *nin) == 0)
    {
        nin_prev = *nin;

        // demod per frame processing
        for(i = 0; i < *nin; i++)
        {
            rx_fdm[i].real = (float)input_buf[i] / FDMDV_SCALE;
            rx_fdm[i].imag = 0.0;
        }
        fdmdv_freq_shift(rx_fdm_offset, rx_fdm, g_RxFreqOffsetHz, &g_RxFreqOffsetPhaseRect, &g_RxFreqOffsetFreqRect, *nin);
        fdmdv_demod(g_pFDMDV, rx_bits, &reliable_sync_bit, rx_fdm_offset, nin);

        // compute rx spectrum & get demod stats, and update GUI plot data
        fdmdv_get_rx_spectrum(g_pFDMDV, rx_spec, rx_fdm, nin_prev);
        fdmdv_get_demod_stats(g_pFDMDV, &g_stats);

        // Average rx spectrum data using a simple IIR low pass filter
        for(i = 0; i < FDMDV_NSPEC; i++)
        {
            g_avmag[i] = BETA * g_avmag[i] + (1.0 - BETA) * rx_spec[i];
        }

        //
        //   State machine to:
        //
        //   + Mute decoded audio when out of sync.  The demod is synced
        //     when we are using the fine freq estimate and SNR is above
        //     a thresh.
        //
        //   + Decode codec bits only if we have a 0,1 sync bit
        //     sequence.  Collects two frames of demod bits to decode
        //     one frame of codec bits.
        //
        next_state = *state;
        switch(*state)
        {
            case 0:
                // mute output audio when out of sync

                for(i = 0; i < N8; i++)
                    output_buf[i] = 0;
                fifo_write(output_fifo, output_buf, N8);

                if(g_stats.sync == 1)
                {
                    next_state = 1;
                }
                //printf("sync state: %d\n", *state);
                break;

            case 1:
                next_state = 2;
                if(g_stats.sync == 0)
                {
                    next_state = 0;
                }
                if (reliable_sync_bit)
                {
                    next_state = 1;
                }

                // first half of frame of codec bits
                memcpy(codec_bits, rx_bits, bits_per_fdmdv_frame * sizeof(int));

                break;

            case 2:
                next_state = 1;
                if(g_stats.sync == 0)
                {
                    next_state = 0;
                }

                int  data_flag_index, valid;

                // second half of frame of codec bits
                memcpy(&codec_bits[bits_per_fdmdv_frame], rx_bits, bits_per_fdmdv_frame *sizeof(int));

                if (g_testFrames) {
                    int bit_errors, ntest_bits, test_frame_sync;

                    // test frame processing, g_test_frame_sync will be asserted when we detect a
                    // valid test frame.

                    fdmdv_put_test_bits(g_pFDMDV, &test_frame_sync, g_error_pattern, &bit_errors, &ntest_bits, codec_bits);

                    if (test_frame_sync == 1) {
                        g_test_frame_sync_state = 1;
                        g_test_frame_count = 0;
                    }

                    if (g_test_frame_sync_state) {
                        if (g_test_frame_count == 0) {
                            //printf("bit_errors: %d ntest_bits: %d\n", bit_errors, ntest_bits);
                            g_total_bit_errors += bit_errors;
                            g_total_bits       += ntest_bits;
                            fifo_write(g_errorFifo, g_error_pattern, g_sz_error_pattern);
                        }
                        g_test_frame_count++;
                        if (g_test_frame_count == 4)
                            g_test_frame_count = 0;
                    }

                    fdmdv_put_test_bits(g_pFDMDV, &test_frame_sync, g_error_pattern, &bit_errors, &ntest_bits, &codec_bits[bits_per_fdmdv_frame]);

                    if (test_frame_sync == 1) {
                        g_test_frame_sync_state = 1;
                        g_test_frame_count = 0;
                    }

                    if (g_test_frame_sync_state) {
                        if (g_test_frame_count == 0) {
                            //printf("bit_errors: %d ntest_bits: %d\n", bit_errors, ntest_bits);
                            g_total_bit_errors += bit_errors;
                            g_total_bits       += ntest_bits;
                            fifo_write(g_errorFifo, g_error_pattern, g_sz_error_pattern);
                        }
                        g_test_frame_count++;
                        if (g_test_frame_count == 4)
                            g_test_frame_count = 0;
                    }

                    // silent audio

                    for(i=0; i<2*N8; i++)
                        output_buf[i] = 0;

                }
                else {
                    // regular Codec 2 frame decode

                    // FEC Decoding  --------------------------------------------------------------

                    if (g_mode == MODE_2000) {
                        int recd_codeword, codeword1, codeword2;

                        /* decode first codeword */

                        recd_codeword = 0;
                        for(i=0; i<12; i++) {
                            recd_codeword <<= 1;
                            recd_codeword |= codec_bits[i];
                        }
                        for(i=bits_per_codec_frame; i<bits_per_codec_frame+11; i++) {
                            recd_codeword <<= 1;
                            recd_codeword |= codec_bits[i];
                        }
                        codeword1 = golay23_decode(recd_codeword);
                        g_total_bit_errors += golay23_count_errors(recd_codeword, codeword1);
                        g_total_bits       += 23;

                        //if (codeword1 != recd_codeword)
                        //    printf("codeword1: 0x%x  recd_codeword: 0x%x\n", codeword1,recd_codeword );
                        //fprintf(stderr, "received codeword1: 0x%x  decoded codeword1: 0x%x\n", recd_codeword, codeword1);

                        for(i=0; i<12; i++) {
                            codec_bits[i] = codeword1 >> (22-i);
                        }

                        /* decode second codeword */

                        recd_codeword = 0;
                        for(i=12; i<24; i++) {
                            recd_codeword <<= 1;
                            recd_codeword |= codec_bits[i];
                        }
                        for(i=bits_per_codec_frame+11; i<bits_per_codec_frame+11+11; i++) {
                            recd_codeword <<= 1;
                            recd_codeword |= codec_bits[i];
                        }
                        codeword2 = golay23_decode(recd_codeword);
                        g_total_bit_errors += golay23_count_errors(recd_codeword, codeword2);
                        g_total_bits       += 23;

                        // if (codeword2 != recd_codeword)
                        //    printf("codeword2: 0x%x  recd_codeword: 0x%x\n", codeword2,recd_codeword );
                        //fprintf(stderr, "received codeword2: 0x%x  decoded codeword2: 0x%x\n", recd_codeword, codeword2);

                        for(i=0; i<12; i++) {
                            codec_bits[12+i] = codeword2 >> (22-i);
                        }
                    }

                    if ((g_mode == MODE_1600) || (g_mode == MODE_1600_WIDE)) {
                        int recd_codeword, codeword1, j;

                        recd_codeword = 0;
                        for(i=0; i<8; i++) {
                            recd_codeword <<= 1;
                            recd_codeword |= codec_bits[i];
                        }
                        for(i=11; i<15; i++) {
                            recd_codeword <<= 1;
                            recd_codeword |= codec_bits[i];
                        }
                        for(i=bits_per_codec_frame; i<bits_per_codec_frame+11; i++) {
                            recd_codeword <<= 1;
                            recd_codeword |= codec_bits[i];
                        }
                        codeword1 = golay23_decode(recd_codeword);
                        g_total_bit_errors += golay23_count_errors(recd_codeword, codeword1);
                        g_total_bits       += 23;

                        //if (codeword1 != recd_codeword)
                        //    printf("codeword1: 0x%x  recd_codeword: 0x%x\n", codeword1,recd_codeword );
                        //codeword1 = recd_codeword;
                        //fprintf(stderr, "received codeword1: 0x%x  decoded codeword1: 0x%x\n", recd_codeword, codeword1);

                        for(i=0; i<8; i++) {
                            codec_bits[i] = (codeword1 >> (22-i)) & 0x1;
                        }
                        for(i=8,j=11; i<12; i++,j++) {
                            codec_bits[j] = (codeword1 >> (22-i)) & 0x1;
                        }
                    }

                    // extract data bit ------------------------------------------------------------

                    data_flag_index = codec2_get_spare_bit_index(c2);
                    //printf("data_flag_index: %d\n", data_flag_index);
                    assert(data_flag_index != -1); // not supported for all rates

                    short abit = codec_bits[data_flag_index];
                    char  ascii_out;

                    int n_ascii = varicode_decode(&g_varicode_dec_states, &ascii_out, &abit, 1, 1);
                    assert((n_ascii == 0) || (n_ascii == 1));
                    if (n_ascii) {
                        short ashort = ascii_out;
                        fifo_write(g_rxDataOutFifo, &ashort, 1);
                    }

                    // reconstruct missing bit we steal for data bit and decode speech

                    valid = codec2_rebuild_spare_bit(c2, codec_bits);
                    assert(valid != -1);

                    // pack bits, MSB received first

                    bit  = 7;
                    byte = 0;
                    memset(packed_bits, 0,  bytes_per_codec_frame);
                    for(i = 0; i < bits_per_codec_frame; i++)
                        {
                            packed_bits[byte] |= (codec_bits[i] << bit);
                            bit--;
                            if(bit < 0)
                                {
                                    bit = 7;
                                    byte++;
                                }
                        }

                    // add decoded speech to end of output buffer

                    assert(codec2_samples_per_frame(c2) == (2*N8));
                    codec2_decode(c2, output_buf, packed_bits);
                }

                fifo_write(output_fifo, output_buf, codec2_samples_per_frame(c2));

                break;
        }
        //printf("state: %d next_state: %d reliable_sync_bit: %d\n", *state, next_state, reliable_sync_bit);
        *state = next_state;
    }
}

void per_frame_tx_processing(
                                            short   tx_fdm_scaled[],// ouput modulated samples
                                            short   input_buf[],    // speech sample input
                                            CODEC2  *c2             // Codec 2 states
                                        )
{
    unsigned char  packed_bits[MAX_BYTES_PER_CODEC_FRAME];
    int            bits[2*MAX_BITS_PER_FDMDV_FRAME];
    COMP           tx_fdm[2*FDMDV_NOM_SAMPLES_PER_FRAME];
    COMP           tx_fdm_offset[2*FDMDV_NOM_SAMPLES_PER_FRAME];
    int            sync_bit;
    int            i, j, bit, byte, data_flag_index;
    short          abit;
    int            bits_per_fdmdv_frame, bits_per_codec_frame, bytes_per_codec_frame;

    bits_per_fdmdv_frame = fdmdv_bits_per_frame(g_pFDMDV);
    assert(bits_per_fdmdv_frame <= MAX_BITS_PER_FDMDV_FRAME);
    bits_per_codec_frame = codec2_bits_per_frame(c2);
    assert(bits_per_codec_frame <= MAX_BITS_PER_CODEC_FRAME);
    bytes_per_codec_frame = (bits_per_codec_frame+7)/8;
    assert(bytes_per_codec_frame <= MAX_BYTES_PER_CODEC_FRAME);

    codec2_encode(c2, packed_bits, input_buf);

    /* unpack bits, MSB first */

    bit = 7; byte = 0;
    for(i=0; i<bits_per_codec_frame; i++) {
        bits[i] = (packed_bits[byte] >> bit) & 0x1;
        bit--;
        if (bit < 0) {
            bit = 7;
            byte++;
        }
    }

    /* add data bit  ----------------------------------*/

    // spare bit in frame that codec defines.  Use this 1
    // bit/frame to send call sign data

    data_flag_index = codec2_get_spare_bit_index(c2);
    assert(data_flag_index != -1); // not supported for all rates

    if (fifo_read(g_txDataInFifo, &abit, 1) == 0)
        bits[data_flag_index] = abit;
    else
        bits[data_flag_index] = 0;

    /* add FEC  ---------------------------------------*/

    if (g_mode == MODE_2000) {
        int data, codeword1, codeword2;

        /* Protect first 24 bits with (23,12) Golay Code.  The first
           24 bits are the most sensitive, as they contain the
           pitch/energy VQ and voicing bits. This uses 56 + 11 + 11 =
           78 bits, so we have two spare in 80 bit frame sent to
           modem. */

        /* first codeword */

        data = 0;
        for(i=0; i<12; i++) {
            data <<= 1;
            data |= bits[i];
        }
        codeword1 = golay23_encode(data);
        //fprintf(stderr, "data1: 0x%x codeword1: 0x%x\n", data, codeword1);

        /* second codeword */

        data = 0;
        for(i=12; i<24; i++) {
            data <<= 1;
            data |= bits[i];
        }
        codeword2 = golay23_encode(data);
        //fprintf(stderr, "data: 0x%x codeword2: 0x%x\n", data, codeword2);

        /* now pack output frame with parity bits at end to make them
           as far apart as possible from the data the protect.  Parity
           bits are LSB of the Golay codeword */

        for(i=bits_per_codec_frame,j=0; i<bits_per_codec_frame+11; i++,j++) {
            bits[i] = (codeword1 >> (10-j)) & 0x1;
        }
        for(j=0; i<bits_per_codec_frame+11+11; i++,j++) {
            bits[i] = (codeword2 >> (10-j)) & 0x1;
        }
        assert(i <= 2*bits_per_fdmdv_frame);
    }

    if ((g_mode == MODE_1600) || (g_mode == MODE_1600_WIDE)) {
        int data, codeword1;

        /* Protect first 12 out of first 16 excitation bits with (23,12) Golay Code:

           0,1,2,3: v[0]..v[3]
           4,5,6,7: MSB of pitch
           11,12,13,14: MSB of energy

        */

        data = 0;
        for(i=0; i<8; i++) {
            data <<= 1;
            data |= bits[i];
        }
        for(i=11; i<15; i++) {
            data <<= 1;
            data |= bits[i];
        }
        data =
        codeword1 = golay23_encode(data);

        /* now pack output frame with parity bits at end to make them
           as far apart as possible from the data they protect.  Parity
           bits are LSB of the Golay codeword */

        for(j=0,i=bits_per_codec_frame; i<bits_per_codec_frame+11; i++,j++) {
            bits[i] = (codeword1 >> (10-j)) & 0x1;
        }

        bits[i] = 0; /* spare bit */
    }


    /* if in test frame mode replace codec payload data with test frames */

    if (g_testFrames) {
	fdmdv_get_test_bits(g_pFDMDV, bits);
	fdmdv_get_test_bits(g_pFDMDV, &bits[bits_per_fdmdv_frame]);
    }

    /* modulate even and odd frames */

    fdmdv_mod(g_pFDMDV, tx_fdm, bits, &sync_bit);
    assert(sync_bit == 1);

    fdmdv_mod(g_pFDMDV, &tx_fdm[FDMDV_NOM_SAMPLES_PER_FRAME], &bits[bits_per_fdmdv_frame], &sync_bit);
    assert(sync_bit == 0);

    fdmdv_freq_shift(tx_fdm_offset, tx_fdm, g_TxFreqOffsetHz, &g_TxFreqOffsetPhaseRect, &g_TxFreqOffsetFreqRect, 2*FDMDV_NOM_SAMPLES_PER_FRAME);

    /* compute scale factor to normalise tx power for all modes */

    /* scale and convert to shorts */

    for(i=0; i<2*FDMDV_NOM_SAMPLES_PER_FRAME; i++) {
        float s =  tx_fdm_offset[i].real;
        if (s > g_clip)
            s = (float)g_clip;
        if (s < -g_clip)
            s = (float)g_clip;
        tx_fdm_scaled[i] = FDMDV_SCALE * g_pwr_scale * s;
    }

}

//-------------------------------------------------------------------------
// txCallback()
//-------------------------------------------------------------------------
int MainFrame::txCallback(
                            const void *inputBuffer,
                            void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *outTime,
                            PaStreamCallbackFlags statusFlags,
                            void *userData
                        )
{
    paCallBackData  *cbData = (paCallBackData*)userData;
    unsigned int    i;
    short           *rptr    = (short*)inputBuffer;
    short           *wptr    = (short*)outputBuffer;
    short           indata[MAX_FPB];
    short           outdata[MAX_FPB];

    wxMutexLocker lock(g_mutexProtectingCallbackData);

    //    if (statusFlags)
    //  printf("cb2 statusFlags: 0x%x\n", (int)statusFlags);

    // assemble a mono buffer (just use left channel) and write to FIFO

    assert(framesPerBuffer < MAX_FPB);

    for(i = 0; i < framesPerBuffer; i++, rptr += cbData->inputChannels2)
    {
        indata[i] = *rptr;
    }

    //#define SC2_LOOPBACK
#ifdef SC2_LOOPBACK

    for(i = 0; i < framesPerBuffer; i++, wptr += 2)
    {
        wptr[0] = indata[i];
        wptr[1] = indata[i];
    }

#else
    fifo_write(cbData->infifo2, indata, framesPerBuffer);

    // OK now set up output samples for this callback

    if (fifo_read(cbData->outfifo2, outdata, framesPerBuffer) == 0)
    {
        // write signal to both channels */
        for(i = 0; i < framesPerBuffer; i++, wptr += 2)
        {
            wptr[0] = outdata[i];
            wptr[1] = outdata[i];
        }
    }
    else
    {
        // zero output if no data available
        for(i = 0; i < framesPerBuffer; i++, wptr += 2)
        {
            wptr[0] = 0;
            wptr[1] = 0;
        }
    }
#endif
    return paContinue;
}

// Callback from plot_spectrum & plot_waterfall.  would be nice to
// work out a way to do this without globals.

void fdmdv2_clickTune(float freq) {

    // The demod is hard-wired to expect a centre frequency of
    // FDMDV_FCENTRE.  So we want to take the signal centered on the
    // click tune freq and re-centre it on FDMDV_FCENTRE.  For example
    // if the click tune freq is 1500Hz, and FDMDV_CENTRE is 1200 Hz,
    // we need to shift the input signal centred on 1500Hz down to
    // 1200Hz, an offset of -300Hz.

    if (g_split) {
        g_RxFreqOffsetHz = FDMDV_FCENTRE - freq;
    }
    else {
        g_TxFreqOffsetHz = freq - FDMDV_FCENTRE;
        g_RxFreqOffsetHz = FDMDV_FCENTRE - freq;
    }
}

//----------------------------------------------------------------
// SetupSerialPort()
//----------------------------------------------------------------
void MainFrame::SetupSerialPort(void)
{
    long baudrate;

    wxGetApp().m_strRigCtrlBaud.ToLong(&baudrate, 10);
    if(!wxGetApp().m_strRigCtrlPort.IsEmpty())
    {
        wxString protocol = wxGetApp().m_strRigCtrlDatabits + wxGetApp().m_strRigCtrlParity + wxGetApp().m_strRigCtrlStopbits;
        m_serialPort = new ctb::SerialPort();
        if(m_serialPort->Open(wxGetApp().m_strRigCtrlPort.c_str(), baudrate, protocol.c_str(), ctb::SerialPort::NoFlowControl ) >= 0 )
        {
            m_device = m_serialPort;
            //  always start with PTT cleared
            if(wxGetApp().m_boolRTSPos) // RTS cleared LOW
            {
                m_serialPort->ClrLineState(ctb::LinestateRts);
            }
            else                        // RTS cleared HIGH
            {
                m_serialPort->SetLineState(ctb::LinestateRts);
            }
            if(wxGetApp().m_boolDTRPos) // DTR cleared LOW
            {
                m_serialPort->ClrLineState(ctb::LinestateDtr);
            }
            else                        // DTR cleared HIGH
            {
                m_serialPort->SetLineState(ctb::LinestateDtr);
            }
            //m_btnTogPTT->Enable(true);
            m_btnTogPTT->SetValue(false);
        }
        else
        {
            m_serialPort = NULL;
            m_device     = NULL;
            //m_btnTogPTT->Disable();
        }
    }
/*
    else
    {
        wxMessageBox(wxT("You must select a Serial port to Open!"), wxT("Error"), wxOK);
    }
*/
}

//----------------------------------------------------------------
// CloseSerialPort()
//----------------------------------------------------------------
void MainFrame::CloseSerialPort(void)
{
    if((m_serialPort != NULL) && m_serialPort->IsOpen())
    {
        m_serialPort->Close();
        m_serialPort = NULL;
        m_device     = NULL;
        //m_btnTogPTT->SetLabel(wxT("PTT"));
        //m_btnTogPTT->Enable(false);
    }
}
