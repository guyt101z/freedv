set(PORTAUDIO_TARBALL "pa_stable_v19_20111121")

include(ExternalProject)
ExternalProject_Add(portaudio
   URL http://www.portaudio.com/archives/${PORTAUDIO_TARBALL}.tgz
   BUILD_IN_SOURCE 1
   INSTALL_DIR external/dist
   CONFIGURE_COMMAND ./configure --enable-cxx --without-jack --prefix=${CMAKE_BINARY_DIR}/external/dist
   BUILD_COMMAND make
   INSTALL_COMMAND make install
)
if(WIN32)
   set(PORTAUDIO_LIBRARIES
      ${CMAKE_BINARY_DIR}/external/dist/lib/portaudio.lib)
else(WIN32)
   find_library(RT rt)
   find_library(ASOUND asound)
   set(PORTAUDIO_LIBRARIES
	${CMAKE_BINARY_DIR}/external/dist/lib/libportaudio.a
	${RT}
	${ASOUND}
   )
endif(WIN32)
set(PORTAUDIO_INCLUDES external/dist/include)
list(APPEND FREEDV_STATIC_LIBS portaudio)
