#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

AC_INIT([fdmdv2], [0.2], [wittend@wwrinc.com])
AM_INIT_AUTOMAKE([-Wall -Werror])
#AC_CONFIG_SRCDIR([fdmdv2_main.cpp])
AC_CONFIG_HEADERS([config.h])


m4_define([SVN_REV], m4_esyscmd([sh -c "svn info|grep Revision|cut -d' ' -f 2|tr '\n' ' '"]))
AC_DEFINE(SVN_REVISION, ["SVN_REV"], [SVN Revision])

AC_ARG_ENABLE([about-dialog],
        [AS_HELP_STRING([--enable-about-dialog],
                [use the new about dialog])],
        [enable_about_dialog=yes],
        [enable_about_dialog=no])

if test "$enable_about_dialog" == "yes"; then
   AC_DEFINE([_USE_ABOUT_DIALOG],,[use dlg_about.cpp/h])
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG([C++])
# Checks for libraries.
# FIXME: Replace `main' with a function in `-ladvapi32':
AC_CHECK_LIB([advapi32], [main])
# FIXME: Replace `main' with a function in `-lcodec2':
AC_CHECK_LIB([codec2], [main])
# FIXME: Replace `main' with a function in `-lcomctl32':
AC_CHECK_LIB([comctl32], [main])
# FIXME: Replace `main' with a function in `-lcomdlg32':
AC_CHECK_LIB([comdlg32], [main])
# FIXME: Replace `main' with a function in `-lgdi32':
AC_CHECK_LIB([gdi32], [main])
# FIXME: Replace `main' with a function in `-lkernel32':
AC_CHECK_LIB([kernel32], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lole32':
AC_CHECK_LIB([ole32], [main])
# FIXME: Replace `main' with a function in `-loleaut32':
AC_CHECK_LIB([oleaut32], [main])
# FIXME: Replace `main' with a function in `-lportaudio':
AC_CHECK_LIB([portaudio], [src_new])
# FIXME: Replace `main' with a function in `-lportaudiocpp':
AC_CHECK_LIB([portaudiocpp], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lrpcrt4':
AC_CHECK_LIB([rpcrt4], [main])
# FIXME: Replace `main' with a function in `-lshell32':
AC_CHECK_LIB([shell32], [main])
# FIXME: Replace `main' with a function in `-luser32':
AC_CHECK_LIB([user32], [main])
# FIXME: Replace `main' with a function in `-luuid':
AC_CHECK_LIB([uuid], [main])
# FIXME: Replace `main' with a function in `-lwinmm':
AC_CHECK_LIB([winmm], [main])
# FIXME: Replace `main' with a function in `-lwinspool':
AC_CHECK_LIB([winspool], [main])
# FIXME: Replace `main' with a function in `-lwsock32':
AC_CHECK_LIB([wsock32], [main])
# FIXME: Replace `main' with a function in `-lwxexpat':
AC_CHECK_LIB([wxexpat], [main])
# FIXME: Replace `main' with a function in `-lwxjpeg':
AC_CHECK_LIB([wxjpeg], [main])
# FIXME: Replace `main' with a function in `-lwxmsw29u':
AC_CHECK_LIB([wxmsw29u], [main])
# FIXME: Replace `main' with a function in `-lwxpng':
AC_CHECK_LIB([wxpng], [main])
# FIXME: Replace `main' with a function in `-lwxregexu':
AC_CHECK_LIB([wxregexu], [main])
# FIXME: Replace `main' with a function in `-lwxtiff':
AC_CHECK_LIB([wxtiff], [main])
# FIXME: Replace `main' with a function in `-lwxzlib':
AC_CHECK_LIB([wxzlib], [main])

AC_SEARCH_LIBS([src_new], [samplerate])
AC_SEARCH_LIBS([sf_format_check], [sndfile])
AC_SEARCH_LIBS([sox_init], [sox])

AC_MSG_CHECKING([for libctb-0.16])
AC_LANG_PUSH(C++)
LIBS="-lctb-0.16 $LIBS"
AC_LINK_IFELSE([AC_LANG_PROGRAM(
                   [#include <ctb-0.16/ctb.h>],
                   [ctb::SerialPort*        m_serialPort;]
                   [m_serialPort = new ctb::SerialPort();])],
               ,
               [AC_MSG_ERROR([failed. Maybe you need to set LDPATH or CXXPATH. See configure --help])])
AC_LANG_POP
AC_MSG_RESULT([yes])

# Checks for header files. 
AC_CHECK_HEADERS([limits.h stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memset pow sqrt])

AC_CONFIG_FILES([Makefile src/Makefile])

## wxWidgets, from http://wiki.wxwidgets.org/Autoconf
AM_OPTIONS_WXCONFIG
WX_CONFIG_CHECK([2.9.0], [wxWin=1], , [aui,html,net] )
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
else 
     AC_MSG_NOTICE([Found wxWidgets 2.9])
fi

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

AC_MSG_NOTICE([-----------])
AC_MSG_NOTICE([  SVN_REVISION: SVN_REV])
AC_MSG_NOTICE([  enable about dialog: $enable_about_dialog ])
AC_MSG_NOTICE([-----------])

AC_OUTPUT