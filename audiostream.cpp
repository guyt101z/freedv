//============================================================
// AudioStream.h
//
//
//============================================================
#include "audiostream.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class AudioStream constructor
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
AudioStream::AudioStream(int tableSize) : tableSize_(tableSize), leftPhase_(0), rightPhase_(0)
{
    const double PI = 3.14159265;
    table_ = new float[tableSize];
    for (int i = 0; i < tableSize; ++i)
    {
        table_[i] = 0.125f * (float)sin(((double)i/(double)tableSize)*PI*2.);
    }
}

//------------------------------------------------------------
// Class AudioStream destructor
//------------------------------------------------------------
AudioStream::~AudioStream()
{
    delete[] table_;
}

//------------------------------------------------------------
// generate()
//------------------------------------------------------------
int AudioStream::generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    assert(outputBuffer != NULL);

    float **out = static_cast<float **>(outputBuffer);

    for (unsigned int i = 0; i < framesPerBuffer; ++i)
    {
        out[0][i] = table_[leftPhase_];
        out[1][i] = table_[rightPhase_];

        leftPhase_ += 1;
        if (leftPhase_ >= tableSize_)
            leftPhase_ -= tableSize_;

        rightPhase_ += 3;
        if (rightPhase_ >= tableSize_)
            rightPhase_ -= tableSize_;
    }

    return paContinue;
}

//------------------------------------------------------------
// Open()
//------------------------------------------------------------
void AudioStream::Open()
{
    wxString estr;
    try
    {
        // Create a SineGenerator object:
        AudioStream AudioStream(TABLE_SIZE);

        // Set up the System:
        portaudio::AutoSystem autoSys;
        portaudio::System &sys = portaudio::System::instance();
        // Set up the parameters required to open a (Callback)Stream:
        portaudio::DirectionSpecificStreamParameters outParams(sys.defaultOutputDevice(), 2, portaudio::FLOAT32, false, sys.defaultOutputDevice().defaultLowOutputLatency(), NULL);
        portaudio::StreamParameters params(portaudio::DirectionSpecificStreamParameters::null(), outParams, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff);

        wxMessageBox(wxT("Opening stereo output stream..."), wxT("Info"), wxOK);
        // Create (and open) a new Stream, using the SineGenerator::generate function as a callback:
        portaudio::MemFunCallbackStream<AudioStream> stream(params, AudioStream, &AudioStream::generate);
        wxMessageBox(wxT("Starting playback for %i  seconds."), wxT("Info"), wxOK);
        // Start the Stream (audio playback starts):
//        stream.start();
        // Wait for 5 seconds:
        sys.sleep(NUM_SECONDS * 1000);
        wxMessageBox(wxT("Closing stream..."), wxT("Info"), wxOK);
        // Stop the Stream (not strictly needed as termintating the System will also stop all open Streams):
//        stream.stop();
        // Close the Stream (not strictly needed as terminating the System will also close all open Streams):
//        stream.close();
        // Terminate the System (not strictly needed as the AutoSystem will also take care of this when it
        // goes out of scope):
        sys.terminate();
        wxMessageBox(wxT("Test finished."), wxT(""), wxOK);
    }
    catch (const portaudio::PaException &e)
    {
        estr.Format(wxT("A PortAudio error occured: %s"), e.paErrorText());
        wxMessageBox(estr, wxT("Error"), wxOK);
    }
    catch (const portaudio::PaCppException &e)
    {
        estr.Format(wxT("A PortAudioCpp error occured: %s"), e.what());
        wxMessageBox(estr, wxT("Error"), wxOK);
    }
    catch (const std::exception &e)
    {
        estr.Format(wxT("A generic exception occured: %s"), e.what());
        wxMessageBox(estr, wxT("Error"), wxOK);
    }
    catch (...)
    {
        wxMessageBox(wxT("An unknown exception occured."), wxT("Error"), wxOK);
    }
}
