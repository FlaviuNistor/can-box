#include <inttypes.h>
#ifndef LOG_H
#define LOG_H

#define gotoxy(x,y)             printf("\033[%d;%dH", (y), (x))
#define move_up_x_lines(x)      printf("\033[%dA", (x))
#define move_down_x_lines(x)    printf("\033[%dB", (x))
#define move_right_x_column(x)  printf("\033[%dC", (x))
#define move_left_x_column(x)   printf("\033[%dD", (x))
#define clear_screen()          printf("\e[1;1H\e[2J")
#define clear_line()            printf("\33[2K\r")
#define set_color_black()       printf("\033[30m")
#define set_color_red()         printf("\033[31m")
#define set_color_green()       printf("\033[32m")
#define set_color_yellow()      printf("\033[33m")


extern FILE *log_file;
extern FILE *dump_file;
extern uint64_t initial_timestamp;
extern uint64_t current_timestamp;
struct timestamp {
    unsigned int minutes;
    unsigned int seconds;
    unsigned int miliseconds;
};

unsigned int create_log_files(char * file_name);
void log_can_frame_to_dump(struct canfd_frame * can_frame);
void print_can_frame_to_console(struct canfd_frame * can_frame);
uint64_t get_current_timestamp();
void populate_timestamp(struct timestamp * ts, uint64_t milisec);

#endif /* LOG_H */