#include <syslog.h>
#include "log_system.h"


void configure_log(){
    // setlogmask (LOG_UPTO(LOG_DEBUG));
    openlog("MONITORING", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
}

void log_error(char *msg){
    syslog(LOG_ERR, "%s", msg);
}

void log_info(char *msg){
    syslog(LOG_INFO, "%s", msg);
}

void log_debug(char *msg){
    syslog(LOG_DEBUG, "%s", msg);
}

void log_trace(char *msg){
    syslog(LOG_DEBUG, "[TRACE] %s", msg);
}

void log_warn(char *msg){
    syslog(LOG_WARNING, "%s", msg);
}

void log_critical(char *msg){
    syslog(LOG_CRIT, "%s", msg);
}

void close_log(){
    closelog();
}