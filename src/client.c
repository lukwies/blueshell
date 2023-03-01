#include "blueshell.h"
#include "util.h"

#define PROMPT 	"\033[34m[blueshell]\033[0m$ "
#define WELCOME	"Welcome to the interactive blueshell client!\n"\
		"Commands starting with a '!' are executed locally.\n"\
		"Type '/exit' to quit.\n"\
		"\n"

static const char *exit_cmds[] = {"/bye", "/exit", "/quit", "/q"};

static int done = 0;

static int connect_to_server(blueshell_opts *, int *fd);
static int exec_local_command(const char *);
static int exec_remote_command(int, const char *, blueshell_opts*);
static int getstr(char *, size_t);


int blueshell_start_client(blueshell_opts *o)
{
	char cmd[256];
	int  fd, res;

	res = connect_to_server(o, &fd);
	if (res) return -1;

	fputs(WELCOME, stdout);

	while (!done) {

		res = getstr(cmd, sizeof(cmd));
		if (res) continue;

		if (cmd[0] == '/') {
			if (str_isoneof(cmd, exit_cmds, 4))
				done = 1;
			else	log_warn("Invalid command '%s'", cmd);
		}
		else if (cmd[0] == '!') {
			exec_local_command(cmd);
		}
		else {
			done = exec_remote_command(fd, cmd, o);
		}
	}
	bt_close(fd);

	return 0;
}

static int connect_to_server(blueshell_opts *o, int *fd)
{
	if (ba_isany(&o->addr)) {
		log_warn("Can't connect to 00:00:00:00:00:00");
		return -1;
	}
	return bt_connect(&o->addr, o->port, fd);
}

static int exec_local_command(const char *cmd)
{
	char *res = run_subprocess(cmd+1);
	if (res) {
		fputs(res, stdout);
		free(res);
	}
	return (res) ? 0 : -1;
}

static int exec_remote_command(int fd, const char *cmd,
		blueshell_opts *o)
{
	int res;
	struct bt_hdr hdr;
	char data[4096];

	res = bt_send_pckt(fd, T_CMD, 0, cmd, strlen(cmd));
	if (res != 0) return -1;

	res = bt_recv_pckt(fd, &hdr, data, sizeof(data),
			o->recv_timeout);
	if (res < 0) return -1;

	switch (hdr.type) {
	case T_CMDRES:
		fputs(data, stdout);
		break;
	case T_ERROR:
		log_error(data);
		break;
	default:
		log_warn("Received invalid pckt type 0x02x", hdr.type);
		break;
	}
	return 0;
}


static int getstr(char *s, size_t n)
{
	fputs(PROMPT, stdout);
	s = fgets(s, n, stdin);
	s[strlen(s)-1] = '\0';
	return (!*s) ? -1 : 0;
}
