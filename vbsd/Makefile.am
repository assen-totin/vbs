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

sbin_PROGRAMS = vbsd
vbsd_SOURCES = vbsd.c
LDADD += ../common/libvbscommon.a -lm

if WITH_GSTREAMER
AM_CPPFLAGS += `pkg-config --cflags gstreamer-0.10`
endif

install-data-hook:
	test -z $(DESTDIR)/etc/init.d || mkdir -p $(DESTDIR)/etc/init.d; cp -f init/vbsd $(DESTDIR)/etc/init.d/vbsd 
if WITH_SYSTEMD
	test -z $(DESTDIR)/etc/systemd/system || mkdir -p $(DESTDIR)/etc/systemd/system; cp -f init/vbsd.service $(DESTDIR)/etc/systemd/system/vbsd.service
endif

uninstall-hook: 


