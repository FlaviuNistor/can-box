#include <stdio.h>
#include <sys/time.h>
#include <linux/can.h>
/* added to support mem leack debugging */ 
#include <mcheck.h>

/* add local headers */
#include "log.h"

FILE *log_file = NULL;
FILE *dump_file = NULL;
uint64_t current_timestamp = 0;
uint64_t initial_timestamp = 0;

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

void print_can_frame_to_console(struct canfd_frame * can_frame){
    unsigned int i;
    
    printf("%" PRIu64 ": ", current_timestamp);
    /* check if error frame was received */
    if (can_frame->can_id & CAN_ERR_FLAG )
        printf("ERR Frame: ");
    /* check if standard or extended ID should be printed */
    if (can_frame->can_id & CAN_EFF_FLAG)
        printf("0x%08X ", (can_frame->can_id & CAN_EFF_MASK));
    else
        printf("0x%03X ", (can_frame->can_id & CAN_EFF_MASK));
    /* check if RTR frame */ 
    if (can_frame->can_id & CAN_RTR_FLAG)
        printf("RTR");
    else{
        printf("[%d]", can_frame->len);
            for (i = 0; i < can_frame->len; i++)
                printf(" %02x", (can_frame->data[i]));
    }
    printf("\n");
}

void log_can_frame_to_dump(struct canfd_frame * can_frame){
    unsigned int i;

    fprintf(dump_file,"%" PRIu64 ": ", current_timestamp);
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

uint64_t get_current_timestamp() {
    struct timeval te;
    uint64_t milliseconds;
    gettimeofday(&te, NULL); /* get current time */
    milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; /* calculate milliseconds */
    if (initial_timestamp != 0){
        return (milliseconds - initial_timestamp);
    }
    else{
        initial_timestamp = milliseconds;
        return 0;
    }
}
