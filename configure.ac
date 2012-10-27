# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT([vbs], [3.0], [assen.totin@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror])

ORIG_CMDLINE_ARGS=$@
AC_SUBST([ORIG_CMDLINE_ARGS])

AC_PROG_RANLIB

AM_CONFIG_HEADER([config.h])

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
LOCALEDIR=$prefix/share/locale
AC_SUBST([LOCALEDIR])
AC_DEFINE_UNQUOTED([LOCALEDIR],["$LOCALEDIR"],[Locale files])
TOPLEVELDIR=`pwd`
AC_SUBST([TOPLEVELDIR])

# Checks for programs.
AC_PROG_CC
AC_SUBST([CC])

AC_CHECK_PROG([FOUND_MSGFMT],[msgfmt],["yes"],["no"])
if test $FOUND_MSGFMT = "no"; then
	echo "*** msgfmt not found. Cannot continue."
	exit
else
	MSGFMT=`which msgfmt`
	AC_SUBST([MSGFMT])
fi

# Check for video back-ends
ECHO_BACKEND_MPLAYER="no"
ECHO_BACKEND_GSTREAMER="no"
FORCE_BACKEND="no"
HAVE_MPLAYER="0"
HAVE_GSTREAMER="0"
FOUND_GSTREAMER="no"
FOUND_GSTREAMER_INTERFACES="no"
FOUND_GSTREAMER_PLUGINS_BASE="no"
USE_MPLAYER="no"
USE_GSTREAMER="no"
RPM_REQUIRE_MPLAYER=" "
RPM_REQUIRE_GSTREAMER=" "
AC_ARG_ENABLE([backend],[AS_HELP_STRING([--enable-backend],[compile only for the specified video backend: mplayer or gstreamer (default: compile for both))])],[FORCE_BACKEND=$enableval])

AC_CHECK_PROG([FOUND_MPLAYER],[mplayer],[yes],[no])
PKG_CHECK_MODULES([GSTREAMER], [gstreamer-0.10], [FOUND_GSTREAMER="yes"], [FOUND_GSTREAMER="no"])
PKG_CHECK_MODULES([GSTREAMER_INTERFACES], [gstreamer-interfaces-0.10], [FOUND_GSTREAMER_INTERFACES="yes"], [FOUND_GSTREAMER_INTERFACES="no"])
PKG_CHECK_MODULES([GSTREAMER_PLUGINS_BASE], [gstreamer-plugins-base-0.10], [FOUND_GSTREAMER_PLUGINS_BASE="yes"], [FOUND_GSTREAMER_PLUGINS_BASE="no"])

if test $FORCE_BACKEND = "mplayer" ; then
	if test $FOUND_MPLAYER = "no" ; then
		AC_MSG_ERROR(["*** MPlayer forced as backend, but not found."])
	else
		USE_MPLAYER="yes"
	fi
elif test $FORCE_BACKEND = "gstreamer" ; then
	if test $FOUND_GSTREAMER = "no" ; then
		AC_MSG_ERROR(["*** GStreamer forced as backend, but not found."])
	elif test $FOUND_GSTREAMER_INTERFACES = "no" ; then
		AC_MSG_ERROR(["*** GStreamer forced as backend, but GStreamer-interfaces not found."])
	elif test $FOUND_GSTREAMER_PLUGINS_BASE = "no" ; then
		AC_MSG_ERROR(["*** GStreamer forced as backend, but GStreamer-plugins-base not found."])
	else
		USE_GSTREAMER="yes"
	fi
else
	if test $FOUND_MPLAYER = "no" ; then
		echo "*** MPlayer not found. vbsm Won't be able to use it."
	else
		USE_MPLAYER="yes"
	fi
	if test $FOUND_GSTREAMER = "no" ; then
		echo "*** GStreamer not found. vbsm Won't be able to use it."
	elif test $FOUND_GSTREAMER_INTERFACES = "no" ; then
		echo "*** GStreamer found, but GStreamer-interfaces not found. Disabling GStreamer."
	elif test $FOUND_GSTREAMER_PLUGINS_BASE = "no" ; then
		echo "*** GStreamer found, but GStreamer-plugins-base not found. Disabling GStreamer."
	else
		USE_GSTREAMER="yes"
	fi
fi

if test $USE_MPLAYER = "yes" ; then
	AC_DEFINE([HAVE_MPLAYER],[1],["Define if mplayer was found"])
	HAVE_MPLAYER="1"
	ECHO_BACKEND_MPLAYER="yes"
	RPM_REQUIRE_MPLAYER="Requires: mplayer >= 1.0"
fi
if test $USE_GSTREAMER = "yes" ; then
	AC_DEFINE([HAVE_GSTREAMER], [1], ["Define if GStreamer was found"])
	AM_CONDITIONAL(WITH_GSTREAMER, [test 1 = 1])
	ECHO_BACKEND_GSTREAMER="yes"
	RPM_REQUIRE_GSTREAMER="Requires: gstreamer-plugins-base >= 0.10"
fi

AC_SUBST([HAVE_MPLAYER])
AC_SUBST([HAVE_GSTREAMER])
AC_SUBST([RPM_REQUIRE_MPLAYER])
AC_SUBST([RPM_REQUIRE_GSTREAMER])

# Check for GTK and determine version
FORCE_GTK="no"
RPM_REQUIRE_GTK=" "
USE_GTK="0"
AC_ARG_ENABLE([gtk],[AS_HELP_STRING([--enable-gtk],[compile only for the specified GTK version, 2 or 3 (default: check both, use GTK-3, if not found, use GTK-2))])],[FORCE_GTK=$enableval])

if test $FORCE_GTK = "3" ; then
	AM_PATH_GTK_3_0([3.0.0], [USE_GTK="3"], [AC_MSG_ERROR(["*** GTK3 forced, but not found."])])
elif test $FORCE_GTK = "2" ; then
	AM_PATH_GTK_2_0([2.24.0], [USE_GTK="2"], [AC_MSG_ERROR(["*** GTK2 forced, but not found."])])
else
	AM_PATH_GTK_3_0([3.0.0], [USE_GTK="3"], [
		AM_PATH_GTK_2_0([2.24.0], [USE_GTK="2"])
	])
fi

if test $USE_GTK = "3" ; then
        AC_DEFINE([HAVE_GTK3], [1], ["Define GTK3"])
        AC_DEFINE([GETTEXT_PACKAGE], ["gtk30"], ["GTK gettext"])
        RPM_REQUIRE_GTK="Requires: gtk3 >= 3.4.0"
elif test $USE_GTK = "2" ; then
        AC_DEFINE([HAVE_GTK2], [1], ["Define GTK2"])
        AC_DEFINE([GETTEXT_PACKAGE], ["gtk20"], ["GTK gettext"])
        RPM_REQUIRE_GTK="Requires: gtk2 >= 2.24.0"
fi

AC_SUBST([RPM_REQUIRE_GTK])

# Check for GLib
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])

# systemd
RPM_SYSTEMD_MKDIR=""
RPM_SYSTEMD_SCRIPT=""
RPM_SYSTEMD_POST=""
AC_ARG_ENABLE([systemd],[AS_HELP_STRING([--enable-systemd],[install init script for systemd even if systemd is not present (default: autodetect))])],[FORCE_SYSTEMD=1])
AC_CHECK_PROG([FOUND_SYSTEMD],[systemctl],["1"],["0"])
if test $FOUND_SYSTEMD = "0" ; then
	if test $FORCE_SYSTEMD = "1" ; then
		FOUND_SYSTEMD = "1"
		echo "*** systemd not found, but forced. Will install both systemd and SystemV init scripts."
	else
		echo "*** systemd not found. Will only install SystemV init script."
	fi
else
	echo "*** systemd found. Will install both systemd and SystemV init scripts."
fi
if test $FOUND_SYSTEMD = "1" ; then
	RPM_SYSTEMD_MKDIR="mkdir -p $RPM_BUILD_ROOT/etc/systemd/system"
	RPM_SYSTEMD_SCRIPT="/etc/systemd/system/vbsd.service"
	RPM_SYSTEMD_POST="systemctl enable vbsd.service"
	AM_CONDITIONAL(WITH_SYSTEMD, [test 1 = 1])
fi
AC_SUBST([RPM_SYSTEMD_MKDIR])
AC_SUBST([RPM_SYSTEMD_SCRIPT])
AC_SUBST([RPM_SYSTEMD_POST])

# Makefiles
#MAKEFILE_CC=`which gcc`
#AC_SUBST([MAKEFILE_CC])
AC_CONFIG_FILES([Makefile common/Makefile vbsd/Makefile vbsm/Makefile vbss/Makefile vbsc/Makefile po/Makefile vbsd/init/vbsd vbsd/init/vbsd.service])

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
echo "* MPlayer: " $ECHO_BACKEND_MPLAYER
echo "* GStreamer: " $ECHO_BACKEND_GSTREAMER
echo "* GTK version: " $USE_GTK
echo
echo "Now, run make."
echo

