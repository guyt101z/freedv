include(ExternalProject)
ExternalProject_Add(codec2
   SVN_REPOSITORY https://freetel.svn.sourceforge.net/svnroot/freetel/codec2-dev
   CMAKE_ARGS -DBUILD_SHARED_LIBS=FALSE 
   INSTALL_COMMAND ""
)
find_library(CODEC2_LIBRARIES codec2
    PATHS ${CMAKE_BINARY_DIR}/codec2-prefix/src/codec2-build/src)
include_directories(${CMAKE_BINARY_DIR}/codec2-prefix/src/codec2/src)
list(APPEND FREEDV_LINK_LIBS ${CODEC2_LIBRARIES})
list(APPEND FREEDV_STATIC_DEPS codec2)
