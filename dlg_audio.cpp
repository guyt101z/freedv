//==========================================================================
// Name:            dlg_audio.cpp
//
// Purpose:         Creates simple wxWidgets dialog GUI for audio
//                  devices/options.
//
// Date:            May 05 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include "dlg_audio.h"

AudioDlg::AudioDlg( wxWindow* parent ) : DlgAudio( parent )
{

}

void AudioDlg::OnCancel( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}

void AudioDlg::OnOK( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}

void AudioDlg::OnApply( wxCommandEvent& event )
{
    this->EndModal(wxID_OK);
}

void AudioDlg::OnClose( wxCloseEvent& event )
{
    this->EndModal(wxID_OK);
}

void AudioDlg::OnInitDialog( wxInitDialogEvent& event )
{
    //wxMessageBox(wxT("got OnInitDialog()"), wxT("Select"), wxOK);
    populateAudioInfo();
}

void AudioDlg::OnRxInputSelect( wxCommandEvent& event )
{
    wxMessageBox(wxT("got OnRxInputSelect()"), wxT("Select"), wxOK);
}

void AudioDlg::OnTxOutputSelect( wxCommandEvent& event )
{
    wxMessageBox(wxT("got OnTxOutputSelect()"), wxT("Select"), wxOK);
}

void AudioDlg::OnVoiceInputSelect( wxCommandEvent& event )
{
    wxMessageBox(wxT("got OnVoiceInputSelect()"), wxT("Select"), wxOK);
}

void AudioDlg::OnVoiceOutputSelect( wxCommandEvent& event )
{
    wxMessageBox(wxT("got OnVoiceOutputSelect()"), wxT("Select"), wxOK);
}

void AudioDlg::populateStandardSampleRates(
        wxListBox* target,
        const portaudio::DirectionSpecificStreamParameters &inputParameters,
        const portaudio::DirectionSpecificStreamParameters &outputParameters)
{
    static double STANDARD_SAMPLE_RATES[] = {
                                                8000.0, 9600.0, 11025.0, 12000.0,
                                                16000.0, 22050.0, 24000.0, 32000.0,
                                                44100.0, 48000.0, 88200.0, 96000.0,
                                                -1 // negative terminated list
                                            };
    int printCount = 0;
    wxString tStr;

    for (int i = 0; STANDARD_SAMPLE_RATES[i] > 0; ++i)
    {
        portaudio::StreamParameters tmp = portaudio::StreamParameters(inputParameters, outputParameters, STANDARD_SAMPLE_RATES[i], 0, paNoFlag);
        if (tmp.isSupported())
        {
            tStr.Printf("%i %8.2f", printCount, STANDARD_SAMPLE_RATES[i]);
            target->InsertItems(1, &tStr, 0);
            ++printCount;
        }
    }
    if (printCount == 0)
    {
        tStr = "None\n";
        target->InsertItems(1, &tStr, 0);
    }
}

int AudioDlg::populateAudioInfo()
{
    wxString tStr;
    wxString ttStr;
    try
    {
        portaudio::AutoSystem autoSys;
        portaudio::System &sys = portaudio::System::instance();
/*

        tStr.Printf("PortAudio version number: %i\n", sys.version());
        m_textTopRight->AppendText(tStr);

        tStr.Printf("PortAudio version number: %s\n", sys.versionText());
        m_textTopRight->AppendText(tStr);

        int numDevices = sys.deviceCount();
        tStr.Printf("Number of devices: %i\n\n", numDevices);
        m_textTopRight->AppendText(tStr);
*/
        for (portaudio::System::DeviceIterator i = sys.devicesBegin(); i != sys.devicesEnd(); ++i)
        {
/*
            tStr.Printf("---------- device #: %i ----------\n", (*i).index());
            m_textTopRight->AppendText(tStr);

            bool defaultDisplayed = false;

            if ((*i).isSystemDefaultInputDevice())
            {
                tStr.Printf("[ System Default Input %s", (*i).hostApi().name());
                m_textTopRight->AppendText(tStr);
                defaultDisplayed = true;
            }
            else if ((*i).isHostApiDefaultInputDevice())
            {
                tStr.Printf("[ HostApi Default %s Input", (*i).hostApi().name());
                m_textTopRight->AppendText(tStr);
                defaultDisplayed = true;
            }
            if ((*i).isSystemDefaultOutputDevice())
            {
                tStr.Printf(defaultDisplayed ? "," : "[");
                m_textTopRight->AppendText(tStr);
                tStr = (" System Default Output");
                m_textTopRight->AppendText(tStr);
                defaultDisplayed = true;
            }
            else if ((*i).isHostApiDefaultOutputDevice())
            {
                tStr.Printf(defaultDisplayed ? "," : "[");
                m_textTopRight->AppendText(tStr);
                tStr.Printf(" HostApi Default %s Output", (*i).hostApi().name());
                m_textTopRight->AppendText(tStr);
                defaultDisplayed = true;
            }

            if (defaultDisplayed)
            {
               tStr  =  " ]\n";
                m_textTopRight->AppendText(tStr);
            }

            ttStr.Printf("Name                        : %s\n", (*i).name());
            tStr  += ttStr;
            ttStr.Printf("Host API                    : %s\n", (*i).hostApi().name());
            tStr  += ttStr;
            ttStr.Printf("Max inputs                  : %i\n", (*i).maxInputChannels());
            tStr  += ttStr;
            ttStr.Printf("Max outputs                 : %i\n\n", (*i).maxOutputChannels());
            tStr  += ttStr;

            ttStr.Printf("Default low input latency   : %8.3f\n", (*i).defaultLowInputLatency());
            tStr  += ttStr;
            ttStr.Printf("Default low output latency  : %8.3f\n", (*i).defaultLowOutputLatency());
            tStr  += ttStr;
            ttStr.Printf("Default high input latency  : %8.3f\n", (*i).defaultHighInputLatency());
            tStr  += ttStr;
            ttStr.Printf("Default high output latency : %8.3f\n\n", (*i).defaultHighOutputLatency());
            tStr  += ttStr;
            m_textTopRight->AppendText(tStr);

#ifdef WIN32_ASIO
            // ASIO specific latency information:
            if ((*i).hostApi().typeId() == paASIO)
            {
                portaudio::AsioDeviceAdapter asioDevice((*i));

                //std::cout << "ASIO minimum buffer size    = " << asioDevice.minBufferSize() << std::endl;
                //std::cout << "ASIO maximum buffer size    = " << asioDevice.maxBufferSize() << std::endl;
                //std::cout << "ASIO preferred buffer size  = " << asioDevice.preferredBufferSize() << std::endl;

                if (asioDevice.granularity() == -1)
                {
                    //std::cout << "ASIO buffer granularity     = power of 2" << std::endl;
                }
                else
                {
                    //std::cout << "ASIO buffer granularity     = " << asioDevice.granularity() << std::endl;
                }
            }
#endif // WIN32_ASIO

            tStr.Printf("Default sample rate         : %8.2f\n", (*i).defaultSampleRate());
            m_textTopRight->AppendText(tStr);
*/
            // Poll for standard sample rates:
            portaudio::DirectionSpecificStreamParameters inputParameters((*i), (*i).maxInputChannels(), portaudio::INT16, true, 0.0, NULL);
            portaudio::DirectionSpecificStreamParameters outputParameters((*i), (*i).maxOutputChannels(), portaudio::INT16, true, 0.0, NULL);
            if (inputParameters.numChannels() > 0)
            {
                tStr = "Supported standard Input sample rates\n";
                m_lbRxInput->InsertItems(1, &tStr, 0);
                tStr.Printf("   for half-duplex 16 bit %i channel input = ", inputParameters.numChannels());
                m_lbRxInput->InsertItems(1, &tStr, 0);
                populateStandardSampleRates(m_lbRxInput, inputParameters, portaudio::DirectionSpecificStreamParameters::null());
            }
            if (outputParameters.numChannels() > 0)
            {
                tStr = "Supported standard Output sample rates\n";
                m_lbTxOutput->InsertItems(1, &tStr, 0);
                tStr.Printf("   for half-duplex 16 bit %i channel output = ", outputParameters.numChannels());
                m_lbTxOutput->InsertItems(1, &tStr, 0);
                populateStandardSampleRates(m_lbTxOutput, portaudio::DirectionSpecificStreamParameters::null(), outputParameters);
            }
            if (inputParameters.numChannels() > 0 && outputParameters.numChannels() > 0)
            {
                tStr = "Supported full-duplex sample rates\n";
                m_lbVoiceInput->InsertItems(1, &tStr, 0);
                tStr.Printf("   for full-duplex 16 bit %i channel input, %i", inputParameters.numChannels(), outputParameters.numChannels());
                m_lbVoiceInput->InsertItems(1, &tStr, 0);
                populateStandardSampleRates(m_lbVoiceInput, inputParameters, outputParameters);
            }
        }
    }
    catch (const portaudio::PaException &e)
    {
        tStr.Printf("A PortAudio error occured: %s\n",  e.paErrorText());
        wxMessageBox(tStr, wxT("Portaudio exception"), wxOK);
    }
    catch (const portaudio::PaCppException &e)
    {
        tStr.Printf("A PortAudioCpp error occured: %s\n", e.what());
        wxMessageBox(tStr, wxT("PortAudioCpp error"), wxOK);
    }
    catch (const std::exception &e)
    {
        tStr.Printf("A generic exception occured: %s\n", e.what());
        wxMessageBox(tStr, wxT("Generic Exception"), wxOK);
    }
    catch (...)
    {
        tStr.Printf("An unknown exception occured.\n");
        wxMessageBox(tStr, wxT("Unknown error"), wxOK);
    }
    return 0;
}


