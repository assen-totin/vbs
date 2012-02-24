#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT([VBS], [2.0.0], [assen.totin@gmail.com])
#AC_CONFIG_SRCDIR([config.h])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
#AC_PROG_INSTALL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
#AC_CHECK_HEADERS([fcntl.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/socket.h sys/timeb.h syslog.h unistd.h])
AC_CHECK_HEADERS([errno.h fcntl.h gdk/gdk.h gdk/gdkkeysyms.h glib.h gtk/gtk.h math.h netdb.h netinet/in.h pwd.h signal.h stdbool.h stdio.h stdlib.h string.h strings.h sys/ipc.h syslog.h sys/socket.h sys/stat.h sys/timeb.h sys/types.h sys/wait.h time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([bzero dup2 gethostbyname memset mkdir socket strstr])

# Check for packages
PKG_CHECK_MODULES([GTK], [gtk+-2.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])

# Makefiles
MAKEFILE_LIBTOOL=`which libtool`
MAKEFILE_CC=`which gcc`
MAKEFILE_SHELL=`which bash`

AC_SUBST([MAKEFILE_LIBTOOL])
AC_SUBST([MAKEFILE_CC])
AC_SUBST([MAKEFILE_SHELL])

AC_CONFIG_FILES([Makefile
                 common/Makefile
                 vbsd/Makefile
                 vbsm/Makefile
                 vbss/Makefile])
# Grand finale
AC_OUTPUT

