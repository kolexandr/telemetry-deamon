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
        exit(EXIT_FAILURE);
    }

    if (p != 0){
        exit(EXIT_SUCCESS);
    }

    if (setsid() == -1){
        log_error("setsid() failed");
        exit(EXIT_FAILURE);
    } 

    if(chdir("/") < 0){
        log_error("Error changing directory");
        exit(1);
    }

    int fd = open("/dev/null", O_RDWR);

    if (fd == -1){
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDIN_FILENO) == -1 || dup2(fd, STDOUT_FILENO) == -1 || dup2(fd, STDERR_FILENO) == -1){
        close(fd);
        log_error("Error assigning file descriptors");
        exit(EXIT_FAILURE);
    }
    if (fd > STDERR_FILENO){
        close(fd);
    }

    log_info("Daemon has started");
}