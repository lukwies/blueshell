#include "logger.h"

static FILE *l_file  = NULL;
static int   l_level = LOG_WARN;
static const char *l_path = NULL;

static const char *l_prefix[] = {
	"[\033[0;31mx\033[0m]",
	"[\033[0;33m!\033[0m]",
	"[\033[0;32m+\033[0m]",
	"[.]"
};

#define can_log(l) ((l) <= l_level)

int logger_init(int level, const char *logfile)
{
	FILE *f;
	l_path  = logfile;
	l_file  = stdout;
	logger_set_level(level);

	if (l_path) {
		if (!(f = fopen(l_path, "w"))) {
			fprintf(stderr, "! Failed to open logfile '%s'\n",
				l_path);
			return -1;
		}
		l_file = f;
	}
	return 0;
}

void logger_set_level(int l)
{
	if (l >= LOG_QUIET && l <= LOG_DEBUG)
		l_level = l;
}

void logger_free(void)
{
	if (l_path) {
		fclose(l_file);
		l_file = stdout;
		l_path = NULL;
	}
}

void logger_log(int level, const char *fmt, ...)
{
	char buf[512];

	if (!can_log(level))
		return;
	va_list val;
	va_start(val, fmt);
	vsnprintf(buf, sizeof(buf), fmt, val);
	va_end(val);

	fprintf(l_file, "%s %s\n", l_prefix[level], buf);
}

void logger_logbuf(int level, const char *msg, const void *buf, size_t n)
{
	size_t i;
	const unsigned char *p = buf;

	if (!can_log(level))
		return;

	fprintf(l_file, "%s %s (%zu byte)\n", l_prefix[level], msg, n);
	for (i = 0; i < n; i++, p++) {

		fprintf(l_file, " %02x", (int)*p);
		if ((i+1)%16 == 0 && i != n-1)
			fputc('\n', l_file);
	}
	fputc('\n', l_file);
}

/*
int main(void)
{
	logger_init(LOG_DEBUG, "test.log");
	log_error("Hello world, this is an error");
	log_warn("Hello world, this is a warning");
	log_info("Hello world, this is an info message");
	log_debug("Hello world, this is a debug message");
	logger_free();
}
*/
