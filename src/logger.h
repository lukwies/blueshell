#ifndef BLUESYNC_LOGGER_H
#define BLUESYNC_LOGGER_H 1

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

enum {
	LOG_QUIET=-1,
	LOG_ERROR=0,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG
};

int  logger_init(int level, const char *logfile);
void logger_set_level(int level);
void logger_free(void);

void logger_log(int level, const char *, ...);
void logger_logbuf(int level, const char *msg, const void *buf, size_t n);

#define log_error(s,...) logger_log(LOG_ERROR, (s), ##__VA_ARGS__)
#define log_warn(s,...)  logger_log(LOG_WARN, (s), ##__VA_ARGS__)
#define log_info(s,...)  logger_log(LOG_INFO, (s), ##__VA_ARGS__)
#define log_debug(s,...) logger_log(LOG_DEBUG, (s), ##__VA_ARGS__)

#endif
