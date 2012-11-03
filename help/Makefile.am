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

XSL_PLAINTEXT = /usr/share/sgml/docbook/xsl-stylesheets-1.76.1/html/docbook.xsl

help_textonly:
	for HELPFILE in `ls docbook`; do NEWFILE=`echo $$HELPFILE | sed s/\.xml//g`;  echo "Creating plaintext docs for lang " $NEWFILE; xsltproc --nonet $(XSL_PLAINTEXT) docbook/$$HELPFILE > gtk/$$NEWFILE.html; lynx -dump gtk/$$NEWFILE.html > gtk/$$NEWFILE.mo.tmp; grep -v "file://" gtk/$$NEWFILE.mo.tmp > gtk/$$NEWFILE.mo  rm -f gtk/$$NEWFILE.html gtk/$$NEWFILE.mo.tmp; done

help_chm: 

exe_prep:
	for HELPFILE in `find gtk | grep mo`; do HELPDIR=`echo $$HELPFILE | sed s/\.mo$\//g | sed s/gtk\\\\///g`; echo "Installing help for lang " $$HELPDIR; mkdir -p exe/$$HELPDIR/LC_MESSAGES; cp -f $$HELPFILE exe/$$HELPDIR/LC_MESSAGES/vbs-help-gtk.mo; done;

install-data-hook: 
	for HELPFILE in `find gtk | grep mo`; do HELPDIR=`echo $$HELPFILE | sed s/\.mo$\//g | sed s/gtk\\\\///g`;  echo "Installing help for lang " $$HELPDIR; test -z $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/ || mkdir -p $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/; cp -f $$HELPFILE $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/vbs-help-gtk.mo; done

