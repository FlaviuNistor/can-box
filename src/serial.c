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


unsigned int configure_serial_port(){
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

    //tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
    //tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
    //tty.c_oflag &= ~OPOST; // Raw output
    tcsetattr(fd, TCSANOW, &tty);
    return 0;
}

unsigned int read_command(){
    int i;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    if ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0){
	printf("buffer[0]: %x\n", buffer[0]);
        /* Check if the lengh is correct*/
        if (bytes_read != buffer[0]){
            if (debug){
                printf("Wrong length: %lx. Expected: %x\n", bytes_read, buffer[0]);
            }
            //return 1;
        }
        /* Check if the ID is correct*/
        if (buffer[1] != 0x1e){
            if (debug){
                printf("Wrong ID Signature. Received: %x; Expected: 0x1e\n", buffer[1]);
            }
            //return 1;
        } else {
            printf("Command ID: %x\n", buffer[2]);
        }

        

        //if (! strncmp(buffer, "quit", (bytes_read -1)))
            //break;
        //buffer[bytes_read] = '\0'; // Null-terminate the string
        //write(fd, buffer, bytes_read);
        printf("Received: ");
	// Last byte is enter
        for (i = 0; i < bytes_read; i++) {
            printf("%x ", buffer[i]);
        }
        printf("\n");


        if (buffer[3] == CMD_VERSION){
            return 2;
        }
    }
    if (bytes_read < 0) {
        perror("Error reading from USB serial port");
        return 1;
    }

    return 0;
}
