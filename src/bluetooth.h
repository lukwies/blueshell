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

#include "proto.h"

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
int bt_listen(const bdaddr_t *addr, int port, int *fd);


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
int bt_connect(const bdaddr_t *addr, int port, int *fd);


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


/* Receive bt_hdr and convert hdr->size to hostbyte order.
 * \param   fd           Bluetooth socket
 * \param   hdr          Header context
 * \param   timeout_sec  Receive timeout in seconds (0=No timeout)
 * \return  Zero on success, -1 on error, -2 on timeout
 */
int bt_recv_hdr(int fd, struct bt_hdr *, int timeout_sec);

int bt_recv_pckt(int fd, struct bt_hdr *, void *data, size_t len,
		int timeout_sec);

/* Send packet. */
int bt_send_pckt(int fd, uint8_t type, uint8_t flags,
		const void *data, size_t len);

/* Close bluetooth socket. */
void bt_close(int fd);

#endif
