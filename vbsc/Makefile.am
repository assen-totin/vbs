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

bin_PROGRAMS = vbsc
vbsc_SOURCES = vbsc.c menu.c
LDADD += ../common/libvbscommon.a

if WITH_GSTREAMER
AM_CPPFLAGS += `pkg-config --cflags gstreamer-0.10`
endif

desktopdir = $(datadir)/applications
desktop_DATA = vbsc.desktop

