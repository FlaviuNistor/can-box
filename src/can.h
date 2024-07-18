#ifndef CAN_H
#define CAN_H

#define CAN_INTERFACE_NUMBER_MAX_DIGITS 1
#define CAN_INTERFACE_NAME_MAX_DIGITS   4
#define CAN0 0
#define CAN1 1

extern unsigned int can0_socket;

unsigned int open_can_socket(unsigned int * can_socket_idx, char can_interface_number);
unsigned int close_can_socket(unsigned int can_socket_idx);
unsigned int send_can_message(unsigned int can_socket_idx, struct canfd_frame * txframe);
unsigned int read_can_message(unsigned int can_socket_idx, struct canfd_frame * rxframe);

#endif /* CAN_H */