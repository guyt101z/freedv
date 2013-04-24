set(LIBCTB_TARBALL "libctb-0.16")

include(ExternalProject)
ExternalProject_Add(libctb
   URL https://iftools.com/download/ctb/0.16/${LIBCTB_TARBALL}.tar.gz
   # Project does not have configure. Abuse to change directory.
   CONFIGURE_COMMAND ""
   BINARY_DIR libctb-prefix/src/libctb/build
   #BUILD_IN_SOURCE 1
   BUILD_COMMAND $(MAKE) -f GNUmakefile
   INSTALL_DIR external/dist
   INSTALL_COMMAND $(MAKE) install prefix=${CMAKE_BINARY_DIR}/external/dist
)
if(WIN32)
   set(LIBCTB_LIBRARY ${CMAKE_BINARY_DIR}/external/dist/lib/ctb-0.16.lib)
else(WIN32)
   set(LIBCTB_LIBRARY ${CMAKE_BINARY_DIR}/external/dist/lib/libctb-0.16.a)
endif(WIN32)
set(LIBCTB_INCLUDE_DIR external/dist/include)
list(APPEND FREEDV_STATIC_LIBS libctb)
