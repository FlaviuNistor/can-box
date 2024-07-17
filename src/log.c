#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/* added to support mem leack debugging */ 
#include <mcheck.h>

/* add local headers */
#include "log.h"

FILE *log_file = NULL;
FILE *dump_file = NULL;

unsigned int create_log_files(char * file_name){
    static char log_name[83]; /* suggested by -Wformat-overflow= */
    static char dump_name[83]; /* suggested by -Wformat-overflow= */
    sprintf(log_name, "%s.log", file_name);
    sprintf(dump_name, "%s-dump.log", file_name);  
	log_file = fopen(log_name, "w");
	if (!log_file) {
            perror("logfile");
            return 1;
	}
    dump_file = fopen(dump_name, "w");
	if (!dump_file) {
            perror("dumpfile");
            return 1;
	}
    return 0;
}

void log_can_frame_to_dump(struct canfd_frame * can_frame){
    unsigned int i;
    /* check if error frame was received */
    if (can_frame->can_id & CAN_ERR_FLAG )
        fprintf(dump_file, "ERR Frame: ");
    /* check if standard or extended ID should be printed */
    if (can_frame->can_id & CAN_EFF_FLAG)
        fprintf(dump_file, "0x%08X ", (can_frame->can_id & CAN_EFF_MASK));
    else
        fprintf(dump_file, "0x%03X ", (can_frame->can_id & CAN_EFF_MASK));
    /* check if RTR frame */ 
    if (can_frame->can_id & CAN_RTR_FLAG)
        fprintf(dump_file, "RTR");
    else{
        fprintf(dump_file, "[%d]", can_frame->len);
            for (i = 0; i < can_frame->len; i++)
                fprintf(dump_file, " %02x", (can_frame->data[i]));
    }
    fprintf(dump_file, "\n");
}