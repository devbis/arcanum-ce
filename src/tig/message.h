#ifndef ARCANUM_TIG_MESSAGE_H_
#define ARCANUM_TIG_MESSAGE_H_

#include "tig/types.h"

#define TIG_MESSAGE_ID_0x401 (WM_USER + 1)
#define TIG_MESSAGE_ID_0x402 (WM_USER + 2)

typedef enum TigMessageType {
    TIG_MESSAGE_TYPE_0,
    TIG_MESSAGE_TYPE_1,
    TIG_MESSAGE_TYPE_2,
    TIG_MESSAGE_TYPE_3,
    TIG_MESSAGE_TYPE_4,
    TIG_MESSAGE_TYPE_5,
    TIG_MESSAGE_TYPE_6,
    TIG_MESSAGE_TYPE_7,
    TIG_MESSAGE_TYPE_8,
    TIG_MESSAGE_TYPE_9,
};

typedef struct TigMessage {
    unsigned int time;
    int type;
    union {
        struct {
            int field_8;
            int field_C;
            int field_10;
            int field_14;
            int field_18;
        };
        struct {
            int key;
            unsigned char pressed;
        } keyboard;
        struct {
            int x;
            int y;
            int z;
        } mouse;
    };
};

static_assert(sizeof(TigMessage) == 0x1C, "wrong size");

int tig_message_init(TigContext* ctx);
void tig_message_exit();
void tig_message_ping();
int tig_message_enqueue(TigMessage* message);

#endif /* ARCANUM_TIG_MESSAGE_H_ */
