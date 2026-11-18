#ifndef LOG_UTILS_H
#define LOG_UTILS_H

void configure_log();

void log_error_m(char *msg);

void log_info_m(char *msg);

void log_debug_m(char *msg);

void log_trace_m(char *msg);

void log_warn_m(char *msg);

void log_critical_m(char *msg);

void close_log();

#endif