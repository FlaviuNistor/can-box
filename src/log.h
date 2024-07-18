#ifndef LOG_H
#define LOG_H

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/if_link.h>
#include <libsocketcan.h>

extern FILE *log_file;
extern FILE *dump_file;
unsigned int create_log_files(char * file_name);
void log_can_frame_to_dump(struct canfd_frame * can_frame);

#endif /* LOG_H */