#!/bin/sh
set -x

CFLAGS="-c -Wall -g -fPIE"

# build the application

# build the source (pre-process, compile, assemble)
${CROSS_COMPILE}gcc $CFLAGS main.c

# create binary (linking)
${CROSS_COMPILE}gcc -pie main.o -lusb-1.0 -o can-box

# print the dependencies
${CROSS_COMPILE}objdump -x can-box | grep NEEDED