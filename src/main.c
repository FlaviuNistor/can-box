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
#include "can.h"

#define VERSION "v0.0.2"

unsigned int debug;
unsigned int fd;

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
    int ret;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_written;
    struct canfd_frame my_txframe;
    struct canfd_frame my_rxframe;
    char i;

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

    memset(&my_rxframe, 0, sizeof(struct canfd_frame));
    my_txframe.can_id = 0x123;
    my_txframe.len = 2;
    my_txframe.data[0] = 0xAA;
    my_txframe.data[1] = 0x55;

    ret  = open_can_socket(0);
    if (ret)
        perror("Error opening socket");
    ret = send_can_message(&my_txframe);
    if (ret ==0 )
        printf("CAN Frame send out!\n");
    else
        printf("CAN Frame not send out!\n");
    ret = read_can_message(&my_rxframe);
    if (ret == 0){
        printf("CAN Frame received\n");
        printf("ID:0x%03X!\n", my_rxframe.can_id & CAN_EFF_MASK);
        printf("[%d]", my_rxframe.len);
        for (i = 0; i < my_rxframe.len; i++)
            printf(" %02x", (frame->data[i]));
    }
    else
        printf("CAN Frame not received!\n");   

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
