#define USB_SERIAL_PORT "/dev/ttyGS0"
#define BUFFER_SIZE 256
#define ID_BYTE1 '1'
#define ID_BYTE2 'e'

extern int debug;
extern int fd;
int configure_serial_port();
int read_command();