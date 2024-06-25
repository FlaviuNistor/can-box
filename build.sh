#!/bin/sh
set -x

CFLAGS="-c -Wall -g -fPIE"

# build the application

# build the source (pre-process, compile, assemble)
${CROSS_COMPILE}gcc $CFLAGS src/main.c src/serial.c src/can.c

# create binary (linking)
${CROSS_COMPILE}gcc -pie main.o serial.o can.o -lusb-1.0 -lcansocket -o can-box

# remove object files
rm -f main.o serial.o can.o

# print the dependencies
${CROSS_COMPILE}objdump -x can-box | grep NEEDED