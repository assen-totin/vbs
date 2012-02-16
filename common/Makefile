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

OBJS = config.o error.o network.o util.o

INCLUDES=`pkg-config --cflags gtk+-2.0`
ADD_FLAGS=`pkg-config --cflags --libs gtk+-2.0`

TARGET = config

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

$(TARGET): config.c error.c network.c util.c
	$(CC) $(CFLAGS) $(INCLUDES) error.c
	$(CC) $(CFLAGS) $(INCLUDES) config.c
	$(CC) $(CFLAGS) $(INCLUDES) network.c
	$(CC) $(CFLAGS) $(INCLUDES) util.c

