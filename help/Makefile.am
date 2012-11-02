# This file is a part of Voody Blue Subtitler suit.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# Home page: http://www.zavedil.com/software-desktop-vbs
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

noinst_HEADERS = bg_BG.mo en_US.mo en_GB.mo ru_RU.mo

exe_prep:
	for HELPDIR in `ls *mo | sed s/\.mo$\//g`; do mkdir -p exe/$$HELPDIR/LC_MESSAGES; HELPFILE=$$HELPDIR.mo; cp -f $$HELPFILE exe/$$HELPDIR/LC_MESSAGES/vbs-help.mo; done;

install-data-hook: 
	echo "Copying help files..."
	for HELPFILE in `ls *mo | sed s/\.mo$\//g`; do test -z $(DESTDIR)$(localedir)/$$HELPFILE/LC_MESSAGES/ || mkdir -p $(DESTDIR)$(localedir)/$$HELPFILE/LC_MESSAGES/; echo -n "Help file for lang "; echo $$HELPFILE; cp -f $$HELPFILE.mo $(DESTDIR)$(localedir)/$$HELPFILE/LC_MESSAGES/vbs-help.mo; done

