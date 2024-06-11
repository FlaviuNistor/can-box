#define USB_SERIAL_PORT "/dev/ttyGS0"
#define BUFFER_SIZE 256
#define ID_BYTE1 \x49
#define ID_BYTE2 \x69

static int configure_serial_port();
int read_command();