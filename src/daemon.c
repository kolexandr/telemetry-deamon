// configure daemon
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


#include "log_utils.h"

void daemonize(){    
    pid_t p = fork();

    if (p < 0){
        log_error("Fork error");
        exit(1);
    }

    if (p != 0){
        exit(0);
    }

    setsid();

    if(chdir("/") < 0){
        log_error("Error changing directory");
        exit(1);
    }

    for (int i = 0; i<1024; i++){
        close(i);
    }

    int fd0 = open("/dev/null", O_RDWR);
    int fd1 = dup(0);
    int fd2 = dup(0);

    if (fd0 != 0 || fd1 != 1 || fd2 != 2){
        log_error("Error assigning file descriptors");
        exit(1);
    }
}