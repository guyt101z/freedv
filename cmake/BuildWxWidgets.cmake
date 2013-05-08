set(WXWIDGETS_TARBALL "wxWidgets-2.9.4")

include(ExternalProject)
ExternalProject_Add(wxWidgets
    URL http://downloads.sourceforge.net/wxwindows/${WXWIDGETS_TARBALL}.tar.bz2
    BUILD_IN_SOURCE 1
    INSTALL_DIR external/dist
    CONFIGURE_COMMAND ./configure --disable-shared --prefix=${CMAKE_BINARY_DIR}/external/dist
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
)
set(WXCONFIG "${CMAKE_BINARY_DIR}/external/dist/bin/wx-config")
set(WXRC "${CMAKE_BINARY_DIR}/external/dist/bin/wxrc")
#list(APPEND FREEDV_LINK_LIBS ${wxWidgets_LIBRARIES})
#list(APPEND FREEDV_STATIC_DEPS wxWidgets)
