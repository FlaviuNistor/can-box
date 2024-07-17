#ifndef LOG_H
#define LOG_H

extern FILE *log_file;
extern FILE *dump_file;
unsigned int create_log_files(char * file_name);
void log_can_frame_to_dump(struct canfd_frame * can_frame);

#endif /* LOG_H */