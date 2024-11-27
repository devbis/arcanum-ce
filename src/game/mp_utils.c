#include "game/mp_utils.h"

#include "game/a_name.h"
#include "game/area.h"
#include "game/critter.h"
#include "game/gsound.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/random.h"
#include "game/sector.h"
#include "game/spell.h"
#include "game/townmap.h"
#include "game/ui.h"

typedef struct Packet29 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet29;

static_assert(sizeof(Packet29) == 0x20, "wrong size");

typedef struct Packet35 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int damage;
} Packet35;

static_assert(sizeof(Packet35) == 0x28, "wrong size");

typedef struct Packet72 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet72;

static_assert(sizeof(Packet72) == 0x20, "wrong size");

typedef struct Packet85 {
    /* 0000 */ int type;
    /* 0004 */ int extra_length;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet85;

static_assert(sizeof(Packet85) == 0x28, "wrong size");

typedef struct Packet87 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ int field_38;
} Packet87;

static_assert(sizeof(Packet87) == 0x40, "wrong size");

typedef struct Packet90 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ int field_38;
} Packet90;

static_assert(sizeof(Packet90) == 0x40, "wrong size");

typedef struct Packet92 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ tig_art_id_t art_id;
} Packet92;

static_assert(sizeof(Packet92) == 0x28, "wrong size");

typedef struct Packet94 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet94;

static_assert(sizeof(Packet94) == 0x20, "wrong size");

typedef struct Packet95 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet95;

static_assert(sizeof(Packet95) == 0x20, "wrong size");

typedef struct Packet96 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet96;

static_assert(sizeof(Packet96) == 0x28, "wrong size");

typedef struct Packet99 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int64_t location;
    /* 0028 */ int dx;
    /* 002C */ int dy;
    /* 0030 */ int field_30;
} Packet99;

static_assert(sizeof(Packet99) == 0x38, "wrong size");

typedef struct Packet100 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    union {
        struct {
            /* 0008 */ int field_8;
            /* 000C */ int field_C;
        } a;
        struct {
            /* 0008 */ ObjectID field_8;
        } b;
        struct {
            /* 0008 */ int field_8;
            /* 0010 */ ObjectID field_10;
            /* 0028 */ ObjectID field_28;
        } c;
        struct {
            /* 0008 */ ObjectID field_8;
            /* 0020 */ ObjectID field_20;
        } s;
        struct {
            /* 0008 */ ObjectID field_8;
            /* 0020 */ int field_20;
        } x;
        struct {
            /* 0008 */ ObjectID field_8;
            /* 0020 */ ObjectID field_20;
            /* 0038 */ int field_38;
            /* 003C */ int field_3C;
        } z;
    } d;
} Packet100;

static_assert(sizeof(Packet100) == 0x40, "wrong size");

typedef struct Packet103 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet103;

static_assert(sizeof(Packet103) == 0x28, "wrong size");

typedef struct Packet106 {
    /* 0000 */ int type;
    /* 0004 */ int subtype;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ ObjectID oid;
} Packet106;

static_assert(sizeof(Packet106) == 0x28, "wrong size");

typedef struct Packet107 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
} Packet107;

static_assert(sizeof(Packet107) == 0x20, "wrong size");

typedef struct Packet108 {
    /* 0000 */ int type;
    /* 0008 */ int64_t field_8;
    /* 0010 */ bool field_10;
} Packet108;

static_assert(sizeof(Packet108) == 0x18, "wrong size");

typedef struct Packet109 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet109;

static_assert(sizeof(Packet109) == 0x28, "wrong size");

typedef struct Packet110 {
    /* 0000 */ int type;
    /* 0004 */ int map;
    /* 0008 */ int field_8;
} Packet110;

static_assert(sizeof(Packet110) == 0xC, "wrong size");

typedef struct Packet111 {
    /* 0000 */ int type;
    /* 0004 */ tig_art_id_t art_id;
} Packet111;

static_assert(sizeof(Packet111) == 0x8, "wrong size");

typedef struct Packet112 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t field_8;
    /* 0010 */ ObjectID oid;
} Packet112;

static_assert(sizeof(Packet112) == 0x28, "wrong size");

typedef struct Packet116 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 0010 */ ObjectID oid;
} Packet116;

static_assert(sizeof(Packet116) == 0x28, "wrong size");

typedef struct Packet117 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
    /* 0038 */ ObjectID field_38;
} Packet117;

static_assert(sizeof(Packet117) == 0x50, "wrong size");

typedef struct Packet118 {
    /* 0000 */ int type;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ ObjectID field_20;
} Packet118;

static_assert(sizeof(Packet118) == 0x38, "wrong size");

typedef struct Packet120 {
    /* 0000 */ int type;
    /* 0004 */ struct {
        int field_4;
        int field_8;
        int field_C;
    } data;
} Packet120;

static_assert(sizeof(Packet120) == 0x10, "wrong size");

typedef struct Packet121 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int field_20;
} Packet121;

static_assert(sizeof(Packet121) == 0x28, "wrong size");

typedef struct Packet122 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID oid;
} Packet122;

static_assert(sizeof(Packet122) == 0x20, "wrong size");

typedef struct Packet129 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int fld;
    /* 0024 */ int subtype;
    union {
        struct {
            /* 0028 */ int field_28;
        } a;
        struct {
            /* 0028 */ int field_28;
        } b;
        struct {
            /* 0028 */ int field_28;
        } c;
        struct {
            /* 0028 */ ObjectID oid;
        } d;
        struct {
            /* 0028 */ int field_28;
            /* 0030 */ ObjectID oid;
        } e;
        struct {
            /* 0028 */ int field_28;
            /* 002C */ int field_2C;
        } f;
        struct {
            /* 0028 */ int field_28;
            /* 002C */ int field_2C;
        } g;
        struct {
            /* 0028 */ tig_art_id_t art_id;
        } h;
        struct {
            /* 0028 */ int field_28;
            /* 002C */ int field_2C;
            /* 0030 */ int field_30;
        } i;
        struct {
            /* 0028 */ unsigned int flags;
        } j;
    } d;
} Packet129;

static_assert(sizeof(Packet129) == 0x48, "wrong size");

typedef struct Packet130 {
    /* 0000 */ int type;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID oid;
    /* 0020 */ int fld;
    /* 0024 */ int length;
} Packet130;

static_assert(sizeof(Packet130) == 0x28, "wrong size");

// 0x4ED510
void sub_4ED510()
{
    // TODO: Incomplete.
}

// 0x4ED630
void sub_4ED630()
{
    // TODO: Incomplete.
}

// 0x4ED6C0
bool sub_4ED6C0(int64_t obj)
{
    Packet29 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        return sub_463540(obj);
    }

    pkt.type = 29;
    sub_4F0640(obj, &(pkt.oid));
    tig_net_send_app_all(&pkt, sizeof(pkt));

    return false;
}

// 0x4ED720
void sub_4ED720(int64_t obj, int damage)
{
    Packet35 pkt;

    critter_fatigue_damage_set(obj, damage);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 35;
        sub_4F0640(obj, &(pkt.oid));
        pkt.damage = damage;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4ED780
void sub_4ED780()
{
    // TODO: Incomplete.
}

// 0x4ED8B0
void sub_4ED8B0()
{
    // TODO: Incomplete.
}

// 0x4ED9E0
void sub_4ED9E0(int64_t obj)
{
    Packet72 pkt;

    pkt.type = 72;
    pkt.oid = sub_407EF0(obj);
    tig_net_send_app_all(&pkt, sizeof(pkt));
}

// 0x4EDA60
void sub_4EDA60()
{
    // TODO: Incomplete.
}

// 0x4EDB70
void sub_4EDB70(int64_t obj, int a3, const char* str)
{
    Packet85* pkt;
    int extra_length;

    extra_length = strlen(str);
    pkt = (Packet85*)MALLOC(sizeof(*pkt) + extra_length);
    pkt->type = 85;
    pkt->extra_length = extra_length;
    pkt->oid = sub_407EF0(obj);
    pkt->field_20 = a3;
    strncpy(pkt + 1, str, extra_length);
    tig_net_send_app_all(pkt, sizeof(*pkt) + extra_length);
    FREE(pkt);
}

// 0x4EDC00
void sub_4EDC00(int64_t a1, int64_t a2, int a3)
{
    Packet90 pkt;

    pkt.type = 87;
    pkt.field_8 = sub_407EF0(a1);
    pkt.field_20 = sub_407EF0(a2);
    pkt.field_38 = a3;
    tig_net_send_app_all(&pkt, sizeof(pkt));
}

// 0x4EDC70
void sub_4EDC70(int64_t a1, int64_t a2, int a3)
{
    Packet90 pkt;

    pkt.type = 90;
    pkt.field_8 = sub_407EF0(a1);
    pkt.field_20 = sub_407EF0(a2);
    pkt.field_38 = a3;
    tig_net_send_app_all(&pkt, sizeof(pkt));
}

// 0x4EDCE0
void sub_4EDCE0(int64_t obj, tig_art_id_t art_id)
{
    Packet92 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        object_set_current_aid(obj, art_id);

        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            pkt.type = 92;
            pkt.oid = sub_407EF0(obj);
            pkt.art_id = art_id;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
    }
}

// 0x4EDD50
void sub_4EDD50(int64_t obj)
{
    Packet94 pkt;

    sub_4601F0(obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 94;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EDDB0
void sub_4EDDB0(Packet94* pkt)
{
    int64_t obj;

    obj = objp_perm_lookup(pkt->oid);
    if (obj != OBJ_HANDLE_NULL) {
        sub_4601F0(obj);
    }
}

// 0x4EDDE0
void sub_4EDDE0(int64_t obj)
{
    Packet95 pkt;

    if (obj == OBJ_HANDLE_NULL || player_is_pc_obj(obj)) {
        sub_4601C0();
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 95;
        pkt.oid.type = 0;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EDE40
void sub_4EDE40(Packet95* pkt)
{
    if (pkt->oid.type == 0 || player_is_pc_obj(objp_perm_lookup(pkt->oid))) {
        sub_4601C0();
    }
}

// 0x4EDE80
void sub_4EDE80(int64_t obj, int a2)
{
    Packet96 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0) {
        pkt.type = 96;
        pkt.oid = sub_407EF0(obj);
        pkt.field_20 = a2;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EDEE0
void sub_4EDEE0(Packet96* pkt)
{
    int64_t obj;

    if ((tig_net_flags & TIG_NET_HOST) != 0) {
        obj = objp_perm_lookup(pkt->oid);
        if (obj != OBJ_HANDLE_NULL) {
            sub_4670A0(obj, pkt->field_20);
        }
    }
}

// 0x4EDF20
void sub_4EDF20(int64_t obj, int64_t location, int dx, int dy, bool a7)
{
    Packet99 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        if (a7 && player_is_pc_obj(obj)) {
            sub_4B8CE0(location);
        }
        sub_43E770(obj, location, dx, dy);
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 99;
        pkt.oid = sub_407EF0(obj);
        pkt.location = location;
        pkt.dx = dx;
        pkt.dy = dy;
        pkt.field_30 = a7;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EDFF0
void sub_4EDFF0(Packet99* pkt)
{
    int64_t obj;

    if ((tig_net_flags & TIG_NET_HOST) != 0) {
        obj = objp_perm_lookup(pkt->oid);
        sub_43E770(obj, pkt->location, pkt->dx, pkt->dy);

        if (pkt->field_30 && player_is_pc_obj(obj)) {
            sub_4B8CE0(pkt->location);
        }
    }
}

// 0x4EE060
void sub_4EE060(int64_t a1, int64_t a2)
{
    Packet100 pkt;

    if (player_is_pc_obj(a1)) {
        sub_460740(a1, a2);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 100;
        pkt.subtype = 5;
        sub_4F0640(a1, &(pkt.d.s.field_8));
        sub_4F0640(a2, &(pkt.d.s.field_20));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE0F0
void sub_4EE0F0(int a1, int64_t a2, int64_t a3)
{
    Packet100 pkt;

    sub_460900(a1, a2, a3);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 100;
        pkt.subtype = 4;
        pkt.d.c.field_8 = a1;
        pkt.d.c.field_10 = sub_407EF0(a2);
        pkt.d.c.field_28 = sub_407EF0(a3);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE190
void sub_4EE190()
{
    Packet100 pkt;

    sub_460770();

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 100;
        pkt.subtype = 0;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE1D0
void sub_4EE1D0(int64_t obj)
{
    Packet100 pkt;

    sub_460240(obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 100;
        pkt.subtype = 1;
        pkt.d.b.field_8 = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE230
void sub_4EE230(int a1, int a2, int client_id)
{
    Packet100 pkt;

    if (sub_529520() == client_id) {
        sub_460790(a1, a2);
        return;
    }

    pkt.type = 100;
    pkt.subtype = 2;
    pkt.d.a.field_8 = a1;
    pkt.d.a.field_C = a2;
    tig_net_send_app(&pkt, sizeof(pkt), client_id);
}

// 0x4EE290
void sub_4EE290(int64_t obj, int a2, int a3)
{
    Packet100 pkt;
    int client_id;

    if (player_get_pc_obj() == obj) {
        sub_460720(a2, a3);
        return;
    }

    client_id = sub_4A2B10(obj);
    if (client_id != -1) {
        pkt.type = 100;
        pkt.subtype = 3;
        pkt.d.a.field_8 = a2;
        pkt.d.a.field_C = a3;
        tig_net_send_app(&pkt, sizeof(pkt), client_id);
    }
}

// 0x4EE310
void sub_4EE310(int64_t obj, int64_t a2)
{
    Packet100 pkt;

    if (player_is_pc_obj(obj)) {
        sub_4602D0(obj, a2);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 100;
        pkt.subtype = 6;
        sub_4F0640(obj, &(pkt.d.s.field_8));
        sub_4F0640(obj, &(pkt.d.s.field_20));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE3A0
void sub_4EE3A0(int64_t obj, int64_t a2)
{
    Packet100 pkt;

    if (player_is_pc_obj(obj)) {
        sub_4604F0(obj, a2);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 100;
        pkt.subtype = 7;
        sub_4F0640(obj, &(pkt.d.s.field_8));
        sub_4F0640(obj, &(pkt.d.s.field_20));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE430
void sub_4EE430(int64_t obj, int64_t a2)
{
    Packet100 pkt;

    if (player_is_pc_obj(obj)) {
        sub_460300(obj, a2);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 100;
        pkt.subtype = 8;
        sub_4F0640(obj, &(pkt.d.s.field_8));
        sub_4F0640(obj, &(pkt.d.s.field_20));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE4C0
void sub_4EE4C0(int64_t obj, int64_t a2)
{
    Packet100 pkt;

    if (player_is_pc_obj(obj)) {
        sub_460930(obj, a2);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 100;
        pkt.subtype = 10;
        sub_4F0640(obj, &(pkt.d.s.field_8));
        sub_4F0640(obj, &(pkt.d.s.field_20));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE550
void sub_4EE550(int64_t obj, int64_t a2)
{
    Packet100 pkt;

    if (player_is_pc_obj(obj)) {
        sub_460DE0(obj, a2);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 100;
        pkt.subtype = 9;
        sub_4F0640(obj, &(pkt.d.s.field_8));
        sub_4F0640(obj, &(pkt.d.s.field_20));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EE5E0
void sub_4EE5E0(Packet100* pkt)
{
    int64_t v1;
    int64_t v2;
    Packet100 new_pkt;

    switch (pkt->subtype) {
    case 0:
        sub_460770();
        break;
    case 1:
        sub_460240(objp_perm_lookup(pkt->d.b.field_8));
        break;
    case 2:
        sub_460790(pkt->d.a.field_8, pkt->d.a.field_C);
        break;
    case 3:
        sub_460790(pkt->d.a.field_8, pkt->d.a.field_C);
        break;
    case 4:
        sub_460900(pkt->d.c.field_8,
            objp_perm_lookup(pkt->d.c.field_10),
            objp_perm_lookup(pkt->d.c.field_28));
        break;
    case 5:
        sub_4F0690(pkt->d.s.field_20, &v2);
        sub_4F0690(pkt->d.s.field_8, &v1);
        if (player_is_pc_obj(v1)) {
            sub_460740(v1, v2);
        }
        break;
    case 6:
        sub_4F0690(pkt->d.s.field_8, &v1);
        sub_4F0690(pkt->d.s.field_20, &v2);
        if (player_is_pc_obj(v1)) {
            sub_4602D0(v1, v2);
        }
        break;
    case 7:
        sub_4F0690(pkt->d.s.field_8, &v1);
        sub_4F0690(pkt->d.s.field_20, &v2);
        if (player_is_pc_obj(v1)) {
            sub_4604F0(v1, v2);
        }
        break;
    case 8:
        sub_4F0690(pkt->d.s.field_8, &v1);
        sub_4F0690(pkt->d.s.field_20, &v2);
        if (player_is_pc_obj(v1)) {
            sub_460300(v1, v2);
        }
        break;
    case 9:
        sub_4F0690(pkt->d.s.field_8, &v1);
        sub_4F0690(pkt->d.s.field_20, &v2);
        if (player_is_pc_obj(v1)) {
            sub_460DE0(v1, v2);
        }
        break;
    case 10:
        sub_4F0690(pkt->d.s.field_8, &v1);
        sub_4F0690(pkt->d.s.field_20, &v2);
        if (player_is_pc_obj(v1)) {
            sub_460930(v1, v2);
        }
        break;
    case 11:
        sub_4F0690(pkt->d.x.field_8, &v1);
        if (player_is_pc_obj(v1)) {
            sub_4673F0(v1, pkt->d.x.field_20);
        }
        // FIXME: Fallthrough?
    case 12:
        sub_4F0690(pkt->d.z.field_8, &v1);
        sub_4F0690(pkt->d.z.field_20, &v2);
        switch (pkt->d.z.field_3C) {
        case 0:
            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                if (sub_460C50(v1, v2, pkt->d.z.field_38)) {
                    new_pkt = *pkt;
                    new_pkt.d.z.field_3C = 1;
                    tig_net_send_app(&new_pkt, sizeof(new_pkt), sub_52A530());
                }
            }
            break;
        case 1:
            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                if (sub_460C80(v1, v2, pkt->d.z.field_38)) {
                    new_pkt = *pkt;
                    new_pkt.d.z.field_3C = 2;
                    tig_net_send_app_all(&new_pkt, sizeof(new_pkt));
                }
            }
            break;
        case 2:
            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                sub_460CB0(v1, v2, pkt->d.z.field_38);

                new_pkt = *pkt;
                new_pkt.d.z.field_3C = 3;
                tig_net_send_app(&new_pkt, sizeof(new_pkt), sub_52A530());
            }
            break;
        case 3:
            if ((tig_net_flags & TIG_NET_HOST) != 0) {
                sub_460CE0(v1, v2, pkt->d.z.field_38);
            }
            break;
        }
        break;
    }
}

// 0x4EEB90
void sub_4EEB90(Packet102* pkt)
{
    int64_t pc_obj;

    pc_obj = objp_perm_lookup(pkt->oid);
    sub_4A2BC0();
    area_reset_last_known_area(pc_obj);
    sub_4A2BD0();
}

// 0x4EEBD0
void sub_4EEBD0()
{
    // TODO: Incomplete.
}

// 0x4EEC10
void sub_4EEC10(int64_t obj, int a2)
{
    Packet103 pkt;

    sub_441DD0(obj, a2);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 103;
        pkt.oid = sub_407EF0(obj);
        pkt.field_20 = a2;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EEC80
void sub_4EEC80(Packet103* pkt)
{
    sub_441DD0(objp_perm_lookup(pkt->oid), pkt->field_20);
}

// 0x4EECB0
void sub_4EECB0(int sound_id)
{
    Packet106 pkt;

    gsound_play_sfx_id(sound_id, 1);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 106;
        pkt.subtype = 0;
        pkt.field_8 = sound_id;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EED00
void sub_4EED00(int64_t obj, int sound_id)
{
    Packet106 pkt;
    int client_id;

    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    if (player_is_pc_obj(obj)) {
        gsound_play_sfx_id(sound_id, 1);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        client_id = sub_4A2B10(obj);
        if (client_id != -1) {
            pkt.type = 106;
            pkt.subtype = 0;
            pkt.field_8 = sound_id;
            tig_net_send_app(&pkt, sizeof(pkt), client_id);
        }
    }
}

// 0x4EED80
void sub_4EED80(int sound_id, int loops, int64_t obj)
{
    Packet106 pkt;

    sub_41B930(sound_id, loops, obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 106;
        pkt.subtype = 1;
        pkt.field_8 = sound_id;
        pkt.field_C = loops;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EEE00
void sub_4EEE00(int a1, int a2)
{
    Packet106 pkt;

    sub_41BD50(a1, a2);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 106;
        pkt.subtype = 2;
        pkt.field_8 = a1;
        pkt.field_C = a2;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EEE50
void sub_4EEE50(Packet106* pkt)
{
    switch (pkt->subtype) {
    case 0:
        gsound_play_sfx_id(pkt->field_8, 1);
        break;
    case 1:
        sub_41B930(pkt->field_8, pkt->field_C, objp_perm_lookup(pkt->oid));
        break;
    case 2:
        sub_41BD50(pkt->field_8, pkt->field_C);
        break;
    }
}

// 0x4EEEC0
void sub_4EEEC0(int64_t obj)
{
    Packet129 pkt;

    object_dec_current_aid(obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 11;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EEF20
void sub_4EEF20(int64_t obj)
{
    Packet129 pkt;

    object_inc_current_aid(obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 10;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EEF80
void sub_4EEF80(int64_t obj)
{
    Packet107 pkt;

    sub_4F08F0(obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 107;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EEFE0
void sub_4EEFE0(Packet107* pkt)
{
    sub_4F08F0(objp_perm_lookup(pkt->oid));
}

// 0x4EF010
void sub_4EF010(int64_t a1, bool a2)
{
    Packet108 pkt;

    sub_4D0F00(a1, a2);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 108;
        pkt.field_8 = a1;
        pkt.field_10 = a2;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EF060
void sub_4EF060(Packet108* pkt)
{
    sub_4D0F00(pkt->field_8, pkt->field_10);
}

// 0x4EF080
void sub_4EF080(int64_t obj, int a2)
{
    Packet109 pkt;

    sub_4B1CF0(obj, a2);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 109;
        pkt.oid = sub_407EF0(obj);
        pkt.field_20 = a2;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EF0F0
void sub_4EF0F0(Packet109* pkt)
{
    sub_4B1CF0(objp_perm_lookup(pkt->oid), pkt->field_20);
}

// 0x4EF120
void sub_4EF120(int map, int a2)
{
    Packet110 pkt;

    sub_4BEAB0(map, a2);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 110;
        pkt.map = map;
        pkt.field_8 = a2;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EF170
void sub_4EF170(Packet110* pkt)
{
    sub_4BEAB0(pkt->map, pkt->field_8);
}

// 0x4EF190
void sub_4EF190(tig_art_id_t art_id)
{
    Packet111 pkt;

    sub_502290(art_id);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 111;
        pkt.art_id = art_id;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EF1D0
void sub_4EF1D0(Packet111* pkt)
{
    sub_502290(pkt->art_id);
}

// 0x4EF1E0
void sub_4EF1E0(int64_t a1, int64_t obj)
{
    Packet112 pkt;

    if (player_get_pc_obj() == obj) {
        sub_40FE00(a1);
        return;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 112;
        pkt.oid = sub_407EF0(obj);
        pkt.field_8 = a1;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EF260
void sub_4EF260(Packet112* pkt)
{
    int64_t obj;

    obj = objp_perm_lookup(pkt->oid);
    if (obj == player_get_pc_obj()) {
        sub_40FE00(pkt->field_8);
    }
}

// 0x4EF320
void sub_4EF320()
{
    // TODO: Incomplete.
}

// 0x4EF3D0
void sub_4EF3D0()
{
    // TODO: Incomplete.
}

// 0x4EF540
void sub_4EF540()
{
    // TODO: Incomplete.
}

// 0x4EF5C0
void sub_4EF5C0(int64_t obj)
{
    Packet116 pkt;

    sub_4D56C0(obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 116;
        pkt.field_4 = 1;
        pkt.field_8 = 0;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EF630
void sub_4EF630(int64_t obj)
{
    Packet116 pkt;

    sub_4D62B0(obj);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 116;
        pkt.field_4 = 0;
        pkt.field_8 = 0;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EF690
void sub_4EF690(Packet116* pkt)
{
    if (pkt->field_8 == 0) {
        switch (pkt->field_4) {
        case 0:
            sub_4D62B0(objp_perm_lookup(pkt->oid));
            break;
        case 1:
            sub_4D56C0(objp_perm_lookup(pkt->oid));
            break;
        }
    }
}

// 0x4EF6F0
void sub_4EF6F0(int64_t a1, int64_t a2, int64_t a3)
{
    Packet117 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        sub_4445A0(a1, a2, a3);
        return;
    }

    pkt.type = 117;
    sub_4F0640(a1, &(pkt.field_8));
    sub_4F0640(a2, &(pkt.field_20));
    sub_4F0640(a3, &(pkt.field_38));
    tig_net_send_app_all(&pkt, sizeof(pkt));
}

// 0x4EF790
void sub_4EF790(Packet117* pkt)
{
    int64_t v1;
    int64_t v2;
    int64_t v3;

    if (pkt == NULL) {
        return;
    }

    if ((tig_net_flags & TIG_NET_HOST) != 0) {
        sub_4F0690(pkt->field_8, &v1);
        sub_4F0690(pkt->field_20, &v2);
        sub_4F0690(pkt->field_38, &v3);
        sub_462CC0(v1, v2, v3);
    }
}

// 0x4EF830
void sub_4EF830(int64_t a1, int64_t a2)
{
    Packet118 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        sub_4445A0(a1, a2);
        return;
    }

    pkt.type = 118;
    sub_4F0640(a1, &(pkt.field_8));
    sub_4F0640(a2, &(pkt.field_20));
    tig_net_send_app_all(&pkt, sizeof(pkt));
}

// 0x4EF8B0
void sub_4EF8B0(Packet118* pkt)
{
    int64_t v1;
    int64_t v2;

    if ((tig_net_flags & TIG_NET_HOST) != 0) {
        sub_4F0690(pkt->field_8, &v1);
        sub_4F0690(pkt->field_20, &v2);
        sub_4445A0(v1, v2);
    }
}

// 0x4EF920
void sub_4EF920()
{
    // TODO: Incomplete.
}

// 0x4EFA10
void sub_4EFA10()
{
    // TODO: Incomplete.
}

// 0x4EFA70
void sub_4EFA70(int a1, int a2, int a3)
{
    Packet120 pkt;

    pkt.type = 120;
    pkt.data.field_4 = a1;
    pkt.data.field_8 = a2;
    pkt.data.field_C = a3;
    tig_net_send_app_all(&pkt, sizeof(pkt));
}

// 0x4EFAB0
void sub_4EFAB0(Packet120* pkt)
{
    sub_44E2C0(&(pkt->data), 6);

    if ((tig_net_flags & TIG_NET_HOST) != 0) {
        tig_net_send_app_all(pkt, sizeof(*pkt));
    }
}

// 0x4EFAE0
void sub_4EFAE0(int64_t obj, int a2)
{
    Packet121 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        sub_463B30(obj, a2);
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        if (!sub_4A2BA0()) {
            pkt.type = 121;
            sub_4F0640(obj, &(pkt.oid));
            pkt.field_20 = a2;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
    }
}

// 0x4EFB50
void sub_4EFB50(Packet121* pkt)
{
    int64_t obj;

    sub_4A2BC0();
    sub_4F0690(pkt->oid, &obj);
    sub_463B30(obj, pkt->field_20);
    sub_4A2BD0();
}

// 0x4EFBA0
void sub_4EFBA0(int64_t obj)
{
    Packet122 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0) {
        pkt.type = 122;
        sub_4F0640(obj, &(pkt.oid));
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EFBE0
void sub_4EFBE0()
{
    // TODO: Incomplete.
}

// 0x4EFC30
void sub_4EFC30()
{
    // TODO: Incomplete.
}

// 0x4EFCD0
void sub_4EFCD0()
{
    // TODO: Incomplete.
}

// 0x4EFDD0
void sub_4EFDD0(int64_t obj, int fld, int value)
{
    Packet129 pkt;

    obj_field_int32_set(obj, fld, value);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 1;
        pkt.oid = sub_407EF0(obj);
        pkt.d.a.field_28 = value;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EFE50
void sub_4EFE50(int64_t obj, int fld, int64_t value)
{
    Packet129 pkt;

    obj_field_int64_set(obj, fld, value);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 1;
        pkt.oid = sub_407EF0(obj);
        pkt.d.a.field_28 = value;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EFEE0
void sub_4EFEE0(int64_t obj, unsigned int flags)
{
    Packet129 pkt;

    sub_43D0E0(obj, flags);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 2;
        pkt.oid = sub_407EF0(obj);
        pkt.d.b.field_28 = flags;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EFF50
void sub_4EFF50(int64_t obj, unsigned int flags)
{
    Packet129 pkt;

    sub_43D0E0(obj, flags);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 3;
        pkt.oid = sub_407EF0(obj);
        pkt.d.c.field_28 = flags;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4EFFC0
void mp_obj_field_obj_set(int64_t obj, int fld, int64_t value)
{
    Packet129 pkt;

    obj_field_handle_set(obj, fld, value);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 4;
        pkt.oid = sub_407EF0(obj);
        pkt.fld = fld;
        if (value != OBJ_HANDLE_NULL) {
            pkt.d.d.oid = sub_407EF0(value);
        } else {
            pkt.d.d.oid.type = 0;
        }
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4F0070
void sub_4F0070(int64_t obj, int fld, int index, int64_t value)
{
    Packet129 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        obj_arrayfield_obj_set(obj, fld, index, value);
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 129;
        pkt.subtype = 5;
        pkt.oid = sub_407EF0(obj);
        pkt.fld = fld;
        if (value != OBJ_HANDLE_NULL) {
            pkt.d.e.oid = sub_407EF0(value);
        } else {
            pkt.d.e.oid.type = 0;
        }
        pkt.d.e.field_28 = index;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4F0150
void sub_4F0150(int64_t obj, int fld, int index, int value)
{
    Packet129 pkt;

    obj_arrayfield_int32_set(obj, fld, index, value);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 129;
        pkt.subtype = 8;
        sub_4F0640(obj, &(pkt.oid));
        pkt.fld = fld;
        pkt.d.f.field_28 = index;
        pkt.d.f.field_2C = value;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4F01D0
void sub_4F01D0(int64_t obj, int fld, int index, int value)
{
    // TODO: Incomplete.
}

// 0x4F0270
void obj_f_set_int32_with_network(int64_t obj, int fld, int index, int value)
{
    Packet129 pkt;

    sub_4074E0(obj, fld, index, value);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 129;
        pkt.subtype = 7;
        sub_4F0640(obj, &(pkt.oid));
        pkt.fld = fld;
        pkt.d.g.field_28 = index;
        pkt.d.g.field_2C = value;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4F02F0
void sub_4F02F0(int64_t obj, tig_art_id_t art_id)
{
    Packet129 pkt;

    object_set_current_aid(obj, art_id);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 9;
        pkt.oid = sub_407EF0(obj);
        pkt.d.h.art_id = art_id;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4F0360
void sub_4F0360(int64_t obj, int fld, int index, int value)
{
    Packet129 pkt;

    sub_43ECF0(obj, fld, value);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 129;
        pkt.subtype = 12;
        sub_4F0640(obj, &(pkt.oid));
        pkt.d.i.field_28 = fld;
        pkt.d.i.field_2C = index;
        pkt.d.i.field_30 = value;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4F03E0
void sub_4F03E0(int64_t obj, unsigned int flags_to_add)
{
    unsigned int flags;
    Packet129 pkt;

    flags = obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS);
    flags |= flags_to_add;
    obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, flags);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            sub_4EFDD0(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS));
        } else {
            pkt.type = 129;
            pkt.subtype = 13;
            sub_4F0640(obj, &(pkt.oid));
            pkt.d.j.flags = flags_to_add;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
    }
}

// 0x4F0470
void sub_4F0470(int64_t obj, unsigned int flags_to_remove)
{
    unsigned int flags;
    Packet129 pkt;

    flags = obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS);
    flags &= ~flags_to_remove;
    obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, flags);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            sub_4EFDD0(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS));
        } else {
            pkt.type = 129;
            pkt.subtype = 14;
            sub_4F0640(obj, &(pkt.oid));
            pkt.d.j.flags = flags_to_remove;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
    }
}

// 0x4F0500
void sub_4F0500(int64_t obj, int fld)
{
    Packet130 pkt;

    sub_407D50(obj, fld);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 130;
        pkt.field_4 = 0;
        pkt.fld = fld;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4F0570
void sub_4F0570(int64_t obj, int fld, int length)
{
    Packet130 pkt;

    obj_arrayfield_length_set(obj, fld, length);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        pkt.type = 130;
        pkt.field_4 = 1;
        pkt.fld = fld;
        pkt.length = length;
        pkt.oid = sub_407EF0(obj);
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4F05F0
void sub_4F05F0()
{
    // TODO: Incomplete.
}

// 0x4F0640
void sub_4F0640(int64_t obj, ObjectID* oid_ptr)
{
    if (obj != OBJ_HANDLE_NULL) {
        *oid_ptr = sub_407EF0(obj);
    } else {
        oid_ptr->type = 0;
    }
}

// 0x4F0690
void sub_4F0690(ObjectID oid, int64_t* obj_ptr)
{
    if (oid.type != 0) {
        *obj_ptr = objp_perm_lookup(oid);
    } else {
        *obj_ptr = OBJ_HANDLE_NULL;
    }
}

// 0x4F06E0
void sub_4F06E0(int64_t a1, int64_t a2)
{
    tig_art_id_t art_id;
    unsigned int flags;
    int sound_id;

    art_id = obj_field_int32_get(a1, OBJ_F_CURRENT_AID);

    // FIXME: Unused.
    sub_504260(art_id);

    art_id = sub_4EC830(art_id);
    if (art_id != TIG_ART_ID_INVALID) {
        flags = obj_field_int32_get(a1, OBJ_F_PORTAL_FLAGS);
        flags |= OPF_BUSTED;
        obj_field_int32_set(a1, OBJ_F_PORTAL_FLAGS, flags);
        object_set_current_aid(a1, art_id);
        sound_id = sub_4F1050(a1, 0);
        sub_441980(a2, a1, 0, 0, 30, 0);
        sub_41B930(sound_id, 1, a1);
    } else {
        sub_43CCA0(a1);
        sound_id = sub_4F1050(a1, 1);
        sub_41B930(sound_id, 1, a1);
    }
}

// 0x4F0790
void sub_4F0790()
{
    // TODO: Incomplete.
}

// 0x4F08C0
bool sub_4F08C0(int64_t obj)
{
    return tig_art_id_frame_get(obj_field_int32_get(obj, OBJ_F_CURRENT_AID)) != 0;
}

// 0x4F08F0
void sub_4F08F0(int64_t obj)
{
    if (tig_art_id_frame_get(obj_field_int32_get(obj, OBJ_F_CURRENT_AID)) == 0) {
        while (sub_4F0950(obj)) {
        }
    } else {
        while (sub_4F0A90(obj)) {
        }
    }
}

// 0x4F0950
bool sub_4F0950(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4F0A90
bool sub_4F0A90(int64_t obj)
{
    // TODO: Incomplete.
}

// 0x4F0BF0
void sub_4F0BF0()
{
    // TODO: Incomplete.
}

// 0x4F0ED0
void sub_4F0ED0()
{
    // TODO: Incomplete.
}

// 0x4F0FD0
int sub_4F0FD0(int64_t obj, int a2)
{
    return obj != OBJ_HANDLE_NULL && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_CONTAINER
        ? obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT) + a2
        : -1;
}

// 0x4F1010
int sub_4F1010(int64_t obj, int a2)
{
    return obj != OBJ_HANDLE_NULL && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PORTAL
        ? obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT) + a2
        : -1;
}

// 0x4F1050
int sub_4F1050(int64_t obj, int a2)
{
    // TODO: Incomplete.
}
