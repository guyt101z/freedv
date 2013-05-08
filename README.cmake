The CMake configuration for FDMDV2 (FreeDV) should be considered experimental at
this time but has been thouroughly tested on Fedora Linux and will likely work
well on most *nix systems and has many advanages over the autotools config. 

- Builds against system libraries (default).
- Or optionally download, build, and statically link with required libraries on
  an individual basis. See USE_STATIC_??? options.
- Has experimental NSIS packaing support for Windows (WIN32) targets. *nix
  systems should rely on 'make install' as the packages (RPM & DEB) created by
  CPack are questionable.

==========================
 Building and installing
==========================

    To test the cmake build make a directory anywhere underneath (or outside of)
the source directory.

Linux command line example:

$ cd /path/to/fdmdv2
$ mkdir build_linux
$ cd build_linux
$ cmake ../ (defaults to /usr/local, use CMAKE_INSTALL_PREFIX to override)
(if no errors)
$ make
(as root)
$ make install

===============================
 Bootstrapping wxWidgets build
===============================

If wxWidgets (>= 2.9) is not available then one option is to have CMake boot-
strap the build for FreeDV.

This is required because the tool wx-config is used to get the correct compiler
and linker flags of the wxWidgets components needed by FreeDV. Since this is
normally done at configure time, not during "make", it is not possible for CMake
to have this information prior to building wxWidgets.

In order to work around this issue you can "bootstrap" the wxWidgets build using
the CMake "BOOTSTRAP_WXWIDGETS" option. wxWidgets will be built using static 
libraries.

NOTE: This forces "USE_STATIC_WXWIDGETS" to be true.

(from any prefered directory outside of the source)
$ cmake -DBOOTSTRAP_WXWIDGETS=TRUE <path to source>
$ make
(wxWidgets is downloaded and built)
$ cmake -DBOOTSTRAP_WXWIDGETS=FALSE .
$ make
(if all goes well, as root)
$ make install
