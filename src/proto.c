#include "proto.h"

void bt_hdr_pack(struct bt_hdr *hdr, uint16_t type,
		uint16_t flags, size_t size)
{
	hdr->type  = htobs((uint16_t)type);
	hdr->flags = htobs((uint16_t)flags);
	hdr->size  = htobl((uint32_t)size);
}

void bt_hdr_unpack(struct bt_hdr *hdr)
{
	hdr->type  = btohs(hdr->type);
	hdr->flags = btohs(hdr->flags);
	hdr->size  = btohl(hdr->size);
}
