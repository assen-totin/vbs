# This file is a part of Voody Blue Subtitler suit.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# Home page: http://www.zavedil.com/software-desktop-vbs
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

SUBDIRS = common vbsm vbss vbsc vbsd po
DIST_SUBDIRS = $(SUBDIRS)

dist_doc_DATA = AUTHOR BUGS CHANGES INSTALL LICENSE README TODO

rpm:
	rpmbuild -bb --define "_builddir @TOPLEVELDIR@" vbs.spec

# ORIGINAL Makefile.in BELOW

#all clean install uninstall: 
#	$(MAKE) -C common $@
#	$(MAKE) -C vbsm $@
#	$(MAKE) -C vbss $@
#	$(MAKE) -C vbsc $@
#	$(MAKE) -C vbsd $@
#	$(MAKE) -C po $@
#
#distclean:
#	$(MAKE) -C common $@
#	$(MAKE) -C vbsm $@
#	$(MAKE) -C vbss $@
#	$(MAKE) -C vbsc $@
#	$(MAKE) -C vbsd $@
#	$(MAKE) -C po $@
#	rm -f Makefile
#	rm -f config.h
#
#maintainer-clean:
#	make distclean
#	rm -f configure aclocal.m4 config.log config.status
#	rm -rf autom4te.cache
#
#rpmbuild:
#	rpmbuild -bb --define "_builddir @TOPLEVELDIR@" vbs.spec

