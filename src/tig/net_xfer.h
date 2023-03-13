#ifndef ARCANUM_TIG_NET_XFER_H_
#define ARCANUM_TIG_NET_XFER_H_

#include "tig/types.h"

typedef struct PacketType10 {
    int32_t xfer_id;
    int32_t size;
    uint8_t data[2048];
};

// See 0x534ED0.
static_assert(sizeof(PacketType10) == 0x808, "wrong size");

typedef struct PacketType11 {
    int32_t type;
    int32_t xfer_id;
    char path[MAX_PATH];
    int32_t size;
};

// See 0x534ED0.
static_assert(sizeof(PacketType11) == 0x110, "wrong size");

int tig_net_xfer_init();
void tig_net_xfer_exit();
void tig_net_xfer_ping();
int tig_net_xfer_send(const char* path, int client_id, int* xfer_id);
int tig_net_xfer_send_as(const char* path, const char* alias, int client_id, int* xfer_id);
void tig_net_xfer_recv_packet_type_10(PacketType10* incoming);
void tig_net_xfer_recv_packet_type_11(PacketType11* incoming);
int tig_net_xfer_count(int client_id);
void tig_net_xfer_remove_all(int client_id);
int sub_5350D0(int id, char* path, int* progress);

#endif /* ARCANUM_TIG_NET_XFER_H_ */
