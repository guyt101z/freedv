set(SNDFILE_TARBALL "libsndfile-1.0.25")

include(ExternalProject)
ExternalProject_Add(sndfile
    URL http://www.mega-nerd.com/libsndfile/files/${SNDFILE_TARBALL}.tar.gz
    BUILD_IN_SOURCE 1
    INSTALL_DIR external/dist
    CONFIGURE_COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}/external/dist --disable-external-libs
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
)
if(WIN32)
    set(SNDFILE_LIBRARIES
        ${CMAKE_BINARY_DIR}/external/dist/lib/sndfile.lib)
else(WIN32)
    set(SNDFILE_LIBRARIES
        ${CMAKE_BINARY_DIR}/external/dist/lib/libsndfile.a)
endif(WIN32)
include_directories(${CMAKE_BINARY_DIR}/external/dist/include)
list(APPEND FREEDV_LINK_LIBS ${SNDFILE_LIBRARIES})
list(APPEND FREEDV_STATIC_DEPS sndfile)
