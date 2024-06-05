#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <string.h>

// added to support mem leack debugging 
#include <mcheck.h>

#define USB_SERIAL_PORT "/dev/ttyGS0"
#define BUFFER_SIZE 256
#define VERSION "v0.0.1"

static int debug;

static void print_usage(char *prg)
{
    fprintf(stderr, "-h help\n");
    fprintf(stderr, "-d extra debug messages\n");
    fprintf(stderr, "-v version\n");
    fprintf(stderr, "Example: %s -d \n", prg);
}

static void print_version()
{
    fprintf(stderr, "%s\n", VERSION);
}

int main(int argc, char **argv){
    int opt;
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    // this will not affect in any way the execution but it is useful in case
    // a mem leak happens and debug is needed
    mtrace();
    
    debug = 0;

    while ((opt = getopt(argc, argv, "vdh?")) != -1) {
        switch (opt) {
            case 'v':
                print_version();
                break;
            case 'd':
                debug ++;
                break;
            case '?':
            case 'h':
            default:
                    print_usage(argv[0]);
                    return 1;
                    break;
                    }
            }

    if (0 != (argc - optind)) {
		print_usage(argv[0]);
		return 1;
	}

    // Open the USB serial port
    fd = open(USB_SERIAL_PORT, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Error opening USB serial port");
        return 1;
    }

    // Configure the serial port
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error from tcgetattr");
        close(fd);
        return 1;
    }
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD); // Enable receiver
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; // 8-bit characters
    tty.c_cflag &= ~PARENB; // No parity bit
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CRTSCTS; // No hardware flow control
    tcsetattr(fd, TCSANOW, &tty);

    // Write data to the USB serial port
    const char *message = "CAN Box connected\n";
    bytes_written = write(fd, message, strlen(message));
    if (bytes_written < 0) {
        perror("Error writing to USB serial port");
        close(fd);
        return 1;
    }

    // Read data from the USB serial port
    while(1) {
        if ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            if (! strncmp(buffer, "quit", (bytes_read -1)))
                break;
            buffer[bytes_read] = '\0'; // Null-terminate the string
            write(fd, buffer, bytes_read);
            printf("Received: %s", buffer);
        }
        if (bytes_read < 0) {
            perror("Error reading from USB serial port");
        }
    }
    fflush(stdout);
    close(fd);
    return 0;
}
