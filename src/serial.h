#include <ctype.h>

#define USB_SERIAL_PORT "/dev/ttyGS0"
#define BUFFER_SIZE 256

#define SIGNATURE_ID_BYTE 0x1e

#define CMD_VERSION 0x01

extern int debug;
extern int fd;
int configure_serial_port();
int read_command();
