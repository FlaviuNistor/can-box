#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <string.h>

// added to support mem leack debugging 
#include <mcheck.h>

// add local headers
#include "serial.h"

#define VERSION "v0.0.1"

int debug;

static void print_usage(char *prg){
    fprintf(stderr, "-h help\n");
    fprintf(stderr, "-d extra debug messages\n");
    fprintf(stderr, "-v version\n");
    fprintf(stderr, "Example: %s -d \n", prg);
}

static void print_version(){
    fprintf(stderr, "%s\n", VERSION);
}


int main(int argc, char **argv){
    int opt;
    int fd;
    int ret;
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
                return 0;
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

    ret = configure_serial_port();
    if (ret){
        perror("Error configuring the serial port");
        return 1;
    }

    // Write data to the USB serial port
    const char *message = "CAN Box connected.\n";
    bytes_written = write(fd, message, strlen(message));
    if (bytes_written < 0) {
        perror("Error writing to USB serial port");
        close(fd);
        return 1;
    }

    // Read data from the USB serial port
    while(1) {
        ret = read_command();
        if (ret == 2)
            print_version();
    }
    fflush(stdout);
    close(fd);
    return 0;
}
