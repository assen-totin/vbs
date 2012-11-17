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
vbsd_SOURCES = vbsd.c vbsd.h
LDADD += ../common/libvbscommon.a -lm

if WITH_GSTREAMER
AM_CPPFLAGS += `pkg-config --cflags gstreamer-0.10`
endif

install-data-hook:
	test -z $(DESTDIR)$(sysconfdir)/init.d || mkdir -p $(DESTDIR)$(sysconfdir)/init.d; chmod 755 init/vbsd; cp -f init/vbsd $(DESTDIR)$(sysconfdir)/init.d/vbsd
if WITH_SYSTEMD
	test -z $(DESTDIR)$(sysconfdir)/systemd/system || mkdir -p $(DESTDIR)$(sysconfdir)/systemd/system; chmod 755 init/vbsd.service; cp -f init/vbsd.service $(DESTDIR)$(sysconfdir)/systemd/system/vbsd.service
endif

uninstall-hook: 


