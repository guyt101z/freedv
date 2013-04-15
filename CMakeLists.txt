project(FreeDV)
cmake_minimum_required(VERSION 2.6)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")

add_definitions(-D_NO_AUTOTOOLS_ -DSVN_REVISION="1231")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# Setup empty list to add to
set(FREEDV_LINK_LIBS "")

# Check for a bunch of optional libraries
# Would it be better to assume these are available?
if(MSVC)
	find_library(ADVAPI32 advapi32)
	find_library(COMCTL32 comctl32)
	find_library(COMDLG32 comdlg32)
	find_library(GDI32 gdi32)
	find_library(KERNEL32 kernel32)
	find_library(OLE32 ole32)
	find_library(OLEAUT32 oleaut32)
	find_library(RPCRT4 rpcrt4)
	find_library(SHELL32 shell32)
	find_library(USER32 user32)
	find_package(UUID uuid)
	find_library(WINMM winmm)
	find_library(WINSPOOL winspool)
	find_library(WSOCK32 wsock32)
	find_library(WXEXPAT wxexpat)
	find_library(WXJPEG wxjpeg)
	find_library(WXMSW29U wxmsw29u)
	find_library(WXPNG wxpng)
	find_library(WXREGEXU wxregexu)
	find_library(WXTIFF wxtiff)
	find_library(WXZLIB wxzlib)

	set(MSVC_LINK_LIBS ${ADVAPI32} ${COMCTL32} ${COMDLG32} ${GDI32} ${KERNEL32} ${OLE32} ${OLEAUT32} ${RPCRT4} ${SHELL32} ${USER32} ${UUID} ${WINMM} ${WINSPOOL} ${WINSOCK32} ${WZEXPAT} ${WXJPEG} ${WXMSW29U} ${WXPNG} ${WXREGEXU} ${WXTIFF} ${WXZLIB})
	list(APPEND FREEDV_LINK_LIBS ${MSVC_LINK_LIBS})
endif(MSVC)

find_library(LIBM m )
if(LIBM)
	set(CMAKE_REQUIRED_INCLUDES math.h)
	set(CMAKE_REQUIRED_LIBRARIES m)
endif(LIBM)

find_library(PORTAUDIO portaudio)
if(PORTAUDIO)
	list(APPEND FREEDV_LINK_LIBS ${PORTAUDIO})
endif(PORTAUDIO)
find_library(PORTAUDIOCPP portaudiocpp)
find_library(PTHREAD pthread)
find_library(LIBSAMPLERATE samplerate)
if(LIBSAMPLERATE)
	list(APPEND FREEDV_LINK_LIBS ${LIBSAMPLERATE})
endif(LIBSAMPLERATE)
find_library(LIBSNDFILE sndfile)
if(LIBSNDFILE)
	list(APPEND FREEDV_LINK_LIBS ${LIBSNDFILE})
endif(LIBSNDFILE)
find_library(LIBSOX sox)
if(LIBSOX)
	list(APPEND FREEDV_LINK_LIBS ${LIBSOX})
endif(LIBSOX)

# Find some standard headers.
include(CheckIncludeFiles)
check_include_files("limits.h;stddef.h;stdlib.h;string.h" STDINC_FOUND)

include(CheckFunctionExists)
check_function_exists(floor HAVE_FLOOR)
check_function_exists(memset HAVE_MEMSET)
check_function_exists(pow HAVE_POW)
check_function_exists(sqrt HAVE_SQRT)
add_definitions(-DHAVE_FLOOR=${HAVE_FLOOR}
                -DHAVE_MEMSET=${HAVE_MEMSET}
                -DHAVE_POW=${HAVE_POW}
                -DHAVE_SQRT${HAVE_SQRT} )

# Find wxWidgets
message(STATUS "Looking for wxWidgets...")
if(NOT WXCONFIG)
	set(WXCONFIG "/usr/bin/wx-config")
endif()
set(wxWidgets_CONFIG_EXECUTABLE ${WXCONFIG})
if(NOT WXRC)
	set(WXRC "/usr/bin/wxrc")
endif()
set(wxWidgets_wxrc_EXECUTABLE ${WXRC})
set(WX_VERSION_MIN 2.9.0)
find_package(wxWidgets REQUIRED COMPONENTS core base aui html net adv REQUIRED)
execute_process(COMMAND sh "${wxWidgets_CONFIG_EXECUTABLE}" --version
	OUTPUT_VARIABLE WX_VERSION)
if(WX_VERSION VERSION_EQUAL ${WX_VERSION_MIN}
	OR WX_VERSION VERSION_GREATER ${WX_VERSION_MIN})
	message(STATUS "wxWidgets version: ${WX_VERSION}")
else()
	message(FATAL_ERROR "wxWidgets must be installed on your system.

                Please check that wx-config is in path, the directory
                where wxWidgets libraries are installed (returned by
                'wx-config --libs' or 'wx-config --static --libs' command)
                is in LD_LIBRARY_PATH or equivalent variable and
                wxWidgets version is ${WX_VERSION_MIN} or above."
	)
endif()
if(wxWidgets_FOUND)
	include("${wxWidgets_USE_FILE}")
	list(APPEND FREEDV_LINK_LIBS ${wxWidgets_LIBRARIES})
endif(wxWidgets_FOUND)


#
# Find codec2
#
message(STATUS "Looking for codec2...")
find_path(CODEC2_INCLUDE_DIR codec2.h
          PATH_SUFFIXES codec2
         )
find_library(CODEC2_LIBRARY NAMES codec2)
set(CODEC2_LIBRARIES ${CODEC2_LIBRARY})
set(CODEC2_INCLUDE_DIRS ${CODEC2_INCLUDE_DIR})
message(STATUS "  codec2 library: ${CODEC2_LIBRARIES}")
message(STATUS "  codec2 headers: ${CODEC2_INCLUDE_DIRS}")
include_directories(${CODEC2_INCLUDE_DIRS})
list(APPEND FREEDV_LINK_LIBS ${CODEC2_LIBRARIES})

#
# Find libctb. Assumes version 0.16
#
message(STATUS "Looking for libctb...")
find_path(LIBCTB_INCLUDE_DIR ctb.h
          PATH_SUFFIXES ctb-0.16
         )
find_library(LIBCTB_LIBRARY NAMES ctb ctb-0.16)
set(LIBCTB_LIBRARIES ${LIBCTB_LIBRARY})
set(LIBCTB_INCLUDE_DIRS ${LIBCTB_INCLUDE_DIR})
message(STATUS "  libctb library: ${LIBCTB_LIBRARIES}")
message(STATUS "  libctb headers: ${LIBCTB_INCLUDE_DIRS}")
# Check to make sure linking with libctb works.
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_LIBRARIES ${LIBCTB_LIBRARIES})
check_cxx_source_compiles("
	#include <ctb-0.16/ctb.h>
	int main() {
	ctb::SerialPort*        m_serialPort;
	m_serialPort = new ctb::SerialPort();
	};"
	LIBCTB_LINKS)
if(NOT LIBCTB_LINKS)
	message(FATAL_ERROR "Linking libctb failed.")
endif()

include_directories(${LIBCTB_INCLUDE_DIRS})
list(APPEND FREEDV_LINK_LIBS ${LIBCTB_LIBRARIES})


add_subdirectory(src)
