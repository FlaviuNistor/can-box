#ifndef CAN_H
#define CAN_H

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/if_link.h>
#include <libsocketcan.h>

#define CAN_INTERFACE_NUMBER_MAX_DIGITS 1
#define CAN_INTERFACE_NAME_MAX_DIGITS   4

unsigned int open_can_socket(char can_interface_number);
unsigned int close_can_socket();
unsigned int send_can_message(struct canfd_frame * txframe);
unsigned int read_can_message(struct canfd_frame * rxframe);

#endif /* CAN_H */