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

LIBS = -lcrypto

TARGET = vbss 

OBJS = gui.o ../common/network.o ../common/error.o ../common/config.o

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

install:
	cp -f $(TARGET) $(BINDIR)
	@if !(test -e $(IMGDIR)) ; then mkdir -p $(IMGDIR); fi
	cp -f vbs.png  $(IMGDIR)
	cp vbs.desktop.template vbs.desktop
	echo "Icon=$(IMGDIR)/vbs.png"  >> vbs.desktop
	cp -f vbs.desktop /usr/share/applications

uninstall:
	rm -f $(TARGET) $(BINDIR)/$(TARGET)
	rm -f $(IMGDIR)/vbs.png
	rm -f /usr/share/applications/vbs.desktop

$(TARGET): gui.c 
	$(CC) $(CFLAGS) $(INCLUDES) gui.c
	$(CC) -o $@ $(ADD_FLAGS) $(LIBS) $(OBJS)

