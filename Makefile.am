# This file is a part of Voody Blue Subtitler suit.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# Home page: http://www.zavedil.com/software-desktop-vbs
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

MINGW_PREFIX=@MINGW_PREFIX@

SUBDIRS = common vbsm vbss vbsc po help

if WITH_POSIX
SUBDIRS += vbsd
endif

DIST_SUBDIRS = common vbsm vbss vbsc vbsd po help 

dist_doc_DATA = AUTHORS BUGS ChangeLog COPYING INSTALL NEWS README TODO

rpm:
	rpmbuild -bb --define "_builddir @TOPLEVELDIR@" vbs.spec

exe:
	export PATH=$(PATH):$(MINGW_PREFIX)
	cd po; make exe_prep; cd ..
	cd help; make exe_prep; cd ..
	makensis vbs.nsis

