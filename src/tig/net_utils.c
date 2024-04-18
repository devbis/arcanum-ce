#include "tig/net_utils.h"

#include <string.h>

#include "tig/memory.h"

typedef struct S638BB0 {
    /* 0000 */ char* field_0;
    /* 0004 */ char* field_4;
    /* 0008 */ struct S638BB0* next;
} S638BB0;

static_assert(sizeof(S638BB0) == 0xC, "wrong size");

static bool sub_53AB30();

// 0x638BB0
static S638BB0* off_638BB0;

// 0x638BB4
static int dword_638BB4;

// 0x53AB00
bool sub_53AB00()
{
    off_638BB0 = 0;
    dword_638BB4 = 0;

    return true;
}

// 0x53AB20
void sub_53AB20()
{
    sub_53AB30();
}


// 0x53AB30
bool sub_53AB30()
{
    S638BB0* node;

    node = off_638BB0;
    while (node != NULL) {
        off_638BB0 = node->next;
        if (node->field_0 != NULL) {
            FREE(node->field_0);
        }
        if (node->field_4 != NULL) {
            FREE(node->field_4);
        }
        FREE(node);
        node = off_638BB0;
        dword_638BB4--;
    }

    return true;
}

// 0x53AB90
bool sub_53AB90(const char* a1, const char* a2)
{
    S638BB0* node;

    if (a1 == NULL) {
        a1 = "*";
    }

    if (a2 == NULL) {
        a2 = "*";
    }

    node = (S638BB0*)MALLOC(sizeof(*node));
    node->field_0 = STRDUP(a1);
    node->field_4 = STRDUP(a2);
    node->next = off_638BB0;
    dword_638BB4++;

    return true;
}

// 0x53ABF0
bool sub_53ABF0(int a1, const char* a2)
{
    (void)a1;

    S638BB0* node;

    node = off_638BB0;
    while (node != NULL) {
        if (strcmpi(node->field_4, a2) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}
