#include <syslog.h>
#include "log_utils.h"


void configure_log(){
    setlogmask (LOG_UPTO(LOG_DEBUG));
    openlog("TUYAD", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
}

void log_error_m(char *msg){
    syslog(LOG_ERR, "%s", msg);
}

void log_info_m(char *msg){
    syslog(LOG_INFO, "%s", msg);
}

void log_debug_m(char *msg){
    syslog(LOG_DEBUG, "%s", msg);
}


void log_warn_m(char *msg){
    syslog(LOG_WARNING, "%s", msg);
}

void log_critical_m(char *msg){
    syslog(LOG_CRIT, "%s", msg);
}

void close_log(){
    closelog();
}