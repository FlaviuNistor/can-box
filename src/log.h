#ifndef LOG_H
#define LOG_H

extern FILE *log_file;
extern FILE *dump_file;
unsigned int create_log_files(char * file_name);

#endif /* LOG_H */