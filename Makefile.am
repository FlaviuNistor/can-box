AUTOMAKE_OPTIONS = foreign

CFLAGS = -g -fPIE -Wall
LDFLAGS  = -pie 

bin_PROGRAMS = can-box
can_box_SOURCES = src/main.c src/serial.c src/can.c src/log.c
can_box_LDADD = -lsocketcan -lusb-1.0