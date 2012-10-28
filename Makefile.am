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

dist_doc_DATA = AUTHORS BUGS ChangeLog INSTALL COPYING README TODO

rpm:
	rpmbuild -bb --define "_builddir @TOPLEVELDIR@" vbs.spec

