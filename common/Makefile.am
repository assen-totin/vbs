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

noinst_LIBRARIES = libvbscommon.a
libvbscommon_a_SOURCES = cfg.c error.c network.c util.c menu.c

if WITH_GSTREAMER
AM_CPPFLAGS += `pkg-config --cflags gstreamer-0.10`
endif

icondir = $(datadir)/icons
icon_DATA = vbs.png
EXTRA_DIST = $(icon_DATA)

UPDATE_ICON_CACHE = gtk-update-icon-cache -f -t $(datadir)/icons || :
install-data-hook: 
	$(UPDATE_ICON_CACHE)
uninstall-hook: 
	$(UPDATE_ICON_CACHE)

