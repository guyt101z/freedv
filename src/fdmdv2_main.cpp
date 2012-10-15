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
    m_sound             = NULL;
    m_zoom              = 1.;
    m_SquelchActive     = false;
    if(Pa_Initialize())
    {
        wxMessageBox(wxT("Port Audio failed to initialize"), wxT("Pa_Initialize"), wxOK);
    }

    tools->AppendSeparator();
    wxMenuItem* m_menuItemToolsConfigDelete;
    m_menuItemToolsConfigDelete = new wxMenuItem(tools, wxID_ANY, wxString(_("&Delete stored config")) , wxT("Delete config file/keys"), wxITEM_NORMAL);
    this->Connect(m_menuItemToolsConfigDelete->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnDeleteConfig));

    tools->Append(m_menuItemToolsConfigDelete);

    // Add Waterfall Plot window
    m_panelWaterfall = new PlotWaterfall((wxFrame*) m_auiNbookCtrl);
    m_auiNbookCtrl->AddPage(m_panelWaterfall, _("Waterfall"), true, wxNullBitmap);

    // Add Spectrum Plot window
    m_panelSpectrum = new PlotSpectrum((wxFrame*) m_auiNbookCtrl);
    m_auiNbookCtrl->AddPage(m_panelSpectrum, _("Spectrum"), true, wxNullBitmap);

//    m_panelScatter = new PlotScatter((wxFrame*) m_auiNbookCtrl);
//    m_auiNbookCtrl->AddPage(m_panelWaterfall, _("Scatter"), true, wxNullBitmap);

//    m_panelScalar = new PlotScalar((wxFrame*) m_auiNbookCtrl, 500, 500);
//    m_auiNbookCtrl->AddPage(m_panelWaterfall, _("Scalar"), true, wxNullBitmap);

    // Add generic plot window
//    m_panelDefaultA = new PlotPanel((wxFrame*) m_auiNbookCtrl);
//    m_auiNbookCtrl->AddPage(m_panelDefaultA, _("Test A"), true, wxNullBitmap);

    wxConfigBase *pConfig = wxConfigBase::Get();

    // restore frame position and size
    int x = pConfig->Read(wxT("/MainFrame/top"),       50);
    int y = pConfig->Read(wxT("/MainFrame/left"),      50);
    int w = pConfig->Read(wxT("/MainFrame/width"),     650);
    int h = pConfig->Read(wxT("/MainFrame/height"),    400);
    Move(x, y);
    SetClientSize(w, h);

    wxGetApp().m_strRxInAudio       = pConfig->Read(wxT("/Audio/RxIn"),         wxT("<m_strRxInAudio>"));
    wxGetApp().m_strRxOutAudio      = pConfig->Read(wxT("/Audio/RxOut"),        wxT("<m_strRxOutAudio>"));
    wxGetApp().m_textVoiceInput     = pConfig->Read(wxT("/Audio/TxIn"),         wxT("<m_textVoiceInput>"));
    wxGetApp().m_textVoiceOutput    = pConfig->Read(wxT("/Audio/TxOut"),        wxT("<m_textVoiceOutput>"));
    wxGetApp().m_strSampleRate      = pConfig->Read(wxT("/Audio/SampleRate"),   wxT("48000"));
//    wxGetApp().m_strSampleRate      = pConfig->Read(wxT("/Audio/SampleRate"),   wxT("48000"));
//    wxGetApp().m_strSampleRate      = pConfig->Read(wxT("/Audio/SampleRate"),   wxT("48000"));

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
    m_togBtnALC->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnALCClickUI), NULL, this);
    m_btnTogTX->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTXClickUI), NULL, this);

    m_togBtnSplit->Disable();
    m_togRxID->Disable();
    m_togTxID->Disable();
    m_togBtnAnalog->Disable();
    m_togBtnALC->Disable();
    m_btnTogTX->Disable();

#ifdef _USE_TIMER
    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);       // ID_MY_WINDOW);
    m_plotTimer.SetOwner(this, ID_TIMER_WATERFALL);
//    m_rxPa = new PortAudioWrap();
//    m_plotTimer.Start(500, wxTIMER_CONTINUOUS);
//    m_panelWaterfall->m_newdata = true;
    m_panelWaterfall->Refresh();
#else
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
        pConfig->Write(wxT("/MainFrame/top"),       (long) x);
        pConfig->Write(wxT("/MainFrame/left"),      (long) y);
        pConfig->Write(wxT("/MainFrame/width"),     (long) w);
        pConfig->Write(wxT("/MainFrame/height"),    (long) h);

        pConfig->Write(wxT("/Audio/RxIn"),          wxGetApp().m_strRxInAudio);
        pConfig->Write(wxT("/Audio/RxOut"),         wxGetApp().m_strRxOutAudio);
        pConfig->Write(wxT("/Audio/TxIn"),          wxGetApp().m_textVoiceInput);
        pConfig->Write(wxT("/Audio/TxOut"),         wxGetApp().m_textVoiceOutput);
        pConfig->Write(wxT("/Audio/SampleRate"),    wxGetApp().m_strSampleRate);

        pConfig->Write(wxT("/Rig/Port"),            wxGetApp().m_strRigCtrlPort);
        pConfig->Write(wxT("/Rig/Baud"),            wxGetApp().m_strRigCtrlBaud);
        pConfig->Write(wxT("/Rig/DataBits"),        wxGetApp().m_strRigCtrlDatabits);
        pConfig->Write(wxT("/Rig/StopBits"),        wxGetApp().m_strRigCtrlStopbits);
        pConfig->Write(wxT("/Rig/Parity"),          wxGetApp().m_strRigCtrlParity);
    }
    m_togRxID->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnRxIDUI), NULL, this);
    m_togTxID->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTxIDUI), NULL, this);
    m_togBtnOnOff->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnOnOffUI), NULL, this);
    m_togBtnSplit->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnSplitClickUI), NULL, this);
    m_togBtnAnalog->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnAnalogClickUI), NULL, this);
    m_togBtnALC->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnALCClickUI), NULL, this);
    m_btnTogTX->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTXClickUI), NULL, this);

#ifdef _USE_TIMER
    if(m_plotTimer.IsRunning())
    {
        m_plotTimer.Stop();
        Unbind(wxEVT_TIMER, &MainFrame::OnTimer, this);
    }
#else
    Disconnect(wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle), NULL, this);
#endif //_USE_TIMER

    delete wxConfigBase::Set((wxConfigBase *) NULL);
}

#ifdef _USE_TIMER

//----------------------------------------------------------------
// OnTimer()
//----------------------------------------------------------------
void MainFrame::OnTimer(wxTimerEvent &evt)
{
    m_panelWaterfall->m_newdata = true;
    m_panelWaterfall->Refresh();
    m_panelSpectrum->m_newdata = true;
    m_panelSpectrum->Refresh();
//    m_panelDefaultA->m_newdata = true;
//    m_panelDefaultA->Refresh();
}

#else

//----------------------------------------------------------------
// OnIdle()
//----------------------------------------------------------------
void MainFrame::OnIdle(wxIdleEvent& event)
{
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
//    wxMessageBox(wxT("Got Click!"), wxT("OnSliderScrollBottom"), wxOK);
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
    }
    else
    {
        m_SquelchActive = false;
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
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnTxID()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnTxID(wxCommandEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnSplitClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnSplitClick(wxCommandEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnAnalogClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnAnalogClick (wxCommandEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnALCClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnALCClick(wxCommandEvent& event)
{
//    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnALCClick"), wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnSplitClickUI()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnSplitClickUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
//    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnTogBtnAnalogClickUI()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnAnalogClickUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnTogBtnALCClickUI()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnALCClickUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnTogBtnRxIDUI()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnRxIDUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnTogBtnTxIDUI()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnTxIDUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnTogBtnTXClickUI()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnTXClickUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnOpenUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnOpenUpdateUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnSaveUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnSaveUpdateUI(wxUpdateUIEvent& event)
{
//    wxUnusedVar(event);
    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void MainFrame::OnClose(wxCommandEvent& event)
{
    wxUnusedVar(event);

    if(m_RxRunning)
    {
        stopRxStream();
    }
    if(m_TxRunning)
    {
        stopTxStream();
    }

    if(m_sound != NULL)
    {
        if(m_sound->IsOk())
        {
            m_sound->Stop();
            m_sound = NULL;
        }
    }
    Close();
}

//-------------------------------------------------------------------------
// OnCloseUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnCloseUpdateUI(wxUpdateUIEvent& event)
{
    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnExit()
//-------------------------------------------------------------------------
void MainFrame::OnExit(wxCommandEvent& event)
{
    OnClose(event);
}

//-------------------------------------------------------------------------
// OnCopy()
//-------------------------------------------------------------------------
void MainFrame::OnCopy(wxCommandEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnCopyUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnCopyUpdateUI(wxUpdateUIEvent& event)
{
    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnCut()
//-------------------------------------------------------------------------
void MainFrame::OnCut(wxCommandEvent& event)
{
    event.Skip();
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
// OnPaste()
//-------------------------------------------------------------------------
void MainFrame::OnPaste(wxCommandEvent& event)
{
    event.Skip();
}

//-------------------------------------------------------------------------
// OnPasteUpdateUI()
//-------------------------------------------------------------------------
void MainFrame::OnPasteUpdateUI(wxUpdateUIEvent& event)
{
    event.Enable(false);
}

//-------------------------------------------------------------------------
// OnCaptureRxStream()
//-------------------------------------------------------------------------
void MainFrame::OnCaptureRxStream(wxCommandEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnCaptureTxStream()
//-------------------------------------------------------------------------
void MainFrame::OnCaptureTxStream(wxCommandEvent& event)
{
    wxUnusedVar(event);
}

//-------------------------------------------------------------------------
// OnPlayAudioFile()
//-------------------------------------------------------------------------
void MainFrame::OnPlayAudioFile(wxCommandEvent& event)
{
    wxUnusedVar(event);
    if(m_sound != NULL)
    {
        if (wxMessageBox(wxT("Current content has not been saved! Proceed?"),wxT("Please confirm"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
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
        return;     // the user changed their mind...
    }
    // proceed loading the file chosen by the user;
    m_sound->Play(openFileDialog.GetPath());
}

//-------------------------------------------------------------------------
// OnToolsAudio()
//-------------------------------------------------------------------------
void MainFrame::OnToolsAudio(wxCommandEvent& event)
{
    wxUnusedVar(event);
    int rv = 0;
    AudioDlg *dlg = new AudioDlg(NULL);
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
// OnTogBtnOnOffUI()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnOnOffUI(wxUpdateUIEvent& event)
{
    wxUnusedVar(event);
}

//----------------------------------------------------------
// Global Codec2 thingys - just one reqd for tx & rx
//----------------------------------------------------------
struct CODEC2   *g_pCodec2;
struct FDMDV    *g_pFDMDV;

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
        m_togBtnALC->Enable();
        m_btnTogTX->Enable();

        g_pFDMDV  = fdmdv_create();
        g_pCodec2 = codec2_create(CODEC2_MODE_1400);

#ifdef _USE_TIMER
	// DR: disable this puppy for now as it's causing a lot of error messages
        //m_plotTimer.Start(500, wxTIMER_CONTINUOUS);
#endif // _USE_TIMER
        startRxStream();
//        startTxStream();
        if (m_RxRunning)
	    m_togBtnOnOff->SetLabel(wxT("Stop"));
    }
    else
    {
	printf("stopping ...\n");
        m_togBtnSplit->Disable();
        m_togRxID->Disable();
        m_togTxID->Disable();
        m_togBtnAnalog->Disable();
        m_togBtnALC->Disable();
        m_btnTogTX->Disable();
#ifdef _USE_TIMER
        //m_plotTimer.Stop();
#endif // _USE_TIMER
        stopRxStream();
//        stopTxStream();
        m_togBtnOnOff->SetLabel(wxT("Start"));
    }
}

//----------------------------------------------------------
// Audio stream processing loop states (globals).
//----------------------------------------------------------
/*
float  Ts = 0.0;
short  input_buf[2*FDMDV_NOM_SAMPLES_PER_FRAME];
short *output_buf;
int    n_input_buf = 0;
int    nin = FDMDV_NOM_SAMPLES_PER_FRAME;
int    n_output_buf = 0;
int    codec_bits[2*FDMDV_BITS_PER_FRAME];
int    state = 0;
*/

int             g_nRxIn = FDMDV_NOM_SAMPLES_PER_FRAME;
float           g_Ts = 0.0;

short           g_RxInBuf[2 * FDMDV_NOM_SAMPLES_PER_FRAME];
short           *g_pRxOutBuf;
int             g_nInputBuf = 0;

short           g_TxInBuf[2 * FDMDV_NOM_SAMPLES_PER_FRAME];
short           *g_pTxOutBuf;
int             g_nOutputBuf = 0;


int             g_CodecBits[2 * FDMDV_BITS_PER_FRAME];
int             g_State = 0;

float           g_avmag[FDMDV_NSPEC];


//-------------------------------------------------------------------------
// startRxStream()
//-------------------------------------------------------------------------
void MainFrame::startRxStream()
{
    if(!m_RxRunning)
    {
        m_RxRunning = true;
        m_rxPa = new PortAudioWrap();

#ifdef _DUMMY_DATA
        for(int i = 0; i < FDMDV_NSPEC; i++)
        {
//            m_rxPa->m_av_mag[i] = sin(((double)i / M_PI)) * 100.0;
            g_avmag[i] = sin(((double)i / M_PI)) * 100.0;
        }
#else
        for(int i = 0; i < FDMDV_NSPEC; i++)
        {
//            m_rxPa->m_av_mag[i] = -40.0;
            g_avmag[i] = -40.0;
        }
#endif // _DUMMY_DATA

        g_pRxOutBuf = (short*)malloc(2 * sizeof(short) * codec2_samples_per_frame(g_pCodec2));

        m_rxDevIn = m_rxPa->getDefaultInputDevice();                        // default input device
        if(m_rxDevIn == paNoDevice)
        {
            wxMessageBox(wxT("Rx Error: No default input device."), wxT("Error"), wxOK);
	    delete m_rxPa;
	    m_RxRunning = false;
	    return;
        }
        m_rxErr = m_rxPa->setInputDevice(m_rxDevIn);
        m_rxErr = m_rxPa->setInputChannelCount(2);                          // stereo input
        m_rxErr = m_rxPa->setInputSampleFormat(PA_SAMPLE_TYPE);
        m_rxErr = m_rxPa->setInputLatency(m_rxPa->getInputDefaultLowLatency());
        m_rxPa->setInputHostApiStreamInfo(NULL);

        m_rxDevOut = m_rxPa->getDefaultOutputDevice();                      // default output device
        if (m_rxDevOut == paNoDevice)
        {
            wxMessageBox(wxT("Rx Error: No default output device."), wxT("Error"), wxOK);
	    delete m_rxPa;
	    m_RxRunning = false;	    
            return;
        }
        m_rxErr = m_rxPa->setOutputDevice(m_rxDevOut);
        m_rxErr = m_rxPa->setOutputChannelCount(2);                         // stereo input
        m_rxErr = m_rxPa->setOutputSampleFormat(PA_SAMPLE_TYPE);

        m_rxErr = m_rxPa->setOutputLatency(m_rxPa->getOutputDefaultLowLatency());
        m_rxPa->setOutputHostApiStreamInfo(NULL);

	m_rxErr = m_rxPa->setFramesPerBuffer(PA_FPB);
        m_rxErr = m_rxPa->setSampleRate(SAMPLE_RATE);
        m_rxErr = m_rxPa->setStreamFlags(0);

        m_rxUserdata = new paCallBackData;
        m_rxUserdata->pWFPanel = m_panelWaterfall;
        m_rxUserdata->pSPPanel = m_panelSpectrum;

        for(int i = 0; i < MEM8; i++)
        {
            m_rxUserdata->in8k[i] = 0.0;
        }
        for(int i = 0; i < FDMDV_OS_TAPS; i++)
        {
            m_rxUserdata->in48k[i] = 0.0;
        }

	m_rxUserdata->infifo = fifo_create(2*N48);
	m_rxUserdata->outfifo = fifo_create(2*N48);

        m_rxPa->setUserData(m_rxUserdata);
        m_rxErr = m_rxPa->setCallback(rxCallback);
        m_rxErr = m_rxPa->streamOpen();

        if(m_rxErr != paNoError)
        {
            wxMessageBox(wxT("Rx Stream Open/Setup error."), wxT("Error"), wxOK);
 	    delete m_rxPa;
	    fifo_destroy(m_rxUserdata->infifo);
	    fifo_destroy(m_rxUserdata->outfifo);
	    m_RxRunning = false;	    
	    return;
        }
        m_rxErr = m_rxPa->streamStart();
        if(m_rxErr != paNoError)
        {
            wxMessageBox(wxT("Rx Stream Start Error."), wxT("Error"), wxOK);
  	    delete m_rxPa;
	    fifo_destroy(m_rxUserdata->infifo);
	    fifo_destroy(m_rxUserdata->outfifo);
	    m_RxRunning = false;	    
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
        fdmdv_destroy(g_pFDMDV);
        codec2_destroy(g_pCodec2);
//        delete g_RxInBuf;
	fifo_destroy(m_rxUserdata->infifo);
	fifo_destroy(m_rxUserdata->outfifo);
        delete m_rxUserdata;
    }
/*
        if(m_rxPa->isActive())
        {
            m_rxPa->stop();
            m_rxPa->streamClose();
        }
        if(m_rxPa->isOpen())
        {
            m_rxPa->streamClose();
        }
        m_TxRunning = false;
*/
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

        m_txDevIn = m_txPa->getDefaultInputDevice();                        // default input device
        if(m_txDevIn == paNoDevice)
        {
            wxMessageBox(wxT("Tx Error: No default input device."), wxT("Error"), wxOK);
            return;
        }

        g_pTxOutBuf = (short*)malloc(2*sizeof(short)*codec2_samples_per_frame(g_pCodec2));

        m_txErr = m_txPa->setInputDevice(m_txDevIn);
        m_txErr = m_txPa->setInputChannelCount(2);                          // stereo input
        m_txErr = m_txPa->setInputSampleFormat(PA_SAMPLE_TYPE);
        m_txErr = m_txPa->setInputLatency(m_txPa->getInputDefaultLowLatency());
        m_txPa->setInputHostApiStreamInfo(NULL);

        m_txDevOut = m_txPa->getDefaultOutputDevice();                      // default output device
        if (m_txDevOut == paNoDevice)
        {
            wxMessageBox(wxT("Tx Error: No default output device."), wxT("Error"), wxOK);
            return;
        }
        m_txErr = m_txPa->setOutputDevice(m_txDevOut);
        m_txErr = m_txPa->setOutputChannelCount(2);                         // stereo input
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
/*
        if(m_txPa->isActive())
        {
            m_txPa->stop();
        }
        if(m_txPa->isOpen())
        {
            m_txPa->streamClose();
        }
        m_TxRunning = false;
*/
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
void MainFrame::OnOpen(wxCommandEvent& event)
{
    wxUnusedVar(event);
}

//----------------------------------------------------------------
// update average of each spectrum point
//----------------------------------------------------------------
void MainFrame::averageData(float mag_dB[])
{
    int i;

    for(i = 0; i < FDMDV_NSPEC; i++)
    {
//        m_rxPa->m_av_mag[i] = (1.0 - BETA) * m_rxPa->m_av_mag[i] + BETA * mag_dB[i];
        g_avmag[i] = (1.0 - BETA) * g_avmag[i] + BETA * mag_dB[i];
    }
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
    unsigned int    i;
    short           *rptr    = (short*)inputBuffer;
    short           *wptr    = (short*)outputBuffer;
    float           *in8k    = cbData->in8k;
    float           *in48k   = cbData->in48k;
    float           out8k[N8];
    float           out48k[N48];
    short           out48k_short[N48];
    short           in48k_short[N48];
    short           indata[MAX_FPB];
    short           outdata[MAX_FPB];

    (void) timeInfo;
    (void) statusFlags;

    assert(inputBuffer != NULL);
    assert(outputBuffer != NULL);

    /* 
       framesPerBuffer is portaudio-speak for number of samples we
       actually get from the record side and need to provide to the
       play side. On Linux (at least) it was found that
       framesPerBuffer may not always be what we ask for in the
       framesPerBuffer field of Pa_OpenStream.  For example a request
       for 960 sample buffers lead to framesPerBuffer = 1024.

       To perform the 48 to 8 kHz conversion we need an integer
       multiple of FDMDV_OS samples to support the interpolation and
       decimation.  As we can't guarantee the size of framesPerBuffer
       we do a little FIFO buffering.
    */

    /* assemble a mono buffer (just use left channel) and write to FIFO */

    assert(framesPerBuffer < MAX_FPB);
    for(i=0; i<framesPerBuffer; i++,rptr+=2)
	indata[i] = *rptr;
    fifo_write(cbData->infifo, indata, framesPerBuffer);

    /* while we have enough samples available ... */

    while (fifo_read(cbData->infifo, in48k_short, N48) == 0) {

	/* convert to float */

	for(i=0; i<N48; i++)
	    in48k[FDMDV_OS_TAPS + i] = in48k_short[i];

	// downsample and update filter memory
	fdmdv_48_to_8(out8k, &in48k[FDMDV_OS_TAPS], N8);
	for(i = 0; i < FDMDV_OS_TAPS; i++)
	{
	    in48k[i] = in48k[i + N48];
	}

	assert((g_nInputBuf + N8) <= 2 * FDMDV_NOM_SAMPLES_PER_FRAME);
	// run demod, decoder and update GUI info
	for(i = 0; i < N8; i++)
	{
	    g_RxInBuf[g_nInputBuf + i] = (short)out8k[i];
	}
	g_nInputBuf += FDMDV_NOM_SAMPLES_PER_FRAME;
	per_frame_rx_processing(g_pRxOutBuf, &g_nOutputBuf, g_CodecBits, g_RxInBuf, &g_nInputBuf, &g_nRxIn, &g_State, g_pCodec2);
    
	//cbData->pWFPanel->m_newdata = true;
	//cbData->pSPPanel->m_newdata = true;
    
	// if demod out of sync copy input audio from A/D to aid in tuning
	if (g_nOutputBuf >= N8)
        {
	    if(g_State == 0)
            {
		for(i = 0; i < N8; i++)
                {
		    in8k[MEM8 + i] = out8k[i];       // A/D signal
		}
	    }
	    else
	    {
		for(i = 0; i < N8; i++)
                {
		    in8k[MEM8+i] = g_pRxOutBuf[i];   // decoded spech
		}
	    }
	    g_nOutputBuf -= N8;
	}
	assert(g_nOutputBuf >= 0);
	// shift speech samples in output buffer
	for(i = 0; i < (unsigned int)g_nOutputBuf; i++)
        {
	    g_pRxOutBuf[i] = g_pRxOutBuf[i + N8];
	}
  
	/* test: echo input to output, make this loopback option */
	for(i=0; i<N8; i++)
	    in8k[MEM8+i] = out8k[i];

	// Convert output speech to 48 kHz sample rate
	// upsample and update filter memory
	fdmdv_8_to_48(out48k, &in8k[MEM8], N8);
	for(i = 0; i < MEM8; i++)
        {
	    in8k[i] = in8k[i + N8];
        }
	assert(outputBuffer != NULL);

	// write signal to fifo
	for(i = 0; i < N48; i++)
        {
	    out48k_short[i] = (short)out48k[i];
	}

	fifo_write(cbData->outfifo, out48k_short, N48);
    }

    /* OK now set up output samples */

    if (fifo_read(cbData->outfifo, outdata, framesPerBuffer) == 0) {

	/* write signal to both channels */

	for(i=0; i<framesPerBuffer; i++,wptr+=2) {
	    wptr[0] = outdata[i]; 
	    wptr[1] = outdata[i]; 
	}
    }
    else {
	//printf("no data\n");
	/* zero output if no data available */
	for(i=0; i<framesPerBuffer; i++,wptr+=2) {
	    wptr[0] = 0; 
	    wptr[1] = 0; 
	}
    }

    return paContinue;
}

//----------------------------------------------------------------
// per_frame_rx_processing()
//----------------------------------------------------------------
 void MainFrame::per_frame_rx_processing(
                                            short   output_buf[],   // output buf of decoded speech samples
                                            int     *n_output_buf,  // how many samples currently in output_buf[]
                                            int     codec_bits[],   // current frame of bits for decoder
                                            short   input_buf[],    // input buf of modem samples input to demod
                                            int     *n_input_buf,   // how many samples currently in input_buf[]
                                            int     *nin,           // amount of samples demod needs for next call
                                            int     *state,         // used to collect codec_bits[] halves
                                            CODEC2  *c2             // Codec 2 states
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
            rx_fdm[i] = (float)input_buf[i] / FDMDV_SCALE;
        }
        nin_prev = *nin;
        fdmdv_demod(g_pFDMDV, rx_bits, &sync_bit, rx_fdm, nin);
        *n_input_buf -= nin_prev;
        assert(*n_input_buf >= 0);

        // shift input buffer
        for(i = 0; i < *n_input_buf; i++)
        {
            input_buf[i] = input_buf[i + nin_prev];
        }

        // compute rx spectrum & get demod stats, and update GUI plot data
        fdmdv_get_rx_spectrum(g_pFDMDV, rx_spec, rx_fdm, nin_prev);
        fdmdv_get_demod_stats(g_pFDMDV, &stats);
        // Average Data
        // averageData(rx_spec);
        for(i = 0; i < FDMDV_NSPEC; i++)
        {
            // m_rxPa->m_av_mag[i] = (1.0 - BETA) * m_rxPa->m_av_mag[i] + BETA * rx_spec[i];
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
                if(*n_output_buf < 2 * codec2_samples_per_frame(c2) - N8)
                {
                    for(i = 0; i < N8; i++)
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

//-------------------------------------------------------------------------
// OnSave()
//-------------------------------------------------------------------------
void MainFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
/*
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
*/
}

/*
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
#ifdef _AUDIO_PASSTHROUGH
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
#else   // _AUDIO_PASSTHROUGH
//    per_frame_rx_processing((short *) outBuffer, &framesPerBuffer, int codec_bits[], (short *) inBuffer, &framesPerBuffer, int *nin, int *state, m_rxPa);//
//    per_frame_rx_processing(outBuffer, &framesPerBuffer, int codec_bits[], inBuffer, &framesPerBuffer, int *nin, int *state, m_rxPa);
#endif  // _AUDIO_PASSTHROUGH
    return paContinue;                              // 0;
}
*/
