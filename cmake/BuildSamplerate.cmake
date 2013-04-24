set(SAMPLERATE_TARBALL "libsamplerate-0.1.8")

include(ExternalProject)
ExternalProject_Add(samplerate
   URL http://www.mega-nerd.com/SRC/${SAMPLERATE_TARBALL}.tar.gz 
   BUILD_IN_SOURCE 1
   INSTALL_DIR external/dist
   CONFIGURE_COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}/external/dist
   BUILD_COMMAND make
   INSTALL_COMMAND make install
)
if(WIN32)
   set(SAMPLERATE_LIBRARIES
      ${CMAKE_BINARY_DIR}/external/dist/lib/samplerate.lib)
else(WIN32)
   set(SAMPLERATE_LIBRARIES
	${CMAKE_BINARY_DIR}/external/dist/lib/libsamplerate.a)
endif(WIN32)
set(SAMPLERATE_INCLUDES external/dist/include)
list(APPEND FREEDV_STATIC_LIBS samplerate)
