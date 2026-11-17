// configure daemon
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

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

    int fd = open("/dev/null", O_RDWR);

    if (fd == -1){
        exit(1);
    }

    if (dup2(fd, STDIN_FILENO) == -1 || dup2(fd, STDOUT_FILENO) == -1 || dup2(fd, STDERR_FILENO) == -1){
        // `
        if (fd > STDERR_FILENO){
            close(fd);
        }
        log_error("Error assigning file descriptors");
        exit(1);
    }
    if (fd > STDERR_FILENO){
        close(fd);
    }

}