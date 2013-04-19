cmake_minimum_required(VERSION 2.8)

# Prevent in-source builds to protect automake/autoconf config.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(FreeDV)

#
# Setup version.
# This should probably be pulled from an external file at some point.
#
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
# Set to value for patch level releases, otherwise leave as FALSE.
set(VERSION_PATCH FALSE)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
if(VERSION_PATCH)
	set(VERSION ${VERSION}.${VERSION_PATCH})
endif(VERSION_PATCH)

# Work around for not using a svn working copy.
add_definitions(-DSVN_REVISION="1234")

# Set default build flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Setup empty list to add to
set(FREEDV_LINK_LIBS "")

# Check for a bunch of libraries
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

	list(APPEND FREEDV_LINK_LIBS ${ADVAPI32} ${COMCTL32} ${COMDLG32} ${GDI32} ${KERNEL32} ${OLE32} ${OLEAUT32} ${RPCRT4} ${SHELL32} ${USER32} ${UUID} ${WINMM} ${WINSPOOL} ${WINSOCK32} ${WZEXPAT} ${WXJPEG} ${WXMSW29U} ${WXPNG} ${WXREGEXU} ${WXTIFF} ${WXZLIB})
endif(MSVC)

# Math library is automatic on MSVC
if(UNIX)
	set(CMAKE_REQUIRED_INCLUDES math.h)
	set(CMAKE_REQUIRED_LIBRARIES m)
endif(UNIX)

# Find some standard headers and functions.
include(CheckIncludeFiles)
check_include_files("limits.h" HAVE_STDINT_H)
check_include_files("stddef.h" HAVE_STDDEF_H)
check_include_files("stdlib.h" HAVE_STDLIB_H)
check_include_files("string.h" HAVE_STRING_H)

include(CheckTypeSize)
check_type_size("int" SIZEOF_INT)

include(CheckFunctionExists)
check_function_exists(floor HAVE_FLOOR)
check_function_exists(memset HAVE_MEMSET)
check_function_exists(pow HAVE_POW)
check_function_exists(sqrt HAVE_SQRT)

configure_file ("${PROJECT_SOURCE_DIR}/cmake/config.h.in"
                "${PROJECT_BINARY_DIR}/config.h" )
include_directories(${PROJECT_BINARY_DIR})

# Find portaudio Library
message(STATUS "Looking for portaudio...")
find_library(PORTAUDIO portaudio)
if(PORTAUDIO)
	list(APPEND FREEDV_LINK_LIBS ${PORTAUDIO})
	message(STATUS "  portaudio found: ${PORTAUDIO}")
else(PORTAUDIO)
	message(FATAL_ERROR "portaudio library not found.")
endif(PORTAUDIO)
# Do we need this?
find_library(PORTAUDIOCPP portaudiocpp)

# Pthread Library
message(STATUS "Looking for pthread...")
find_library(PTHREAD pthread)
if(PTHREAD)
	# Don't need to add linker flag, wx-config adds it already.
	message(STATUS "  pthread found: ${PTHREAD}")
else(PTHREAD)
	message(FATAL_ERROR "pthread library not found.")
endif(PTHREAD)

# Samplerate Library
message(STATUS "Looking for samplerate...")
find_library(LIBSAMPLERATE samplerate)
if(LIBSAMPLERATE)
	list(APPEND FREEDV_LINK_LIBS ${LIBSAMPLERATE})
	message(STATUS "  samplerate found: ${LIBSAMPLERATE}")
else(LIBSTAMPLERATE)
	message(FATAL_ERROR "samplerate library not found.")
endif(LIBSAMPLERATE)

# sndfile Library
message(STATUS "Looking for sndfile...")
find_library(LIBSNDFILE sndfile)
if(LIBSNDFILE)
	list(APPEND FREEDV_LINK_LIBS ${LIBSNDFILE})
	message(STATUS "  sndfile found: ${LIBSNDFILE}")
else(LIBSNDFILE)
	message(FATAL_ERROR "sndfile library not found.")
endif(LIBSNDFILE)

# Find sox library
message(STATUS "Looking for sox...")
find_library(LIBSOX sox)
if(LIBSOX)
	list(APPEND FREEDV_LINK_LIBS ${LIBSOX})
	message(STATUS "  sox found: ${LIBSOX}")
else(LIBSOX)
	message(FATAL_ERROR "sox library not found.")
endif(LIBSOX)

# Find wxWidgets
set(WXCONFIG "" CACHE FILEPATH "Location of wx-config binary.")
set(WXRC "" CACHE FILEPATH "Location of wxrc binary.")
message(STATUS "Looking for wxWidgets...")
if(WXCONFIG)
	set(wxWidgets_CONFIG_EXECUTABLE ${WXCONFIG})
endif()
if(WXRC)
	set(wxWidgets_wxrc_EXECUTABLE ${WXRC})
endif()
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
          PATH_SUFFIXES codec2)
find_library(CODEC2_LIBRARY NAMES codec2)
message(STATUS "  codec2 library: ${CODEC2_LIBRARY}")
message(STATUS "  codec2 headers: ${CODEC2_INCLUDE_DIR}")
include_directories(${CODEC2_INCLUDE_DIR})
list(APPEND FREEDV_LINK_LIBS ${CODEC2_LIBRARY})

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
