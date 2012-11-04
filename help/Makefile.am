# This file is a part of Voody Blue Subtitler suit.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# Home page: http://www.zavedil.com/software-desktop-vbs
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

noinst_HEADERS = docbook/* gtk/* win/* 

XSL_PREFIX = http://docbook.sourceforge.net/release/xsl/current
XSL_COMPILER = xsltproc
XSL_PLAINTEXT = $(XSL_PREFIX)/html/docbook.xsl
XSL_CHM = $(XSL_PREFIX)/htmlhelp/htmlhelp.xsl

help_textonly:
	for HELPFILE in `find docbook | grep xml`; do HELPDIR=`echo $$HELPFILE | sed s/\.xml$\//g | sed s/docbook\\\\///g`;  echo "Creating plaintext docs for lang " $$HELPDIR; $(XSL_COMPILER) $(XSL_PLAINTEXT) $$HELPFILE > gtk/$$HELPDIR.html; lynx -dump gtk/$$HELPDIR.html > gtk/$$HELPDIR.mo.tmp; grep -v "file://" gtk/$$HELPDIR.mo.tmp > gtk/$$HELPDIR.mo; rm -f gtk/$$HELPDIR.html gtk/$$HELPDIR.mo.tmp; done

CHM_COMPILER = tt2chm
CHM_TITLE = "VBS Guide"
CHM_OLD_INDEX = "index.html"
CHM_NEW_INDEX = "ar01s02.html"
CHM_TMP_DIR_1 = "chm_tmp_dir_1"
CHM_TMP_DIR_2 = "chm_tmp_dir_1"
CHM_TITLE_FILE = "title.txt"

help_chm: 
	for HELPFILE in `find docbook | grep xml`; do HELPDIR=`echo $$HELPFILE | sed s/\.xml$\//g | sed s/docbook\\\\///g`;  echo "Creating CHM docs for lang " $$HELPDIR; mkdir $(CHM_TMP_DIR_1); mkdir $(CHM_TMP_DIR_2); cd $(CHM_TMP_DIR_1); $(XSL_COMPILER) $(XSL_CHM) ../$$HELPFILE; rm -f htmlhelp.hhp toc.hhc; mv $(CHM_OLD_INDEX) $(CHM_NEW_INDEX); cd ..; cp $(CHM_TITLE_FILE) $(CHM_TMP_DIR_2); $(CHM_COMPILER) -t $(CHM_TITLE) -f $(CHM_NEW_INDEX) $(CHM_TMP_DIR_1) $(CHM_TMP_DIR_2); mv $(CHM_TMP_DIR_2)/$(CHM_TMP_DIR_2).chm win/$$HELPDIR.chm; chmod 644 win/$$HELPDIR.chm; rm -rf $(CHM_TMP_DIR_1) $(CHM_TMP_DIR_2); done

exe_prep:
	for HELPFILE in `find gtk | grep mo`; do HELPDIR=`echo $$HELPFILE | sed s/\.mo$\//g | sed s/gtk\\\\///g`; echo "Installing GTK help for lang " $$HELPDIR; mkdir -p exe/$$HELPDIR/LC_MESSAGES; cp -f $$HELPFILE exe/$$HELPDIR/LC_MESSAGES/vbs-help-gtk.mo; done;
	for HELPFILE in `find win | grep chm`; do HELPDIR=`echo $$HELPFILE | sed s/\.chm$\//g | sed s/win\\\\///g`; echo "Installing CHM help for lang " $$HELPDIR; mkdir -p exe/$$HELPDIR/LC_MESSAGES; cp -f $$HELPFILE exe/$$HELPDIR/LC_MESSAGES/vbs-help-win.chm; done;

rpm_prep:
	for HELPFILE in `find docbook | grep xml`; do HELPDIR=`echo $$HELPFILE | sed s/\.xml$\//g | sed s/docbook\\\\///g`;  echo "Renaming GNOME help for lang " $$HELPDIR; mv -f $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/vbs-help-gnome.mo.xml $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/vbs-help-gnome.mo; done

install-data-hook: 
	for HELPFILE in `find gtk | grep mo`; do HELPDIR=`echo $$HELPFILE | sed s/\.mo$\//g | sed s/gtk\\\\///g`;  echo "Installing GTK help for lang " $$HELPDIR; test -z $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/ || mkdir -p $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/; cp -f $$HELPFILE $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/vbs-help-gtk.mo; done
	for HELPFILE in `find docbook | grep xml`; do HELPDIR=`echo $$HELPFILE | sed s/\.xml$\//g | sed s/docbook\\\\///g`;  echo "Installing GNOME help for lang " $$HELPDIR; test -z $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/ || mkdir -p $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/; cp -f $$HELPFILE $(DESTDIR)$(localedir)/$$HELPDIR/LC_MESSAGES/vbs-help-gnome.mo.xml; done

