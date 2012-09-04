AM_CFLAGS = -I../src -fPIC -Wall -O3 -g 
AUTOMAKE_OPTS = gnu
NAME = fdmdv2
AM_CPPFLAGS = $(AM_CFLAGS) 
WX_FLAGS = -mthreads -DHAVE_W32API_H -D__WXMSW__ -DNDEBUG -D_UNICODE -IC:\bin\wxWidgets-2.9.4\lib\gcc_dll\mswu -IC:\bin\wxWidgets-2.9.4\include -DWXUSINGDLL -Wno-ctor-dtor-privacy -pipe -fmessage-length=0

CONFIG_CLEAN_FILES = *.o *.exe

EXE_EXT = .exe
OBJEXT = o
GPP = mingw32-g++.exe

WXWIN=C:\bin\wxWidgets-2.9.4
# PATH=$(WXWIN)\lib\gcc_dll;$(PATH)
WXCFG=gcc_dll\mswu
UNIT_TEST_PP_SRC_DIR=C:\bin\UnitTest++-1.3

SRC = topFrame.cpp \
fdmdv2_main.cpp \
fdmdv2_plot.cpp \
fdmdv2_plot_scalar.cpp \
fdmdv2_plot_scatter.cpp \
fdmdv2_plot_spectrum.cpp \
fdmdv2_plot_waterfall.cpp \
fdmdv2_process_audio.cpp\
fdmdv2_pa_wrapper.cpp \
dlg_about.cpp \
dlg_audio.cpp \
dlg_comports.cpp \
dlg_options.cpp

DEFINES = -D__WX__

OBJS = topFrame.$(OBJEXT) \
fdmdv2_main.$(OBJEXT) \
fdmdv2_plot.$(OBJEXT) \
fdmdv2_plot_scalar.$(OBJEXT) \
fdmdv2_plot_scatter.$(OBJEXT) \
fdmdv2_plot_spectrum.$(OBJEXT) \
fdmdv2_plot_waterfall.$(OBJEXT) \
fdmdv2_process_audio.$(OBJEXT)\
fdmdv2_pa_wrapper.$(OBJEXT)\
dlg_about.$(OBJEXT) \
dlg_audio.$(OBJEXT) \
dlg_comports.$(OBJEXT) \
dlg_options.$(OBJEXT)


INCLUDE_PATHS = -I. \
-IC:\bin\wxWidgets-2.9.4\lib\gcc_dll\mswu \
-IC:\bin\wxWidgets-2.9.4\include \
-I/bin/MinGW/msys/1.0/local/include \
-I../../codec2-dev/src \
-I../3rdparty/portaudio/include \
-I../3rdparty/portaudio/bindings/cpp/include \
-I../3rdparty/libsndfile/include \
-I../3rdparty/libsamplerate-0.1.8/src 

#INCLUDE = ../../codec2-dev/src \
#../3rdparty/portaudio/include \
#../3rdparty/portaudio/bindings/cpp/include \
#../3rdparty/libsndfile/include \
#../3rdparty/libsamplerate-0.1.8/src 

LIBS = -lsndfile-1 \
-lcodec2 \
-lportaudio \
-lportaudiocpp \
-mwindows -s  \
-mthreads \
-lwxmsw29u \
-lwxtiff \
-lwxjpeg \
-lwxpng \
-lwxzlib \
-lwxregexu \
-lwxexpat \
-lkernel32 \
-luser32 \
-lgdi32 \
-lcomdlg32 \
-lwxregexu \
-lwinspool \
-lwinmm \
-lshell32 \
-lcomctl32 \
-lole32 \
-loleaut32 \
-luuid \
-lrpcrt4 \
-ladvapi32 \
-lwsock32 
 
LIB_FLAGS =	-lm -lpthread 

LIB_PATHS = -LC:\bin\wxWidgets-2.9.4\lib\gcc_dll -L../extern/lib 
#../3rdparty/portaudio/lib \
#../3rdparty/portaudio/bindings/cpp/lib \
#../3rdparty/libsndfile/lib 
#../3rdparty/libsamplerate-0.1.8/src 


topFrame.$(OBJEXT): topFrame.cpp topFrame.h 
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) topframe.cpp

fdmdv2_main.$(OBJEXT):  fdmdv2_main.cpp fdmdv2_main.h 
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) fdmdv2_main.cpp

fdmdv2_plot.$(OBJEXT):  fdmdv2_plot.cpp fdmdv2_plot.h 
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) fdmdv2_plot.cpp

fdmdv2_plot_scalar.$(OBJEXT):  fdmdv2_plot_scalar.cpp fdmdv2_plot_scalar.h fdmdv2_plot.h fdmdv2_defines.h
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) fdmdv2_plot_scalar.cpp

fdmdv2_plot_scatter.$(OBJEXT):  fdmdv2_plot_scatter.cpp fdmdv2_plot_scatter.h fdmdv2_plot.h fdmdv2_defines.h
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) fdmdv2_plot_scatter.cpp

fdmdv2_plot_spectrum.$(OBJEXT):  fdmdv2_plot_spectrum.cpp fdmdv2_plot_spectrum.h fdmdv2_plot.h fdmdv2_defines.h
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) fdmdv2_plot_spectrum.cpp

fdmdv2_plot_waterfall.$(OBJEXT):  fdmdv2_plot_waterfall.cpp fdmdv2_plot_waterfall.h fdmdv2_plot.h fdmdv2_defines.h
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) fdmdv2_plot_waterfall.cpp

fdmdv2_process_audio.$(OBJEXT):  fdmdv2_process_audio.cpp 
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) fdmdv2_process_audio.cpp

fdmdv2_pa_wrapper.$(OBJEXT): fdmdv2_pa_wrapper.cpp  fdmdv2_pa_wrapper.h
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) fdmdv2_pa_wrapper.cpp

dlg_about.$(OBJEXT):  dlg_about.cpp dlg_about.h 
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) dlg_about.cpp

dlg_audio.$(OBJEXT):  dlg_audio.cpp dlg_audio.h 
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) dlg_audio.cpp

dlg_comports.$(OBJEXT):  dlg_comports.cpp dlg_comports.h 
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) dlg_comports.cpp

dlg_options.$(OBJEXT):  dlg_options.cpp dlg_options.h 
	$(GPP) -c $(DEFINES) $(INCLUDE_PATHS) $(CFLAGS) $(WX_FLAGS) dlg_options.cpp

#fdmdv2$(EXE_EXT): topFrame.$(OBJEXT) topFrame.$(OBJEXT) fdmdv2_main.$(OBJEXT) fdmdv2_plot.$(OBJEXT) dlg_about.$(OBJEXT) dlg_audio.$(OBJEXT) dlg_comports.$(OBJEXT) dlg_options.$(OBJEXT) paclass.$(OBJEXT)

fdmdv2$(EXE_EXT): $(OBJS)
	$(GPP) -o fdmdv2$(EXE_EXT) $(OBJS) $(DEFINES) $(CFLAGS) $(INCLUDE_PATHS) $(WX_FLAGS) $(LIB_PATHS) $(LIB_FLAGS) $(LIBS)  

all: fdmdv2$(EXE_EXT)

clean:
	rm -f *.o *.exe
