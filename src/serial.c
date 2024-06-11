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


int configure_serial_port(){
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
    return 0;
}

int read_command(){
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    if ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0){
        /* Check if the lengh is correct*/
        if ((bytes_read - 1) != (buffer[0] - 0x30)){
            if (debug){
                printf("Wrong lenght: %x. Expected: %x\n", (bytes_read - 1), (buffer[0] - 0x30));
            }
            return 1;
        }
        /* Check if the ID is correct*/
        if (('1' != buffer[1]) || ('e' != buffer[2])){
            if (debug){
                printf("Wrong ID. BYTE1: %x BYTE2: %x\n", buffer[1], buffer[2]);
            }
            return 1;
        }

        //if (! strncmp(buffer, "quit", (bytes_read -1)))
            //break;
        buffer[bytes_read] = '\0'; // Null-terminate the string
        write(fd, buffer, bytes_read);
        printf("Received: %s", buffer);
        if (buffer[3] == '1'){
            return 2;
        }
    }
    if (bytes_read < 0) {
        perror("Error reading from USB serial port");
        return 1;
    }
}