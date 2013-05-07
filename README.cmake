The CMake configuration for FDMDV2 (FreeDV) should be considered experimental at
this time but has been thouroughly tested on Fedora Linux and will likely work
well on most *nix systems and has many advanages over the autotools config. 

- Builds against system libraries (default).
- Or optionally download, build, and statically link with required libraries on
  an individual basis. See USE_STATIC_??? options.
- Has experimental NSIS packaing support for Windows (WIN32) targets. *nix
  systems should rely on 'make install' as the packages (RPM & DEB) created by
  CPack are questionable.

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
