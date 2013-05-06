#
# FreeDV - HF Digital Voice for Radio Amateurs
#
# CMake configuration contributed by Richard Shaw (KF5OIM)
# Please report questions, comments, problems, or patches to the freetel
# mailing list: https://lists.sourceforge.net/lists/listinfo/freetel-codec2
#

cmake_minimum_required(VERSION 2.8)

# Prevent in-source builds to protect automake/autoconf config.
# If an in-source build is attempted, you will still need to clean up a few
# files manually.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(FreeDV)

#
# Setup version.
# This should probably be pulled from an external file at some point.
#
set(FREEDV_VERSION_MAJOR 0)
set(FREEDV_VERSION_MINOR 2)
# Set to value for patch level releases, otherwise leave as FALSE.
set(FREEDV_VERSION_PATCH FALSE)
set(FREEDV_VERSION "${FREEDV_VERSION_MAJOR}.${FREEDV_VERSION_MINOR}")
if(FREEDV_VERSION_PATCH)
    set(FREEDV_VERSION ${FREEDV_VERSION}.${FREEDV_VERSION_PATCH})
endif(FREEDV_VERSION_PATCH)

# Set default build type
set(CMAKE_BUILD_TYPE "Release")

message(STATUS "FreeDV version: ${FREEDV_VERSION}")

#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Work around for not using a svn working copy.
add_definitions(-DSVN_REVISION="1238")

# Set default build flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

#
# Setup cmake options
#
set(USE_STATIC_DEPS FALSE CACHE BOOL 
    "Download and build static libraries instead of system libraries.")
set(USE_STATIC_PORTAUDIO FALSE CACHE BOOL 
    "Download and build static portaudio instead of the system library.")
set(USE_STATIC_SNDFILE FALSE CACHE BOOL
    "Download and build static sndfile instead of the system library.")
set(USE_STATIC_SAMPLERATE FALSE CACHE BOOL
    "Download and build static samplerate instead of the system library.")
set(USE_STATIC_SOX FALSE CACHE BOOL
    "Download and build static sox instead of the system library.")
set(USE_STATIC_LIBCTB FALSE CACHE BOOL
    "Download and build static libctb instead of the system library.")
set(USE_STATIC_CODEC2 FALSE CACHE BOOL
    "Download and build static codec2 instead of the system library.")
set(USE_STATIC_WXWIDGETS FALSE CACHE BOOL
    "Download and build static wxWidgets instead of the system library.")
if(USE_STATIC_DEPS)
    set(USE_STATIC_PORTAUDIO TRUE FORCE)
    set(USE_STATIC_SNDFILE TRUE FORCE)
    set(USE_STATIC_SAMPLERATE TRUE FORCE)
    set(USE_STATIC_SOX TRUE FORCE)
    set(USE_STATIC_LIBCTB TRUE FORCE)
    set(USE_STATIC_CODEC2 TRUE FORCE)
    #set(USE_STATIC_WXWIDGETS TRUE FORCE)
endif(USE_STATIC_DEPS)


# Setup empty list to append to.
set(FREEDV_LINK_LIBS "")
set(FREEDV_STATIC_LIBS "")

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
check_include_files("limits.h" HAVE_LIMITS_H)
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

# fdmdv2_main.h requires patching to find config.h as it current looks in the
# source directory and the generated file goes in the binary directory.
configure_file ("${PROJECT_SOURCE_DIR}/cmake/config.h.in"
                "${PROJECT_BINARY_DIR}/config.h" )
include_directories(${PROJECT_BINARY_DIR})
add_definitions(-DHAVE_CONFIG_H)

# Find or build portaudio Library
if(NOT USE_STATIC_PORTAUDIO)
    message(STATUS "Looking for portaudio...")
    find_library(PORTAUDIO portaudio)
    if(PORTAUDIO)
        list(APPEND FREEDV_LINK_LIBS ${PORTAUDIO})
        message(STATUS "  portaudio found: ${PORTAUDIO}")
    else(PORTAUDIO)
        message(FATAL_ERROR "portaudio library not found.
On Linux systems try installing:
    portaudio-devel  (RPM based systems)
    libportaudio-dev (DEB based systems)
On Windows it's easiest to use the cmake option: USE_STATIC_PORTAUDIO"
        )
    endif(PORTAUDIO)
else(NOT USE_STATIC_PORTAUDIO)
    message(STATUS "Will attempt static build of portaudio.")
    include(cmake/BuildPortaudio.cmake)
endif(NOT USE_STATIC_PORTAUDIO)
# Do we need this?
#find_library(PORTAUDIOCPP portaudiocpp)

# Pthread Library
find_package(Threads REQUIRED)
message(STATUS "Threads library flags: ${CMAKE_THREAD_LIBS_INIT}")

# Samplerate Library
if(NOT USE_STATIC_SAMPLERATE)
    message(STATUS "Looking for samplerate...")
    find_library(LIBSAMPLERATE samplerate)
    if(LIBSAMPLERATE)
        list(APPEND FREEDV_LINK_LIBS ${LIBSAMPLERATE})
        message(STATUS "  samplerate found: ${LIBSAMPLERATE}")
    else(LIBSTAMPLERATE)
        message(FATAL_ERROR "samplerate library not found.
On Linux systems try installing:
    samplerate-devel  (RPM based systems)
    libsamplerate-dev (DEB based systems)
On Windows it's easiest to use the cmake option: USE_STATIC_SAMPLERATE"
        )
    endif(LIBSAMPLERATE)
else(NOT USE_STATIC_SAMPLERATE)
    message(STATUS "Will attempt static build of samplerate.")
    include(cmake/BuildSamplerate.cmake)
endif(NOT USE_STATIC_SAMPLERATE)
    
# sndfile Library
if(NOT USE_STATIC_SNDFILE)
    message(STATUS "Looking for sndfile...")
    find_library(LIBSNDFILE sndfile)
    if(LIBSNDFILE)
        list(APPEND FREEDV_LINK_LIBS ${LIBSNDFILE})
        message(STATUS "  sndfile found: ${LIBSNDFILE}")
    else(LIBSNDFILE)
        message(FATAL_ERROR "sndfile library not found.
On Linux systems try installing:
    libsndfile-devel (RPM based systems)
    libsndfile-dev   (DEB based systems)
On Windows it's easiest to use the cmake option: USE_STATIC_SNDFILE"
        )
    endif(LIBSNDFILE)
else(NOT USE_STATIC_SNDFILE)
    message(STATUS "Will attempt static build of sndfile.")
    include(cmake/BuildSndfile.cmake)
endif(NOT USE_STATIC_SNDFILE)

# Find sox library
if(NOT USE_STATIC_SOX)
    message(STATUS "Looking for sox...")
    find_library(LIBSOX sox)
    if(LIBSOX)
        list(APPEND FREEDV_LINK_LIBS ${LIBSOX})
        message(STATUS "  sox found: ${LIBSOX}")
    else(LIBSOX)
        message(FATAL_ERROR "sox library not found.
On Linux systems try installing:
    sox-devel (RPM based systems)
    libsox-dev (DEB based systems)
On Windows it's easiest to use the cmake option: USE_STATIC_SOX"
        )
    endif(LIBSOX)
else(NOT USE_STATIC_SOX)
    message(STATUS "Will attempt static build of sox.")
    include(cmake/BuildSox.cmake)
endif(NOT USE_STATIC_SOX)

#
# Find wxWidgets
#
set(WXCONFIG "" CACHE FILEPATH "Location of wx-config binary.")
set(WXRC "" CACHE FILEPATH "Location of wxrc binary.")
if(USE_STATIC_WXWIDGETS)
    include(cmake/BuildWxWidgets.cmake)
else(USE_STATIC_WXWIDGETS)
    message(STATUS "Looking for wxWidgets...")
    if(WXCONFIG)
        message(STATUS "wx-config: ${WXCONFIG}")
        set(wxWidgets_CONFIG_EXECUTABLE ${WXCONFIG})
    endif(WXCONFIG)
    if(WXRC)
        message(STATUS "wxrc: ${WXRC}")
        set(wxWidgets_wxrc_EXECUTABLE ${WXRC})
    endif(WXRC)
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
endif(USE_STATIC_WXWIDGETS)

#
# Find codec2
#
if(NOT USE_STATIC_CODEC2)
    message(STATUS "Looking for codec2...")
    find_path(CODEC2_INCLUDE_DIR codec2.h
              PATH_SUFFIXES codec2)
    find_library(CODEC2_LIBRARY NAMES codec2)
    message(STATUS "  codec2 library: ${CODEC2_LIBRARY}")
    message(STATUS "  codec2 headers: ${CODEC2_INCLUDE_DIR}")
    if(CODEC2_INCLUDE_DIR AND CODEC2_LIBRARY)
        include_directories(${CODEC2_INCLUDE_DIR})
        list(APPEND FREEDV_LINK_LIBS ${CODEC2_LIBRARY})
    else(CODEC2_INCLUDE_DIR AND CODEC2_LIBRARY)
        message(FATAL_ERROR "codec2 library not found.
Linux: 
Codec2 may not be in your distribution so build yourself or use the cmake option to build statically into FreeDV.
Windws:
It's easiest to use the cmake option: USE_STATIC_PORTAUDIO"
        )
    endif(CODEC2_INCLUDE_DIR AND CODEC2_LIBRARY)
else(NOT USE_STATIC_CODEC2)
    message(STATUS "Will attempt static build of codec2.")
    include(cmake/BuildCodec2.cmake)
endif(NOT USE_STATIC_CODEC2)

#
# Find libctb. Assumes version 0.16
#
if(NOT USE_STATIC_LIBCTB)
    message(STATUS "Looking for libctb...")
    find_path(LIBCTB_INCLUDE_DIR ctb.h
              PATH_SUFFIXES ctb-0.16
             )
    find_library(LIBCTB_LIBRARY NAMES ctb ctb-0.16)
    message(STATUS "  libctb library: ${LIBCTB_LIBRARY}")
    message(STATUS "  libctb headers: ${LIBCTB_INCLUDE_DIR}")
        if(LIBCTB_LIBRARY AND LIBCTB_INCLUDE_DIR)
            set(CMAKE_REQUIRED_LIBRARIES ${LIBCTB_LIBRARY})
            # Check to make sure linking with libctb works.
            include(CheckCXXSourceCompiles)
            check_cxx_source_compiles("
                #include <ctb-0.16/ctb.h>
                int main() {
                ctb::SerialPort*        m_serialPort;
                m_serialPort = new ctb::SerialPort();
                };"
                LIBCTB_LINKS)
            if(NOT LIBCTB_LINKS)
                message(FATAL_ERROR "Linking libctb failed.")
            endif(NOT LIBCTB_LINKS)
        else(LIBCTB_LIBRARY AND LIBCTB_INCLUDE_DIR)
            message(FATAL_ERROR "libctb not found.
Linux:
  libctb may not be available in your distribution. Either build and install or use the cmake option to build statically.
Windws:
It's easiest to use the cmake option: USE_STATIC_LIBCTB"
            )
        endif(LIBCTB_LIBRARY AND LIBCTB_INCLUDE_DIR)
else(NOT USE_STATIC_LIBCTB)
    include(cmake/BuildLibctb.cmake)
endif(NOT USE_STATIC_LIBCTB)

include_directories(${LIBCTB_INCLUDE_DIR})
list(APPEND FREEDV_LINK_LIBS ${LIBCTB_LIBRARY})

# Freedv 
add_subdirectory(src)

# Icons and desktop file
add_subdirectory(contrib)

#
# Cpack NSIS configuration for Windows. EXPERIMENTAL!!!
#
if(WIN32)
    include(InstallRequiredSystemLibraries)
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HF Digital Voice for Radio Amateurs")
    set(CPACK_PACKAGE_VENDOR "CMake")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
    set(CPACK_PACKAGE_VERSION_MAJOR ${FREEDV_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${FREEDV_VERSION_MINOR})
    if(FREEDV_VERSION_PATCH)
        set(CPACK_PACKAGE_VERSION_PATCH ${FREEDV_VERSION_PATCH})
    endif(FREEDV_VERSION_PATCH)
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "FreeDV")
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    #set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    #set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\freedv.org")
    set(CPACK_NSIS_MODIFY_PATH ON)
    include(CPack)
endif(WIN32)
