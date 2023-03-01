#include "blueshell.h"

static void client_loop(int cfd, struct sockaddr_rc *, blueshell_opts *);
static int  handle_cmd(int cfd, const char *cmd, blueshell_opts *);

static int done = 0;

/*
 * Main server loop
 */
int blueshell_start_server(blueshell_opts *o)
{
	int res, fd, cfd;
	struct sockaddr_rc ca;


	res = bt_listen(&o->addr, o->port, &fd);
	if (res != 0) return -1;

	while (!done) {

		res = bt_accept(fd, &cfd, &ca, o->accept_timeout);
		if (res == -1) break;
		else if (res == -2) continue;

		client_loop(cfd, &ca, o);
	}
	return 0;
}


/*
 * This is executed after a client connected.
 */
static void client_loop(int cfd, struct sockaddr_rc *ca,
		blueshell_opts *o)
{
	int    res=0;
	struct bt_hdr hdr;
	uint8_t data[2048];
	char tmp[32];

	log_info("Accepted client %s/%d",
		ba_to_str(&ca->rc_bdaddr, tmp, 32),
		(int)ca->rc_channel);

	while (res == 0) {
		res = bt_recv_pckt(cfd, &hdr, data,
				sizeof(data), 0);
		if (res < 0) break;

		switch (hdr.type) {
		case T_CMD:
			res = handle_cmd(cfd, (char*)data, o);
			break;
		case T_BYE:
			res = -1;
			break;
		}
	}
	log_info("Client disconnected");
}

/*
 * Execute shell command received by the client.
 * \return  Zero on success, -1 if client disconnected
 */
static int handle_cmd(int cfd, const char *cmd, blueshell_opts *o)
{
	int res;
	char *buf;
	(void)o; // TODO

	log_debug("Executing command '%s'", cmd);

	buf = run_subprocess(cmd);
	if (!buf) {
		buf = strerror(errno);
		return bt_send_pckts(cfd, T_ERROR, 0,
			"Unsupported command");
	}

	res = bt_send_pckt(cfd, T_CMDRES, 0, buf, strlen(buf));
	free(buf);
	return res;
}
