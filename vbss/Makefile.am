# This file is a part of Voody Blue Subtitler suit.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# Home page: http://www.zavedil.com/software-desktop-vbs
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

GSTREAMER_LIBS = @GSTREAMER_DASH_LIBS@
GSTREAMER_CFLAGS = @GSTREAMER_DASH_CFLAGS@

VLC_LIBS = @VLC_DASH_LIBS@
VLC_CFLAGS = @VLC_DASH_CFLAGS@

AM_CPPFLAGS = @GTK_CFLAGS@
#LDADD = ../common/libvbscommon.a @GTK_LIBS@ @GTHREAD_LIBS@
LDADD = ../common/libvbscommon.a @GTK_LIBS@

bin_PROGRAMS = vbss
vbss_SOURCES = vbss.c keyboard.c keyboard.h vbss.h 

if WITH_GSTREAMER
AM_CPPFLAGS += $(GSTREAMER_CFLAGS)
LDADD += $(GSTREAMER_LIBS)
endif

if WITH_VLC
AM_CPPFLAGS += $(VLC_CFLAGS)
LDADD += $(VLC_LIBS)
endif

if WITH_WINDOWS
LDADD += -lwsock32 -lws2_32
AM_CFLAGS = -mwindows
endif

desktopdir = $(datadir)/applications
desktop_DATA = vbss.desktop

