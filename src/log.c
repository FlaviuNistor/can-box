#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/* added to support mem leack debugging */ 
#include <mcheck.h>

FILE *logfile = NULL;

unsigned int create_log_file(char * file_name){
    static char fname[83]; /* suggested by -Wformat-overflow= */
    sprintf(fname, "%s.log", file_name);  
	logfile = fopen(fname, "w");
	if (!logfile) {
            perror("logfile");
            return 1;
	}
    return 0;
}

/* add local headers */
#include "log.h"

