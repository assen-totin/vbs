# This file is a part of Voody Blue Subtitler suit.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# Home page: http://bilbo.online.bg/~assen/vbs
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

SHELL = /bin/bash

CC = gcc
CFLAGS = -c -O2

BINDIR = /usr/local/bin
IMGDIR = /usr/local/share/vbs

INCLUDES=`pkg-config --cflags gtk+-2.0`
ADD_FLAGS=`pkg-config --cflags --libs gtk+-2.0`

TARGET = vbsc

OBJS_LOCAL = vbsc.o 
OBJS_REMOTE = ../common/error.o ../common/config.o ../common/network.o ../common/util.o ../common/menu.o

all: $(TARGET)

clean:
	rm -f $(OBJS_LOCAL) $(TARGET) vbs.desktop

install:
	cp -f $(TARGET) $(BINDIR)
	@if !(test -e $(IMGDIR)) ; then mkdir -p $(IMGDIR); fi
	cp -f ../common/vbs.png  $(IMGDIR)
	cp vbsm.desktop.template vbsm.desktop
	echo "Icon=$(IMGDIR)/vbs.png"  >> vbsm.desktop
	cp -f vbsm.desktop /usr/share/applications

uninstall:
	rm -f $(TARGET) $(BINDIR)/$(TARGET)
	rm -f $(IMGDIR)/vbs.png
	rm -f /usr/share/applications/vbsm.desktop

$(TARGET): vbsc.c
	$(CC) $(CFLAGS) $(INCLUDES) vbsc.c
	$(CC) -o $@ $(ADD_FLAGS) $(OBJS_LOCAL) $(OBJS_REMOTE)

