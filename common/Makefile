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

TARGET = config 

OBJS = config.vbss.o error.o network.o

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

$(TARGET): config.vbss.c error.c network.c
	$(CC) $(CFLAGS) error.c
	$(CC) $(CFLAGS) config.vbss.c
	$(CC) $(CFLAGS) network.c

