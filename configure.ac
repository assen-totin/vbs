#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT([VBS], [2.2], [assen.totin@gmail.com])
ORIG_CMDLINE_ARGS=$@
AC_SUBST([ORIG_CMDLINE_ARGS])
AC_CONFIG_HEADERS([config.h])

# prefix
if test "x$prefix" = "xNONE" ; then
	prefix="/usr/local"
fi
if test "x$exec_prefix" = "xNONE" ; then
	exec_prefix=$prefix
fi
PREFIX=$prefix
AC_SUBST([PREFIX])
BINDIR=$exec_prefix/bin
AC_SUBST([BINDIR])
SBINDIR=$exec_prefix/sbin
AC_SUBST([SBINDIR])
DATAROOTDIR=$prefix/share
AC_SUBST([DATAROOTDIR])
IMGDIR=$DATAROOTDIR/icons
AC_SUBST([IMGDIR])

# Checks for programs.
AC_PROG_CC

AC_CHECK_PROG([CHECK_MPLAYER],[mplayer],[yes],[no])
if test $CHECK_MPLAYER = "no" ; then
	echo "*** MPlayer not found. You're losing video in vbsm."
else
	AC_DEFINE([HAVE_MPLAYER],[1],["Define if mplayer was found"])
fi

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Check for packages
PKG_CHECK_MODULES([GTK], [gtk+-2.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])

# Makefiles
MAKEFILE_CC=`which gcc`
AC_SUBST([MAKEFILE_CC])
AC_CONFIG_FILES([Makefile common/Makefile vbsd/Makefile vbsm/Makefile vbss/Makefile vbsc/Makefile])

# Desktop files
AC_CONFIG_FILES([vbsm/vbsm.desktop vbss/vbss.desktop vbsc/vbsc.desktop])

# RPM spec file
AC_CONFIG_FILES([vbs.spec])

# Icons
AC_DEFINE_UNQUOTED([VBS_ICON], ["$IMGDIR/vbs.png"], [Define icons location])

# Grand finale
AC_OUTPUT

echo
echo "Configuration complete."
echo
echo "Now, run make."
echo

