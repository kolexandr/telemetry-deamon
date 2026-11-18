// configure daemon
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "log_utils.h"
#include "tuya_client.h"
#include "config.h"

void daemonize(struct arguments *config){    
    pid_t p = fork();

    if (p < 0){
        log_error_m("Fork error");
        exit(EXIT_FAILURE);
    }

    if (p != 0){
        exit(EXIT_SUCCESS);
    }

    if (setsid() == -1){
        log_error_m("setsid() failed");
        exit(EXIT_FAILURE);
    } 

    if(chdir("/") < 0){
        log_error_m("Error changing directory");
        exit(1);
    }

    int fd = open("/dev/null", O_RDWR);

    if (fd == -1){
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDIN_FILENO) == -1 || dup2(fd, STDOUT_FILENO) == -1 || dup2(fd, STDERR_FILENO) == -1){
        close(fd);
        log_error_m("Error assigning file descriptors");
        exit(EXIT_FAILURE);
    }
    if (fd > STDERR_FILENO){
        close(fd);
    }

    log_info_m("Daemon has started");

    if (config->daemon == 1){
        logging_tuya(config);
    }
}