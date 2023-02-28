#include "bluetooth.h"
#include "logger.h"
#include "util.h"

int bt_get_devinfo(struct hci_dev_info *info)
{
	int dev;
	return (dev = hci_get_route(NULL)) != 0 ||
		hci_devinfo(dev, info) != 0;
}

void bt_print_devinfo(FILE *f, const struct hci_dev_info *info,
		const char *prefix)
{
	char buf[32];
	const char *p = prefix;

	if (!f) f=stdout;
	if (!p) p="";

	ba2str(&info->bdaddr, buf);
	fprintf(f, "%sId:      %d\n", p, (int)info->dev_id);
	fprintf(f, "%sName:    %s\n", p, info->name);
	fprintf(f, "%sAddress: %s\n", p, buf);
	fprintf(f, "%sType:    %s\n", p, hci_typetostr(info->type));
	fprintf(f, "%sFlags:   %s\n", p, hci_dflagstostr(info->flags));
}

int bt_scan(void)
{
    	int max_rsp, num_rsp;
    	int dev_id, fd, len, flags;
    	int i;
    	char addr[19] = { 0 };
    	char name[248] = { 0 };
    	inquiry_info *ii = NULL;

	log_info("Scanning for devices ...");

    	dev_id = hci_get_route(NULL);
    	fd     = hci_open_dev( dev_id );

    	if (dev_id < 0 || fd < 0) {
		log_error("Failed to open socket: %s", strerror(errno));
		return -1;
    	}

    	len  = 8;
    	max_rsp = 255;
    	flags = IREQ_CACHE_FLUSH;
    	ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    	num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    	if (num_rsp < 0) {
		log_error("hci_inquiry: %s", strerror(errno));
		return -1;
	}

    	for (i = 0; i < num_rsp; i++) {
        	ba2str(&(ii+i)->bdaddr, addr);
        	memset(name, 0, sizeof(name));
        	if (hci_read_remote_name(fd, &(ii+i)->bdaddr, sizeof(name),
            		name, 0) < 0)
        		strcpy(name, "[unknown]");
		printf("\033[1;33m%s\033[0m %s\n", addr, name);
    	}
    	free(ii);
    	close(fd);
    	return 0;
}

int bt_listen(const char *addr, int port, int *fd)
{
	int    res;
	struct sockaddr_rc sa = {0};

	*fd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if (*fd < 0) {
		log_error("socket: %s", strerror(errno));
		return -1;
	}

	if (addr)
		str2ba(addr, &sa.rc_bdaddr);
	else	sa.rc_bdaddr = *BDADDR_ANY;

	sa.rc_family  = AF_BLUETOOTH;
	sa.rc_channel = (uint8_t)port;

	res = bind(*fd, (struct sockaddr*)&sa, sizeof(sa));
	if (res != 0) {
		log_error("bind: %s", strerror(errno));
		goto error;
	}

	res = listen(*fd, 1);
	if (res != 0) {
		log_error("listen: %s", strerror(errno));
		goto error;
	}
	return 0;
error:
	close(*fd);
	return -1;
}

int bt_accept(int fd, int *cfd, struct sockaddr_rc *sa,
                        int timeout_sec)
{
	int    res;
	fd_set rfds;
	socklen_t slen = sizeof(struct sockaddr_rc);
	struct timeval tv = {timeout_sec, 0};

	if (timeout_sec > 0) {
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		res = select(fd+1, &rfds, NULL, NULL, &tv);
		if (res < 0) {
			log_error("accept/select: %s", strerror(errno));
			return -1;
		} else if (res == 0) {
			log_debug("accept timeout after %d sec", timeout_sec);
			return -2;
		}
	}
	*cfd = accept(fd, (struct sockaddr*)&sa, &slen);
	if (*cfd == -1) {
		log_error("accept: %s", strerror(errno));
		return -1;
	}
	return 0;
}

int bt_connect(const char *addr, int port, int *fd)
{
	struct sockaddr_rc sa = {0};
	int res;

	*fd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if (*fd == -1) {
		log_error("socket: %s", strerror(errno));
		return -1;
	}

	sa.rc_family = AF_BLUETOOTH;
	sa.rc_channel = (uint8_t)port;
	str2ba(addr, &sa.rc_bdaddr);

	res = connect(*fd, (struct sockaddr*)&sa, sizeof(sa));
	if (res != 0) {
		log_error("connect: %s", strerror(errno));
		close(*fd);
		return -1;
	}
	return 0;
}

int bt_send(int fd, const void *data, size_t len)
{
	size_t nsent = 0;
	int res;

	while (nsent < len) {
		res = send(fd, data+nsent, len-nsent, 0);
		if (res < 0) {
			log_warn("send: %s", strerror(errno));
			return -1;
		}
		nsent += (size_t)res;
	}
	return 0;
}


int bt_recv(int fd, void *data, size_t len, int timeout_sec)
{
	int    res;
	fd_set rfds;
	struct timeval tv = {timeout_sec, 0};

	if (timeout_sec > 0) {
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		res = select(fd+1, &rfds, NULL, NULL, &tv);
		if (res < 0) {
			log_error("recv/select: %s", strerror(errno));
			return -1;
		} else if (res == 0) {
			log_debug("recv timeout after %d sec", timeout_sec);
			return -2;
		}
	}
	return recv(fd, data, len, 0);
}

int bt_recvall(int fd, void *data, size_t len, int timeout_sec)
{
	size_t nrecv = 0;
	int    res;

	while (nrecv < len) {
		res = bt_recv(fd, data+nrecv, len-nrecv, timeout_sec);
		if (res < 0) return res;
		nrecv += res;
	}
	return 0;
}



int bt_parse_addrstr(const char *str, bdaddr_t *mac, uint8_t *port,
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
