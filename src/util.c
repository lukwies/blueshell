#include "util.h"
#include "logger.h"

int str_isoneof(const char *s, const char *list[], size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		if (!strcmp(s, list[i]))
			return 1;
	}
	return 0;
}

int str_isnumber(const char *s)
{
	for (; *s; s++)
		if (*s < '0' || *s > '9')
			return 0;
	return 1;
}

int str_ismac(const char *s)
{
	int a,b,c,d,e,f;
	return (sscanf(s, "%02x:%02x:%02x:%02x:%02x:%02x",
		&a, &b, &c, &d, &e, &f) == 6) ? 1 : 0;
}
char *str_trim(char *s)
{
        size_t n = strlen(s), i;
        for (i = n-1; i > 0; i--)
                if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
                        s[i] = '\0';
                else break;

        while (*s && (*s == ' ' || *s=='\t' || *s=='\n'))
                s++;
        return s;
}

int ba_isany(const bdaddr_t *ba)
{
        static const uint8_t x[6] = {0,0,0,0,0,0};
        return (memcmp(ba, x, 6) == 0);
}

char *run_subprocess(const char *command)
{
	FILE *p;
	char *s=NULL, line[1024];
	size_t n, len = 0;

	if (!(p = popen(command, "r"))) {
		log_warn("popen: %s", strerror(errno));
		return NULL;
	}

	while (fgets(line, sizeof(line), p)) {

		n = strlen(line);
		s = (char*)realloc(s, len+n+2);

		if (!s) {
			log_error("malloc: %s", strerror(errno));
			return NULL;
		}
		s[len] = ' ';
		memcpy(s + len+1, line, n);
		len += n +1;
		s[len] = '\0';
	}
	pclose(p);
	return s;
}

int parse_addrstr(const char *str, bdaddr_t *mac, uint8_t *port,
		uint8_t default_port)
{
	const char *p;
	int res = 0;

	str2ba("00:00:00:00:00:00", mac);
	*port = default_port;

	p = strchr(str, '/');
	if (!p) {
		if (str_isnumber(str)) {
			*port = (uint8_t)atoi(str);
		} else {
			res = str2ba(str, mac);
		}
	}
	else if (*(p+1) && str_isnumber(p+1)) {
		if (p != str && p-str == 17) {
			char buf[32] = {0};
			memcpy(buf, str, p-str);
			res = str2ba(buf, mac);
		}
		*port = (uint8_t)atoi(p+1);
	}
	else {
		res = -1;
	}
	return res;
}

char *ba_to_str(const bdaddr_t *ba, char *s, size_t n)
{
	if (n < 18) return "";
	ba2str(ba, s);
	return s;
}


