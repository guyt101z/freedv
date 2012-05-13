int MyPortaudioClass::myMemberCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo,  PaStreamCallbackFlags statusFlags)
{
    // Do what you need (having access to every part of MyPortaudioClass)
    return paContinue;
}

// Once that’s setup, you can open your stream using “this” as your userData:
PaError pa_error = Pa_OpenStream(
                                    [snip]...,
                                    &MyPortaudioClass::myPaCallback,        // streamCallback
                                    this);                                  // userData
                    
// You could also just use a reference to your class in your static function 
// like (make sure the static function is a member of the class so
// that you have access to “private” declared variables):
MyPortaudioClass* pThis = (MyPortaudioClass*)userData;
