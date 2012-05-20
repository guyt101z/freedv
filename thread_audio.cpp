//==========================================================================
// Name:            thread_audio.cpp
//
// Purpose:         Implements simple wxWidgets application with GUI.
// Created:         May 11, 2012
// Initial author:  David Witten
// License:         BSD License (other licenses may apply to other
//                  components of this project)
//==========================================================================
#include "thread_audio.h"

// declare a new type of event, to be used by our MyThread class:
wxDECLARE_EVENT(wxEVT_COMMAND_AUDIOTHREAD_COMPLETED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_AUDIOTHREAD_UPDATE, wxThreadEvent);

#include "stdio.h"
#include "extern/include/portaudio.h"

/* This will be called asynchronously by the PortAudio engine. */
static int audioCallback( void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, PaTime outTime, void *userData )
{
    float *out = (float *) outputBuffer;
    float *in = (float *) inputBuffer;
    float leftInput, rightInput;
    unsigned int i;

    if( inputBuffer == NULL )
    {
        return 0;
    }
    /* Read input buffer, process data, and fill output buffer. */
    for(i = 0; i < framesPerBuffer; i++)
    {
        leftInput = *in++;                          /* Get interleaved samples from input buffer. */
        rightInput = *in++;
        *out++ = leftInput * rightInput;            /* ring modulation */
        *out++ = 0.5f * (leftInput + rightInput);   /* mixing */
    }
    return 0;
}
#ifdef PORTAUDIO_MAIN
/* Use a PortAudioStream to process audio data. */
int main(void)
{
    PortAudioWrap *stream;
    Pa_Initialize();
    //Pa_OpenDefaultStream(&stream, 2, 2, /* stereo input and output */ paFloat32, 44100.0, 64, 0, /* 64 frames per buffer, let PA determine numBuffers */audioCallback, NULL );
    Pa_OpenDefaultStream(&stream, 2, 2, paFloat32, 44100.0, 64, 0, audioCallback, NULL );
    Pa_StartStream( stream );
    Pa_Sleep( 10000 );                              /* Sleep for 10 seconds while processing. */
    Pa_StopStream( stream );
    Pa_CloseStream( stream );
    Pa_Terminate();
    return 0;
}
#endif  //PORTAUDIO_MAIN
