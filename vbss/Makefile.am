# This file is a part of Voody Blue Subtitler suit.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# Home page: http://www.zavedil.com/software-desktop-vbs
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

AM_CPPFLAGS = @GTK_CFLAGS@
LDADD = @GTK_LIBS@

bin_PROGRAMS = vbss
vbss_SOURCES = vbss.c keyboard.c keyboard.h vbss.h 
LDADD += ../common/libvbscommon.a

if WITH_GSTREAMER
AM_CPPFLAGS += `pkg-config --cflags gstreamer-0.10`
AM_LDFLAGS = `pkg-config --libs gthread-2.0`
endif

desktopdir = $(datadir)/applications
desktop_DATA = vbss.desktop

