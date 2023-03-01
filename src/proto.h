#ifndef BLUESHELL_PROTO_H
#define BLUESHELL_PROTO_H 1

#include <stdint.h>
#include <bluetooth/bluetooth.h>

enum {
	T_OK       = 0x0001,
	T_CMD      = 0x0002,
	T_CMDRES   = 0x0004,
	T_BYE      = 0x0008,
	T_ERROR    = 0x0010
};

/* Each transmission buffer starts with the following
 * 8 byte structure.
 */
struct bt_hdr
{
	uint16_t type;
	uint16_t flags;
	uint32_t size;

} __attribute__((packed));

#define BT_HDR_SIZE 8

void bt_hdr_pack(struct bt_hdr *, uint16_t type, uint16_t flags, size_t size);
void bt_hdr_unpack(struct bt_hdr *);


#endif
