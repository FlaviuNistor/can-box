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
/* added to support mem leack debugging */ 
#include <mcheck.h>

/* add local headers */
#include "can.h"
#include "log.h"

unsigned int can0_socket;

unsigned int open_can_socket(unsigned int * can_socket_idx, char can_interface_number){
    const int canfd_on = 1;
    struct sockaddr_can addr;
	struct ifreq ifr;      
    char can_interface_name[CAN_INTERFACE_NAME_MAX_DIGITS + 1];
    char number[CAN_INTERFACE_NUMBER_MAX_DIGITS + 1];
    static can_err_mask_t err_mask = 0xFFFFFFFF;   /* receive all error types */
    
    if (can_interface_number > 1 || can_interface_number < 0){
        printf("CAN interface number not valid.\n");
        return 1;
    }

    /* Create the string for the CAN interface number from string "can" and char to string 
    can_interface_number passed as a argument to the function call */
    snprintf(number, (CAN_INTERFACE_NUMBER_MAX_DIGITS + 1), "%d", can_interface_number );
    strcpy(can_interface_name, "can");
    strcat(can_interface_name, number);

    /* get the name of the CAN interface that should be used */ 
    strncpy(ifr.ifr_name, can_interface_name, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
    if (!ifr.ifr_ifindex) {
        perror("if_nametoindex");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    /* create the socket*/
    if ((*can_socket_idx = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket");
        return 1;
    }

    /* send command to the socket */
    ioctl(*can_socket_idx, SIOCGIFINDEX, &ifr);
            
    /* try to switch the socket into CAN FD mode */
    if (setsockopt(*can_socket_idx, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on))){
        perror("CAN-FD switch");
        return 1;
    }

    /* try to force to receive also error frames */
    if (setsockopt(*can_socket_idx, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask))){
        perror("error frame receive switch");
        return 1;
    }
            
    /* assign a instance to the socket */
    if (bind(*can_socket_idx, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        return 1;
    }
    return 0;              
}

unsigned int close_can_socket(unsigned int can_socket_idx){
    if ((close(can_socket_idx) < 0)) {
		perror("Close");
		return 1;
	}
    return 0;
}

unsigned int send_can_message(unsigned int can_socket_idx, struct canfd_frame * txframe){                   
    if (write(can_socket_idx, txframe, sizeof(struct canfd_frame)) != sizeof(struct canfd_frame)) {
        perror("Write");
        return 1;
    }
    current_timestamp = get_current_timestamp();
    return 0;           
}

unsigned int read_can_message(unsigned int can_socket_idx, struct canfd_frame * rxframe){
    unsigned int received_bytes;

    received_bytes = 0;
    /* wait for frame to be received in a while(1) manner */ 
    received_bytes = read(can_socket_idx, rxframe, sizeof(struct canfd_frame));
    current_timestamp = get_current_timestamp();                                    
    if (received_bytes < 0) {
        perror("Read");
        return 1;
    }
    return 0;
}