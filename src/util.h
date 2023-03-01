#ifndef BLUESHELL_UTIL_H
#define BLUESHELL_UTIL_H 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <bluetooth/bluetooth.h>


int str_isoneof(const char *, const char *list[], size_t n);
int str_isnumber(const char *);
int str_ismac(const char *);

/* Remove leading/trailing whitespaces, tabs and
 * newlines from given string. */
char *str_trim(char *);

/* Convert mac address to string */
char *ba_to_str(const bdaddr_t *, char *, size_t);

/* Returns true if given mac address is 00:00:00:00:00:00 */
int ba_isany(const bdaddr_t *ba);

/* Parse macaddress and (optional) port from bluetooth
 * address string.
 *
 * \param   str           Address string to parse
 * \param   mac           Stores mac address
 * \param   port          Stores port
 * \param   default_port  Default port to take if no
 *                        port were provided in str.
 * \return                Zero on success, -1 on error
 *
 * +----------------------+-------------------+------+
 * | Address String       | MAC               | Port |
 * +----------------------+-------------------+------+
 * | aa:bb:cc:dd:ee:ff    | aa:bb:cc:dd:ee:ff | 0    |
 * | aa:bb:cc:dd:ee:ff/13 | aa:bb:cc:dd:ee:ff | 13   |
 * | /13                  | 00:00:00:00:00:00 | 13   |
 * | 13                   | 00:00:00:00:00:00 | 13   |
 * +----------------------+-------------------+------+
 */
int parse_addrstr(const char *addrstr, bdaddr_t *mac,
		uint8_t *port, uint8_t default_port);

/* Start a subprocess and return the output of that process
 * as allocated (!!) string.
 * \param  command   Command to execute
 * \return           Allocated string with process output
 */
char *run_subprocess(const char *command);


#endif
