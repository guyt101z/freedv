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

//----------------------------------------------------------
// Global Codec2 & modem states - just one reqd for tx & rx
//----------------------------------------------------------
struct CODEC2      *g_pCodec2;
struct FDMDV       *g_pFDMDV;

struct FDMDV_STATS  g_stats;

int g_nSoundCards = 2;
int g_soundCard1DeviceNum = 0;
int g_soundCard2DeviceNum = 1;

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
//    m_sound             = NULL;
    m_sfFile            = NULL;
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
    if(wxGetApp().m_show_timing)
    {
        // Add Timing Offset window
        m_panelTimeOffset = new PlotScalar((wxFrame*) m_auiNbookCtrl);
        m_auiNbookCtrl->AddPage(m_panelTimeOffset, L"Timing \u0394", true, wxNullBitmap);
    }
    if(wxGetApp().m_show_freq)
    {
        // Add Frequency Offset window
        m_panelFreqOffset = new PlotScalar((wxFrame*) m_auiNbookCtrl);
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
    m_togBtnALC->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnALCClickUI), NULL, this);
    m_btnTogTX->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnTXClickUI), NULL, this);

    m_togBtnSplit->Disable();
    m_togRxID->Disable();
    m_togTxID->Disable();
    m_togBtnAnalog->Disable();
    m_togBtnALC->Disable();
    m_btnTogTX->Disable();
    m_togBtnLoopRx->Disable();
    m_togBtnLoopTx->Disable();

//    m_menuItemPlayAudioFile->Enable(false);

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
    m_togBtnALC->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrame::OnTogBtnALCClickUI), NULL, this);
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
void MainFrame::OnTimer(wxTimerEvent &evt)
{
    m_panelWaterfall->m_newdata = true;
    m_panelWaterfall->Refresh();
    m_panelSpectrum->m_newdata = true;
    m_panelSpectrum->Refresh();

    m_panelScatter->add_new_samples(g_stats.rx_symbols);
    m_panelScatter->m_newdata = true;
    m_panelScatter->Refresh();
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
    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnALCClick"), wxOK);

    event.Skip();
}

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
    if(m_TxRunning)
    {
        stopTxStream();
    }
    m_togBtnSplit->Disable();
    m_togRxID->Disable();
    m_togTxID->Disable();
    m_togBtnAnalog->Disable();
    m_togBtnALC->Disable();
    m_btnTogTX->Disable();
    m_togBtnLoopRx->Disable();
    m_togBtnLoopTx->Disable();
/*
    This is an artifact of my early use of the default wxWidgets
    sound facility. the wxSound interface is simplistic and and
    unsuitable for this project's purposes.

    if(m_sound != NULL)
    {
        if(m_sound->IsOk())
        {
            m_sound->Stop();
            m_sound = NULL;
        }
    }
*/
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
    if(m_TxRunning)
    {
        stopTxStream();
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
        m_togBtnALC->Enable();
        m_btnTogTX->Enable();
        m_togBtnLoopRx->Enable();
        m_togBtnLoopTx->Enable();

        g_pFDMDV  = fdmdv_create();
        g_pCodec2 = codec2_create(CODEC2_MODE_1400);

#ifdef _USE_TIMER
        //DMW Reenable for the nonce... // DR: disable this puppy for now as it's causing a lot of error messages
        m_plotTimer.Start(_REFRESH_TIMER_PERIOD, wxTIMER_CONTINUOUS);
#endif // _USE_TIMER
        startRxStream();
        //startTxStream();
        if (m_RxRunning)
        {
            m_togBtnOnOff->SetLabel(wxT("Stop"));
        }
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
        m_togBtnLoopRx->Disable();
        m_togBtnLoopTx->Disable();
#ifdef _USE_TIMER
        //DMW Reenable for the nonce...
        m_plotTimer.Stop();
#endif // _USE_TIMER
        stopRxStream();
//        stopTxStream();
        m_togBtnOnOff->SetLabel(wxT("Start"));
    }
}

//----------------------------------------------------------
// OnTogBtnLoopRx()
//----------------------------------------------------------
void MainFrame::OnTogBtnLoopRx( wxCommandEvent& event )
{
     wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnLoopRx()"), wxOK);
}

//----------------------------------------------------------
// OnTogBtnLoopTx()
//----------------------------------------------------------
void MainFrame::OnTogBtnLoopTx( wxCommandEvent& event )
{
    wxMessageBox(wxT("Got Click!"), wxT("OnTogBtnLoopRx()"), wxOK);
}

//----------------------------------------------------------
// Audio stream processing loop states (globals).
//----------------------------------------------------------
int             g_nRxIn = FDMDV_NOM_SAMPLES_PER_FRAME;
float           g_Ts = 0.0;

int             g_CodecBits[2 * FDMDV_BITS_PER_FRAME];
int             g_State = 0;

float           g_avmag[FDMDV_NSPEC];


void MainFrame::destroy_fifos(void)
{
    fifo_destroy(m_rxUserdata->infifo1);
    fifo_destroy(m_rxUserdata->outfifo1);
    fifo_destroy(m_rxUserdata->infifo2);
    fifo_destroy(m_rxUserdata->outfifo2);
    fifo_destroy(m_rxUserdata->rxinfifo);
    fifo_destroy(m_rxUserdata->rxoutfifo);
}

int MainFrame::initPortAudioDevice(PortAudioWrap *pa, int inDevice, int outDevice, int soundCard)
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

    // init input params

    pa->setInputDevice(m_rxDevIn);
    pa->setInputChannelCount(2);                          // stereo input
    pa->setInputSampleFormat(PA_SAMPLE_TYPE);
    pa->setInputLatency(m_rxPa->getInputDefaultLowLatency());
    pa->setInputHostApiStreamInfo(NULL);

    // init output params

    pa->setOutputDevice(m_rxDevOut);
    pa->setOutputChannelCount(2);                         // stereo input
    pa->setOutputSampleFormat(PA_SAMPLE_TYPE);
    pa->setOutputLatency(m_rxPa->getOutputDefaultLowLatency());
    pa->setOutputHostApiStreamInfo(NULL);

    // init params that affect input and output

    /* 
       On a good day framesPerBuffer in callback will be PA_FPB.  It
       was found that you dont always get framesPerBuffer exactly
       equal PA_PFB, for example when I wanted framesPerBuffer ==
       PA_PFB == 960 samples.
    */
    pa->setFramesPerBuffer(PA_FPB);
    pa->setSampleRate(SAMPLE_RATE);
    pa->setStreamFlags(0);    

    return 0;
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

	// Init Sound card 1 ----------------------------------------------

        if (g_soundCard1DeviceNum != -1) {

	    // user has specified the sound card device

	    if (m_rxPa->getDeviceCount() < g_soundCard1DeviceNum) {
		wxMessageBox(wxT("Sound Card 1 not present"), wxT("Error"), wxOK);
		delete m_rxPa;
		return;
	    }

	    m_rxDevIn = g_soundCard1DeviceNum;
	    m_rxDevOut = g_soundCard1DeviceNum;	    
	}
	else {
	    // not specified - use default
	    m_rxDevIn = m_rxPa->getDefaultInputDevice();
	    m_rxDevOut = m_rxPa->getDefaultOutputDevice();
	}

	if (initPortAudioDevice(m_rxPa, m_rxDevIn, m_rxDevOut, 1) != 0) {
            delete m_rxPa;
            m_RxRunning = false;
            return;	    
	}

	// Init Sound Card 2 ------------------------------------------------

	if (g_nSoundCards == 2) {

	    m_txPa = new PortAudioWrap();

	    assert(g_soundCard2DeviceNum != -1);
	    printf("m_txPa->getDeviceCount() %d\n", m_txPa->getDeviceCount());

	    if (m_txPa->getDeviceCount() < g_soundCard2DeviceNum) {
		wxMessageBox(wxT("Sound Card 2 not present"), wxT("Error"), wxOK);
		delete m_rxPa;
		delete m_txPa;
		return;
	    }
	    
	    m_txDevIn = g_soundCard2DeviceNum;
	    m_txDevOut = g_soundCard2DeviceNum;

	    if (initPortAudioDevice(m_txPa, m_txDevIn, m_txDevOut, 2) != 0) {
		delete m_rxPa;
		delete m_txPa;
		m_RxRunning = false;
		return;	    
	    }

	    printf("finish init sound card 2...\n");
	}

	// Init call back data structure ----------------------------------------------

        m_rxUserdata = new paCallBackData;
        m_rxUserdata->pWFPanel = m_panelWaterfall;
        m_rxUserdata->pSPPanel = m_panelSpectrum;

	// init 48 - 8 kHz sample rate conversion filter memories

        for(int i = 0; i < MEM8; i++)
        {
            m_rxUserdata->in8k1[i] = (float)0.0;
            m_rxUserdata->in8k2[i] = (float)0.0;
        }
        for(int i = 0; i < FDMDV_OS_TAPS; i++)
        {
            m_rxUserdata->in48k1[i] = (float)0.0;
            m_rxUserdata->in48k2[i] = (float)0.0;
        }

	// create FIFOs used to interface between different buffer sizes

        m_rxUserdata->infifo1 = fifo_create(2*N48);

	/*
	  During soundcard 1 callback, outfifo1 is read in PA_FPB = 1024 sample blocks.

	  In tx processing:
	    + if outfifo1 has less than framesPerBuffer = PA_FPB = 1024 samples
	    + then tx process generates a 40ms @ 48kHz = 1920 sample blocks
	    + so we could have 1023 + 1920 samples
	    + so lets make it 1920*2 in size
	*/

        m_rxUserdata->outfifo1 = fifo_create(4*N48);

        m_rxUserdata->infifo2 = fifo_create(2*N48);
        m_rxUserdata->outfifo2 = fifo_create(2*N48);

        m_rxUserdata->rxinfifo = fifo_create(2 * FDMDV_NOM_SAMPLES_PER_FRAME);
        m_rxUserdata->rxoutfifo = fifo_create(2 * codec2_samples_per_frame(g_pCodec2));

	// question: can same callback data be used for both callbacks?
	// Is there a chance of them both being called at the same time?
        // we could maybe use a mutex ...

	// Start sound card 1 ----------------------------------------------------------

	m_rxPa->setUserData(m_rxUserdata);
        m_rxErr = m_rxPa->setCallback(rxCallback);
        m_rxErr = m_rxPa->streamOpen();

        if(m_rxErr != paNoError)
        {
            wxMessageBox(wxT("Sound Card 1 Open/Setup error."), wxT("Error"), wxOK);
            delete m_rxPa;
            delete m_txPa;
            destroy_fifos();
	    delete m_rxUserdata;
            return;
        }
        m_rxErr = m_rxPa->streamStart();
        if(m_rxErr != paNoError)
        {
            wxMessageBox(wxT("Sound Card 1 Stream Start Error."), wxT("Error"), wxOK);
            delete m_rxPa;
	    destroy_fifos();
	    delete m_rxUserdata;
	    return;
        }

	// Start sound card 2 ----------------------------------------------------------

	if (g_nSoundCards == 2) {
	    printf("starting sound card 2...\n");
#ifdef TMP
	    m_txPa->setUserData(m_rxUserdata);          // note same user-data as first card call back!
	    m_txErr = m_txPa->setCallback(txCallback);
	    m_txErr = m_txPa->streamOpen();

	    if(m_txErr != paNoError)
	    {
		wxMessageBox(wxT("Sound Card 2 Open/Setup error."), wxT("Error"), wxOK);
		m_rxPa->stop();
		m_rxPa->streamClose();
		delete m_rxPa;
		delete m_txPa;
		destroy_fifos();
		delete m_rxUserdata;
		return;
	    }
	    m_txErr = m_txPa->streamStart();
	    if(m_txErr != paNoError)
	    {
		wxMessageBox(wxT("Sound Card 2 Start Error."), wxT("Error"), wxOK);
		m_rxPa->stop();
		m_rxPa->streamClose();
		delete m_rxPa;
		delete m_txPa;
		destroy_fifos();
		delete m_rxUserdata;
		return;
	    }
#endif
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
	printf("  stopRxStream(), m_RxRunning true\n");
        m_RxRunning = false;
        m_rxPa->stop();
        m_rxPa->streamClose();
	delete m_rxPa;
	destroy_fifos();
        delete m_rxUserdata;
        fdmdv_destroy(g_pFDMDV);
        codec2_destroy(g_pCodec2);
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

        m_txDevIn = m_txPa->getDefaultInputDevice();                        // default input device
        if(m_txDevIn == paNoDevice)
        {
            wxMessageBox(wxT("Tx Error: No default input device."), wxT("Error"), wxOK);
            return;
        }

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

inline void short_to_float(float out_float[], short in_short[], int n) {
    int i;
    for(i=0; i<n; i++)
	out_float[i] = (float)in_short[i];
}

inline void float_to_short(short out_short[], float in_float[], int n) {
    int i;
    for(i=0; i<n; i++)
	out_short[i] = (short)in_float[i];
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

    // 48 to 8 kHz sample rate conversion filter states

    float           *in8k1   = cbData->in8k1;
    float           *in48k1  = cbData->in48k1;
    float           *in8k2   = cbData->in8k2;
    float           *in48k2  = cbData->in48k2;

    // temp buffers re-used by tx and rx processing

    short           in8k_short[N8];
    float           out8k[2*N8];
    short           out8k_short[N8];
    float           out48k[2*N48];
    short           out48k_short[2*N48];
    short           in48k_short[2*N48];
    short           indata[MAX_FPB];
    short           outdata[MAX_FPB];

    int             ret;
    unsigned int    i;

    (void) timeInfo;
    (void) statusFlags;

    //if (statusFlags)
    //	printf("0x%x\n", statusFlags);

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
       multiple of FDMDV_OS samples.  As we can't guarantee the size
       of framesPerBuffer we do a little FIFO buffering to interface
       the different input and output number of sample requirements.

       We also use FIFOs at the input and output of the rx and tx
       processing to interface between different buffer sizes.  For
       example the number of samples rqd for the demod is time
       varying. 

       It does result in a bunch of code that's a hard to
       understand. So I can't help think there is a better way to do this
       ....
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
    ret = fifo_write(cbData->infifo1, indata, framesPerBuffer);
    assert(ret != -1);
    
    // while we have enough input samples available ...

    while (fifo_read(cbData->infifo1, in48k_short, N48) == 0)
    {
	// note: modifying fdmdv_48_to_8() to use shorts for sample I/O
        //       would remove all these float to short conversions

	short_to_float(&in48k1[FDMDV_OS_TAPS], in48k_short, N48);
        fdmdv_48_to_8(out8k, &in48k1[FDMDV_OS_TAPS], N8);
	float_to_short(out8k_short, out8k, N8);
	fifo_write(cbData->rxinfifo, out8k_short, N8);

        per_frame_rx_processing(cbData->rxoutfifo, g_CodecBits, cbData->rxinfifo, &g_nRxIn, &g_State, g_pCodec2);

	// if demod out of sync echo audio at input to demod to
	// headset aid in tuning (ie we hear SSB radio output)

	if (g_State == 0)
	    memcpy(&in8k1[MEM8], out8k, sizeof(float)*N8);
	else {
	    fifo_read(cbData->rxoutfifo, in8k_short, N8);	    
	    short_to_float(&in8k1[MEM8], in8k_short, N8);
	}

        fdmdv_8_to_48(out48k, &in8k1[MEM8], N8);
	float_to_short(out48k_short, out48k, N48);
        if (g_nSoundCards == 1)
	    fifo_write(cbData->outfifo1, out48k_short, N48);
	else
	    fifo_write(cbData->outfifo2, out48k_short, N48);
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
	
	// Typical values are framesPerBuffer=1024, and each iteration
	// of the tx processing generates 320*FDMDV_OS=1920 samples at
	// 48kHz, so this loop will run 0 or 1 times.

	while((unsigned)fifo_n(cbData->outfifo1) < framesPerBuffer) {
	    short tx_speech_in[2*N8];
	    short tx_mod_out[2*N8];

	    // infifo2 is written to by another sound card so it may
	    // over or underflow, but we don't realy care.  It will
	    // just result in a short interruption in audio being fed
	    // to codec2_enc, possibly making a click every now and
	    // again in the decoded audio at the other end.

	    // zero speech input just in case infifo2 underflows

	    memset(in48k_short, 0, sizeof(short)*2*N48); 

	    // Codec 2 @ 1400 bit/s requires 40ms of input speech

	    fifo_read(cbData->infifo2, in48k_short, 2*N48);
	    short_to_float(&in48k2[FDMDV_OS_TAPS], in48k_short, 2*N48);
	    fdmdv_48_to_8(out8k, &in48k2[FDMDV_OS_TAPS], 2*N8);
	    float_to_short(tx_speech_in, out8k, 2*N8);
	    assert(codec2_samples_per_frame(g_pCodec2) == (2*N8));

	    per_frame_tx_processing(tx_mod_out, tx_speech_in, g_pCodec2);
	    
	    // output 40ms of modem tone

	    short_to_float(&in8k2[MEM8], tx_mod_out, 2*N8);
	    fdmdv_8_to_48(out48k, &in8k2[MEM8], 2*N8);
	    float_to_short(out48k_short, out48k, 2*N48);
	    fifo_write(cbData->outfifo1, out48k_short, 2*N48);
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
    float               rx_fdm[FDMDV_MAX_SAMPLES_PER_FRAME];
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

    while (fifo_read(input_fifo, input_buf, *nin) == 0)    
    {
        // demod per frame processing
        for(i = 0; i < *nin; i++)
        {
            rx_fdm[i] = (float)input_buf[i] / FDMDV_SCALE;
        }
        nin_prev = *nin;
        fdmdv_demod(g_pFDMDV, rx_bits, &sync_bit, rx_fdm, nin);

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

		    assert(codec2_samples_per_frame(c2) <= (2*N8));
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

    // assemble a mono buffer (just use left channel) and write to FIFO

    assert(framesPerBuffer < MAX_FPB);

    for(i = 0; i < framesPerBuffer; i++, rptr += 2)
    {
        indata[i] = *rptr;
    }
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

    return paContinue;
}

