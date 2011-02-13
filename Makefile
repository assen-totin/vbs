# Makefile for netflow collector.
#
# autoconf not supported.
#
#

SHELL = /bin/bash

CC = gcc
CFLAGS = -c -O2

BINDIR = /usr/local/bin
IMGDIR = /usr/local/share/vbs

INCLUDES=`pkg-config --cflags gtk+-2.0`
ADD_FLAGS=`pkg-config --cflags --libs gtk+-2.0`

TARGET = vbs 

OBJS = gui.o keyboard.o mouse.o export.o edit.o import.o video.o menu.o vbs.o 

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) vbs.desktop

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

$(TARGET): gui.c keyboard.c mouse.c export.c import.c video.c edit.c menu.c vbs.c
	$(CC) $(CFLAGS) $(INCLUDES) gui.c
	$(CC) $(CFLAGS) $(INCLUDES) keyboard.c
	$(CC) $(CFLAGS) $(INCLUDES) mouse.c
	$(CC) $(CFLAGS) $(INCLUDES) export.c
	$(CC) $(CFLAGS) $(INCLUDES) import.c
	$(CC) $(CFLAGS) $(INCLUDES) video.c
	$(CC) $(CFLAGS) $(INCLUDES) edit.c
	$(CC) $(CFLAGS) $(INCLUDES) menu.c
	$(CC) $(CFLAGS) $(INCLUDES) vbs.c
	$(CC) -o $@ $(ADD_FLAGS) $(OBJS)

