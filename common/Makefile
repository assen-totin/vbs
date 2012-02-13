# Makefile for netflow collector.
#
# autoconf not supported.
#
#

SHELL = /bin/bash

CC = gcc
CFLAGS = -c -O2

TARGET = config 

OBJS = config.o error.o network.o

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

$(TARGET): config.c error.c network.c
	$(CC) $(CFLAGS) error.c
	$(CC) $(CFLAGS) config.c
	$(CC) $(CFLAGS) network.c

