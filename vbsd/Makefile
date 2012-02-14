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

OBJS = server.o ../common/config.o ../common/error.o

all: $(TARGET)

clean:
	rm -f $(OBJS_CLIENT) $(OBJS_SERVER) $(TARGET_SERVER) $(TARGET_CLIENT) 

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
	$(CC) -o $@ $(LIBS) $(OBJS)

