set(SOX_TARBALL "sox-14.4.1")

# required linking libraries on linux. Not sure about windows.
find_library(ALSA_LIBRARIES asound)
find_library(AO_LIBRARIES ao)

if(UNIX AND NOT ALSA_LIBRARIES)
    message(ERROR "Could not find alsa library.
On Linux systems try installing:
    alsa-lib-devel  (RPM based systems)
    libasound2-dev  (DEB based systems)"
    )
endif(UNIX AND NOT ALSA_LIBRARIES)

if(UNIX AND NOT AO_LIBRARIES)
    message(ERROR "Could not find libao.
On Linux systems try installing:
    libao-devel  (RPM based systems)
    libao-dev    (DEB based systems)"
    )
endif(UNIX AND NOT AO_LIBRARIES)


include(ExternalProject)
ExternalProject_Add(sox
    URL http://downloads.sourceforge.net/sox/${SOX_TARBALL}.tar.gz
    BUILD_IN_SOURCE 1
    INSTALL_DIR external/dist
    CONFIGURE_COMMAND ./configure --enable-shared=no --without-id3tag --without-png --disable-gomp --with-oggvorbis=no --with-oss=no --with-flac=no --with-amrnb=no --with-amrwb=no --with-mp3=no --with-wavpack=no --disable-dl-sndfile --with-pulseaudio=no --without-magic --prefix=${CMAKE_BINARY_DIR}/external/dist
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
)
if(WIN32)
    set(SOX_LIBRARIES
        ${CMAKE_BINARY_DIR}/external/dist/lib/sox.lib)
else(WIN32)
    set(SOX_LIBRARIES
        ${CMAKE_BINARY_DIR}/external/dist/lib/libsox.a
        ${ALSA_LIBRARIES}
        ${AO_LIBRARIES}
    )
endif(WIN32)
include_directories(${CMAKE_BINARY_DIR}/external/dist/include)
list(APPEND FREEDV_LINK_LIBS ${SOX_LIBRARIES})
list(APPEND FREEDV_STATIC_DEPS sox)

if(USE_STATIC_SNDFILE)
    add_dependencies(sox sndfile)
endif(USE_STATIC_SNDFILE)
