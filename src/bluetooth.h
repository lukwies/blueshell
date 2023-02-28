#ifndef BLUETOOTH_H
#define BLUETOOTH_H 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/select.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


/* Get information about the default bluetooth device.
 * This can also be used to check if bluetooth is enabled.
 * \param  info  Bluetooth device info
 * \return       0 on success, -1 on error (bluetooth disabled)
 */
int bt_get_devinfo(struct hci_dev_info *info);


/* Print bluetooth device info.
 * \param  file     Output file stream (NULL -> stdout)
 * \param  info     Bluetooth device info
 * \param  prefix   Prefix to place at the beginning of a line
 *		    (NULL -> "")
 */
void bt_print_devinfo(FILE *f, const struct hci_dev_info *info,
			const char *prefix);


/* Scan for nearby bluetooth devices and print a list with the
 * mac addresses and remote names of these discovered devices.
 */
int bt_scan(void);


/* Get listening rfcomm socket.
 * \param   addr   Mac address to bind to (Can be NULL)
 * \param   port   Port to listen at
 * \param   fd     To store the socket
 * \return         Zero on success, -1 on error
 */
int bt_listen(const char *addr, int port, int *fd);


/* Accept client connection.
 * \param   fd            The listening bluetooth socket
 * \param   cfd           To store the accepted client socket
 * \param   csa           To store the accepted client address
 * \param   timeout_sec   Accept timeout in seconds (0=No timeout)
 * \return                Zero on success, -1 on error, -2 on timeout
 */
int bt_accept(int fd, int *cfd, struct sockaddr_rc *csa,
			int timeout_sec);

/* Connect to bluetooth service (rfcomm).
 * \param   addr     Mac address of server
 * \param   port     Port of server
 * \param   fd       To store the connected socket
 * \return           Zero on success, -1 on error
 */
int bt_connect(const char *addr, int port, int *fd);


/* Send `len` bytes of given data to peer.
 * \param   fd    Socket to use for sending
 * \param   data  Data to send
 * \param   len   Number of bytes to send
 * \return        Zero on success, -1 on error
 */
int bt_send(int fd, const void *data, size_t len);


/* Receive data from socket.
 * \param   fd           Bluetooth socket
 * \param   data         Buffer to store received data
 * \param   len          Maximal number of bytes to receive
 * \param   timeout_sec  Receive timeout in seconds (0=No timeout)
 * \return               Number of bytes received on success,
 *			 -1 on error, -2 on timeout
 */
int bt_recv(int fd, void *data, size_t len, int timeout_sec);


/* Receive all `len` number of bytes.
 * \param   fd           Bluetooth socket
 * \param   data         Buffer to store received data
 * \param   len          Maximal number of bytes to receive
 * \param   timeout_sec  Receive timeout in seconds (0=No timeout)
 * \return               Zero on success, -1 on error, -2 on timeout
 */
int bt_recvall(int fd, void *data, size_t len, int timeout_sec);


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
 * | aa:bb:cc:dd:ee:ff	  | aa:bb:cc:dd:ee:ff | 0    |
 * | aa:bb:cc:dd:ee:ff/13 | aa:bb:cc:dd:ee:ff | 13   |
 * | /13                  | 00:00:00:00:00:00 | 13   |
 * | 13                   | 00:00:00:00:00:00 | 13   |
 * +----------------------+-------------------+------+
 */
int bt_parse_addrstr(const char *str, bdaddr_t *mac, uint8_t *port,
		uint8_t default_port);


#endif
