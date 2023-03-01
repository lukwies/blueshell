#include <getopt.h>
#include "blueshell.h"

#define HELP "\n"\
 "blueshell - Interactive bluetooth shell\n"\
 "\n"\
 "Usage: blueshell [OPTIONS] ...\n"\
 "\n"\
 "-h, --help             Show this helptext\n"\
 "-v, --verbose          Run with more verbose output\n"\
 "\n"\
 "-D, --device-info      Print default device information\n"\
 "-s, --scan             Scan for nearby bluetooth devices\n"\
 "\n"\
 "-c, --client           Run in client mode\n"\
 "-l, --listen           Run in server mode (default)\n"\
 "-a, --addr=MAC(/PORT)  Set mac address and optional port\n"\
 "\n"

static void print_device_info(void);

static struct option lopt[] = {
	{"address",	required_argument, 0, 'a'},
	{"client",	no_argument,       0, 'c'},
	{"device-info",	no_argument,       0, 'D'},
	{"help",	no_argument,       0, 'h'},
	{"listen",	no_argument,       0, 'l'},
	{"scan",	no_argument,       0, 's'},
	{"verbose",	no_argument,       0, 'v'},
	{0,0,0,0}
};


int main(int argc, char *argv[])
{
	int c, oi, res;
	blueshell_opts opts;
	blueshell_opts_init(&opts);
	logger_init(LOG_INFO, NULL);

	do {
		c = getopt_long(argc, argv, "a:cDhlsv", lopt, &oi);

		switch (c) {

		case 'a': // -a, --addr=MACADDR(/PORT)
			res = parse_addrstr(optarg, &opts.addr, &opts.port,
					BLUESHELL_DEFAULT_PORT);
			if (res != 0) {
				fprintf(stderr, "[\033[1;31mx\033[0m] "
					"Invalid address: %s\n", optarg);
				return -1;
			}
			break;
		case 'c': // -c, --client
			opts.mode = 1;
			break;


		case 'D': // -D, --device-info
			print_device_info();
			return 0;
		case 'h': // -h, --help
			fputs(HELP, stdout);
			return 0;
		case 'l': // -l, --listen
			opts.mode = 0;
			break;
		case 's': // -s, --scan
			return bt_scan();
		case 'v': // -v, --verbose
			opts.loglevel = LOG_DEBUG;
			break;
		}
	} while (c != -1);


	logger_init(opts.loglevel, NULL);

	if (opts.mode == 1)
		blueshell_start_client(&opts);
	else	blueshell_start_server(&opts);

	logger_free();
	return 0;
}


/* Print default device info (if bluetooth enabled) */
static void print_device_info(void)
{
	struct hci_dev_info info;
	if (bt_get_devinfo(&info))
		fprintf(stderr, "[\033[1;31mx\033[0m] "
			"No bluetooth device found!\n");
	else	bt_print_devinfo(NULL, &info, " ");
}



