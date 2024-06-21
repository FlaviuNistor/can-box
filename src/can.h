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

#define TRANS_INTERFACE  1
#define REC_INTERFACE    0

static int trans_can_socket;
static int rec_can_socket;

#endif /* CAN_H */