#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

void configure_log();

void log_error(char *msg);

void log_info(char *msg);

void log_debug(char *msg);

void log_trace(char *msg);

void log_warn(char *msg);

void log_critical(char *msg);

void close_log();

#endif