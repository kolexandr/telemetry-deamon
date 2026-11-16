#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "log_utils.h"

#include "signal_utils.h"

static volatile sig_atomic_t interrupted = 0;

static void signal_handler(int signal_number){
    switch(signal_number){
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGABRT:
            interrupted = 1;
            break;
        default: 
            break;
    }
}

int configure_signals(){
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    sigemptyset(&action.sa_mask);
    action.sa_handler = signal_handler;
    action.sa_flags = 0;
    
    if (sigaction(SIGINT, &action, NULL) == -1){
        goto error;
    }
    if (sigaction(SIGQUIT, &action, NULL) == -1){
        goto error;
    }
    if (sigaction(SIGTERM, &action, NULL) == -1){
        goto error;
    }
    if (sigaction(SIGABRT, &action, NULL) == -1){
        goto error;
    }

    log_info("Signals are configured");
    return 0;

error:
    log_error("Failed to configure signals");
    return 1;
}

int is_interrupted(){
    return interrupted ? 1 : 0;
}