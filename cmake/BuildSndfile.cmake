set(SNDFILE_TARBALL "libsndfile-1.0.25")

include(ExternalProject)
ExternalProject_Add(sndfile
   URL http://www.mega-nerd.com/libsndfile/files/${SNDFILE_TARBALL}.tar.gz
   BUILD_IN_SOURCE 1
   INSTALL_DIR external/dist
   CONFIGURE_COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}/external/dist
   BUILD_COMMAND make
   INSTALL_COMMAND make install
)
if(WIN32)
   set(SNDFILE_LIBRARIES
      ${CMAKE_BINARY_DIR}/external/dist/lib/sndfile.lib)
else(WIN32)
   set(SNDFILE_LIBRARIES
	${CMAKE_BINARY_DIR}/external/dist/lib/libsndfile.a)
endif(WIN32)
set(SNDFILE_INCLUDES external/dist/include)
list(APPEND FREEDV_STATIC_LIBS sndfile)
