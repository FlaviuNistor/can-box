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

static int can_socket;
static struct canfd_frame rxframe;
static can_err_mask_t err_mask = 0xFFFFFFFF;   // receive all error types

#endif /* CAN_H */