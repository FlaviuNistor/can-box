#include <inttypes.h>
#ifndef LOG_H
#define LOG_H

extern FILE *log_file;
extern FILE *dump_file;
extern uint64_t initial_timestamp;
extern uint64_t current_timestamp;
struct timestamp {
    unsigned int minutes;
    unsigned int seconds;
    unsigned int miliseconds;
}

unsigned int create_log_files(char * file_name);
void log_can_frame_to_dump(struct canfd_frame * can_frame);
void print_can_frame_to_console(struct canfd_frame * can_frame);
uint64_t get_current_timestamp();
void populate_timestamp(struct timestamp * ts, uint64_t milisec)

#endif /* LOG_H */