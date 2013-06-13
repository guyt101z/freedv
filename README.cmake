The CMake configuration for FDMDV2 (FreeDV) should be considered experimental at
this time but has been thouroughly tested on Fedora Linux and will likely work
well on most *nix systems and has many advanages over the autotools config. 

- Builds against system libraries (by default).
- Or optionally download, build, and statically link with required libraries on
  an individual basis. See USE_STATIC_??? options. 
  NOTE: We need test reports on mingw/Windows, not all dependencies will build.
- Has experimental NSIS packaing support for Windows (WIN32) targets. *nix
  systems should rely on 'make install' or tradional packaing methods as the
  packages (RPM & DEB) created by CPack are questionable.

==================================
 Building and installing on Linux
==================================

To perform a cmake build create a directory anywhere underneath (or outside of)
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

====================================
 Building and installing on Windows
====================================

The windows build is similar to linux and follows the same basic workflow.

Only MinGW is supported. While it is likely possible to perform a pure MinGW
build, installing MSYS will make your life easier.

CMake may not automatically detect that you're in the MSYS environment. If this
occurs you need to pass cmake the proper generator:

cmake -G"MSYS Makefiles" [other options] </path/to/source>

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
the CMake option, "BOOTSTRAP_WXWIDGETS". wxWidgets will be built using static 
libraries.

NOTE: This forces "USE_STATIC_WXWIDGETS" to be true internally regarless of the
value set manually.

(from any prefered directory outside of the source)
$ cmake -DBOOTSTRAP_WXWIDGETS=TRUE <path to source>
$ make
(wxWidgets is downloaded and built)
$ cmake .
$ make
(if all goes well, as root)
$ make install
