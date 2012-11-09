 //==========================================================================
// Name:            fdmdv2_main.cpp
//
// Purpose:         Implements simple wxWidgets application with GUI.
// Created:         Apr. 9, 2012
// Initial author:  David Witten
// Derived from:    code written by David Rowe
// License:
//
//  Copyright (C) 2012 David Witten
//
//  All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
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
struct CODEC2      *g_pCodec2;
struct FDMDV       *g_pFDMDV;
struct FDMDV_STATS  g_stats;

// time averaged magnitude spectrum used for waterfall and spectrum display
float               g_avmag[FDMDV_NSPEC];

// GUI controls that affect rx and tx processes
int   g_SquelchActive;
float g_SquelchLevel;
int   g_analog;

// rx processing states
int                 g_nRxIn = FDMDV_NOM_SAMPLES_PER_FRAME;
int                 g_CodecBits[2 * FDMDV_BITS_PER_FRAME];
int                 g_State;

// FIFOs used for plotting waveforms
struct FIFO        *g_plotDemodInFifo;
struct FIFO        *g_plotSpeechOutFifo;
struct FIFO        *g_plotSpeechInFifo;

// Soundcard config
int                 g_nSoundCards = 2;
int                 g_soundCard1InDeviceNum = 0;
int                 g_soundCard1OutDeviceNum = 0;
int                 g_soundCard1SampleRate = 48000;
int                 g_soundCard2InDeviceNum = 1;
int                 g_soundCard2OutDeviceNum = 1;
int                 g_soundCard2SampleRate = 44100;

// Click to tune rx frequency offset states
float               g_RxFreqOffsetHz;
COMP                g_RxFreqOffsetPhaseRect;
COMP                g_RxFreqOffsetFreqRect;

// DRs debug variables, will be cleaned up eventually
int cb_cnt, cb1, cb2;
int mute_mic = 0;
int read_file = 0;
FILE *g_file;
int write_file = 0;
FILE *g_write_file;
int sc1, sc2;
int g_outfifo2_empty;

// experimental mutex to make sound card callbacks mutually exclusive
wxMutex g_mutexProtectingCallbackData;

// WxWidgets - initialize the application
IMPLEMENT_APP(MainApp);

//-------------------------------------------------------------------------
// OnInit()
//-------------------------------------------------------------------------
bool MainApp::OnInit()
{
    g_file = fopen("/home/david/codec2-dev/raw/hts1a.raw","rb");
    if (g_file == NULL)
        printf("reading hts1a disabled...\n");

    if(!wxApp::OnInit())
    {
        return false;
    }
    SetVendorName(wxT("CODEC2-Project"));
    SetAppName(wxT("FDMDV2"));      // not needed, it's the default value

    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->SetRecordDefaults();

    m_rTopWindow = wxRect(0, 0, 0, 0);
    m_strRxInAudio.Empty();
    m_strRxOutAudio.Empty();
    m_textVoiceInput.Empty();
    m_textVoiceOutput.Empty();
    m_strSampleRate.Empty();
    m_strBitrate.Empty();
    // Create the main application window
    MainFrame *frame = new MainFrame(NULL);
    SetTopWindow(frame);
    // Should guarantee that the first plot tab defined is the one
    // displayed. But it doesn't when built from command line.  Why?
    frame->m_auiNbookCtrl->ChangeSelection(0);
    frame->Layout();
    frame->Show();
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
    /*
    // @class  $(Name)
    // @author $(User)
    // @date   $(Date)
    // @file   $(CurrentFileName).$(CurrentFileExt)
    // @brief
    */
//    m_radioRunning      = false;
//    m_sound             = NULL;
    m_sfFile            = NULL;
    m_zoom              = 1.;

    if(Pa_Initialize())
    {
        wxMessageBox(wxT("Port Audio failed to initialize"), wxT("Pa_Initialize"), wxOK);
    }

    tools->AppendSeparator();
    wxMenuItem* m_menuItemToolsConfigDelete;
    m_menuItemToolsConfigDelete = new wxMenuItem(tools, wxID_ANY, wxString(_("&Delete stored config")) , wxT("Delete config file/keys"), wxITEM_NORMAL);
    this->Connect(m_menuItemToolsConfigDelete->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnDeleteConfig));

    tools->Append(m_menuItemToolsConfigDelete);

    wxConfigBase *pConfig = wxConfigBase::Get();

    // restore frame position and size
    int x = pConfig->Read(wxT("/MainFrame/top"),       50);
    int y = pConfig->Read(wxT("/MainFrame/left"),      50);
    int w = pConfig->Read(wxT("/MainFrame/width"),     650);
    int h = pConfig->Read(wxT("/MainFrame/height"),    400);
    wxGetApp().m_show_wf      = pConfig->Read(wxT("/MainFrame/show_wf"),      1);
    wxGetApp().m_show_spect   = pConfig->Read(wxT("/MainFrame/show_spect"),   1);
    wxGetApp().m_show_scatter = pConfig->Read(wxT("/MainFrame/show_scatter"), 1);
    wxGetApp().m_show_timing  = pConfig->Read(wxT("/MainFrame/show_timing"),  1);
    wxGetApp().m_show_freq    = pConfig->Read(wxT("/MainFrame/show_freq"),    1);
    wxGetApp().m_show_speech_in = pConfig->Read(wxT("/MainFrame/show_speech_in"),    1);
    wxGetApp().m_show_speech_out = pConfig->Read(wxT("/MainFrame/show_speech_out"),    1);
    wxGetApp().m_show_demod_in = pConfig->Read(wxT("/MainFrame/show_demod_in"),    1);
    
    g_SquelchActive = pConfig->Read(wxT("/Audio/SquelchActive"), 1);
    g_SquelchLevel = pConfig->Read(wxT("/Audio/SquelchLevel"), (int)(SQ_DEFAULT_SNR*2));
    g_SquelchLevel /= 2.0;

    Move(x, y);
    SetClientSize(w, h);

    if(wxGetApp().m_show_wf)
    {
        // Add Waterfall Plot window
        m_panelWaterfall = new PlotWaterfall((wxFrame*) m_auiNbookCtrl);
        m_auiNbookCtrl->AddPage(m_panelWaterfall, _("Waterfall"), true, wxNullBitmap);
    }
    if(wxGetApp().m_show_spect)
    {
        // Add Spectrum Plot window
        m_panelSpectrum = new PlotSpectrum((wxFrame*) m_auiNbookCtrl);
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

       m_panelDemodIn = new PlotScalar((wxFrame*) m_auiNbookCtrl, WAVEFORM_PLOT_TIME, 1.0/WAVEFORM_PLOT_FS, -1, 1, 1, 0.2, "%2.1f");
       m_auiNbookCtrl->AddPage(m_panelDemodIn, _("Demod In"), true, wxNullBitmap);
       g_plotDemodInFifo = fifo_create(2*WAVEFORM_PLOT_BUF);
    }

    if(wxGetApp().m_show_speech_in)
    {
        // Add Speech Input window

       m_panelSpeechIn = new PlotScalar((wxFrame*) m_auiNbookCtrl, WAVEFORM_PLOT_TIME, 1.0/WAVEFORM_PLOT_FS, -1, 1, 1, 0.2, "%2.1f");
       m_auiNbookCtrl->AddPage(m_panelSpeechIn, _("Speech In"), true, wxNullBitmap);
       g_plotSpeechInFifo = fifo_create(2*WAVEFORM_PLOT_BUF);
    }

    if(wxGetApp().m_show_speech_out)
    {
        // Add Speech Output window

       m_panelSpeechOut = new PlotScalar((wxFrame*) m_auiNbookCtrl, WAVEFORM_PLOT_TIME, 1.0/WAVEFORM_PLOT_FS, -1, 1, 1, 0.2, "%2.1f");
       m_auiNbookCtrl->AddPage(m_panelSpeechOut, _("Speech Out"), true, wxNullBitmap);
       g_plotSpeechOutFifo = fifo_create(2*WAVEFORM_PLOT_BUF);
    }

    if(wxGetApp().m_show_timing)
    {
        // Add Timing Offset window
        m_panelTimeOffset = new PlotScalar((wxFrame*) m_auiNbookCtrl, 5.0, DT, -0.5, 0.5, 1, 0.1, "%2.1f");
        m_auiNbookCtrl->AddPage(m_panelTimeOffset, L"Timing \u0394", true, wxNullBitmap);
    }
    if(wxGetApp().m_show_freq)
    {
        // Add Frequency Offset window
        m_panelFreqOffset = new PlotScalar((wxFrame*) m_auiNbookCtrl, 5.0, DT, -200, 200, 1, 50, "%3.0fHz");
        m_auiNbookCtrl->AddPage(m_panelFreqOffset, L"Frequency \u0394", true, wxNullBitmap);
    }

    wxGetApp().m_strRxInAudio       = pConfig->Read(wxT("/Audio/RxIn"),         wxT("<m_strRxInAudio>"));
    wxGetApp().m_strRxOutAudio      = pConfig->Read(wxT("/Audio/RxOut"),        wxT("<m_strRxOutAudio>"));
    wxGetApp().m_textVoiceInput     = pConfig->Read(wxT("/Audio/TxIn"),         wxT("<m_textVoiceInput>"));
    wxGetApp().m_textVoiceOutput    = pConfig->Read(wxT("/Audio/TxOut"),        wxT("<m_textVoiceOutput>"));
    wxGetApp().m_strSampleRate      = pConfig->Read(wxT("/Audio/SampleRate"),   wxT("48000"));

    wxGetApp().m_strRigCtrlPort     = pConfig->Read("/Rig/Port",                wxT("\\\\.\\com1"));
    wxGetApp().m_strRigCtrlBaud     = pConfig->Read("/Rig/Baud",                wxT("9600"));
    wxGetApp().m_strRigCtrlDatabits = pConfig->Read("/Rig/DataBits",            wxT("8"));
    wxGetApp().m_strRigCtrlStopbits = pConfig->Read("/Rig/StopBits",            wxT("1"));
    wxGetApp().m_strRigCtrlParity   = pConfig->Read("/Rig/Parity",              wxT("n"));

    pConfig->SetPath(wxT("/"));

//    this->Connect(m_menuItemHelpUpdates->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TopFrame::OnHelpCheckUpdatesUI));
    m_togRxID->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnRxIDUI), NULL, this);
    m_togTxID->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTxIDUI), NULL, this);
    m_togBtnOnOff->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnOnOffUI), NULL, this);
    m_togBtnSplit->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnSplitClickUI), NULL, this);
    m_togBtnAnalog->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnAnalogClickUI), NULL, this);
    //m_togBtnALC->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnALCClickUI), NULL, this);
    m_btnTogTX->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTXClickUI), NULL, this);

    m_togBtnSplit->Disable();
    m_togRxID->Disable();
    m_togTxID->Disable();
    m_togBtnAnalog->Disable();
    //m_togBtnALC->Disable();
    m_btnTogTX->Disable();
    m_togBtnLoopRx->Disable();
    m_togBtnLoopTx->Disable();

//    m_menuItemPlayAudioFile->Enable(false);

    // squelch settings

    char sqsnr[15];
    m_sliderSQ->SetValue((int)(g_SquelchLevel*2.0));
    sprintf(sqsnr, "%4.1f", g_SquelchLevel);
    wxString sqsnr_string(sqsnr);
    m_textSQ->SetLabel(sqsnr_string);
    m_ckboxSQ->SetValue(g_SquelchActive);

#ifdef _USE_TIMER
    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);       // ID_MY_WINDOW);
    m_plotTimer.SetOwner(this, ID_TIMER_WATERFALL);
    //m_panelWaterfall->Refresh();
#endif

#ifdef _USE_ONIDLE
    Connect(wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle), NULL, this);
#endif //_USE_TIMER
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

    wxConfigBase *pConfig = wxConfigBase::Get();
    if(pConfig)
    {
        GetClientSize(&w, &h);
        GetPosition(&x, &y);
        pConfig->Write(wxT("/MainFrame/top"),           (long) x);
        pConfig->Write(wxT("/MainFrame/left"),          (long) y);
        pConfig->Write(wxT("/MainFrame/width"),         (long) w);
        pConfig->Write(wxT("/MainFrame/height"),        (long) h);
        pConfig->Write(wxT("/MainFrame/show_wf"),       wxGetApp().m_show_wf);
        pConfig->Write(wxT("/MainFrame/show_spect"),    wxGetApp().m_show_spect);
        pConfig->Write(wxT("/MainFrame/show_scatter"),  wxGetApp().m_show_scatter);
        pConfig->Write(wxT("/MainFrame/show_timing"),   wxGetApp().m_show_timing);
        pConfig->Write(wxT("/MainFrame/show_freq"),     wxGetApp().m_show_freq);
        pConfig->Write(wxT("/MainFrame/show_speech_in"),wxGetApp().m_show_speech_in);
        pConfig->Write(wxT("/MainFrame/show_speech_out"),wxGetApp().m_show_speech_out);
        pConfig->Write(wxT("/MainFrame/show_demod_in"),wxGetApp().m_show_demod_in);

        pConfig->Write(wxT("/Audio/SquelchActive"),     g_SquelchActive);
        pConfig->Write(wxT("/Audio/SquelchLevel"),     (int)(g_SquelchLevel*2.0));

        pConfig->Write(wxT("/Audio/RxIn"),              wxGetApp().m_strRxInAudio);
        pConfig->Write(wxT("/Audio/RxOut"),             wxGetApp().m_strRxOutAudio);
        pConfig->Write(wxT("/Audio/TxIn"),              wxGetApp().m_textVoiceInput);
        pConfig->Write(wxT("/Audio/TxOut"),             wxGetApp().m_textVoiceOutput);
        pConfig->Write(wxT("/Audio/SampleRate"),        wxGetApp().m_strSampleRate);

        pConfig->Write(wxT("/Rig/Port"),                wxGetApp().m_strRigCtrlPort);
        pConfig->Write(wxT("/Rig/Baud"),                wxGetApp().m_strRigCtrlBaud);
        pConfig->Write(wxT("/Rig/DataBits"),            wxGetApp().m_strRigCtrlDatabits);
        pConfig->Write(wxT("/Rig/StopBits"),            wxGetApp().m_strRigCtrlStopbits);
        pConfig->Write(wxT("/Rig/Parity"),              wxGetApp().m_strRigCtrlParity);
    }
    m_togRxID->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnRxIDUI), NULL, this);
    m_togTxID->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTxIDUI), NULL, this);
    m_togBtnOnOff->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnOnOffUI), NULL, this);
    m_togBtnSplit->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnSplitClickUI), NULL, this);
    m_togBtnAnalog->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnAnalogClickUI), NULL, this);
    //m_togBtnALC->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnALCClickUI), NULL, this);
    m_btnTogTX->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTXClickUI), NULL, this);
    if(m_sfFile != NULL)
    {
        sf_close(m_sfFile);
        m_sfFile = NULL;
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
//----------------------------------------------------------------

// when the timer fires every DT seconds we update the GUI displays
// On the tabbed display that is visible actually gets updated, this
// keeps CPU load reasonable

void MainFrame::OnTimer(wxTimerEvent &evt)
{

    m_panelWaterfall->m_newdata = true;
    m_panelWaterfall->Refresh();

    m_panelSpectrum->m_newdata = true;
    m_panelSpectrum->Refresh();

    m_panelScatter->add_new_samples(g_stats.rx_symbols);
    m_panelScatter->Refresh();

    m_panelTimeOffset->add_new_sample((float)g_stats.rx_timing/FDMDV_NOM_SAMPLES_PER_FRAME);
    m_panelTimeOffset->Refresh();

    m_panelFreqOffset->add_new_sample(g_stats.foff);
    m_panelFreqOffset->Refresh();

    // Osciliscope type speech plots -------------------------------------------------------

    short speechInPlotSamples[WAVEFORM_PLOT_BUF];
    if (fifo_read(g_plotSpeechInFifo, speechInPlotSamples, WAVEFORM_PLOT_BUF))
        memset(speechInPlotSamples, 0, WAVEFORM_PLOT_BUF*sizeof(short));
    m_panelSpeechIn->add_new_short_samples(speechInPlotSamples, WAVEFORM_PLOT_BUF, 32767);
    m_panelSpeechIn->Refresh();

    short speechOutPlotSamples[WAVEFORM_PLOT_BUF];
    if (fifo_read(g_plotSpeechOutFifo, speechOutPlotSamples, WAVEFORM_PLOT_BUF))
        memset(speechOutPlotSamples, 0, WAVEFORM_PLOT_BUF*sizeof(short));
    m_panelSpeechOut->add_new_short_samples(speechOutPlotSamples, WAVEFORM_PLOT_BUF, 32767);
    m_panelSpeechOut->Refresh();

    short demodInPlotSamples[WAVEFORM_PLOT_BUF];
    if (fifo_read(g_plotDemodInFifo, demodInPlotSamples, WAVEFORM_PLOT_BUF))
        memset(demodInPlotSamples, 0, WAVEFORM_PLOT_BUF*sizeof(short));    
    m_panelDemodIn->add_new_short_samples(demodInPlotSamples, WAVEFORM_PLOT_BUF, 32767);
    m_panelDemodIn->Refresh();

    // SNR text box and guage ------------------------------------------------------------

    float snr_limited = g_stats.snr_est;

    if (snr_limited < -9.0) snr_limited = -9.0; // stop text box overflow
    char snr[15];
    sprintf(snr, "%4.1f", snr_limited);
    wxString snr_string(snr);
    m_textSNR->SetLabel(snr_string);

    if (snr_limited < 0.0) snr_limited = 0;
    if (snr_limited > 20.0) snr_limited = 20.0;
    m_gaugeSNR->SetValue((int)(snr_limited+0.5));

    // sync LED

    if (g_State)
        m_rbSync->SetForegroundColour( wxColour( 0, 255, 0 ) );
    else
        m_rbSync->SetForegroundColour( wxColour( 255, 0, 0 ) );

}
#endif

#ifdef _USE_ONIDLE
//----------------------------------------------------------------
// OnIdle()
//----------------------------------------------------------------
void MainFrame::OnIdle(wxIdleEvent& event)
{
    printf("OnIdle\n");
    if(m_panelWaterfall->m_newdata)
    {
        m_panelWaterfall->Refresh();
    }
    if(m_panelSpectrum->m_newdata)
    {
        m_panelSpectrum->Refresh();
    }
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
// OnExitClick()
//-------------------------------------------------------------------------
void MainFrame::OnExitClick(wxCommandEvent& event)
{
    Pa_Terminate();
    Destroy();
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
// OnCmdSliderScrollChanged()
//-------------------------------------------------------------------------
void MainFrame::OnCmdSliderScrollChanged(wxScrollEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnSliderScrollTop()
//-------------------------------------------------------------------------
void MainFrame::OnSliderScrollTop(wxScrollEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnSliderScrollBottom()
//-------------------------------------------------------------------------
void MainFrame::OnSliderScrollBottom(wxScrollEvent& event)
{
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

//-------------------------------------------------------------------------
// OnTogBtnTXClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnTXClick(wxCommandEvent& event)
{
//    m_soundFile = wxT("./hts1a.wav");
//    m_sound = new wxSound(m_soundFile, false);
//    m_sound->Play();
}

//-------------------------------------------------------------------------
// OnTogBtnRxID()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnRxID(wxCommandEvent& event)
{
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
    if (write_file == 0) {
    write_file = 1;
    g_write_file = fopen("tmp.raw","wb");
    assert(g_write_file != NULL);
    printf("recording to tmp.raw ...\n");
    }
    else {
    fclose(g_write_file);
    printf("recording stopped.\n");
    write_file = 0;
    }
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

//-------------------------------------------------------------------------
// OnOpen()
//-------------------------------------------------------------------------
void MainFrame::OnOpen(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxFileDialog openFileDialog(
                                    this,
                                    wxT("Open Sound File"),
                                    wxEmptyString,
                                    wxEmptyString,
//                                    (const wxChar *)NULL,
                                    wxT("RAW files (*.raw)|*.raw|")
                                    wxT("WAV files (*.wav)|*.wav|")
                                    wxT("Octave 2.0 files (*.mat4)|*.mat4|")
                                    wxT("Octave 2.1 files (*.mat5)|*.mat5|")
                                    wxT("FLAC files (*.flc)|*.flc|")
                                    wxT("All files (*.*)|*.*"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST
                                );
    if(openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;     // the user changed their mind...
    }
    wxString extension;
    m_soundFile = openFileDialog.GetPath();
    wxFileName::SplitPath(m_soundFile, NULL, NULL, &extension);
    //wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
#ifdef _READ_WITH_SNDFILE
    m_sfInfo.format = 0;
    if(!extension.IsEmpty())
    {
        extension.LowerCase();
        if(extension == wxT("raw"))
        {
            m_sfInfo.format     = SF_FORMAT_RAW | SF_FORMAT_PCM_U8;
            m_sfInfo.channels   = 2;
            m_sfInfo.samplerate = 8000;
        }
    }
    m_sfFile    = sf_open(m_soundFile, SFM_READ, &m_sfInfo);
    if(m_sfFile == NULL)
    {
        wxString strErr = sf_strerror(NULL);
        wxMessageBox(strErr, wxT("File Error"), wxOK);
        return;
    }
#endif // _READ_WITH_SNDFILE
    SetStatusText(wxT("File: ") + m_soundFile, 0);
//    bool saved = false;
}

//-------------------------------------------------------------------------
// OnPlayAudioFile()
//-------------------------------------------------------------------------
void MainFrame::OnPlayAudioFile(wxCommandEvent& event)
{
    wxUnusedVar(event);
/*
    // proceed loading the file chosen by the user;
    m_sound->Play(openFileDialog.GetPath());
*/
}

//-------------------------------------------------------------------------
// OnSave()
//-------------------------------------------------------------------------
void MainFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
/*
    wxString savefilename = wxFileSelector(
                                            wxT("Save Sound File"),
                                            wxEmptyString,
                                            wxEmptyString,
                                            (const wxChar *)NULL,
                                            wxT("WAV files (*.wav)|*.wav|")
                                            wxT("RAW files (*.raw)|*.raw|")
                                            wxT("SPEEX files (*.spx)|*.spx|")
                                            wxT("FLAC files (*.flc)|*.flc|"),
                                            wxFD_SAVE,
                                            this
                                          );
    if(savefilename.empty())
    {
        return;
    }
    wxString extension;
    wxFileName::SplitPath(savefilename, NULL, NULL, &extension);
    bool saved = false;
    if(!saved)
    {
        // This one guesses image format from filename extension
        // (it may fail if the extension is not recognized):
        //image.SaveFile(savefilename);
    }
*/
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void MainFrame::OnClose(wxCommandEvent& event)
{
    wxUnusedVar(event);

#ifdef _USE_TIMER
    //DMW Reenable for the nonce...
    m_plotTimer.Stop();
#endif // _USE_TIMER
    if(m_sfFile != NULL)
    {
        sf_close(m_sfFile);
        m_sfFile = NULL;
    }
    if(m_RxRunning)
    {
        stopRxStream();
    }
    m_togBtnSplit->Disable();
    m_togRxID->Disable();
    m_togTxID->Disable();
    m_togBtnAnalog->Disable();
    //m_togBtnALC->Disable();
    m_btnTogTX->Disable();
    m_togBtnLoopRx->Disable();
    m_togBtnLoopTx->Disable();

}

//-------------------------------------------------------------------------
// OnExit()
//-------------------------------------------------------------------------
void MainFrame::OnExit(wxCommandEvent& event)
{
    if(m_RxRunning)
    {
        stopRxStream();
    }
    Close();
}

//-------------------------------------------------------------------------
// OnCopy()
//-------------------------------------------------------------------------
void MainFrame::OnCopy(wxCommandEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnCut()
//-------------------------------------------------------------------------
void MainFrame::OnCut(wxCommandEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnPaste()
//-------------------------------------------------------------------------
void MainFrame::OnPaste(wxCommandEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnCaptureRxStream()
//-------------------------------------------------------------------------
//void MainFrame::OnCaptureRxStream(wxCommandEvent& event)
//{
//    wxUnusedVar(event);
//}

//-------------------------------------------------------------------------
// OnCaptureTxStream()
//-------------------------------------------------------------------------
//void MainFrame::OnCaptureTxStream(wxCommandEvent& event)
//{
//    wxUnusedVar(event);
//}

/*
//-------------------------------------------------------------------------
// OnSaveUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnSaveUpdateUI(wxUpdateUIEvent& event)
{
//    wxUnusedVar(event);
    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnCloseUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnCloseUpdateUI(wxUpdateUIEvent& event)
{
//    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnCutUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnCutUpdateUI(wxUpdateUIEvent& event)
{
//    wxUnusedVar(event);
    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnCopyUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnCopyUpdateUI(wxUpdateUIEvent& event)
{
    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnPasteUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnPasteUpdateUI(wxUpdateUIEvent& event)
{
    event.Enable(false);
}
*/

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
// OnToolsAudioUI()
//-------------------------------------------------------------------------
void MainFrame::OnToolsAudioUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnToolsComCfg()
//-------------------------------------------------------------------------
void MainFrame::OnToolsComCfg(wxCommandEvent& event)
{
    wxUnusedVar(event);
    int rv = 0;
    ComPortsDlg *dlg = new ComPortsDlg(NULL);
    rv = dlg->ShowModal();
    if(rv == wxID_OK)
    {
        dlg->ExchangeData(EXCHANGE_DATA_OUT);
    }
    delete dlg;
}

//-------------------------------------------------------------------------
// OnToolsComCfgUI()
//-------------------------------------------------------------------------
void MainFrame::OnToolsComCfgUI(wxUpdateUIEvent& event)
{
    event.Enable((!m_TxRunning) && (!m_RxRunning));
}

//-------------------------------------------------------------------------
// OnToolsOptions()
//-------------------------------------------------------------------------
void MainFrame::OnToolsOptions(wxCommandEvent& event)
{
    wxUnusedVar(event);
    int rv = 0;
    OptionsDlg *dlg = new OptionsDlg(NULL);
    rv = dlg->ShowModal();
    if(rv == wxID_OK)
    {
        dlg->ExchangeData(EXCHANGE_DATA_OUT);
    }
    delete dlg;
}

//-------------------------------------------------------------------------
// OnToolsOptionsUI()
//-------------------------------------------------------------------------
void MainFrame::OnToolsOptionsUI(wxUpdateUIEvent& event)
{
//    wxUnusedVar(event);
    event.Enable(false);
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
//    int rv = 0;
//    AboutDlg *dlg = new AboutDlg(NULL);
//    rv = dlg->ShowModal();
//    if(rv == wxID_OK)
//    {
//        dlg->ExchangeData(EXCHANGE_DATA_OUT);
//    }
//    delete dlg;
}

/*
//-------------------------------------------------------------------------
// LoadUserImage()
//-------------------------------------------------------------------------
wxString MainFrame::LoadUserImage(wxImage& image)
{
    wxString filename;

    filename = wxLoadFileSelector(wxT("image"), wxEmptyString);
    if(!filename.empty())
    {
        if(!image.LoadFile(filename))
        {
            wxLogError(wxT("Couldn't load image from '%s'."), filename.c_str());
            return wxEmptyString;
        }
    }
    return filename;
}
*/


//-------------------------------------------------------------------------
// OnTogBtnOnOff()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnOnOff(wxCommandEvent& event)
{
    if((!m_RxRunning))
    {
        printf("starting ...\n");
        
        m_togBtnSplit->Enable();
        m_togRxID->Enable();
        m_togTxID->Enable();
        m_togBtnAnalog->Enable();
        //m_togBtnALC->Enable();
        m_btnTogTX->Enable();
        m_togBtnLoopRx->Enable();
        m_togBtnLoopTx->Enable();

        // init modem and codec states
        g_pFDMDV  = fdmdv_create();
        g_pCodec2 = codec2_create(CODEC2_MODE_1400);
        g_State = 0;

        // init click-tune states
        g_RxFreqOffsetHz = 50.0;
        g_RxFreqOffsetFreqRect.real = cos(g_RxFreqOffsetHz);
        g_RxFreqOffsetFreqRect.imag = sin(g_RxFreqOffsetHz);
        g_RxFreqOffsetPhaseRect.real = cos(0.0);
        g_RxFreqOffsetPhaseRect.imag = sin(0.0);
        
#ifdef _USE_TIMER
        //DMW Reenable for the nonce... // DR: disable this puppy for now as it's causing a lot of error messages
        m_plotTimer.Start(_REFRESH_TIMER_PERIOD, wxTIMER_CONTINUOUS);
#endif // _USE_TIMER

        // start sound cards
        startRxStream();

        if (m_RxRunning)
        {
            m_togBtnOnOff->SetLabel(wxT("Stop"));
        }
    }
    else
    {
        m_togBtnSplit->Disable();
        m_togRxID->Disable();
        m_togTxID->Disable();
        m_togBtnAnalog->Disable();
        //m_togBtnALC->Disable();
        m_btnTogTX->Disable();
        m_togBtnLoopRx->Disable();
        m_togBtnLoopTx->Disable();
#ifdef _USE_TIMER
        //DMW Reenable for the nonce...
        m_plotTimer.Stop();
#endif // _USE_TIMER
        stopRxStream();
        m_togBtnOnOff->SetLabel(wxT("Start"));
    }
}

//----------------------------------------------------------
// OnTogBtnLoopRx()
//----------------------------------------------------------
void MainFrame::OnTogBtnLoopRx( wxCommandEvent& event )
{
    if (mute_mic == 0)
    mute_mic = 1;
    else
    mute_mic = 0;

}

//----------------------------------------------------------
// OnTogBtnLoopTx()
//----------------------------------------------------------
void MainFrame::OnTogBtnLoopTx( wxCommandEvent& event )
{
    if (read_file == 0)
    read_file = 1;
    else
    read_file = 0;

}


void MainFrame::destroy_fifos(void)
{
    fifo_destroy(m_rxUserdata->infifo1);
    fifo_destroy(m_rxUserdata->outfifo1);
    fifo_destroy(m_rxUserdata->infifo2);
    fifo_destroy(m_rxUserdata->outfifo2);
    fifo_destroy(m_rxUserdata->rxinfifo);
    fifo_destroy(m_rxUserdata->rxoutfifo);
}

void MainFrame::destroy_src(void)
{
    src_delete(m_rxUserdata->insrc1);
    src_delete(m_rxUserdata->outsrc1);
    src_delete(m_rxUserdata->insrc2);
    src_delete(m_rxUserdata->outsrc2);
}

void MainFrame::autoDetectSoundCards(PortAudioWrap *pa)
{
    const   PaDeviceInfo *deviceInfo;
    int     i;

    // trap zero sound devices

    if (pa->getDeviceCount() == 0) {
    wxMessageBox(wxT("No sound devices found"), wxT("Error"), wxOK);
    return;
    }

    for(i=0; i<pa->getDeviceCount(); i++) {
        deviceInfo = Pa_GetDeviceInfo( i );

    // supports full duplex and 44800 and 44100
    // is there something unique so we know it's a hw device?
    // does this work on Linux & Windows?

    printf( "--------------------------------------- device #%d\n", i );
        printf( "Name                        = %s\n", deviceInfo->name );
        printf( "Host API                    = %s\n",  Pa_GetHostApiInfo( deviceInfo->hostApi )->name );
        printf( "Max inputs = %d", deviceInfo->maxInputChannels  );
        printf( ", Max outputs = %d\n", deviceInfo->maxOutputChannels  );
        printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );
    }
}

int MainFrame::initPortAudioDevice(PortAudioWrap *pa, int inDevice, int outDevice, int soundCard, int sampleRate)
{
    char s[256];

    if (inDevice == paNoDevice) {
    sprintf(s,"No input audio device available for Sound Card %d", soundCard);
    wxString wxs(s);
    wxMessageBox(wxs, wxT("Error"), wxOK);
    }
    if (outDevice == paNoDevice) {
    sprintf(s,"No output audio device available for Sound Card %d", soundCard);
    wxString wxs(s);
    wxMessageBox(wxs, wxT("Error"), wxOK);
    }

    printf("inDevice = %d outDevice = %d\n", inDevice, outDevice);

    // init input params

    pa->setInputDevice(inDevice);
    pa->setInputChannelCount(2);                          // stereo input
    pa->setInputSampleFormat(PA_SAMPLE_TYPE);
    pa->setInputLatency(pa->getInputDefaultLowLatency());
    pa->setInputHostApiStreamInfo(NULL);

    // init output params

    pa->setOutputDevice(outDevice);
    pa->setOutputChannelCount(2);                         // stereo input
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
    printf("Sound Card %d Sample rate %d\n", soundCard, sampleRate);
    pa->setFramesPerBuffer(PA_FPB);
    pa->setSampleRate(sampleRate);
    pa->setStreamFlags(0);

    return 0;
}

//-------------------------------------------------------------------------
// startRxStream()
//-------------------------------------------------------------------------
void MainFrame::startRxStream()
{
    int src_error;

    if(!m_RxRunning) {
        cb_cnt = 0;
        sc1 = sc2 = 0;
        g_outfifo2_empty = 0;

        m_RxRunning = true;

        m_rxPa = new PortAudioWrap();
        autoDetectSoundCards(m_rxPa);

        // Init Sound card 1 ----------------------------------------------

        if ((g_soundCard1InDeviceNum != -1) || (g_soundCard1OutDeviceNum != -1)) {

            // user has specified the sound card device

            if ((m_rxPa->getDeviceCount() < g_soundCard1InDeviceNum) ||
                (m_rxPa->getDeviceCount() < g_soundCard1OutDeviceNum)) {
                wxMessageBox(wxT("Sound Card 1 not present"), wxT("Error"), wxOK);
                delete m_rxPa;
                return;
            }

            m_rxDevIn = g_soundCard1InDeviceNum;
            m_rxDevOut = g_soundCard1OutDeviceNum;
        }
        else {
            // not specified - use default
            m_rxDevIn = m_rxPa->getDefaultInputDevice();
            m_rxDevOut = m_rxPa->getDefaultOutputDevice();
        }

        if (initPortAudioDevice(m_rxPa, m_rxDevIn, m_rxDevOut, 1, g_soundCard1SampleRate) != 0) {
            delete m_rxPa;
            m_RxRunning = false;
            return;
        }

        // Init Sound Card 2 ------------------------------------------------

        if (g_nSoundCards == 2) {

            m_txPa = new PortAudioWrap();

            assert((g_soundCard2InDeviceNum != -1) && (g_soundCard2OutDeviceNum != -1) );
            printf("m_txPa->getDeviceCount() %d\n", m_txPa->getDeviceCount());

            if ((m_txPa->getDeviceCount() < g_soundCard2InDeviceNum) ||
                (m_txPa->getDeviceCount() < g_soundCard2OutDeviceNum)) {
                wxMessageBox(wxT("Sound Card 2 not present"), wxT("Error"), wxOK);
                delete m_rxPa;
                delete m_txPa;
                return;
            }

            m_txDevIn = g_soundCard2InDeviceNum;
            m_txDevOut = g_soundCard2OutDeviceNum;

            if (initPortAudioDevice(m_txPa, m_txDevIn, m_txDevOut, 2, g_soundCard2SampleRate) != 0) {
                delete m_rxPa;
                delete m_txPa;
                m_RxRunning = false;
                return;
            }
        }

        // Init call back data structure ----------------------------------------------

        m_rxUserdata = new paCallBackData;
        m_rxUserdata->pWFPanel = m_panelWaterfall;
        m_rxUserdata->pSPPanel = m_panelSpectrum;

        // init sample rate conversion states

        m_rxUserdata->insrc1 = src_new(SRC_SINC_FASTEST, 1, &src_error);
        assert(m_rxUserdata->insrc1 != NULL);
        m_rxUserdata->outsrc1 = src_new(SRC_SINC_FASTEST, 1, &src_error);
        assert(m_rxUserdata->outsrc1 != NULL);
        m_rxUserdata->insrc2 = src_new(SRC_SINC_FASTEST, 1, &src_error);
        assert(m_rxUserdata->insrc2 != NULL);
        m_rxUserdata->outsrc2 = src_new(SRC_SINC_FASTEST, 1, &src_error);
        assert(m_rxUserdata->outsrc2 != NULL);

        // create FIFOs used to interface between different buffer sizes

        m_rxUserdata->infifo1 = fifo_create(4*N48);

        m_rxUserdata->outfifo1 = fifo_create(4*N48);

        /*
          During soundcard 1 callback, outfifo1 is read in PA_FPB = 1024 sample blocks.

          In tx processing:
          + if outfifo1 has less than framesPerBuffer = PA_FPB = 1024 samples
          + then tx process generates a 40ms @ 48kHz = 1920 sample blocks
          + so we could have 1023 + 1920 samples
          + so lets make it 1920*2 in size
        */

        m_rxUserdata->outfifo2 = fifo_create(4*N48);

        /*
          infifo2 holds buffers from the microphone.  These get read
          in 40ms (1920 sample @ 48 kHz) blocks by the sound card 1
          callback.  We write to the sound card in 1024 sample
          blocks.  So we need at least 1024+1920 samples, lest also allocate
          1920*2 samples
        */

        m_rxUserdata->infifo2 = fifo_create(4*N48);

        m_rxUserdata->rxinfifo = fifo_create(3 * FDMDV_NOM_SAMPLES_PER_FRAME);
        m_rxUserdata->rxoutfifo = fifo_create(2 * codec2_samples_per_frame(g_pCodec2));
        
        // Start sound card 1 ----------------------------------------------------------

        m_rxPa->setUserData(m_rxUserdata);
        m_rxErr = m_rxPa->setCallback(rxCallback);

        m_rxErr = m_rxPa->streamOpen();

        if(m_rxErr != paNoError) {
            wxMessageBox(wxT("Sound Card 1 Open/Setup error."), wxT("Error"), wxOK);
            delete m_rxPa;
            delete m_txPa;
            destroy_fifos();
            destroy_src();
            delete m_rxUserdata;
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
            delete m_rxUserdata;
            m_RxRunning = false;
            return;
        }

        // Start sound card 2 ----------------------------------------------------------

        if (g_nSoundCards == 2) {
            printf("starting sound card 2...\n");

            // question: can we use same callback data
            // (m_rxUserdata)or both sound card callbacks?  Is there a
            // chance of them both being called at the same time?  We
            // could need a mutex ...

            m_txPa->setUserData(m_rxUserdata);
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
                delete m_rxUserdata;
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
                delete m_rxUserdata;
                m_RxRunning = false;
                return;
            }
        }
        
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
        delete m_rxUserdata;

        fdmdv_destroy(g_pFDMDV);
        codec2_destroy(g_pCodec2);
    }

}


//----------------------------------------------------------------
// update average of each spectrum point
//----------------------------------------------------------------
void MainFrame::averageData(float mag_dB[])
{
    int i;

    for(i = 0; i < FDMDV_NSPEC; i++)
    {
        g_avmag[i] = (1.0 - BETA) * g_avmag[i] + BETA * mag_dB[i];
    }
}

// returns number of output samples generated by resampling

static int resample(SRC_STATE *src,
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

static void resample_for_plot(struct FIFO *plotFifo, short buf[], int length)
{
    int decimation = FS/WAVEFORM_PLOT_FS;
    int nSamples, sample;
    int i, st, en, max, min;
    short dec_samples[2*N8];

    assert(length <= 2*N8);

    nSamples = length/decimation;

    for(sample=0; sample<nSamples; sample+=2) {
    st = decimation*sample;
    en = decimation*(sample+2);
    max = min = 0;
    for(i=st; i<en; i++ ) {
        if (max < buf[i]) max = buf[i];
        if (min > buf[i]) min = buf[i];
    }
    dec_samples[sample] = max;
    dec_samples[sample+1] = min;
    }

    fifo_write(plotFifo, dec_samples, nSamples);
}

//-------------------------------------------------------------------------
// rxCallback()
//-------------------------------------------------------------------------

/*
   todo:

   + add tests to determine if we have real time audio I/O problems,
     for example lost samples, buffer overflow, too slow processing
   + should flag the user so they can tune the system
   + maybe compute time spent doing demod compared to time between calls
*/

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

    // temp buffers re-used by tx and rx processing

    short           in8k_short[2*N8];
    short           in48k_short[2*N48];
    short           out8k_short[2*N8];
    short           out48k_short[2*N48];
    short           indata[MAX_FPB];
    short           outdata[MAX_FPB];
    int             nout;

    unsigned int    i;

    (void) timeInfo;
    (void) statusFlags;

    //printf("%d cb1 .. %d", cb1++, cb1-cb2);
    assert(inputBuffer != NULL);
    assert(outputBuffer != NULL);

    /*
       framesPerBuffer is portaudio-speak for number of samples we
       actually get from the record side and need to provide to the
       play side. On Linux (at least) it was found that
       framesPerBuffer may not always be what we ask for in the
       framesPerBuffer field of Pa_OpenStream.  For example a request
       for 960 sample buffers may lead to framesPerBuffer = 1024 in
       this call back.

       So we use a bunch of FIFOs to interface between different
       buffer sizes required at each processing step, and to
       communicate between the call backs for the two sound cards.
    */

    //
    //  RX side processing --------------------------------------------
    //

    // assemble a mono buffer (just use left channel) and write to FIFO

    assert(framesPerBuffer < MAX_FPB);
    for(i = 0; i < framesPerBuffer; i++, rptr += 2)
    {
        indata[i] = *rptr;
    }
    fifo_write(cbData->infifo1, indata, framesPerBuffer);

    // while we have enough input samples available ...

    while (fifo_read(cbData->infifo1, in48k_short, N48) == 0)
    {
        int n8k;

        n8k = resample(cbData->insrc1, in8k_short, in48k_short, FS, g_soundCard1SampleRate, N8, N48);
        fifo_write(cbData->rxinfifo, in8k_short, n8k);

        resample_for_plot(g_plotDemodInFifo, in8k_short, n8k);

        per_frame_rx_processing(cbData->rxoutfifo, g_CodecBits, cbData->rxinfifo, &g_nRxIn, &g_State, g_pCodec2);

        // if demod out of sync or we are in analog mode just pass thru
        // audio so we can hear SSB radio ouput as an aid to tuning

        if ((g_State == 0) || g_analog)
            memcpy(out8k_short, in8k_short, sizeof(short)*n8k);
        else {
            memset(out8k_short, 0, sizeof(short)*N8);
            fifo_read(cbData->rxoutfifo, out8k_short, N8);
        }

        resample_for_plot(g_plotSpeechOutFifo, out8k_short, N8);

        if (g_nSoundCards == 1) {
            nout = resample(cbData->outsrc2, out48k_short, out8k_short, g_soundCard1SampleRate, FS, N48, N8);
            fifo_write(cbData->outfifo1, out48k_short, nout);
        }
        else {
            nout = resample(cbData->outsrc2, out48k_short, out8k_short, g_soundCard2SampleRate, FS, N48, N8);
            fifo_write(cbData->outfifo2, out48k_short, nout);
        }

    }

    //
    //  TX side processing --------------------------------------------
    //

    if (g_nSoundCards == 2 ) {

    // Make sure we have at least framesPerBuffer modulator output
    // samples.  This locks the modulator to the sample
    // rate of sound card 1.  We want to make sure that modulator
    // samples are uninterrupted by differences in sample rate
    // between this sound card and sound card 2.

        while((unsigned)fifo_n(cbData->outfifo1) < framesPerBuffer) {

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

            if (write_file) {
                fwrite( in8k_short, sizeof(short), nout, g_write_file);
            }

            if (read_file  && (g_file != NULL)) {
                int n = fread( in8k_short , sizeof(short), 2*N8, g_file);
                if (n != 2*N8) {
                    rewind(g_file);
                }
            }

            resample_for_plot(g_plotSpeechInFifo, in8k_short, nout);

            if (g_analog)
                memcpy(out8k_short, in8k_short,  2*N8*sizeof(short));
            else
                per_frame_tx_processing(out8k_short, in8k_short, g_pCodec2);
 
            // output 40ms of modem tone

            nout = resample(cbData->outsrc1, out48k_short, out8k_short, g_soundCard1SampleRate, FS, 2*N48, 2*N8);
            fifo_write(cbData->outfifo1, out48k_short, nout);
        }
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
       // zero output if no data available
        for(i = 0; i < framesPerBuffer; i++, wptr += 2)
        {
            wptr[0] = 0;
            wptr[1] = 0;
        }
    }
    //printf("end cb1\n");

    return paContinue;
}

//----------------------------------------------------------------
// per_frame_rx_processing()
//----------------------------------------------------------------
void MainFrame::per_frame_rx_processing(
                                            FIFO    *output_fifo,   // decoded speech samples
                                            int      codec_bits[],  // current frame of bits for decoder
                                            FIFO    *input_fifo,    // modem samples input to demod
                                            int     *nin,           // amount of samples demod needs for next call
                                            int     *state,         // used to collect codec_bits[] halves
                                            CODEC2  *c2             // Codec 2 states
                                        )
{
    int                 sync_bit;
    short               input_buf[FDMDV_MAX_SAMPLES_PER_FRAME];
    short               output_buf[N8*2];
    COMP                rx_fdm[FDMDV_MAX_SAMPLES_PER_FRAME];
    COMP                rx_fdm_offset[FDMDV_MAX_SAMPLES_PER_FRAME];
    int                 rx_bits[FDMDV_BITS_PER_FRAME];
    unsigned char       packed_bits[BYTES_PER_CODEC_FRAME];
    float               rx_spec[FDMDV_NSPEC];
    int                 i;
    int                 nin_prev;
    int                 bit;
    int                 byte;
    int                 next_state;

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

    //printf("state: %d sync: %d nin %d fifo_n %d\n", *state, sync_bit, *nin, fifo_n(input_fifo));
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
        fdmdv_demod(g_pFDMDV, rx_bits, &sync_bit, rx_fdm_offset, nin);

        // compute rx spectrum & get demod stats, and update GUI plot data
        fdmdv_get_rx_spectrum(g_pFDMDV, rx_spec, rx_fdm, nin_prev);
        fdmdv_get_demod_stats(g_pFDMDV, &g_stats);

        // Average rx spectrum data using a simple IIR low pass filter
        for(i = 0; i < FDMDV_NSPEC; i++)
        {
            g_avmag[i] = (1.0 - BETA) * g_avmag[i] + BETA * rx_spec[i];
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

                if((g_stats.fest_coarse_fine == 1) && (g_stats.snr_est > 3.0))
                {
                    next_state = 1;
                }
        //printf("sync state: %d\n", *state);
                break;

            case 1:
                if(sync_bit == 0)
                {
                    next_state = 2;
                    // first half of frame of codec bits
                    memcpy(codec_bits, rx_bits, FDMDV_BITS_PER_FRAME * sizeof(int));
                }
                else
                {
                    next_state = 1;
                }
                if(g_stats.fest_coarse_fine == 0)
                {
                    next_state = 0;
                }
                break;

            case 2:
                next_state = 1;
                if(g_stats.fest_coarse_fine == 0)
                {
                    next_state = 0;
                }
                if(sync_bit == 1)
                {
                    // second half of frame of codec bits
                    memcpy(&codec_bits[FDMDV_BITS_PER_FRAME], rx_bits, FDMDV_BITS_PER_FRAME*sizeof(int));
                    // pack bits, MSB received first
                    bit  = 7;
                    byte = 0;
                    memset(packed_bits, 0, BYTES_PER_CODEC_FRAME);
                    for(i = 0; i < BITS_PER_CODEC_FRAME; i++)
                    {
                        packed_bits[byte] |= (codec_bits[i] << bit);
                        bit--;
                        if(bit < 0)
                        {
                            bit = 7;
                            byte++;
                        }
                    }
                    assert(byte == BYTES_PER_CODEC_FRAME);

                    // add decoded speech to end of output buffer

            assert(codec2_samples_per_frame(c2) == (2*N8));

            codec2_decode(c2, output_buf, packed_bits);
            fifo_write(output_fifo, output_buf, codec2_samples_per_frame(c2));

                }
                break;
        }
        *state = next_state;
    }
}

void MainFrame::per_frame_tx_processing(
                                            short   tx_fdm_scaled[],// ouput modulated samples
                                            short   input_buf[],    // speech sample input
                                            CODEC2  *c2             // Codec 2 states
                                        )
{
    unsigned char  packed_bits[BYTES_PER_CODEC_FRAME];
    int            bits[BITS_PER_CODEC_FRAME];
    COMP           tx_fdm[2*FDMDV_NOM_SAMPLES_PER_FRAME];
    int            sync_bit;
    int            i, bit, byte;

    codec2_encode(c2, packed_bits, input_buf);

    /* unpack bits, MSB first */

    bit = 7; byte = 0;
    for(i=0; i<BITS_PER_CODEC_FRAME; i++) {
    bits[i] = (packed_bits[byte] >> bit) & 0x1;
    bit--;
    if (bit < 0) {
        bit = 7;
        byte++;
    }
    }
    assert(byte == BYTES_PER_CODEC_FRAME);

    /* modulate even and odd frames */

    fdmdv_mod(g_pFDMDV, tx_fdm, bits, &sync_bit);
    assert(sync_bit == 1);

    fdmdv_mod(g_pFDMDV, &tx_fdm[FDMDV_NOM_SAMPLES_PER_FRAME], &bits[FDMDV_BITS_PER_FRAME], &sync_bit);
    assert(sync_bit == 0);

    /* scale and convert shorts */

    for(i=0; i<2*FDMDV_NOM_SAMPLES_PER_FRAME; i++)
    tx_fdm_scaled[i] = FDMDV_SCALE * tx_fdm[i].real;

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

    //printf("%d cb2 ... %d", cb2++,cb1-cb2);

    // assemble a mono buffer (just use left channel) and write to FIFO

    assert(framesPerBuffer < MAX_FPB);

    //if (statusFlags)
    //    printf("statusFlags: 0x%x\n", statusFlags);

    for(i = 0; i < framesPerBuffer; i++, rptr += 2)
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
    //printf("end cb2\n");
    return paContinue;
}

