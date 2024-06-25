AUTOMAKE_OPTIONS = foreign

CFLAGS = -g -fPIE -Wall
LDFLAGS  = -pie -lusb-1.0

bin_PROGRAMS = can-box
can_box_SOURCES = src/main.c src/serial.c src/can.c
can_box_LDADD = -lsocketcan