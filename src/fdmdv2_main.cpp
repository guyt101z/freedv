//==========================================================================
// Name:            fdmdv2_main.cpp
//
// Purpose:         Implements simple wxWidgets application with GUI.
// Created:         Apr. 9, 2012
// Initial author:  David Witten
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

//float  av_mag[FDMDV_NSPEC];                  // shared between a few classes
// initialize the application
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
    if(!loadConfig())
    {
        wxMessageBox(wxT("Unable to open configuration data.  Create New?"), wxT("Configuration"), wxYES_NO | wxCANCEL);
    }
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
// loadConfig()
//-------------------------------------------------------------------------
bool MainApp::loadConfig()
{
    g_config = new wxConfig("FDMDV2");
    wxString str;
    if(g_config->Read("LastPrompt", &str))
    {
        // last prompt was found in the config file/registry and its value is
        // now in str
        // ...
    }
    else
    {
        // no last prompt...
    }
    // another example: using default values and the full path instead of just
    // key name: if the key is not found , the value 17 is returned
    long value = g_config->ReadLong("/LastRun/CalculatedValues/MaxValue", 17);

    // at the end of the program we would save everything back
    g_config->Write("LastPrompt", str);
    g_config->Write("/LastRun/CalculatedValues/MaxValue", value);

    // the changes will be written back automatically
    delete g_config;
    return true;
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
    m_sound             = NULL;
    m_zoom              = 1.;
    m_SquelchActive     = false;
    if(Pa_Initialize())
    {
        wxMessageBox(wxT("Port Audio failed to initialize"), wxT("Pa_Initialize"), wxOK);
    }
    // Add Waterfall Plot window
    m_panelWaterfall = new PlotWaterfall((wxFrame*) m_auiNbookCtrl );
    m_auiNbookCtrl->AddPage(m_panelWaterfall, _("Waterfall"), true, wxNullBitmap );

    // Add Spectrum Plot window
    m_panelSpectrum = new PlotSpectrum((wxFrame*) m_auiNbookCtrl );
    m_auiNbookCtrl->AddPage(m_panelSpectrum, _("Spectrum"), true, wxNullBitmap );

//    m_panelScatter = new PlotScatter((wxFrame*) m_auiNbookCtrl );
//    m_auiNbookCtrl->AddPage(m_panelWaterfall, _("Scatter"), true, wxNullBitmap );

//    m_panelScalar = new PlotScalar((wxFrame*) m_auiNbookCtrl, 500, 500);
//    m_auiNbookCtrl->AddPage(m_panelWaterfall, _("Scalar"), true, wxNullBitmap );

    // Add generic plot window
    m_panelDefaultA = new PlotPanel((wxFrame*) m_auiNbookCtrl );
    m_auiNbookCtrl->AddPage(m_panelDefaultA, _("Test A"), true, wxNullBitmap );
#ifdef USE_TIMER
    m_rxPa = new PortAudioWrap();
    double f = 0.0;
    for(int i = 0; i < FDMDV_NSPEC; i++)
    {
//        m_rxPa->m_av_mag[i] = sin(i) * 100.0;
        f = ((double)i / M_PI);
        f = sin(f);
        f = 100 * f;
//        f = f - 50;
        m_rxPa->m_av_mag[i] = f;
    }
    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);       // ID_MY_WINDOW);
    m_plotTimer.SetOwner(this, ID_TIMER_WATERFALL);
    m_plotTimer.Start(500, wxTIMER_CONTINUOUS);
#endif
}

//-------------------------------------------------------------------------
// ~MainFrame()
//-------------------------------------------------------------------------
MainFrame::~MainFrame()
{
#ifdef USE_TIMER
    if (m_plotTimer.IsRunning())
    {
        m_plotTimer.Stop();
        Unbind(wxEVT_TIMER, &MainFrame::OnTimer, this);   // ID_MY_WINDOW);
    }
#endif
}

#ifdef USE_TIMER
//static int cnt = 3;

//----------------------------------------------------------------
// OnTimer()
//----------------------------------------------------------------
void MainFrame::OnTimer(wxTimerEvent &evt)
{
//    cnt--;
    m_panelWaterfall->m_newdata = true;
    m_panelWaterfall->Refresh();
    m_panelSpectrum->m_newdata = true;
    m_panelSpectrum->Refresh();
//    m_panelDefaultA->m_newdata = true;
//     m_panelDefaultA->Refresh();
}
#endif

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
// Onpa->Paint()
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
    wxMessageBox(wxT("Got Click!"), wxT("OnSliderScrollBottom"), wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnCheckSQClick()
//-------------------------------------------------------------------------
void MainFrame::OnCheckSQClick(wxCommandEvent& event)
{
    if(!m_SquelchActive)
    {
        m_SquelchActive = true;
        //wxMessageBox(wxT("Squelch On!"), wxT("Squelch On"), wxOK);
    }
    else
    {
        m_SquelchActive = false;
        //wxMessageBox(wxT("Squelch Off!"), wxT("Squelch Off"), wxOK);
    }
}

//-------------------------------------------------------------------------
// OnTogBtnTXClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnTXClick(wxCommandEvent& event)
{
    m_soundFile = wxT("./hts1a.wav");
    m_sound = new wxSound(m_soundFile, false);
    m_sound->Play();
}

//-------------------------------------------------------------------------
// OnTogBtnRxID()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnRxID(wxCommandEvent& event)
{
    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnRxID"), wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnTxID()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnTxID(wxCommandEvent& event)
{
    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnTxID"), wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnSplitClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnSplitClick(wxCommandEvent& event)
{
    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnSplitClick"), wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnAnalogClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnAnalogClick (wxCommandEvent& event)
{
    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnAnalogClick"), wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnALCClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnALCClick(wxCommandEvent& event)
{
    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnALCClick"), wxOK);
    event.Skip();
}


//-------------------------------------------------------------------------
// OnTogBtnOnOff()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnOnOff(wxCommandEvent& event)
{
    if((!m_TxRunning) || (!m_RxRunning))
    {
        startRxStream();
        startTxStream();
        m_togBtnOnOff->SetLabel(wxT("Stop"));
    }
    else
    {
        stopRxStream();
        stopTxStream();
        m_togBtnOnOff->SetLabel(wxT("Start"));
    }
}

//-------------------------------------------------------------------------
// startRxStream()
//-------------------------------------------------------------------------
void MainFrame::startRxStream()
{
    if(!m_RxRunning)
    {
        m_RxRunning = true;
        m_rxPa = new PortAudioWrap();

        m_rxDevIn = m_rxPa->getDefaultInputDevice();                   // default input device
        if(m_rxDevIn == paNoDevice)
        {
            wxMessageBox(wxT("Rx Error: No default input device."), wxT("Error"), wxOK);
            return;
        }
        m_rxErr = m_rxPa->setInputDevice(m_rxDevIn);
        m_rxErr = m_rxPa->setInputChannelCount(2);                           // stereo input
        m_rxErr = m_rxPa->setInputSampleFormat(PA_SAMPLE_TYPE);
        m_rxErr = m_rxPa->setInputLatency(m_rxPa->getInputDefaultLowLatency());
        m_rxPa->setInputHostApiStreamInfo(NULL);

        m_rxDevOut = m_rxPa->getDefaultOutputDevice();                 // default output device
        if (m_rxDevOut == paNoDevice)
        {
            wxMessageBox(wxT("Rx Error: No default output device."), wxT("Error"), wxOK);
            return;
        }
        m_rxErr = m_rxPa->setOutputDevice(m_rxDevOut);
        m_rxErr = m_rxPa->setOutputChannelCount(2);                           // stereo input
        m_rxErr = m_rxPa->setOutputSampleFormat(PA_SAMPLE_TYPE);

        m_rxErr = m_rxPa->setOutputLatency(m_rxPa->getOutputDefaultLowLatency());
        m_rxPa->setOutputHostApiStreamInfo(NULL);

        m_rxErr = m_rxPa->setFramesPerBuffer(FRAMES_PER_BUFFER);
        m_rxErr = m_rxPa->setSampleRate(SAMPLE_RATE);
        m_rxErr = m_rxPa->setStreamFlags(0);
        m_rxErr = m_rxPa->setCallback(rxCallback);
        m_rxErr = m_rxPa->streamOpen();

        if(m_rxErr != paNoError)
        {
            wxMessageBox(wxT("Rx Stream Open/Setup error."), wxT("Error"), wxOK);
            return;
        }
        m_rxErr = m_rxPa->streamStart();
        if(m_rxErr != paNoError)
        {
            wxMessageBox(wxT("Rx Stream Start Error."), wxT("Error"), wxOK);
            return;
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
    }
}

//-------------------------------------------------------------------------
// abortRxStream()
//-------------------------------------------------------------------------
void MainFrame::abortRxStream()
{
    if(m_RxRunning)
    {
        m_RxRunning = false;
        m_rxPa->abort();
    }
}

//-------------------------------------------------------------------------
// startTxStream()
//-------------------------------------------------------------------------
void MainFrame::startTxStream()
{
    if(!m_TxRunning)
    {
        m_TxRunning = true;
        m_txPa = new PortAudioWrap();

        m_txDevIn = m_txPa->getDefaultInputDevice();                   // default input device
        if(m_txDevIn == paNoDevice)
        {
            wxMessageBox(wxT("Tx Error: No default input device."), wxT("Error"), wxOK);
            return;
        }
        m_txErr = m_txPa->setInputDevice(m_txDevIn);
        m_txErr = m_txPa->setInputChannelCount(2);                           // stereo input
        m_txErr = m_txPa->setInputSampleFormat(PA_SAMPLE_TYPE);
        m_txErr = m_txPa->setInputLatency(m_txPa->getInputDefaultLowLatency());
        m_txPa->setInputHostApiStreamInfo(NULL);

        m_txDevOut = m_txPa->getDefaultOutputDevice();                 // default output device
        if (m_txDevOut == paNoDevice)
        {
            wxMessageBox(wxT("Tx Error: No default output device."), wxT("Error"), wxOK);
            return;
        }
        m_txErr = m_txPa->setOutputDevice(m_txDevOut);
        m_txErr = m_txPa->setOutputChannelCount(2);                           // stereo input
        m_txErr = m_txPa->setOutputSampleFormat(PA_SAMPLE_TYPE);

        m_txErr = m_txPa->setOutputLatency(m_txPa->getOutputDefaultLowLatency());
        m_txPa->setOutputHostApiStreamInfo(NULL);

        m_txErr = m_txPa->setFramesPerBuffer(FRAMES_PER_BUFFER);
        m_txErr = m_txPa->setSampleRate(SAMPLE_RATE);
        m_txErr = m_txPa->setStreamFlags(0);
        m_txErr = m_txPa->setCallback(txCallback);
        m_txErr = m_txPa->streamOpen();

        if(m_txErr != paNoError)
        {
            wxMessageBox(wxT("Tx Stream Open/Setup error."), wxT("Error"), wxOK);
            return;
        }
        m_txErr = m_txPa->streamStart();
        if(m_txErr != paNoError)
        {
            wxMessageBox(wxT("Tx Stream Start Error."), wxT("Error"), wxOK);
            return;
        }
    }
}

//-------------------------------------------------------------------------
// stopTxStream()
//-------------------------------------------------------------------------
void MainFrame::stopTxStream()
{
    if(m_TxRunning)
    {
        m_TxRunning = false;
        m_txPa->stop();
        m_txPa->streamClose();
    }
}

//-------------------------------------------------------------------------
// abortTxStream()
//-------------------------------------------------------------------------
void MainFrame::abortTxStream()
{
    if(m_TxRunning)
    {
        m_TxRunning = false;
        m_txPa->abort();
    }
}

//-------------------------------------------------------------------------
// OnOpen()
//-------------------------------------------------------------------------
void MainFrame::OnOpen( wxCommandEvent& event )
{
}

//-------------------------------------------------------------------------
// OnOpenUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnOpenUpdateUI( wxUpdateUIEvent& event )
{
//    wxMessageBox("Got Click!", "OnOpenUpdateUI", wxOK);
//    event.Skip();
}

//-------------------------------------------------------------------------
// OnSaveUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnSaveUpdateUI( wxUpdateUIEvent& event )
{
//    wxMessageBox("Got Click!", "OnSaveUpdateUI", wxOK);
//    event.Skip();
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void MainFrame::OnClose( wxCommandEvent& event )
{
    if(m_sound != NULL)
    {
        if(m_sound->IsOk())
        {
            m_sound->Stop();
            m_sound = NULL;
        }
    }
}

//-------------------------------------------------------------------------
// OnCloseUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnCloseUpdateUI( wxUpdateUIEvent& event )
{
}

//-------------------------------------------------------------------------
// OnExit()
//-------------------------------------------------------------------------
void MainFrame::OnExit( wxCommandEvent& event )
{
//    wxMessageBox("Got Click!", "OnExit", wxOK);
//    event.Skip();
}

//-------------------------------------------------------------------------
// OnCopy()
//-------------------------------------------------------------------------
void MainFrame::OnCopy( wxCommandEvent& event )
{
    wxMessageBox("Got Click!", "OnCopy", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnCopyUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnCopyUpdateUI( wxUpdateUIEvent& event )
{
}

//-------------------------------------------------------------------------
// OnCut()
//-------------------------------------------------------------------------
void MainFrame::OnCut( wxCommandEvent& event )
{
    wxMessageBox("Got Click!", "OnCut", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnCutUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnCutUpdateUI( wxUpdateUIEvent& event )
{
}

//-------------------------------------------------------------------------
// OnPaste()
//-------------------------------------------------------------------------
void MainFrame::OnPaste( wxCommandEvent& event )
{
    wxMessageBox("Got Click!", "Onpa->te", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnPasteUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnPasteUpdateUI( wxUpdateUIEvent& event )
{
}

//-------------------------------------------------------------------------
// OnToolsOptions()
//-------------------------------------------------------------------------
void MainFrame::OnToolsOptions( wxCommandEvent& event )
{
    OptionsDlg *dlg = new OptionsDlg(NULL);
    dlg->ShowModal();
}

//-------------------------------------------------------------------------
// OnToolsOptionsUI()
//-------------------------------------------------------------------------
void MainFrame::OnToolsOptionsUI( wxUpdateUIEvent& event )
{
}

//-------------------------------------------------------------------------
// OnCaptureRxStream()
//-------------------------------------------------------------------------
void MainFrame::OnCaptureRxStream( wxCommandEvent& event )
{
}

//-------------------------------------------------------------------------
// OnCaptureTxStream()
//-------------------------------------------------------------------------
void MainFrame::OnCaptureTxStream( wxCommandEvent& event )
{
}

//-------------------------------------------------------------------------
// OnPlayAudioFile()
//-------------------------------------------------------------------------
void MainFrame::OnPlayAudioFile( wxCommandEvent& event )
{
    if(m_sound != NULL)
    {
        if (wxMessageBox(wxT("Current content has not been saved! Proceed?"),wxT("Please confirm"), wxICON_QUESTION | wxYES_NO, this) == wxNO )
        {
            return;
        }
    }
    wxFileDialog openFileDialog(this,
                                wxT("Open Sound file"),
                                wxEmptyString,
                                wxEmptyString,
                                wxT("WAV files (*.wav)|*.wav|")
                                wxT("RAW files (*.raw)|*.raw|")
                                wxT("SPEEX files (*.spx)|*.spx|")
                                wxT("FLAC files (*.flc)|*.flc|")
                                wxT("All files (*.*)|*.*|"),
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;     // the user changed idea...
    }
    // proceed loading the file chosen by the user;
    m_sound->Play(openFileDialog.GetPath());
}

//-------------------------------------------------------------------------
// OnToolsAudio()
//-------------------------------------------------------------------------
void MainFrame::OnToolsAudio( wxCommandEvent& event )
{
    AudioDlg *dlg = new AudioDlg(NULL);
    dlg->ShowModal();
}

//-------------------------------------------------------------------------
// OnToolsAudioUI()
//-------------------------------------------------------------------------
void MainFrame::OnToolsAudioUI( wxUpdateUIEvent& event )
{
}

//-------------------------------------------------------------------------
// OnToolsComCfg()
//-------------------------------------------------------------------------
void MainFrame::OnToolsComCfg( wxCommandEvent& event )
{
    ComPortsDlg *dlg = new ComPortsDlg(NULL);
    dlg->ShowModal();
}

//-------------------------------------------------------------------------
// OnToolsComCfgUI()
//-------------------------------------------------------------------------
void MainFrame::OnToolsComCfgUI( wxUpdateUIEvent& event )
{
    event.Enable((!m_TxRunning) && (!m_RxRunning));
}

//-------------------------------------------------------------------------
// OnHelpCheckUpdates()
//-------------------------------------------------------------------------
void MainFrame::OnHelpCheckUpdates( wxCommandEvent& event )
{
    wxMessageBox("Got Click!", "OnHelpCheckUpdates", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnHelpCheckUpdatesUI()
//-------------------------------------------------------------------------
void MainFrame::OnHelpCheckUpdatesUI( wxUpdateUIEvent& event )
{
}

//-------------------------------------------------------------------------
//OnHelpAbout()
//-------------------------------------------------------------------------
void MainFrame::OnHelpAbout( wxCommandEvent& event )
{
    AboutDlg *dlg = new AboutDlg(NULL);
    dlg->ShowModal();
}

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

//-------------------------------------------------------------------------
// OnSave()
//-------------------------------------------------------------------------
void MainFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
    wxString savefilename = wxFileSelector(wxT("Save Sound File"),
                                           wxEmptyString,
                                           wxEmptyString,
                                           (const wxChar *)NULL,
                                           wxT("WAV files (*.wav)|*.wav|")
                                           wxT("RAW files (*.raw)|*.raw|")
                                           wxT("SPEEX files (*.spx)|*.spx|")
                                           wxT("FLAC files (*.flc)|*.flc|"),
                                           wxFD_SAVE,
                                           this);
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
}

//-------------------------------------------------------------------------
// rxCallback()
//-------------------------------------------------------------------------
int MainFrame::rxCallback(
                            const void *inBuffer,
                            void *outBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *outTime,
                            PaStreamCallbackFlags statusFlags,
                            void *userData
                         )
{
    float *out = (float *) outBuffer;
    float *in  = (float *) inBuffer;
    float leftIn;
    float rightIn;
    unsigned int i;

    if(inBuffer == NULL)
    {
        return 0;
    }
    // Read input buffer, process data, and fill output buffer.
    for(i = 0; i < framesPerBuffer; i++)
    {
        leftIn  = *in++;                            // Get interleaved samples from input buffer.
        rightIn = *in++;
        *out++  = leftIn * rightIn;                 // ring modulation
        *out++  = 0.5f * (leftIn + rightIn);        // mixing
    }
    return paContinue;                              // 0;
}

//-------------------------------------------------------------------------
// txCallback()
//-------------------------------------------------------------------------
int MainFrame::txCallback(
                            const void *inBuffer,
                            void *outBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *outTime,
                            PaStreamCallbackFlags statusFlags,
                            void *userData
                         )
{
    float *out = (float *) outBuffer;
    float *in  = (float *) inBuffer;
    float leftIn;
    float rightIn;
    unsigned int i;

    if(inBuffer == NULL)
    {
        return 0;
    }
    // Read input buffer, process data, and fill output buffer.
    for(i = 0; i < framesPerBuffer; i++)
    {
        leftIn  = *in++;                            // Get interleaved samples from input buffer.
        rightIn = *in++;
        *out++  = leftIn * rightIn;                 // ring modulation
        *out++  = 0.5f * (leftIn + rightIn);        // mixing
    }
    return paContinue;                              // 0;
}

//----------------------------------------------------------------
// update average of each spectrum point
//----------------------------------------------------------------
void MainFrame::averageData(float mag_dB[])
{
    int i;

    for(i = 0; i < FDMDV_NSPEC; i++)
    {
        m_rxPa->m_av_mag[i] = (1.0 - BETA) * m_rxPa->m_av_mag[i] + BETA * mag_dB[i];
    }
}

//----------------------------------------------------------------
// per_frame_rx_processing()
//----------------------------------------------------------------
void MainFrame::per_frame_rx_processing(
                                            short   output_buf[],  // output buf of decoded speech samples
                                            int     *n_output_buf, // how many samples currently in output_buf[]
                                            int     codec_bits[],  // current frame of bits for decoder
                                            short   input_buf[],   // input buf of modem samples input to demod
                                            int     *n_input_buf,  // how many samples currently in input_buf[]
                                            int     *nin,          // amount of samples demod needs for next call
                                            int     *state,        // used to collect codec_bits[] halves
                                            struct  CODEC2 *c2     // Codec 2 states
                                       )
{
    struct FDMDV_STATS  stats;
    int                 sync_bit;
    float               rx_fdm[FDMDV_MAX_SAMPLES_PER_FRAME];
    int                 rx_bits[FDMDV_BITS_PER_FRAME];
    unsigned char       packed_bits[BYTES_PER_CODEC_FRAME];
    float               rx_spec[FDMDV_NSPEC];
    int                 i;
    int                 nin_prev;
    int                 bit;
    int                 byte;
    int                 next_state;

    assert(*n_input_buf <= (2 * FDMDV_NOM_SAMPLES_PER_FRAME));

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
    //  ppm), case 0 or 1 occured about once every 30 seconds.  This is
    //  no problem for the decoded audio.
    //
    while(*n_input_buf >= *nin)
    {
        // demod per frame processing
        for(i = 0; i < *nin; i++)
        {
            rx_fdm[i] = (float)input_buf[i]/FDMDV_SCALE;
        }
        nin_prev = *nin;
        fdmdv_demod(m_pFDMDV_state, rx_bits, &sync_bit, rx_fdm, nin);
        *n_input_buf -= nin_prev;
        assert(*n_input_buf >= 0);

        // shift input buffer
        for(i = 0; i < *n_input_buf; i++)
        {
            input_buf[i] = input_buf[i+nin_prev];
        }

        // compute rx spectrum & get demod stats, and update GUI plot data
        fdmdv_get_rx_spectrum(m_pFDMDV_state, rx_spec, rx_fdm, nin_prev);
        fdmdv_get_demod_stats(m_pFDMDV_state, &stats);
        averageData(rx_spec);
        //m_panelWaterfall;
        //m_panelScalar;
//        m_panelScatter->add_new_samples(stats.rx_symbols);
//        aTimingEst->add_new_sample(stats.rx_timing);
//        aFreqEst->add_new_sample(stats.foff);
//        aSNR->add_new_sample(stats.snr_est);
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
                if(*n_output_buf < 2 * codec2_samples_per_frame(c2) - N8)
                {
                    for(i=0; i<N8; i++)
                    {
                        output_buf[*n_output_buf + i] = 0;
                    }
                    *n_output_buf += N8;
                }
                assert(*n_output_buf <= (2 * codec2_samples_per_frame(c2)));
                if((stats.fest_coarse_fine == 1) && (stats.snr_est > 3.0))
                {
                    next_state = 1;
                }
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
                if(stats.fest_coarse_fine == 0)
                {
                    next_state = 0;
                }
                break;

            case 2:
                next_state = 1;
                if(stats.fest_coarse_fine == 0)
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
                    if(*n_output_buf <= codec2_samples_per_frame(c2))
                    {
                        codec2_decode(c2, &output_buf[*n_output_buf], packed_bits);
                        *n_output_buf += codec2_samples_per_frame(c2);
                    }
                    assert(*n_output_buf <= (2 * codec2_samples_per_frame(c2)));
                }
                break;
        }
        *state = next_state;
    }
}
