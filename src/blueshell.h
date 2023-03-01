#ifndef BLUESHELL_H
#define BLUESHELL_H 1

#include "bluetooth.h"
#include "logger.h"
#include "util.h"

#define BLUESHELL_VERSION      "0.1"
#define BLUESHELL_DEFAULT_PORT 13
#define BLUESHELL_DEFAULT_MODE 0


/* Blueshell running options. */
typedef struct
{
	bdaddr_t addr;		// Connect/Listen MAC
	uint8_t  port;		// Connect/Listen Port
	int      loglevel;	// Loglevel
	int      mode;		// Mode (0=Server, 1=Client)

	int recv_timeout;
	int accept_timeout;
} blueshell_opts;


/* Initialize default options. */
void blueshell_opts_init(blueshell_opts *);

 
int blueshell_start_client(blueshell_opts *);
int blueshell_start_server(blueshell_opts *);



#endif
