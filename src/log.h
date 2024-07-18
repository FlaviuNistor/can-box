#include <inttypes.h>
#ifndef LOG_H
#define LOG_H

extern FILE *log_file;
extern FILE *dump_file;
extern uint64_t initial_timestamp;
extern uint64_t current_timestamp;

unsigned int create_log_files(char * file_name);
void log_can_frame_to_dump(struct canfd_frame * can_frame);
void print_can_frame_to_console(struct canfd_frame * can_frame);
uint64_t get_current_timestamp();

#endif /* LOG_H */