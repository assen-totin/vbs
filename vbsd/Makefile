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

LIBS = -lm

#BINDIR = /usr/local/bin
#IMGDIR = /usr/local/share/vbs

TARGET = vbsd 

OBJS_LOCAL = server.o 
OBJS_REMOTE = ../common/error.o ../common/config.o ../common/network.o ../common/util.o

INCLUDES=`pkg-config --cflags gtk+-2.0`
ADD_FLAGS=`pkg-config --cflags --libs gtk+-2.0`

all: $(TARGET)

clean:
	rm -f $(OBJS_LOCAL) $(TARGET) 

install:
	#cp -f $(TARGET) $(BINDIR)
	#@if !(test -e $(IMGDIR)) ; then mkdir -p $(IMGDIR); fi
	#cp -f vbs.png  $(IMGDIR)
	#cp vbs.desktop.template vbs.desktop
	#echo "Icon=$(IMGDIR)/vbs.png"  >> vbs.desktop
	#cp -f vbs.desktop /usr/share/applications

uninstall:
	#rm -f $(TARGET) $(BINDIR)/$(TARGET)
	#rm -f $(IMGDIR)/vbs.png
	#rm -f /usr/share/applications/vbs.desktop

$(TARGET): server.c
	$(CC) $(CFLAGS) $(INCLUDES) server.c
	$(CC) -o $@ $(LIBS) $(OBJS_LOCAL) $(OBJS_REMOTE)

