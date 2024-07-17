#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/* added to support mem leack debugging */ 
#include <mcheck.h>

/* add local headers */
#include "log.h"

FILE *log_file = NULL;

unsigned int create_log_file(char * file_name){
    static char fname[83]; /* suggested by -Wformat-overflow= */
    sprintf(fname, "%s.log", file_name);  
	log_file = fopen(fname, "w");
	if (!log_file) {
            perror("logfile");
            return 1;
	}
    return 0;
}