AC_INIT([fdmdv2], [0.2], [wittend@wwrinc.com])
AM_INIT_AUTOMAKE(fdmdv2, 0.2)
# Checks for programs.
AC_PROG_CC
#AC_PROG_LIBTOOL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor pow sqrt])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT