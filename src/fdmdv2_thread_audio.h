#ifndef __THREAD_AUDIO__
#define __THREAD_AUDIO__
#include "wx/thread.h"     // Base class: wxThread

//namespace NSfdmdv2Audio
//{
    class Fdmdv2ThreadAudio : public wxThread
    {
        public:
            Fdmdv2ThreadAudio();
            ~Fdmdv2ThreadAudio();
        public:
            virtual void* Entry();
            virtual void OnDelete();
            virtual void OnExit();
            virtual void OnKill();
            virtual bool TestDestroy();
    };
//}
#endif // __THREAD_AUDIO__
