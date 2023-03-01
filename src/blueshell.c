#include "blueshell.h"

void blueshell_opts_init(blueshell_opts *o)
{
	str2ba("00:00:00:00:00:00", &o->addr);
	o->port     = BLUESHELL_DEFAULT_PORT;
	o->mode     = BLUESHELL_DEFAULT_MODE;
	o->loglevel = LOG_INFO;
	o->recv_timeout   = 10;
	o->accept_timeout = 30;
}
