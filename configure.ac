AC_PREREQ([2.66])
AC_INIT([vbs], [3.1], [assen.totin@gmail.com])
# Commenting out -Werror until somebody fixes m4 for Gtk
#AM_INIT_AUTOMAKE([-Wall -Werror])
AM_INIT_AUTOMAKE([-Wall])

AC_PROG_RANLIB

AM_CONFIG_HEADER([config.h])

# For automake >= 1.12
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# Platform
FOUND_WINDOWS="no"
FOUND_POSIX="no"
AC_CHECK_HEADERS([windows.h], [FOUND_WINDOWS="yes"], [FOUND_POSIX="yes"])
if test $FOUND_WINDOWS = "yes" ; then
	AC_DEFINE([HAVE_WINDOWS],[1],[Windows OS])
elif test $FOUND_POSIX = "yes" ; then
	AC_DEFINE([HAVE_POSIX],[1],[POSIX OS])
fi
AM_CONDITIONAL([WITH_WINDOWS], [test $FOUND_WINDOWS = "yes"])
AM_CONDITIONAL([WITH_POSIX], [test $FOUND_POSIX = "yes"])

# prefix defaults
if test "x$prefix" = "xNONE" ; then
	prefix="/usr/local"
fi
if test "x$exec_prefix" = "xNONE" ; then
	exec_prefix=$prefix
fi
BINDIR=$exec_prefix/bin
if test "x$bindir" != "x\${exec_prefix}/bin" ; then
	BINDIR=$bindir
fi
SBINDIR=$exec_prefix/sbin
if test "x$sbindir" != "x\${exec_prefix}/sbin" ; then
	SBINDIR=$sbindir
fi
SYSCONFDIR=$prefix/etc
if test "x$sysconfdir" != "x\${prefix}/etc" ; then
	SYSCONFDIR=$sysconfdir
fi
DATAROOTDIR=$prefix/share
if test "x$datarootdir" != "x\${prefix}/share" ; then
	DATAROOTDIR=$datarootdir
fi
LOCALEDIR=$DATAROOTDIR/locale
if test "x$localedir" != "x\${datarootdir}/locale" ; then
	LOCALEDIR=$localedir
fi
if test $FOUND_WINDOWS = "yes" ; then
	LOCALEDIR="locale"
fi
TOPLEVELDIR=`pwd`

AC_SUBST([BINDIR])
AC_SUBST([SBINDIR])
AC_SUBST([SYSCONFDIR])
AC_SUBST([DATAROOTDIR])
AC_SUBST([TOPLEVELDIR])
AC_SUBST([LOCALEDIR])
AC_DEFINE_UNQUOTED([SYSCONFDIR],["$SYSCONFDIR"],[System config directory])
AC_DEFINE_UNQUOTED([LOCALEDIR],["$LOCALEDIR"],[Locale files])

# PKG - this will define $PKG_CONFIG
PKG_PROG_PKG_CONFIG

# CC
AC_PROG_CC
AC_SUBST([CC])

# gettext
AM_GNU_GETTEXT_VERSION(0.18.1)
AM_GNU_GETTEXT([external])
if test $USE_NLS != "yes" ; then
	echo "*** gettext not found. You're losing internationalization."
fi

# Check for MPlayer 
ECHO_BACKEND_MPLAYER="no"
FORCE_MPLAYER="undef"
FOUND_MPLAYER="no"
RPM_REQUIRE_MPLAYER=" "
AC_ARG_ENABLE([mplayer],[AS_HELP_STRING([--enable-mplayer],[compile with mplayer (default: yes on POSIX, no on Windows))])],[FORCE_MPLAYER=$enableval])
if test $FOUND_POSIX = "yes" ; then
	if test $FORCE_MPLAYER = "yes" || test $FORCE_MPLAYER = "undef" ; then
		AC_CHECK_PROG([FOUND_MPLAYER],[mplayer],[yes],[no])
	fi
	if test $FORCE_MPLAYER = "yes" && test $FOUND_MPLAYER = "no" ; then
		AC_MSG_ERROR(["*** MPlayer forced as backend, but not found."])
	elif test $FOUND_MPLAYER = "yes" ; then
        	AC_DEFINE([HAVE_MPLAYER],[1],["Define if MPlayer was found"])
        	ECHO_BACKEND_MPLAYER="yes"
	        RPM_REQUIRE_MPLAYER="Requires: mplayer >= 1.0"
	else
		echo "*** MPlayer not found, vbsm won't be able to use it."
	fi
else
	if test $FORCE_MPLAYER = "yes" ; then
		AC_MSG_ERROR(["*** MPlayer forced as backend, but not available on Windows."])
	fi
fi
AM_CONDITIONAL(WITH_MPLAYER, [test $FOUND_MPLAYER = "yes"])
AC_SUBST([RPM_REQUIRE_MPLAYER])

# Check for GStreamer
ECHO_BACKEND_GSTREAMER="no"
FORCE_GSTREAMER="undef"
TEST_GST_0_10="no"
TEST_GST_1_0="no"
HAVE_GSTREAMER="0"
FOUND_GSTREAMER="no"
FOUND_GSTREAMER_INTERFACES="no"
FOUND_GSTREAMER_PLUGINS_BASE="no"
FOUND_GSTREAMER_VIDEO="no"
NSIS_GSTREAMER_DLL=" "
NSIS_GSTREAMER_PATH=" "
USE_GSTREAMER="no"
RPM_REQUIRE_GSTREAMER=" "
AC_ARG_ENABLE([gstreamer],[AS_HELP_STRING([--enable-gstreamer],[compile GStreamer version (POSIX default: try 1.0, if missing, try 0.10; Windows default: try 0.10)])],[FORCE_GSTREAMER=$enableval])

if test $FORCE_GSTREAMER = "1.0" ; then
	if test $FOUND_POSIX = "yes" ; then
		TEST_GST_1_0="yes"
	else 
		AC_MSG_ERROR(["*** GStreamer 1.0 forced as backend, but not available on Windows."])
	fi
elif test $FORCE_GSTREAMER = "0.10" ; then
	TEST_GST_0_10="yes"
elif test $FORCE_GSTREAMER = "undef" ; then
	TEST_GST_0_10="yes";
	if test $FOUND_POSIX = "yes" ; then
		TEST_GST_1_0="yes"
	fi
fi

if test $TEST_GST_0_10 = "yes" ; then
	PKG_CHECK_MODULES([GSTREAMER_0_10], [gstreamer-0.10], [FOUND_GSTREAMER="0.10"], [FOUND_GSTREAMER="no"])
	if test $FOUND_GSTREAMER = "0.10" ; then
		PKG_CHECK_MODULES([GSTREAMER_INTERFACES_0_10], [gstreamer-interfaces-0.10], [FOUND_GSTREAMER_INTERFACES="yes"], [FOUND_GSTREAMER_INTERFACES="no"])
		PKG_CHECK_MODULES([GSTREAMER_PLUGINS_BASE_0_10], [gstreamer-plugins-base-0.10], [FOUND_GSTREAMER_PLUGINS_BASE="yes"], [FOUND_GSTREAMER_PLUGINS_BASE="no"])
	fi
fi
if test $TEST_GST_1_0 = "yes" ; then
        PKG_CHECK_MODULES([GSTREAMER_1_0], [gstreamer-1.0], [FOUND_GSTREAMER="1.0"],[FOUND_GSTREAMER="no"])
        if test $FOUND_GSTREAMER = "1.0" ; then
                PKG_CHECK_MODULES([GSTREAMER_PLUGINS_BASE_1_0], [gstreamer-plugins-base-1.0], [FOUND_GSTREAMER_PLUGINS_BASE="yes"],[FOUND_GSTREAMER_PLUGINS_BASE="no"])
                PKG_CHECK_MODULES([GSTREAMER_VIDEO_1_0], [gstreamer-video-1.0], [FOUND_GSTREAMER_VIDEO="yes"],[FOUND_GSTREAMER_VIDEO="no"])
        fi
fi

if test $FOUND_GSTREAMER = "no" ; then
	if test $FORCE_GSTREAMER = "1.0" || test $FORCE_GSTREAMER = "0.10" ; then
		AC_MSG_ERROR(["*** GStreamer forced as backend, but not found."])
	else
		echo "*** GStreamer not found. vbsm won't be able to use it."
	fi
elif test $FOUND_GSTREAMER = "0.10" ; then
	if test $FORCE_GSTREAMER = "0.10" ; then
		if test $FOUND_GSTREAMER_PLUGINS_BASE = "no" ; then
			AC_MSG_ERROR(["*** GStreamer forced as backend, but GStreamer-plugins-base not found."])
		elif test $FOUND_GSTREAMER_INTERFACES = "no" ; then
			AC_MSG_ERROR(["*** GStreamer forced as backend, but GStreamer-interfaces not found."])
		else 
			USE_GSTREAMER="yes"
		fi
	else
		if test $FOUND_GSTREAMER_PLUGINS_BASE = "no" ; then
			echo "*** GStreamer found, but GStreamer-plugins-base not found. Disabling GStreamer."
		elif test $FOUND_GSTREAMER_INTERFACES = "no" ; then
	                echo "*** GStreamer found, but GStreamer-interfaces not found. Disabling GStreamer."
		else
			USE_GSTREAMER="yes"
		fi
	fi
elif test $FOUND_GSTREAMER = "1.0" ; then
	if test $FORCE_GSTREAMER = "1.0" ; then
		if test $FOUND_GSTREAMER_PLUGINS_BASE = "no" ; then
			AC_MSG_ERROR(["*** GStreamer forced as backend, but GStreamer-plugins-base not found."])
		elif test $FOUND_GSTREAMER_VIDEO = "no" ; then
			AC_MSG_ERROR(["*** GStreamer forced as backend, but GStreamer-video not found."])
		else
			USE_GSTREAMER="yes"
		fi
	else
		if test $FOUND_GSTREAMER_PLUGINS_BASE = "no" ; then
			echo "*** GStreamer found, but GStreamer-plugins-base not found. Disabling GStreamer."
		elif test $FOUND_GSTREAMER_VIDEO = "no" ; then
			echo "*** GStreamer found, but GStreamer-video not found. Disabling GStreamer."
		else
			USE_GSTREAMER="yes"
		fi
	fi
fi

if test $USE_GSTREAMER = "yes" ; then
	AC_DEFINE([HAVE_GSTREAMER], [1], ["Define if GStreamer was found"])
	ECHO_BACKEND_GSTREAMER="yes ($FOUND_GSTREAMER)"
	if test $FOUND_GSTREAMER = "0.10" ; then 
		RPM_REQUIRE_GSTREAMER="Requires: gstreamer-plugins-base >= 0.10"
		# Automake is idiotic and treats --libs as linking argument, thus not allowing it in LDADD - and with Windows, linking order matters
		GSTREAMER_DASH_LIBS=`$PKG_CONFIG --libs gstreamer-0.10 gstreamer-interfaces-0.10`
		GSTREAMER_DASH_CFLAGS=`$PKG_CONFIG --cflags gstreamer-0.10 gstreamer-interfaces-0.10`
		NSIS_GSTREAMER_DLL="libgstreamer-0.10-0.dll"
		NSIS_GSTREAMER_PATH="gstreamer-0.10"
		AC_DEFINE([HAVE_GST_0_10], [1], [GStreamer 0.10])
	elif test  $FOUND_GSTREAMER = "1.0" ; then
		RPM_REQUIRE_GSTREAMER="Requires: gstreamer1-plugins-base >= 1.0"
		GSTREAMER_DASH_LIBS=`$PKG_CONFIG --libs gstreamer-1.0 gstreamer-video-1.0`
		GSTREAMER_DASH_CFLAGS=`$PKG_CONFIG --cflags gstreamer-1.0 gstreamer-video-1.0`
		NSIS_GSTREAMER_DLL="libgstreamer-1.0-0.dll"
		NSIS_GSTREAMER_PATH="gstreamer-1.0"
		AC_DEFINE([HAVE_GST_1_0], [1], [GStreamer 1.0])
	fi
	AC_SUBST([GSTREAMER_DASH_LIBS])
	AC_SUBST([GSTREAMER_DASH_CFLAGS])
	AC_SUBST([NSIS_GSTREAMER_DLL])
	AC_SUBST([NSIS_GSTREAMER_PATH])
fi

AM_CONDITIONAL(WITH_GSTREAMER, [test $USE_GSTREAMER = "yes"])
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
	NSIS_GDK_DLL="libgdk-3-0.dll"
	NSIS_GTK_DLL="libgtk-3-0.dll"
	NSIS_GTK_PATH="gtk-3.0"
elif test $USE_GTK = "2" ; then
	AC_DEFINE([HAVE_GTK2], [1], ["Define GTK2"])
	AC_DEFINE([GETTEXT_PACKAGE], ["gtk20"], ["GTK gettext"])
	RPM_REQUIRE_GTK="Requires: gtk2 >= 2.24.0"
	NSIS_GDK_DLL="libgdk-win32-2.0-0.dll"
	NSIS_GTK_DLL="libgtk-win32-2.0-0.dll"
	NSIS_GTK_PATH="gtk-2.0"
fi

AC_SUBST([RPM_REQUIRE_GTK])
AC_SUBST([NSIS_GDK_DLL])
AC_SUBST([NSIS_GTK_DLL])
AC_SUBST([NSIS_GTK_PATH])

# Check for GLib
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])
GTHREAD_LIBS=`$PKG_CONFIG --libs gthread-2.0`
AC_SUBST([GTHREAD_LIBS])

# systemd
USE_SYSTEMD="0"
if test $FOUND_POSIX = "yes" ; then 
	RPM_SYSTEMD_MKDIR=""
	RPM_SYSTEMD_SCRIPT=""
	RPM_SYSTEMD_POST=""
	AC_ARG_ENABLE([systemd],[AS_HELP_STRING([--enable-systemd],[install init script for systemd even if systemd is not present (default: autodetect))])],[FORCE_SYSTEMD="1"])
	AC_CHECK_PROG([FOUND_SYSTEMD],[systemctl],["1"],["0"])
	if test $FOUND_SYSTEMD = "1" ; then
		USE_SYSTEMD="1"
		echo "*** systemd found. Will install both systemd and SystemV init scripts."
	else
		if test $FORCE_SYSTEMD = "1" ; then
			USE_SYSTEMD="1"
			echo "*** systemd not found, but forced. Will install both systemd and SystemV init scripts."
		else
			echo "*** systemd not found. Will only install SystemV init script."
		fi
	fi
	if test $USE_SYSTEMD = "1" ; then
		RPM_SYSTEMD_MKDIR="mkdir -p \$RPM_BUILD_ROOT$SYSCONFDIR/systemd/system"
		RPM_SYSTEMD_SCRIPT="$SYSCONFDIR/systemd/system/vbsd.service"
		RPM_SYSTEMD_POST="systemctl enable vbsd.service"
	fi
	AC_SUBST([RPM_SYSTEMD_MKDIR])
	AC_SUBST([RPM_SYSTEMD_SCRIPT])
	AC_SUBST([RPM_SYSTEMD_POST])
fi
AM_CONDITIONAL(WITH_SYSTEMD, [test $USE_SYSTEMD = "1"])

# Makefiles
AC_CONFIG_FILES([Makefile common/Makefile vbsm/Makefile vbss/Makefile vbsc/Makefile po/Makefile.in po/Makefile help/Makefile])
if test $FOUND_POSIX = "yes" ; then
	AC_CONFIG_FILES([vbsd/Makefile vbsd/init/vbsd vbsd/init/vbsd.service])
fi

# Desktop files
if test $FOUND_POSIX = "yes" ; then
	AC_CONFIG_FILES([vbsm/vbsm.desktop vbss/vbss.desktop vbsc/vbsc.desktop])
fi

# RPM spec file
if test $FOUND_POSIX = "yes" ; then
	AC_CONFIG_FILES([vbs.spec])
fi

# Widows NSIS file
if test $FOUND_WINDOWS = "yes" ; then
	MINGW_PREFIX=`$CC -print-sysroot`"/mingw"
	AC_SUBST([MINGW_PREFIX])

	WIN_HELP_FILES="File /r \"help/exe/*\""
	AC_SUBST([WIN_HELP_FILES])

	WIN_LOCALE_FILES="File /r \"po/exe/*\""
	AC_SUBST([WIN_LOCALE_FILES])

	AC_CONFIG_FILES([vbs.nsis])
fi

# Icon
if test $FOUND_WINDOWS = "yes" ; then
	AC_DEFINE([VBS_ICON], ["vbs.png"], [Define icons location])
else
	AC_DEFINE_UNQUOTED([VBS_ICON], ["$DATAROOTDIR/icons/vbs.png"], [Define icons location])
fi

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

