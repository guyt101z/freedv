set(PORTAUDIO_TARBALL "pa_stable_v19_20111121")

# required linking libraries on linux. Not sure about windows.
find_library(ALSA_LIBRARIES asound)

if(UNIX AND NOT ALSA_LIBRARIES)
    message(ERROR "Could not find alsa library which is required for portaudio.
On Linux systems try installing:
    alsa-lib-devel  (RPM based systems)
    libasound2-dev  (DEB based systems)"
    )
endif(UNIX AND NOT ALSA_LIBRARIES)

include(ExternalProject)
ExternalProject_Add(portaudio
    URL http://www.portaudio.com/archives/${PORTAUDIO_TARBALL}.tgz
    BUILD_IN_SOURCE 1
    INSTALL_DIR external/dist
    CONFIGURE_COMMAND ./configure --enable-cxx --without-jack --prefix=${CMAKE_BINARY_DIR}/external/dist
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
)
if(WIN32)
    set(PORTAUDIO_LIBRARIES ${CMAKE_BINARY_DIR}/external/dist/lib/portaudio.lib)
else(WIN32)
    find_library(RT rt)
    find_library(ASOUND asound)
    set(PORTAUDIO_LIBRARIES
        ${CMAKE_BINARY_DIR}/external/dist/lib/libportaudio.a
        ${CMAKE_BINARY_DIR}/external/dist/lib/libportaudiocpp.a
        ${RT}
        ${ASOUND}
    )
endif(WIN32)
include_directories(${CMAKE_BINARY_DIR}/external/dist/include)
list(APPEND FREEDV_LINK_LIBS ${PORTAUDIO_LIBRARIES})
list(APPEND FREEDV_STATIC_DEPS portaudio)
