#include "tig/net_xfer.h"

#include <stdio.h>

#include "tig/debug.h"
#include "tig/file.h"
#include "tig/net.h"

#define TIG_NET_XFER_MAX 100

#define TIG_NET_XFER_ACTIVE 0x01
#define TIG_NET_XFER_FLAG_0x02 0x02

typedef struct TigNetXfer {
    int id;
    char* path;
    int client_id;
    unsigned int flags;
    TigFile* stream;
    int recieved_bytes;
    int total_bytes;
};

// See 0x535130.
static_assert(sizeof(TigNetXfer) == 0x1C, "wrong size");

// 0x630208
static TigNetXfer tig_net_xfers[TIG_NET_XFER_MAX];

// 0x630CF8
static uint16_t word_630CF8;

static void tig_net_xfer_reset(TigNetXfer* xfer);
static void sub_534ED0(TigNetXfer* xfer);
static bool sub_535140(TigNetXfer** xfer);
static bool tig_net_xfer_find(int id, TigNetXfer** xfer);

// 0x5349A0
int tig_net_xfer_init()
{
    tig_debug_printf("TIG: net_xfer: init %d MAX_SLOTS\n", TIG_NET_XFER_MAX);

    for (int index = 0; index < TIG_NET_XFER_MAX; index++) {
        tig_net_xfer_reset(&(tig_net_xfers[index]));
    }

    tig_debug_printf("             : initialization complete\n");

    return TIG_OK;
}

// 0x5349E0
void tig_net_xfer_exit()
{
    tig_debug_printf("TIG: net_xfer: exititing\n");

    for (int index = 0; index < TIG_NET_XFER_MAX; index++) {
        if ((tig_net_xfers[index].flags & TIG_NET_XFER_ACTIVE) != 0) {
            tig_debug_printf("             : freeing slot %d\n", index);
            tig_file_fclose(tig_net_xfers[index].stream);
            free(tig_net_xfers[index].path);
        }
    }

    tig_debug_printf("             : exit_complete\n");
}

// 0x535130
void tig_net_xfer_reset(TigNetXfer* xfer)
{
    memset(xfer, 0, sizeof(*xfer));
}

// 0x534A40
void tig_net_xfer_ping()
{
}

// 0x534A50
int tig_net_xfer_send(const char* path, int client_id, int* xfer_id)
{
    return tig_net_xfer_send_as(path, path, client_id, xfer_id);
}

// 0x534A70
int tig_net_xfer_send_as(const char* path, const char* alias, int client_id, int* xfer_id)
{
    if (strcmp(path + strlen(path) - 5, ".RECV") == 0) {
        return TIG_ERR_16;
    }

    TigNetXfer* xfer;
    if (!sub_535140(&xfer)) {
        return TIG_ERR_16;
    }

    TigFileInfo file_info;
    if (!tig_file_info(path, &file_info)) {
        return TIG_ERR_16;
    }

    xfer->stream = tig_file_fopen(path, "rb");
    if (xfer->stream == NULL) {
        return TIG_ERR_16;
    }

    xfer->flags = TIG_NET_XFER_ACTIVE;
    xfer->id = (word_630CF8 << 16) | sub_529520();
    word_630CF8++;

    if (xfer_id != NULL) {
        *xfer_id = xfer->id;
    }

    xfer->path = _strdup(alias);
    xfer->client_id = client_id;
    xfer->flags = TIG_NET_XFER_ACTIVE;
    xfer->total_bytes = file_info.size;

    PacketType11 packet;
    packet.type = 0;
    packet.xfer_id = xfer->id;
    strncpy(packet.path, alias, MAX_PATH);

    tig_debug_printf("TIG: net_xfer: sending ID=%08X, to=%d, file \"%s\" as \"%s\" size %d\n",
        xfer->id,
        xfer->client_id,
        path,
        alias,
        xfer->total_bytes);
    send_generic(&packet, sizeof(packet), 11, client_id);
}

// 0x534C10
void tig_net_xfer_recv_packet_type_10(PacketType10* incoming)
{
    TigNetXfer* xfer;
    if (!tig_net_xfer_find(incoming->xfer_id, &xfer)) {
        tig_debug_printf("TIG: net_xfer: could not find id 0x%08X.\n", incoming->xfer_id);
        return;
    }

    tig_file_fwrite(incoming->data, sizeof(*incoming->data), incoming->size, xfer->stream);
    xfer->recieved_bytes += incoming->size;

    PacketType11 outgoing;
    outgoing.type = 3;
    outgoing.xfer_id = incoming->xfer_id;
    send_generic(&outgoing, sizeof(outgoing), 11, xfer->client_id);
}

// 0x534CB0
void tig_net_xfer_recv_packet_type_11(PacketType11* incoming)
{
    TigNetXfer* xfer;
    char buffer[MAX_PATH];

    if (incoming->type == 0) {
        if (sub_535140(&xfer)) {
            xfer->id = incoming->xfer_id;
            xfer->path = strdup(incoming->path);

            char* path = strdup(incoming->path);
            char* pch = strrchr(path, '\\');
            if (pch != NULL) {
                *pch = '\0';
            }

            if (*path != '\0') {
                tig_file_mkdir(path);
            }

            free(path);

            snprintf(buffer, MAX_PATH, "%s.%s", xfer->path, ".RECV");

            xfer->stream = tig_file_fopen(buffer, "wb");
            xfer->flags = TIG_NET_XFER_ACTIVE | TIG_NET_XFER_FLAG_0x02;
            xfer->client_id = sub_52A530();
            xfer->total_bytes = incoming->size;

            tig_debug_printf("TIG: net_xfer: begin receiving file \"%s\" id 0x%08X size %d\n",
                xfer->path,
                xfer->id,
                xfer->total_bytes);

            PacketType11 outgoing;
            outgoing.type = 3;
            outgoing.xfer_id = incoming->xfer_id;
            send_generic(&outgoing, sizeof(outgoing), 11, xfer->client_id);
        }
    } else if (incoming->type == 1) {
        if (tig_net_xfer_find(incoming->xfer_id, &xfer)) {
            tig_debug_printf("TIG: net_xfer: ID=%08X, to=%d, file \"%s\" is complete\n",
                xfer->id,
                xfer->client_id,
                xfer->path);
            tig_file_fclose(xfer->stream);

            snprintf(buffer, sizeof(buffer), xfer->path, ".RECV");
            tig_file_rename(buffer, xfer->path);
            free(xfer->path);
            tig_net_xfer_reset(xfer);
        }
    } else if (incoming->type == 3) {
        if (tig_net_xfer_find(incoming->xfer_id, &xfer)) {
            sub_534ED0(xfer);
        }
    }
}

// 0x534ED0
void sub_534ED0(TigNetXfer* xfer)
{
    if ((xfer->flags & TIG_NET_XFER_ACTIVE) != 0 && (xfer->flags & TIG_NET_XFER_FLAG_0x02) == 0) {
        if (tig_file_feof(xfer->stream)) {
            PacketType11 packet;
            packet.type = 1;
            packet.xfer_id = xfer->id;
            tig_debug_printf("TIG: net_xfer: ID=%08X, to=%d, file \"%s\" has completed\n", xfer->id, xfer->client_id, xfer->path);
            send_generic(&packet, sizeof(packet), 11, xfer->client_id);
            free(xfer->path);
            tig_file_fclose(xfer->stream);
            tig_net_xfer_reset(xfer);
        } else {
            PacketType10 packet;
            memset(&packet, -1, sizeof(packet));
            packet.size = tig_file_fread(packet.data, sizeof(*packet.data), sizeof(packet.data), xfer->stream);
            if (packet.size > 0) {
                packet.xfer_id = xfer->id;
                send_generic(&packet, sizeof(packet), 10, xfer->client_id);
            }
        }
    }
}

// 0x535050
int tig_net_xfer_count(int client_id)
{
    int count = 0;
    for (int index = 0; index < TIG_NET_XFER_MAX; index++) {
        if ((tig_net_xfers[index].flags & TIG_NET_XFER_ACTIVE) != 0) {
            if (tig_net_xfers[index].client_id == client_id) {
                count++;
            }
        }
    }
    return count;
}

// 0x535080
void tig_net_xfer_remove_all(int client_id)
{
    for (int index = 0; index < TIG_NET_XFER_MAX; index++) {
        if ((tig_net_xfers[index].flags & TIG_NET_XFER_ACTIVE) != 0) {
            if (tig_net_xfers[index].client_id == client_id) {
                if (tig_net_xfers[index].stream != NULL) {
                    tig_file_fclose(tig_net_xfers[index].stream);
                }

                if (tig_net_xfers[index].path != NULL) {
                    free(tig_net_xfers[index].path);
                }

                tig_net_xfer_reset(&(tig_net_xfers[index]));
            }
        }
    }
}

// 0x5350D0
int sub_5350D0(int id, char* path, int* progress)
{
    TigNetXfer* xfer;
    if (!tig_net_xfer_find(id, &xfer)) {
        return TIG_ERR_16;
    }

    if (path != NULL) {
        strncpy(path, xfer->path, MAX_PATH);
    }

    if (progress != NULL) {
        *progress = 100 * xfer->recieved_bytes / xfer->total_bytes;
    }

    return TIG_OK;
}

// 0x535140
bool sub_535140(TigNetXfer** xfer)
{
    for (int index = 0; index < TIG_NET_XFER_MAX; index++) {
        if ((tig_net_xfers[index].flags & TIG_NET_XFER_ACTIVE) == 0) {
            *xfer = &(tig_net_xfers[index]);
            return true;
        }
    }
    return false;
}

// 0x535180
bool tig_net_xfer_find(int id, TigNetXfer** xfer)
{
    for (int index = 0; index < TIG_NET_XFER_MAX; index++) {
        if ((tig_net_xfers[index].flags & TIG_NET_XFER_ACTIVE) != 0) {
            if (tig_net_xfers[index].id == id) {
                *xfer = &(tig_net_xfers[index]);
                return true;
            }
        }
    }
    return false;
}
