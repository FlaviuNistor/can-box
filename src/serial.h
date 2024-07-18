#ifndef SERIAL_H
#define SERIAL_H

#define USB_SERIAL_PORT "/dev/ttyGS0"
#define BUFFER_SIZE 256
#define SIGNATURE_ID_BYTE 0x1e
#define CMD_VERSION 0x01

extern unsigned int debug;
extern unsigned int fd;

unsigned int configure_serial_port();
int read_command();

#endif /* SERIAL_H */
