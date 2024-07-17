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