# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT([VBS], [3.0], [assen.totin@gmail.com])
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

# Check for video back-ends
AC_CHECK_PROG([CHECK_MPLAYER],[mplayer],[yes],[no])
if test $CHECK_MPLAYER = "no" ; then
	AC_MSG_ERROR(["*** MPlayer not found. vbsm Won't be able to use it."])
else
	AC_DEFINE([HAVE_MPLAYER],[1],["Define if mplayer was found"])
fi

# FIXME: Find a way to get rid of the GStreamer version in package name?
PKG_CHECK_MODULES([GSTREAMER], [gstreamer-0.10],
	[AC_DEFINE([HAVE_GSTREAMER], [1], ["Define if GStreamer was found"])],
	[AC_MSG_ERROR(["*** GStreamer not found. vbsm Won't be able to use it."])]

])

PKG_CHECK_MODULES([GSTREAMER], [gstreamer-0.10])

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

