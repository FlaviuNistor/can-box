#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// added to support mem leack debugging 
#include <mcheck.h>

// add local headers
#include "can.h"

int open_can_socket(char can_interface_number){
    // declaration of variables
    static const int canfd_on = 1;
    static char *can_interface_name = ""
	struct sockaddr_can addr;
	struct ifreq ifr;       

    strcpy(can_interface_name, "can%d",n , 4);

    // create the socket
	if ((can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

    // get the name of the CAN interface under test. 
    strncpy(ifr.ifr_name, can_interface_name, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if (!ifr.ifr_ifindex) {
		perror("if_nametoindex");
		return 1;
	}

    // send command to the socket
	ioctl(can_socket, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
        
    /* try to switch the socket into CAN FD mode */
	if (setsockopt(can_socket, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on))){
        perror("CAN-FD switch");
        return 1;
    }

    /* try to force to receive also error frames */
    if (setsockopt(can_socket, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask))){
        perror("error frame receive switch");
        return 1;
    }
        
    // assign a instance to the socket
	if (bind(can_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

    return 0;           
}

int close_can_socket(){
    if (close(can_socket) < 0) {
		perror("Close");
		return 1;
	}
}

int send_can_message(struct canfd_frame txframe){                   
    if (write(can_socket, &txframe, sizeof(struct canfd_frame)) != sizeof(struct canfd_frame)) {
        perror("Write");
        return 1;
    } 

    return 0;           
}

struct canfd_frame read_can_message(){
    struct canfd_frame rxframe;
    int received_bytes;

    received_bytes = 0;
    // wait for frame to be received in a while(1) manner 
    received_bytes = read(can_socket, &rxframe, sizeof(struct canfd_frame));                                    
    if (received_bytes < 0) {
        perror("Read");
        return 1;
    }
    return rxframe;
}