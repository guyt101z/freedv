//==========================================================================
// Name:            fdmdv2_main.cpp
//
// Purpose:         Implements simple wxWidgets application with GUI.
// Created:         Apr. 9, 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
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
    // Create the main application window
    MainFrame *frame = new MainFrame(NULL);

    frame->m_panelSpectrum = new DrawPanel((wxFrame*) frame->m_auiNbookCtrl );
    frame->m_auiNbookCtrl->AddPage(frame->m_panelSpectrum, _("Spectrum"), true, wxNullBitmap );

    frame->m_panelWaterfall = new DrawPanel((wxFrame*) frame->m_auiNbookCtrl );
    frame->m_auiNbookCtrl->AddPage(frame->m_panelWaterfall, _("Waterfall"), true, wxNullBitmap );

    SetTopWindow(frame);
    frame->Layout();
    frame->Show();
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class MainFrame(wxFrame* parent) : TopFrame(parent)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
MainFrame::MainFrame(wxWindow *parent) : TopFrame(parent)
{
    m_radioRunning      = false;
//    m_bitmap = bitmap;
    m_sound = NULL;
    m_zoom  = 1.;
    m_SquelchActive = false;

}

//-------------------------------------------------------------------------
// ~MainFrame()
//-------------------------------------------------------------------------
MainFrame::~MainFrame()
{
}

//-------------------------------------------------------------------------
// OnCloseFrame()
//-------------------------------------------------------------------------
void MainFrame::OnCloseFrame(wxCloseEvent& event)
{
    Destroy();
}

//-------------------------------------------------------------------------
// OnExitClick()
//-------------------------------------------------------------------------
void MainFrame::OnExitClick(wxCommandEvent& event)
{
    Destroy();
}

//-------------------------------------------------------------------------
// OnPaint()
//-------------------------------------------------------------------------
void MainFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    if(GetMenuBar()->IsChecked(ID_PAINT_BG))
    {
        dc.Clear();
    }
    dc.SetUserScale(m_zoom, m_zoom);
    const wxSize size = GetClientSize();
    dc.DrawBitmap(m_bitmap, dc.DeviceToLogicalX((size.x - m_zoom * m_bitmap.GetWidth()) / 2), dc.DeviceToLogicalY((size.y - m_zoom * m_bitmap.GetHeight()) / 2), true);
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
    wxMessageBox("Got Click!", "OnTogBtnRxID", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnTxID()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnTxID(wxCommandEvent& event)
{
    wxMessageBox("Got Click!", "OnTogBtnTxID", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnSplitClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnSplitClick(wxCommandEvent& event)
{
    wxMessageBox("Got Click!", "OnTogBtnSplitClick", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnAnalogClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnAnalogClick (wxCommandEvent& event)
{
    wxMessageBox("Got Click!", "OnTogBtnAnalogClick", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// OnTogBtnALCClick()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnALCClick(wxCommandEvent& event)
{
    wxMessageBox("Got Click!", "OnTogBtnALCClick", wxOK);
    event.Skip();
}

//-------------------------------------------------------------------------
// audioCallback()
//-------------------------------------------------------------------------
static int audioCallback(   const void *inputBuffer,
                            void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *outTime,
                            PaStreamCallbackFlags statusFlags,
                            void *userData
                        )
{
    float *out = (float *) outputBuffer;
    float *in  = (float *) inputBuffer;
    float leftInput;
    float rightInput;
    unsigned int i;

    if(inputBuffer == NULL)
    {
        return 0;
    }
    // Read input buffer, process data, and fill output buffer.
    for(i = 0; i < framesPerBuffer; i++)
    {
        leftInput = *in++;                          // Get interleaved samples from input buffer.
        rightInput = *in++;
        *out++ = leftInput * rightInput;            // ring modulation
        *out++ = 0.5f * (leftInput + rightInput);   // mixing
    }
    return paContinue;                              // 0;
}

/*
#define FUZZ(x) CubicAmplifier(CubicAmplifier(CubicAmplifier(CubicAmplifier(x))))
static int gNumNoInputs = 0;
//-------------------------------------------------------------------------
// fuzzCallback()
//-------------------------------------------------------------------------
static int fuzzCallback(const void *inputBuffer,
                        void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData)
{
    SAMPLE *out = (SAMPLE*)outputBuffer;
    const SAMPLE *in = (const SAMPLE*)inputBuffer;
    unsigned int i;
    (void) timeInfo;                        // Prevent unused variable warnings.
    (void) statusFlags;
    (void) userData;

    if(inputBuffer == NULL)
    {
        for(i = 0; i < framesPerBuffer; i++)
        {
            *out++ = 0;                     // left - silent
            *out++ = 0;                     // right - silent
        }
        gNumNoInputs += 1;
    }
    else
    {
        for(i = 0; i < framesPerBuffer; i++)
        {
            *out++ = FUZZ(*in++);           // left - distorted
            *out++ = *in++;                 // right - clean
        }
    }
    return paContinue;
}
*/

//-------------------------------------------------------------------------
// OnTogBtnOnOff()
//-------------------------------------------------------------------------
void MainFrame::OnTogBtnOnOff(wxCommandEvent& event)
{
    PortAudioWrap pa;
    if(!m_radioRunning)
    {
        m_radioRunning = true;
        pa = PortAudioWrap();
        err = pa.init();

        inputDevice = pa.getDefaultInputDevice();                   // default input device
        if(inputDevice == paNoDevice)
        {
            wxMessageBox("Error: No default input device.", "Error", wxOK);
            return;
        }
        err = pa.setInputDevice(inputDevice);
        err = pa.setInputChannelCount(2);                           // stereo input
        err = pa.setInputSampleFormat(PA_SAMPLE_TYPE);
        err = pa.setInputLatency(pa.getInputDefaultLowLatency());
        pa.setInputHostApiStreamInfo(NULL);

        outputDevice = pa.getDefaultOutputDevice();                 // default output device
        if (outputDevice == paNoDevice)
        {
            wxMessageBox("Error: No default output device.", "Error", wxOK);
            return;
        }
        err = pa.setOutputDevice(outputDevice);
        err = pa.setOutputChannelCount(2);                           // stereo input
        err = pa.setOutputSampleFormat(PA_SAMPLE_TYPE);

        err = pa.setOutputLatency(pa.getOutputDefaultLowLatency());
        pa.setOutputHostApiStreamInfo(NULL);

        err = pa.setFramesPerBuffer(FRAMES_PER_BUFFER);
        err = pa.setSampleRate(SAMPLE_RATE);
        err = pa.setStreamFlags(0);
        err = pa.setCallback(audioCallback);
        err = pa.streamOpen();

        if(err != paNoError)
        {
            wxMessageBox("Open/Setup error.", "Error", wxOK);
            return;
        }
        err = pa.streamStart();
        if(err != paNoError)
        {
            wxMessageBox("Stream Start Error.", "Error", wxOK);
            return;
        }
        m_togBtnOnOff->SetLabel(wxT("Stop"));
    }
    else
    {
        m_radioRunning = false;
        pa.terminate();
        m_togBtnOnOff->SetLabel(wxT("Start"));
    }
}

//-------------------------------------------------------------------------
// OnOpen()
//-------------------------------------------------------------------------
void MainFrame::OnOpen( wxCommandEvent& event )
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
/*
    // this can be done with e.g. wxWidgets input streams:
    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
*/
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
//        if(m_sound->IsPlaying())
//        {
        m_sound->Stop();
        m_sound = NULL;
//        }
    }
//    wxMessageBox("Got Click!", "OnClose", wxOK);
//    event.Skip();
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
    wxMessageBox("Got Click!", "OnExit", wxOK);
    event.Skip();
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
    wxMessageBox("Got Click!", "OnPaste", wxOK);
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
    event.Enable(!m_radioRunning);
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
// OnHelpAbout()
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
    wxImage image = m_bitmap.ConvertToImage();

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
    if(extension == wxT("bmp"))
    {
        static const int bppvalues[] =
        {
            wxBMP_1BPP,
            wxBMP_1BPP_BW,
            wxBMP_4BPP,
            wxBMP_8BPP,
            wxBMP_8BPP_GREY,
            wxBMP_8BPP_RED,
            wxBMP_8BPP_PALETTE,
            wxBMP_24BPP
        };

        const wxString bppchoices[] =
        {
            wxT("1 bpp color"),
            wxT("1 bpp B&W"),
            wxT("4 bpp color"),
            wxT("8 bpp color"),
            wxT("8 bpp greyscale"),
            wxT("8 bpp red"),
            wxT("8 bpp own palette"),
            wxT("24 bpp")
        };

        int bppselection = wxGetSingleChoiceIndex(wxT("Set BMP BPP"),
                           wxT("Image sample: save file"),
                           WXSIZEOF(bppchoices),
                           bppchoices,
                           this);
        if(bppselection != -1)
        {
            int format = bppvalues[bppselection];

            image.SetOption(wxIMAGE_OPTION_BMP_FORMAT, format);
            if(format == wxBMP_8BPP_PALETTE)
            {
                unsigned char *cmap = new unsigned char [256];
                for(int i = 0; i < 256; i++)
                {
                    cmap[i] = (unsigned char)i;
                }
                image.SetPalette(wxPalette(256, cmap, cmap, cmap));
                delete[] cmap;
            }
        }
    }
    else if(extension == wxT("png"))
    {
        static const int pngvalues[] =
        {
            wxPNG_TYPE_COLOUR,
            wxPNG_TYPE_COLOUR,
            wxPNG_TYPE_GREY,
            wxPNG_TYPE_GREY,
            wxPNG_TYPE_GREY_RED,
            wxPNG_TYPE_GREY_RED,
        };

        const wxString pngchoices[] =
        {
            wxT("Colour 8bpp"),
            wxT("Colour 16bpp"),
            wxT("Grey 8bpp"),
            wxT("Grey 16bpp"),
            wxT("Grey red 8bpp"),
            wxT("Grey red 16bpp"),
        };

        int sel = wxGetSingleChoiceIndex(wxT("Set PNG format"),
                                         wxT("Image sample: save file"),
                                         WXSIZEOF(pngchoices),
                                         pngchoices,
                                         this);
        if(sel != -1)
        {
            image.SetOption(wxIMAGE_OPTION_PNG_FORMAT, pngvalues[sel]);
            image.SetOption(wxIMAGE_OPTION_PNG_BITDEPTH, sel % 2 ? 16 : 8);

            // these values are taken from OptiPNG with -o3 switch
            const wxString compressionChoices[] =
            {
                wxT("compression = 9, memory = 8, strategy = 0, filter = 0"),
                wxT("compression = 9, memory = 9, strategy = 0, filter = 0"),
                wxT("compression = 9, memory = 8, strategy = 1, filter = 0"),
                wxT("compression = 9, memory = 9, strategy = 1, filter = 0"),
                wxT("compression = 1, memory = 8, strategy = 2, filter = 0"),
                wxT("compression = 1, memory = 9, strategy = 2, filter = 0"),
                wxT("compression = 9, memory = 8, strategy = 0, filter = 5"),
                wxT("compression = 9, memory = 9, strategy = 0, filter = 5"),
                wxT("compression = 9, memory = 8, strategy = 1, filter = 5"),
                wxT("compression = 9, memory = 9, strategy = 1, filter = 5"),
                wxT("compression = 1, memory = 8, strategy = 2, filter = 5"),
                wxT("compression = 1, memory = 9, strategy = 2, filter = 5"),
            };

            int sel = wxGetSingleChoiceIndex(wxT("Select compression option (Cancel to use default)\n"),
                                             wxT("PNG Compression Options"),
                                             WXSIZEOF(compressionChoices),
                                             compressionChoices,
                                             this);
            if(sel != -1)
            {
                const int zc[] = {9, 9, 9, 9, 1, 1, 9, 9, 9, 9, 1, 1};
                const int zm[] = {8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9};
                const int zs[] = {0, 0, 1, 1, 2, 2, 0, 0, 1, 1, 2, 2};
                const int f[]  = {0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
                                  0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8
                                 };

                image.SetOption(wxIMAGE_OPTION_PNG_COMPRESSION_LEVEL      , zc[sel]);
                image.SetOption(wxIMAGE_OPTION_PNG_COMPRESSION_MEM_LEVEL  , zm[sel]);
                image.SetOption(wxIMAGE_OPTION_PNG_COMPRESSION_STRATEGY   , zs[sel]);
                image.SetOption(wxIMAGE_OPTION_PNG_FILTER                 , f[sel]);
                image.SetOption(wxIMAGE_OPTION_PNG_COMPRESSION_BUFFER_SIZE, 1048576); // 1 MB
            }
        }
    }
    else if(extension == wxT("cur"))
    {
        image.Rescale(32, 32);
        image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 0);
        image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 0);
        // This shows how you can save an image with explicitly
        // specified image format:
        saved = image.SaveFile(savefilename, wxBITMAP_TYPE_CUR);
    }
    if(!saved)
    {
        // This one guesses image format from filename extension
        // (it may fail if the extension is not recognized):
        image.SaveFile(savefilename);
    }
}
