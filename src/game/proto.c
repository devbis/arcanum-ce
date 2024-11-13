#include "game/proto.h"

#include <stdio.h>

#include "game/critter.h"
#include "game/level.h"
#include "game/light.h"
#include "game/magictech.h"
#include "game/obj_private.h"
#include "game/object.h"
#include "game/oname.h"
#include "game/script.h"
#include "game/spell.h"
#include "game/stat.h"

typedef enum ProtoField {
    PROTO_F_DESCRIPTION,
    PROTO_F_INTERNAL_NAME,
    PROTO_F_LEVEL,
    PROTO_F_ART_NUMBER_AND_PALETTE,
    PROTO_F_SCALE,
    PROTO_F_ALIGNMENT,
    PROTO_F_CRITTER_FLAG,
    PROTO_F_CRITTER2_FLAG,
    PROTO_F_NPC_FLAG,
    PROTO_F_BLIT_FLAG,
    PROTO_F_SPELL_FLAG,
    PROTO_F_HIT_CHART,
    PROTO_F_BASIC_STAT,
    PROTO_F_SPELL,
    PROTO_F_FACTION,
    PROTO_F_AI_PACKET,
    PROTO_F_MATERIAL,
    PROTO_F_HIT_POINTS,
    PROTO_F_SCRIPT,
    PROTO_F_DAMAGE_RESISTANCE,
    PROTO_F_FIRE_RESISTANCE,
    PROTO_F_ELECTRICAL_RESISTANCE,
    PROTO_F_POISON_RESISTANCE,
    PROTO_F_MAGIC_RESISTANCE,
    PROTO_F_NORMAL_DAMAGE,
    PROTO_F_POISON_DAMAGE,
    PROTO_F_ELECTRICAL_DAMAGE,
    PROTO_F_FIRE_DAMAGE,
    PROTO_F_FATIGUE_DAMAGE,
    PROTO_F_SOUND_BANK,
    PROTO_F_INVENTORY_SOURCE,
    PROTO_F_PORTRAIT,
    PROTO_F_RETAIL_PRICE_MULTIPLIER,
    PROTO_F_SOCIAL_CLASS,
    PROTO_F_OBJECT_FLAG,
    PROTO_F_AUTO_LEVEL_SCHEME,
    PROTO_F_CATEGORY,
    PROTO_F_COUNT,
} ProtoField;

static ObjectType sub_4685D0(int description);
static int sub_468600(ObjectType object_type);
static void sub_468660(bool* error_ptr);
static int sub_468720(ObjectType object_type);
static void sub_468800();
static bool proto_save(long long obj);
static ObjectID sub_468860(int a1);
static void sub_468890(int description);
static void sub_468930(long long obj, int description);
static void sub_49AFF0(long long obj, int description);
static void sub_49B010(long long obj, int description);
static void sub_49B220(long long obj, int description);
static void sub_49B240(long long obj, int f, int percent);
static void sub_49B2E0(long long obj);
static void sub_49B340(long long obj, int description);
static int sub_49B5A0(TigFile* stream, long long obj, int type);
static void sub_49BB40(long long obj, int a2);
static bool sub_49BB70(const char* str, int* fld_ptr, int* a3, int* a4, int* a5);
static int sub_49BF10(const char* str, const char** identifiers, int size);
static void sub_49C060(long long obj, TigFile* stream, int type);
static void sub_49C610(TigFile* stream, const char* name, int value, const char** identifiers, int size);

// 0x5B37FC
static int dword_5B37FC[OBJ_TYPE_COUNT] = {
    /*          OBJ_TYPE_WALL */ 1000,
    /*        OBJ_TYPE_PORTAL */ 2008,
    /*     OBJ_TYPE_CONTAINER */ 3023,
    /*       OBJ_TYPE_SCENERY */ 4006,
    /*    OBJ_TYPE_PROJECTILE */ 5028,
    /*        OBJ_TYPE_WEAPON */ 6029,
    /*          OBJ_TYPE_AMMO */ 7038,
    /*    OBJ_TYPE_ITEM_ARMOR */ 8042,
    /*     OBJ_TYPE_ITEM_GOLD */ 9056,
    /*     OBJ_TYPE_ITEM_FOOD */ 10057,
    /*   OBJ_TYPE_ITEM_SCROLL */ 11059,
    /*      OBJ_TYPE_ITEM_KEY */ 12060,
    /* OBJ_TYPE_ITEM_KEY_RING */ 13061,
    /*  OBJ_TYPE_ITEM_WRITTEN */ 14062,
    /*  OBJ_TYPE_ITEM_GENERIC */ 15065,
    /*            OBJ_TYPE_PC */ 16066,
    /*           OBJ_TYPE_NPC */ 17067,
    /*          OBJ_TYPE_TRAP */ 26000,
    /*       OBJ_TYPE_MONSTER */ 27309,
    /*    OBJ_TYPE_UNIQUE_NPC */ 28310,
};

// 0x5B384C
static int dword_5B384C[OBJ_TYPE_COUNT] = {
    /*          OBJ_TYPE_WALL */ 1008,
    /*        OBJ_TYPE_PORTAL */ 2023,
    /*     OBJ_TYPE_CONTAINER */ 3061,
    /*       OBJ_TYPE_SCENERY */ 4051,
    /*    OBJ_TYPE_PROJECTILE */ 5029,
    /*        OBJ_TYPE_WEAPON */ 6179,
    /*          OBJ_TYPE_AMMO */ 7042,
    /*    OBJ_TYPE_ITEM_ARMOR */ 8293,
    /*     OBJ_TYPE_ITEM_GOLD */ 9057,
    /*     OBJ_TYPE_ITEM_FOOD */ 10145,
    /*   OBJ_TYPE_ITEM_SCROLL */ 11143,
    /*      OBJ_TYPE_ITEM_KEY */ 12063,
    /* OBJ_TYPE_ITEM_KEY_RING */ 13062,
    /*  OBJ_TYPE_ITEM_WRITTEN */ 14130,
    /*  OBJ_TYPE_ITEM_GENERIC */ 15213,
    /*            OBJ_TYPE_PC */ 16079,
    /*           OBJ_TYPE_NPC */ 17317,
    /*          OBJ_TYPE_TRAP */ 26008,
    /*       OBJ_TYPE_MONSTER */ 27394,
    /*    OBJ_TYPE_UNIQUE_NPC */ 28472,
};

// 0x5B389C
static int dword_5B389C[RACE_COUNT] = {
    /*     RACE_HUMAN */ 0,
    /*     RACE_DWARF */ 1,
    /*       RACE_ELF */ 4,
    /*  RACE_HALF_ELF */ 4,
    /*     RACE_GNOME */ 2,
    /*  RACE_HALFLING */ 2,
    /*  RACE_HALF_ORC */ 0,
    /* RACE_HALF_OGRE */ 3,
    /*  RACE_DARK_ELF */ 4,
    /*      RACE_OGRE */ 3,
    /*       RACE_ORC */ 0,
};

// 0x5B38C8
static const char* off_5B38C8[] = {
    "Description",
    "Internal Name",
    "Level",
    "Art Number and Palette",
    "Scale",
    "Alignment",
    "Critter Flag",
    "Critter2 Flag",
    "NPC Flag",
    "Blit Flag",
    "Spell Flag",
    "Hit Chart",
    "Basic Stat",
    "Spell",
    "Faction",
    "AI Packet",
    "Material",
    "Hit Points",
    "Script",
    "Damage Resistance",
    "Fire Resistance",
    "Electrical Resistance",
    "Poison Resistance",
    "Magic Resistance",
    "Normal Damage",
    "Poison Damage",
    "Electrical Damage",
    "Fire Damage",
    "Fatigue Damage",
    "Sound Bank",
    "Inventory Source",
    "Portrait",
    "Retail Price Multiplier",
    "Social Class",
    "Object Flag",
    "Auto Level Scheme",
    "Category",
};

// 0x5B395C
static const char* off_5B395C[] = {
    "TAB_FLIP_X",
    "TAB_FLIP_Y",
    "TAB_PALETTE_ORIGINAL",
    "TAB_PALETTE_OVERRIDE",
    "TAB_BLEND_ADD",
    "TAB_BLEND_SUB",
    "TAB_BLEND_MUL",
    "TAB_BLEND_ALPHA_AVG",
    "TAB_BLEND_ALPHA_CONST",
    "TAB_BLEND_ALPHA_SRC",
    "TAB_BLEND_ALPHA_LERP_X",
    "TAB_BLEND_ALPHA_LERP_Y",
    "TAB_BLEND_ALPHA_LERP_BOTH",
    "TAB_BLEND_COLOR_CONST",
    "TAB_BLEND_COLOR_ARRAY",
    "TAB_BLEND_ALPHA_STIPPLE_S",
    "TAB_BLEND_ALPHA_STIPPLE_D",
    "TAB_BLEND_COLOR_LERP",
    "TAB_SCRATCH_VALID",
    "TAB_PALETTE_MASK",
    "TAB_BLEND_ALPHA_MASK",
    "TAB_BLEND_COLOR_MASK",
};

// 0x5E8828
static bool in_proto_save;

// 0x5E882C
static ObjectID* dword_5E882C;

// 0x5E8830
static bool initialized;

// 0x4681B0
bool proto_init(GameInitInfo* init_info)
{
    unsigned int index;
    bool error;
    TigFileList file_list;
    char path[TIG_MAX_PATH];
    TigFile* stream;
    long long obj;

    if (initialized) {
        return true;
    }

    dword_5E882C = (ObjectID*)CALLOC(OBJ_TYPE_COUNT, sizeof(ObjectID));

    tig_file_mkdir("proto");

    for (index = 0; index < OBJ_TYPE_COUNT; index++) {
        dword_5E882C[index] = sub_468860(sub_468600(index));
    }

    error = false;
    sub_468660(&error);

    if (error) {
        FREE(dword_5E882C);
        return false;
    }

    obj_exit();
    if (!obj_init(init_info)) {
        FREE(dword_5E882C);
        return false;
    }

    tig_file_list_create(&file_list, "proto\\*.pro");
    for (index = 0; index < file_list.count; index++) {
        sprintf(path, "proto\\%s", file_list.entries[index].path);
        stream = tig_file_fopen(path, "rb");
        if (stream != NULL) {
            obj_read(stream, &obj);
            tig_file_fclose(stream);
        }
    }
    tig_file_list_destroy(&file_list);

    initialized = true;

    return true;
}

// 0x468310
void proto_exit()
{
    sub_468800();
    FREE(dword_5E882C);
    initialized = false;
}

// 0x468330
bool proto_save(long long obj)
{
    ObjectID object_id;
    char* name = NULL;
    char path[TIG_MAX_PATH];
    TigFile* stream;

    object_id = sub_407EF0(obj);
    obj_field_string_get(obj, OBJ_F_NAME, &name);

    if (object_id.type != 1) {
        return false;
    }

    sprintf(path, "proto\\%06d - %s.pro", object_id.d.a, name);
    FREE(name);

    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        tig_debug_printf("Error - unable to open file %s in proto_save()\n", path);
        return false;
    }

    if (!obj_write(stream, obj)) {
        tig_debug_printf("Error - obj_write() returned 0 in proto_save() while trying to save %s\n", path);
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}

// 0x468570
long long sub_468570(ObjectType object_type)
{
    return objp_perm_lookup(dword_5E882C[object_type]);
}

// 0x4685A0
long long sub_4685A0(int a1)
{
    return  objp_perm_lookup(sub_468860(a1));
}

// 0x4685D0
ObjectType sub_4685D0(int description)
{
    ObjectType object_type;

    for (object_type = OBJ_TYPE_WALL; object_type < OBJ_TYPE_COUNT - 1; object_type++) {
        if (description < dword_5B37FC[object_type + 1]) {
            break;
        }
    }

    // NOTE: Should unique NPC be normalized to NPC?
    if (object_type == OBJ_TYPE_MONSTER) {
        object_type = OBJ_TYPE_NPC;
    }

    return object_type;
}

// 0x468600
int sub_468600(ObjectType object_type)
{
    return dword_5B37FC[object_type];
}

// 0x468610
bool sub_468610(int description)
{
    ObjectType object_type;

    object_type = sub_4685D0(description);
    if (object_type == OBJ_TYPE_NPC) {
        if (description >= 28310) {
            object_type = OBJ_TYPE_UNIQUE_NPC;
        } else if (description >= 27309) {
            object_type = OBJ_TYPE_MONSTER;
        }
    }

    return description >= dword_5B37FC[object_type] && description < dword_5B384C[object_type];
}

// 0x468660
void sub_468660(bool* error_ptr)
{
    int object_type;
    int description;
    ObjectID object_id;
    const char* name;
    char path[TIG_MAX_PATH];

    tig_debug_printf("Reading prototypes...\n");

    for (object_type = 0; object_type < 20; object_type++) {
        for (description = dword_5B37FC[object_type]; description < dword_5B384C[object_type]; description++) {
            object_id = sub_468860(description);
            name = sub_4E7400(sub_468720(object_type));
            sprintf(path, "proto\\%06d - %s.pro", object_id.d.a, name);
            if (!tig_file_exists(path, NULL)) {
                sub_468890(description);
                *error_ptr = true;
            }
        }
    }

    tig_debug_printf("Reading prototypes completed.\n");
}

// 0x468720
int sub_468720(ObjectType object_type)
{
    switch (object_type) {
    case OBJ_TYPE_WALL:
        return 400;
    case OBJ_TYPE_PORTAL:
        return 800;
    case OBJ_TYPE_CONTAINER:
        return 1200;
    case OBJ_TYPE_SCENERY:
        return 1600;
    case OBJ_TYPE_PROJECTILE:
        return 1980;
    case OBJ_TYPE_WEAPON:
        return 2000;
    case OBJ_TYPE_AMMO:
        return 2400;
    case OBJ_TYPE_ITEM_ARMOR:
        return 2800;
    case OBJ_TYPE_ITEM_GOLD:
        return 3200;
    case OBJ_TYPE_ITEM_FOOD:
        return 3600;
    case OBJ_TYPE_ITEM_SCROLL:
        return 4000;
    case OBJ_TYPE_ITEM_KEY:
        return 4400;
    case OBJ_TYPE_ITEM_KEY_RING:
        return 4800;
    case OBJ_TYPE_ITEM_WRITTEN:
        return 5200;
    case OBJ_TYPE_ITEM_GENERIC:
        return 5600;
    case OBJ_TYPE_PC:
        return 6000;
    case OBJ_TYPE_NPC:
        return 6400;
    case OBJ_TYPE_TRAP:
        return 10001;
    case OBJ_TYPE_MONSTER:
        return 6401;
    case OBJ_TYPE_UNIQUE_NPC:
        return 6402;
    }
    return 0;
}

// 0x468800
void sub_468800()
{
    ObjectType object_type;
    int description;

    for (object_type = 0; object_type < OBJ_TYPE_COUNT; object_type++) {
        for (description = dword_5B37FC[object_type]; description < dword_5B384C[object_type]; description++) {
            sub_405BF0(objp_perm_lookup(sub_468860(description)));
        }
    }
}

// 0x468860
ObjectID sub_468860(int a1)
{
    return sub_4E6540(a1 + 20);
}

// 0x468890
void sub_468890(int description)
{
    ObjectType object_type;
    long long obj;

    object_type = sub_4685D0(description);
    sub_405800(object_type, &obj);
    obj_field_int32_set(obj, OBJ_F_NAME, sub_468720(object_type));
    sub_408020(obj, description + 20);
    sub_468930(obj, description);
    in_proto_save = true;
    proto_save(obj);
    in_proto_save = false;
    sub_405BF0(obj);
}

// 0x468930
void sub_468930(long long obj, int description)
{
    Script scr;
    tig_art_id_t art_id;
    unsigned int rgb;
    const char* str;

    obj_field_int32_set(obj, OBJ_F_DESCRIPTION, description);
    scr.num = 0;
    scr.hdr.flags = 0;
    scr.hdr.counters = 0;

    switch (sub_4685D0(description)) {
    case OBJ_TYPE_WALL:
        switch (description) {
        case 1000:
            sub_49B240(obj, OBJ_F_HP_PTS, -30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 1);
            break;
        case 1001:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 1);
            break;
        case 1002:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            break;
        case 1003:
            sub_49B240(obj, OBJ_F_HP_PTS, -50);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            break;
        case 1004:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            break;
        case 1005:
            sub_49B240(obj, OBJ_F_HP_PTS, -60);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x30);
            break;
        case 1006:
            sub_49B240(obj, OBJ_F_HP_PTS, -30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x30);
            break;
        case 1007:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_WALL_FLAGS, obj_field_int32_get(obj, OBJ_F_WALL_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x800);
            break;
        }
        break;
    case OBJ_TYPE_PORTAL:
        obj_field_int32_set(obj, OBJ_F_PORTAL_FLAGS, 9);
        switch (description) {
        case 2008:
            sub_49B240(obj, OBJ_F_HP_PTS, -30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8060);
            break;
        case 2009:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8065);
            break;
        case 2010:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x30);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8055);
            break;
        case 2011:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8075);
            break;
        case 2012:
            sub_49B240(obj, OBJ_F_HP_PTS, -70);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x10);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8060);
            break;
        case 2013:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8070);
            break;
        case 2014:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_PORTAL_FLAGS, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8070);
            break;
        case 2015:
            sub_49B240(obj, OBJ_F_HP_PTS, -70);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x10);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8100);
            break;
        case 2016:
            sub_49B240(obj, OBJ_F_HP_PTS, -70);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x10);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_PORTAL_FLAGS, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8100);
            break;
        case 2017:
            sub_49B240(obj, OBJ_F_HP_PTS, -90);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x420);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8100);
            break;
        case 2018:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x30);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_PORTAL_FLAGS, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8100);
            break;
        case 2019:
            sub_49B240(obj, OBJ_F_HP_PTS, -30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8110);
            break;
        case 2020:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8105);
            break;
        case 2021:
            sub_49B240(obj, OBJ_F_HP_PTS, -50);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_PORTAL_FLAGS, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8100);
            break;
        case 2022:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_PORTAL_FLAGS, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8110);
            break;
        }
    case OBJ_TYPE_CONTAINER:
        obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, 9);
        switch (description) {
        case 3023:
            tig_art_container_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400400);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8025);
            break;
        case 3024:
            tig_art_container_id_create(0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xAu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 3025:
            tig_art_container_id_create(0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8015);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 3026:
            tig_art_container_id_create(1, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xBu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            break;
        case 3027:
            tig_art_container_id_create(2, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(3, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            break;
        case 3028:
            tig_art_container_id_create(4, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(5, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            break;
        case 3029:
            tig_art_container_id_create(6, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(7, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            break;
        case 3030:
            tig_art_container_id_create(8, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(9, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            break;
        case 3031:
            tig_art_container_id_create(0xCu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            break;
        case 3032:
            tig_art_container_id_create(0xEu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xFu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            break;
        case 3033:
            tig_art_container_id_create(1, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(2, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8020);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 3034:
            tig_art_container_id_create(3, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(4, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8020);
            break;
        case 3035:
            tig_art_container_id_create(6, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(7, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8005);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 69);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 3036:
            tig_art_container_id_create(6, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(7, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8005);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 70);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 3037:
            tig_art_container_id_create(6, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(7, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8005);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 71);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 3038:
            tig_art_container_id_create(8, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(9, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8005);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 69);
            break;
        case 3039:
            tig_art_container_id_create(8, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(9, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8005);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 70);
            break;
        case 3040:
            tig_art_container_id_create(8, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(9, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8005);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 71);
            break;
        case 3041:
            tig_art_container_id_create(0x2Cu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xAu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8010);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 56);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            break;
        case 3042:
            tig_art_container_id_create(0x2Cu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xAu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8010);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 57);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 3043:
            tig_art_container_id_create(0x2Bu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xAu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8010);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 58);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 3044:
            tig_art_container_id_create(0x2Bu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xAu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8010);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 59);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 3045:
            tig_art_container_id_create(0x2Au, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xAu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8030);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 60);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 3046:
            tig_art_container_id_create(0x29, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xAu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8030);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 64);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 3047:
            tig_art_container_id_create(0x30, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8010);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 56);
            break;
        case 3048:
            tig_art_container_id_create(0x30, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8010);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 57);
            break;
        case 3049:
            tig_art_container_id_create(0x2Fu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8010);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 58);
            break;
        case 3050:
            tig_art_container_id_create(0x2Fu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8010);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 59);
            break;
        case 3051:
            tig_art_container_id_create(0x2Eu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8030);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 60);
            break;
        case 3052:
            tig_art_container_id_create(0x2Du, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8030);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 64);
            break;
        case 3053:
            tig_art_container_id_create(0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xAu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 102);
            break;
        case 3054:
            tig_art_container_id_create(0xCu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8000);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x200);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 102);
            break;
        case 3055:
            tig_art_container_id_create(0x33, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8020);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x400);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_INVENTORY_SOURCE, 64);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 3056:
            tig_art_container_id_create(0x32, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8025);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 14);
            break;
        case 3057:
            tig_art_container_id_create(0x20, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8035);
            break;
        case 3058:
            tig_art_container_id_create(0x1Du, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8040);
            break;
        case 3059:
            tig_art_container_id_create(0x1Bu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8045);
            break;
        case 3060:
            tig_art_container_id_create(0x34, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_container_id_create(0xDu, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_DESTROYED_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_CONTAINER_FLAGS, obj_field_int32_get(obj, OBJ_F_CONTAINER_FLAGS) | 0x600);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8025);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 15);
            break;
        }
        break;
    case OBJ_TYPE_SCENERY:
        switch (description) {
        case 4006:
            tig_art_scenery_id_create(0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4030 | 0x400800);
            tig_art_light_id_create(0x13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_FLAGS, 18);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 4007:
            tig_art_scenery_id_create(0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4030 | 0x400800);
            tig_art_light_id_create(0x14, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_FLAGS, 18);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 4008:
            tig_art_scenery_id_create(0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400800);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 4009:
            tig_art_scenery_id_create(0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4030 | 0x400800);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 4010:
            tig_art_scenery_id_create(0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x400C00);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 4011:
            tig_art_scenery_id_create(0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400400);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 4012:
            tig_art_scenery_id_create(0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 4013:
            tig_art_scenery_id_create(0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4030 | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 14);
            break;
        case 4014:
            tig_art_scenery_id_create(0, 9, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400400);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 4015:
            tig_art_scenery_id_create(0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 4016:
            tig_art_scenery_id_create(0, 11, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4030 | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 4017:
            tig_art_scenery_id_create(0, 12, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400400);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            break;
        case 4018:
            tig_art_scenery_id_create(0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 4019:
            tig_art_scenery_id_create(0, 14, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 4020:
            tig_art_scenery_id_create(0, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 17);
            break;
        case 4021:
            tig_art_scenery_id_create(0, 16, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 16);
            break;
        case 4022:
            tig_art_scenery_id_create(0, 17, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~4030);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 18);
            break;
        case 4023:
            tig_art_scenery_id_create(0, 18, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 4, 80);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4030);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 15);
            break;
        case 4024:
            tig_art_scenery_id_create(0, 19, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 250);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 4, 80);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 15);
            break;
        case 4025:
            tig_art_scenery_id_create(0, 22, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x800 | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 20);
            break;
        case 4026:
            tig_art_scenery_id_create(0, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 21);
            break;
        case 4027:
            tig_art_scenery_id_create(0, 20, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30000);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 4, 100);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4030);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 19);
            break;
        case 4028:
            tig_art_scenery_id_create(0, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400C34);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x201);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            break;
        case 4029:
        case 4034:
            tig_art_scenery_id_create(0xBu, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400C30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 10);
            break;
        case 4030:
            tig_art_scenery_id_create(0x66, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400010);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 11);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            break;
        case 4031:
            tig_art_scenery_id_create(0x65, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400420);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 12);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x10);
            break;
        case 4032:
            tig_art_scenery_id_create(0x64, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            break;
        case 4033:
            tig_art_scenery_id_create(0xBu, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400C20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 10);
            break;
        case 4035:
            tig_art_scenery_id_create(0, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(255, 255, 240, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4036:
            tig_art_scenery_id_create(4, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0xBu, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(244, 254, 255, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4037:
            tig_art_scenery_id_create(4, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(244, 254, 255, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4038:
            tig_art_scenery_id_create(1, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0xBu, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(255, 255, 240, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4039:
            tig_art_scenery_id_create(6, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0xEu, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(244, 254, 255, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4040:
            tig_art_scenery_id_create(6, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0xFu, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(244, 254, 0xFFu, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4041:
            tig_art_scenery_id_create(6, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0x10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(244, 254, 255, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4042:
            tig_art_scenery_id_create(6, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0x11, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(244, 254, 0xFFu, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4043:
            tig_art_scenery_id_create(4, 16, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 250);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 4, 80);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8500);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x10);
            break;
        case 4044:
            tig_art_scenery_id_create(7, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0xBu, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(193, 255, 255, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4045:
            tig_art_scenery_id_create(8, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0xBu, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(193, 255, 255, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4046:
            tig_art_scenery_id_create(9, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100400);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
            tig_art_light_id_create(0xBu, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
            light_build_color(193, 255, 255, &rgb);
            obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);
            obj_field_int32_set(obj, OBJ_F_SCENERY_FLAGS, obj_field_int32_get(obj, OBJ_F_SCENERY_FLAGS) | 0x4);
            break;
        case 4047:
            tig_art_scenery_id_create(0x4Bu, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 250);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8521);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            break;
        case 4048:
            tig_art_scenery_id_create(0x1Du, 21, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) & ~0x4000 | 0x100C00);
            obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 64);
            break;
        case 4049:
            tig_art_scenery_id_create(0x61, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 250);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 8521);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400000);
            break;
        case 4050:
            tig_art_scenery_id_create(0xBu, 15, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x400420);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 12);
            break;
        }
        break;
    case OBJ_TYPE_PROJECTILE:
        tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
        break;
    case OBJ_TYPE_WEAPON:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        switch (description) {
        case 6029:
            tig_art_item_id_create(4, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6030:
            tig_art_item_id_create(12, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6031:
            tig_art_item_id_create(1, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6032:
            tig_art_item_id_create(1, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 8);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 6033:
            tig_art_item_id_create(6, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6034:
            tig_art_item_id_create(0, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6035:
            tig_art_item_id_create(0, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6036:
            tig_art_item_id_create(1, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6037:
            tig_art_item_id_create(2, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6038:
            tig_art_item_id_create(19, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(19, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(19, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 1, 35);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            break;
        case 6039:
            tig_art_item_id_create(4, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6040:
            tig_art_item_id_create(4, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 2);
            tig_art_scenery_id_create(8, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 2, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 2, 40);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 45);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6041:
            tig_art_item_id_create(13, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 3, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 3, 30);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 5070);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6042:
            tig_art_item_id_create(14, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6043:
            tig_art_item_id_create(0, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 16);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            scr.num = 29006;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            scr.num = 29006;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_MISS, &scr);
            break;
        case 6044:
            tig_art_item_id_create(8, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_BONUS_TO_HIT, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -35);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6045:
            tig_art_item_id_create(10, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_NAME, 5678);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6046:
            tig_art_item_id_create(11, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6047:
            tig_art_item_id_create(10, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6048:
            tig_art_item_id_create(4, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 18);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6049:
            tig_art_item_id_create(3, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 3);
            tig_art_scenery_id_create(0x20, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 3, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 3, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 2, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 2, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 8);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 32);
            break;
        case 6050:
            tig_art_item_id_create(5, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 2, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 2, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6051:
            tig_art_item_id_create(2, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6052:
            tig_art_item_id_create(8, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x24, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 536);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6053:
            tig_art_item_id_create(16, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(16, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(16, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 24);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -33);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6054:
            tig_art_item_id_create(7, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29688);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6055:
            tig_art_item_id_create(6, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_SPEED_ADJ, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29688);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6056:
            tig_art_item_id_create(12, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 3, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 3, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6057:
            tig_art_item_id_create(3, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 3, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 3, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6058:
            tig_art_item_id_create(3, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            goto LABEL_252;
        case 6059:
            tig_art_item_id_create(9, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 90);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6060:
            tig_art_item_id_create(8, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 65);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6061:
            tig_art_item_id_create(17, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(17, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(17, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 120);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6062:
            tig_art_item_id_create(16, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(16, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(16, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6063:
            tig_art_item_id_create(15, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
    LABEL_252:
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 120);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6064:
            tig_art_item_id_create(13, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            scr.num = 28001;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28002;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6065:
            tig_art_item_id_create(14, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 3, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 3, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 53);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6066:
            tig_art_item_id_create(5, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6067:
            tig_art_item_id_create(3, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6068:
            tig_art_item_id_create(0, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 18);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6069:
            tig_art_item_id_create(4, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6070:
            tig_art_item_id_create(6, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29723);
            scr.num = 28003;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            obj_field_int32_set(obj, OBJ_F_NAME, 2027);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6071:
            tig_art_item_id_create(7, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29723);
            scr.num = 28004;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28005;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6072:
            tig_art_item_id_create(2, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 6073:
            tig_art_item_id_create(8, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 6074:
            tig_art_item_id_create(0, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 8);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 85);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 6075:
            tig_art_item_id_create(5, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 25);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 6076:
            tig_art_item_id_create(7, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 6077:
            tig_art_item_id_create(10, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 6078:
            tig_art_item_id_create(6, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29691);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 6079:
            tig_art_item_id_create(9, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 3, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 3, 10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29691);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 6080:
            tig_art_item_id_create(9, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 70);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_BONUS_TO_HIT, -10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6081:
            tig_art_item_id_create(11, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 70);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_BONUS_TO_HIT, -5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6082:
            tig_art_item_id_create(0, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 60);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_BONUS_TO_HIT, -10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6083:
            tig_art_item_id_create(8, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6084:
            tig_art_item_id_create(7, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 5080);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6085:
            tig_art_item_id_create(0, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6086:
            tig_art_item_id_create(1, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6087:
            tig_art_item_id_create(2, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            break;
        case 6088:
            tig_art_item_id_create(5, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6089:
            tig_art_item_id_create(3, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 18);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 280);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6090:
            tig_art_item_id_create(4, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6091:
            tig_art_item_id_create(5, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 18);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6092:
            tig_art_item_id_create(8, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 130);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6093:
            tig_art_item_id_create(6, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6094:
            tig_art_item_id_create(1, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6095:
            tig_art_item_id_create(2, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 1, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 1, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6096:
            tig_art_item_id_create(1, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 18);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6097:
            tig_art_item_id_create(4, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 3);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 60);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6098:
            tig_art_item_id_create(3, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 8);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 25);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 130);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6099:
            tig_art_item_id_create(2, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            scr.num = 28007;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6100:
            tig_art_item_id_create(5, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 130);
            scr.num = 28006;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29693);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6101:
            tig_art_item_id_create(5, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            break;
        case 6102:
            tig_art_item_id_create(3, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_NAME, 5678);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            break;
        case 6103:
            tig_art_item_id_create(0, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_NAME, 5678);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            break;
        case 6104:
            tig_art_item_id_create(8, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6105:
            tig_art_item_id_create(7, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 25);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_WEAPON_BONUS_TO_HIT, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6106:
            tig_art_item_id_create(9, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 110);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -52);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6107:
            tig_art_item_id_create(2, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6108:
            tig_art_item_id_create(6, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6109:
            tig_art_item_id_create(12, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_WEAPON_BONUS_TO_HIT, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6110:
            tig_art_item_id_create(3, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 147);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6111:
            tig_art_item_id_create(1, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6112:
            tig_art_item_id_create(2, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 62);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29692);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6113:
            tig_art_item_id_create(2, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 62);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_2, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_3, 54);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29692);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6114:
            tig_art_item_id_create(2, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 65);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_2, 42);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29692);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6115:
            tig_art_item_id_create(12, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6116:
            tig_art_item_id_create(13, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 70);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_BONUS_TO_HIT, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 18);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6117:
            tig_art_item_id_create(11, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 6118:
            tig_art_item_id_create(14, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 35);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 6119:
            tig_art_item_id_create(5, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 110);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_NAME, 2026);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6120:
            tig_art_item_id_create(1, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_EFFECT, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_MISS_CHANCE, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6121:
            tig_art_item_id_create(14, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 70);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6122:
            tig_art_item_id_create(12, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29688);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6123:
            tig_art_item_id_create(17, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(17, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(17, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29688);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6124:
            tig_art_item_id_create(3, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29688);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6125:
            tig_art_item_id_create(11, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 60);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29688);
            obj_field_int32_set(obj, OBJ_F_NAME, 2027);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6126:
            tig_art_item_id_create(2, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 2, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 2, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -45);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6127:
            tig_art_item_id_create(7, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 3, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 3, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6128:
            tig_art_item_id_create(14, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 3, 0, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 3, 0, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 9);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6129:
            tig_art_item_id_create(7, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 75);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6130:
            tig_art_item_id_create(0, 0, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 3, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 65);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 16);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 6131:
            tig_art_item_id_create(8, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 1, 40);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -25);
            break;
        case 6132:
            tig_art_item_id_create(10, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 2, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 2, 30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            break;
        case 6133:
            tig_art_item_id_create(9, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 3, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 3, 50);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -75);
            break;
        case 6134:
            tig_art_item_id_create(11, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 140);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29723);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6135:
            tig_art_item_id_create(12, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 130);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29723);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6136:
            tig_art_item_id_create(13, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 110);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 13);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 13);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29723);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6137:
            tig_art_item_id_create(14, 0, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 4, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 13);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29723);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 6138:
            tig_art_item_id_create(13, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 130);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 110);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29690);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 6139:
            tig_art_item_id_create(16, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(16, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(16, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 125);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 110);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 9);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29690);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 6140:
            tig_art_item_id_create(14, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 24);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29690);
            break;
        case 6141:
            tig_art_item_id_create(17, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(17, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(17, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 110);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 32);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29690);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 6142:
            tig_art_item_id_create(7, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6143:
            tig_art_item_id_create(12, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6144:
            tig_art_item_id_create(13, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6145:
            tig_art_item_id_create(11, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 16);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29689);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 6146:
            tig_art_item_id_create(6, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 3, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 3, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6147:
            tig_art_item_id_create(7, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 1, 30);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6148:
            tig_art_item_id_create(9, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29693);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6149:
            tig_art_item_id_create(8, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29693);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6150:
            tig_art_item_id_create(11, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29693);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6151:
            tig_art_item_id_create(10, 0, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 8, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 0);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 14);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29693);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 6152:
            tig_art_item_id_create(21, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(21, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(21, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 141);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6153:
            tig_art_item_id_create(16, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(16, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(16, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 269);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -48);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6154:
            tig_art_item_id_create(14, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 2);
            tig_art_scenery_id_create(8, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 11);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 2, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 2, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 44);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 500);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6155:
            tig_art_item_id_create(15, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 130);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 3);
            tig_art_scenery_id_create(8, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 3, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 3, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 45);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 800);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 203);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6156:
            tig_art_item_id_create(17, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(17, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(17, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 2, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -48);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6157:
            tig_art_item_id_create(19, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(19, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(19, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            tig_art_scenery_id_create(0, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 60);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -75);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6158:
            tig_art_item_id_create(18, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(18, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(18, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 3, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 3, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -95);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6159:
            tig_art_item_id_create(20, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(20, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(20, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 204);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 6160:
            tig_art_item_id_create(8, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29692);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6161:
            tig_art_item_id_create(9, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 60);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29692);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6162:
            tig_art_item_id_create(10, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 70);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 18);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29692);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6163:
            tig_art_item_id_create(11, 0, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 13, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 12);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4, 24);
            obj_field_int32_set(obj, OBJ_F_ITEM_MANA_STORE, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29692);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            break;
        case 6164:
            tig_art_item_id_create(0, 0, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 60);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x1Cu, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29694);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            break;
        case 6165:
            tig_art_item_id_create(1, 0, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x1Cu, 0, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 2);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29694);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            break;
        case 6166:
            tig_art_item_id_create(2, 0, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x1Cu, 0, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29694);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            break;
        case 6167:
            tig_art_item_id_create(3, 0, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x18, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 120);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29695);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            break;
        case 6168:
            tig_art_item_id_create(4, 0, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x18, 0, 0, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29695);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            break;
        case 6169:
            tig_art_item_id_create(5, 0, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x18, 0, 0, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 7);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 82);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29695);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            break;
        case 6170:
            tig_art_item_id_create(6, 0, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x14, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 16);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE, 50);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 82);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29695);
            break;
        case 6171:
            tig_art_item_id_create(7, 0, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 1, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            tig_art_scenery_id_create(0x10, 0, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 12);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 40);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29695);
            scr.num = 2486;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            break;
        case 6172:
            tig_art_item_id_create(10, 0, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 7, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 14);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 25);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            break;
        case 6173:
            tig_art_item_id_create(15, 0, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 6, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 2, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 2, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            break;
        case 6174:
            tig_art_item_id_create(22, 0, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(22, 1, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(22, 2, 0, 0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 110);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 5);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 15);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 25);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 7);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 13);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            break;
        case 6175:
            tig_art_item_id_create(12, 0, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 5, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 60);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 4);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 6176:
            tig_art_item_id_create(13, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 8);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6177:
            tig_art_item_id_create(15, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 1);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_NAME, 2027);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 6178:
            tig_art_item_id_create(18, 0, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(18, 1, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(18, 2, 0, 0, 2, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_WEAPON_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_TYPE, 10000);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MISSILE_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_VISUAL_EFFECT_AID, -1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_SPEED_FACTOR, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_RANGE, 1);
            obj_field_int32_set(obj, OBJ_F_WEAPON_MIN_STRENGTH, 3);
            obj_field_int32_set(obj, OBJ_F_WEAPON_AMMO_CONSUMPTION, 0);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_HIT_CHART, 2);
            obj_field_int32_set(obj, OBJ_F_WEAPON_CRIT_MISS_CHART, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_WEAPON_FLAGS, 26);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        }
        sub_49B2E0(obj);
        if (obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT) == 0) {
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 2 * (5 * (tig_art_item_id_subtype_get(obj_field_int32_get(obj, OBJ_F_ITEM_INV_AID)) - 2) + 2000));
        }
        break;
    case OBJ_TYPE_AMMO:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        switch (description) {
        case 7038:
            tig_art_item_id_create(0, 0, 0, 0, 0, 1, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 1, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_AMMO_TYPE, 0);
            obj_field_int32_set(obj, OBJ_F_AMMO_QUANTITY, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 7039:
            tig_art_item_id_create(0, 0, 0, 0, 1, 1, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 1, 1, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_AMMO_TYPE, 1);
            obj_field_int32_set(obj, OBJ_F_AMMO_QUANTITY, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 3);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        case 7040:
            tig_art_item_id_create(0, 0, 0, 0, 2, 1, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 2, 1, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_AMMO_TYPE, 2);
            obj_field_int32_set(obj, OBJ_F_AMMO_QUANTITY, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 3);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 7041:
            tig_art_item_id_create(0, 0, 0, 0, 3, 1, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 3, 1, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_AMMO_TYPE, 3);
            obj_field_int32_set(obj, OBJ_F_AMMO_QUANTITY, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 6);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        }
        break;
    case OBJ_TYPE_ITEM_ARMOR:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        switch (description) {
        case 8042:
        case 8058:
        case 8066:
            tig_art_item_id_create(9, 0, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 1, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8058:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(10, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(10, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(10, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8066:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(11, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(11, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(11, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8043:
        case 8059:
        case 8067:
            tig_art_item_id_create(0, 0, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 470);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8059:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 145);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 15);
                break;
            case 8067:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(2, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(2, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(2, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 155);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 17);
                break;
            }
            break;
        case 8044:
            tig_art_item_id_create(0, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 670);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 8045:
        case 8061:
            tig_art_item_id_create(0, 0, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1200);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -90);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 600);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, -5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, -10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, -20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2000);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            scr.num = 30303;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28005;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            switch (description) {
            case 8061:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 15);
                break;
            }
            break;
        case 8046:
        case 8062:
            tig_art_item_id_create(2, 0, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, -15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8062:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(3, 0, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(3, 1, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(3, 2, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8047:
        case 8063:
        case 8068:
            tig_art_item_id_create(13, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 600);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 3, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 4, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 950);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8063:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(14, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(14, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(14, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 800);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 15);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            case 8068:
                 obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(15, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(15, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(15, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 25);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29719);
                break;
            }
            break;
        case 8048:
        case 8064:
            tig_art_item_id_create(3, 0, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 7, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10000);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 19);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 31);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 60);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 500);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 10;
            scr.num = 30287;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30288;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            switch (description) {
            case 8064:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                break;
            }
            break;
        case 8049:
        case 8065:
        case 8069:
            tig_art_item_id_create(0, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8065:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 9);
                tig_art_item_id_create(1, 0, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8069:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 12);
                tig_art_item_id_create(2, 0, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(2, 1, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(2, 2, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            default:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 10);
                break;
            }
            break;
        case 8050:
            tig_art_item_id_create(0, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8051:
            tig_art_item_id_create(7, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 125);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8052:
            tig_art_item_id_create(2, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 90);
            obj_field_int32_set(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE, 3);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 2;
            scr.num = 28014;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28015;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8053:
            tig_art_item_id_create(5, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8054:
            tig_art_item_id_create(0, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8055:
            tig_art_item_id_create(0, 0, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8056:
            tig_art_item_id_create(3, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 2);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8057:
            tig_art_item_id_create(3, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            scr.hdr.counters &= 0xFF7602FF;
            scr.hdr.counters |= 0x760200;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8060:
            tig_art_item_id_create(1, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 450);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 21);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 670);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 8070:
        case 8078:
        case 8086:
            tig_art_item_id_create(0, 0, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 1, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 3);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 275);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8078:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8086:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(2, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(2, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(2, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8071:
        case 8079:
        case 8087:
            tig_art_item_id_create(3, 0, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 2, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 500);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            scr.num = 28008;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28009;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8079:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(4, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(4, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(4, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 145);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 14);
                obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 5);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            case 8087:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(4, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(4, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(4, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 160);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 18);
                obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -5);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29719);
                break;
            }
            break;
        case 8072:
        case 8080:
            tig_art_item_id_create(8, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, -350);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 670);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            switch (description) {
            case 8080:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(9, 0, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(9, 1, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(9, 2, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 450);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 21);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8073:
        case 8081:
        case 8291:
            tig_art_item_id_create(0, 0, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1200);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 25);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 45);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 3, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -90);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 600);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, -10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, -10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, -20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 900);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            scr.num = 30303;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28005;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            switch (description) {
            case 8081:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 38);
                break;
            case 8291:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(2, 0, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(2, 1, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(2, 2, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                tig_art_item_id_create(2, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1400);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 53);
                break;
            }
            break;
        case 8074:
        case 8082:
            tig_art_item_id_create(2, 0, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            scr.hdr.counters &= 0xFF8F05FF;
            scr.hdr.counters |= 0x8F0500;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8082:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(2, 0, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(2, 1, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(2, 2, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8075:
        case 8088:
            tig_art_item_id_create(0, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 6, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 36);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 870);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8083:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 30);
                break;
            case 8088:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(2, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(2, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(2, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1200);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 38);
                break;
            }
            break;
        case 8076:
        case 8084:
            tig_art_item_id_create(4, 0, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 7, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 11);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 60);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 70);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 0x0F;
            scr.num = 30287;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30288;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            switch (description) {
            case 8084:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                break;
            }
            break;
        case 8077:
        case 8085:
        case 8089:
            tig_art_item_id_create(10, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 8, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            scr.num = 30020;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30021;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8085:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 9);
                tig_art_item_id_create(11, 0, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(11, 1, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(11, 2, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8089:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 12);
                tig_art_item_id_create(12, 0, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(12, 1, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(12, 2, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            default:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 10);
                break;
            }
            break;
        case 8083:
            tig_art_item_id_create(1, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 6, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 250);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 800);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 8090:
        case 8098:
        case 8106:
            tig_art_item_id_create(3, 0, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 1, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 340);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8098:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(4, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(4, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(4, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8106:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(5, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(5, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(5, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8091:
        case 8099:
        case 8107:
            tig_art_item_id_create(0, 0, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 470);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8099:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 140);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 14);
                break;
            case 8107:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(2, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(2, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(2, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 160);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 17);
                break;
            }
            break;
        case 8092:
            tig_art_item_id_create(10, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 3, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 680);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 8093:
        case 8101:
            tig_art_item_id_create(0, 0, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1500);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8101:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 25);
                break;
            }
            break;
        case 8094:
        case 8102:
            tig_art_item_id_create(0, 0, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, -5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8102:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8095:
        case 8103:
        case 8108:
            tig_art_item_id_create(3, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 6, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 36);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 930);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8103:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(4, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(4, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(4, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 32);
                break;
            case 8108:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(5, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(5, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(5, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1100);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 38);
                break;
            }
            break;
        case 8096:
        case 8104:
            tig_art_item_id_create(1, 0, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 7, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 11);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 0x05;
            scr.num = 30287;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30288;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            switch (description) {
            case 8104:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                break;
            }
            break;
        case 8097:
        case 8105:
        case 8109:
            tig_art_item_id_create(13, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 8, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_NAME, 2805);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8105:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 9);
                tig_art_item_id_create(14, 0, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(14, 1, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(14, 2, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8109:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 12);
                tig_art_item_id_create(15, 0, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(15, 1, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(15, 2, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            default:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 10);
                break;
            }
            break;
        case 8100:
            tig_art_item_id_create(11, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 3, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 430);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 700);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 8110:
        case 8118:
        case 8126:
            tig_art_item_id_create(6, 0, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8118:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8126:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(8, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(8, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(8, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8111:
        case 8119:
        case 8127:
            tig_art_item_id_create(6, 0, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 2, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 18);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 540);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8119:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 480);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
                break;
            case 8127:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(8, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(8, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(8, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 520);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 20);
                break;
            }
            break;
        case 8112:
        case 8120:
            tig_art_item_id_create(0, 0, 0, 0, 3, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 3, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 3, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 3, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 670);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            switch (description) {
            case 8120:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 3, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 3, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 3, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 450);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 21);
                break;
            }
            break;
        case 8113:
        case 8121:
            tig_art_item_id_create(0, 0, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 4, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 37);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 900);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            switch (description) {
            case 8121:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 33);
                break;
            }
            break;
        case 8114:
        case 8122:
            tig_art_item_id_create(2, 0, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 3);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, -30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 8);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8122:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(3, 0, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(3, 1, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(3, 2, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8115:
        case 8123:
        case 8128:
            tig_art_item_id_create(17, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(17, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(17, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(17, 2, 0, 0, 6, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 800);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -35);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8123:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(18, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(18, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(18, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 600);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 35);
                break;
            case 8128:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(19, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(19, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(19, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 45);
                break;
            }
            break;
        case 8116:
        case 8124:
            tig_art_item_id_create(2, 0, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 7, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 11);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 0x05;
            scr.num = 30287;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30288;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 7);
            switch (description) {
            case 8124:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                break;
            }
            break;
        case 8117:
        case 8125:
        case 8129:
            tig_art_item_id_create(3, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 8, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8125:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 9);
                tig_art_item_id_create(8, 0, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(8, 1, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(8, 2, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8129:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 12);
                tig_art_item_id_create(9, 0, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(9, 1, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(9, 2, 0, 0, 8, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            default:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 10);
                break;
            }
            break;
        case 8130:
        case 8131:
        case 8292:
            tig_art_item_id_create(3, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 3, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 250);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 13);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 24);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            switch (description) {
            case 8131:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(4, 0, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(4, 1, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(4, 2, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
                break;
            case 8292:
                tig_art_item_id_create(3, 2, 0, 0, 6, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(5, 0, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(5, 1, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(5, 2, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 270);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 25);
                break;
            }
            break;
        case 8132:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 5, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 5, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8133:
        case 8134:
        case 8135:
            tig_art_item_id_create(5, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8134:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 17);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            case 8135:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 20);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            default:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 18);
                break;
            }
            break;
        case 8136:
        case 8137:
        case 8138:
            tig_art_item_id_create(7, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 8, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            scr.num = 30020;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30021;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8137:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 17);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            case 8138:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 20);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            default:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 18);
                break;
            }
            break;
        case 8139:
        case 8140:
        case 8141:
            tig_art_item_id_create(4, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 8, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8140:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 17);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            case 8141:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 20);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            default:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 18);
                break;
            }
            break;
        case 8142:
        case 8143:
        case 8144:
            tig_art_item_id_create(6, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 8, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8143:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 17);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            case 8144:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 20);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            default:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 18);
                break;
            }
            break;
        case 8145:
        case 8146:
        case 8147:
            tig_art_item_id_create(9, 0, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 1, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 140);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29701);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8146:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(10, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(10, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(10, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29718);
                break;
            case 8147:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(11, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(11, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(11, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29721);
                break;
            }
            break;
        case 8148:
        case 8149:
        case 8150:
            tig_art_item_id_create(15, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 250);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 168);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            switch (description) {
            case 8149:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(16, 0, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(16, 1, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(16, 2, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            case 8150:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(17, 0, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(17, 1, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(17, 2, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8151:
        case 8152:
        case 8153:
            tig_art_item_id_create(9, 0, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 2, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 470);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8152:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(10, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(10, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(10, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 140);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 10);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            case 8153:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(11, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(11, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(11, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 160);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 20);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29719);
                break;
            }
            break;
        case 8154:
            tig_art_item_id_create(18, 0, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(18, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(18, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(18, 2, 0, 0, 2, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 70);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            scr.num = 28010;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28011;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            break;
        case 8155:
            tig_art_item_id_create(14, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 400);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 13);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 25);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 735);
            break;
        case 8156:
            tig_art_item_id_create(13, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 650);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 27);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 800);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 8157:
            tig_art_item_id_create(2, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, -150);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 670);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 8158:
            tig_art_item_id_create(12, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 670);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 8159:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_NAME, 2807);
            scr.num = 1341;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_EXAMINE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8160:
            tig_art_item_id_create(3, 0, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29708);
            obj_field_int32_set(obj, OBJ_F_NAME, 2827);
            scr.hdr.counters &= 0xFF8602FF;
            scr.hdr.counters |= 0x860200;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8161:
            tig_art_item_id_create(2, 0, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29708);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8162:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8163:
            tig_art_item_id_create(2, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8164:
            tig_art_item_id_create(4, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29706);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8165:
            tig_art_item_id_create(5, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29706);
            scr.hdr.counters &= 0xFF8701FF;
            scr.hdr.counters |= 0x870100;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8166:
            tig_art_item_id_create(0, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8167:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 8);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 130);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8168:
            tig_art_item_id_create(8, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 35);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8169:
            tig_art_item_id_create(3, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 15);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8170:
            tig_art_item_id_create(9, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 15);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8171:
            tig_art_item_id_create(6, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 15);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 16);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8172:
            tig_art_item_id_create(6, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 35);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 16);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8173:
            tig_art_item_id_create(4, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29711);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8174:
            tig_art_item_id_create(7, 0, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 0, 2, 4, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 60);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29711);
            scr.hdr.counters &= 0xFF8805FF;
            scr.hdr.counters |= 0x880500;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 11);
            break;
        case 8175:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 145);
            obj_field_int32_set(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE, 7);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 0x02;
            scr.num = 28014;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28015;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8176:
            tig_art_item_id_create(0, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE, 15);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 0x04;
            scr.num = 28014;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28015;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8177:
            tig_art_item_id_create(6, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 8);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 260);
            obj_field_int32_set(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE, 10);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 0x05;
            scr.num = 28014;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28015;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8178:
            tig_art_item_id_create(4, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 183);
            obj_field_int32_set(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE, 2);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 0x01;
            scr.num = 28014;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28015;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8179:
            tig_art_item_id_create(5, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29705);
            scr.hdr.counters &= 0xFF8A01FF;
            scr.hdr.counters |= 0x8A0100;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8180:
            tig_art_item_id_create(7, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29705);
            scr.hdr.counters &= 0xFF8B02FF;
            scr.hdr.counters |= 0x8B0200;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8181:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8182:
            tig_art_item_id_create(3, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 275);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8183:
            tig_art_item_id_create(4, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            scr.hdr.counters &= 0xFF8C02FF;
            scr.hdr.counters |= 0x8C0200;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8184:
            tig_art_item_id_create(5, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            scr.num = 28017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 8185:
            tig_art_item_id_create(6, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case OBJ_F_KEY_KEY_ID:
            tig_art_item_id_create(19, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(19, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(19, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 130);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8187:
            tig_art_item_id_create(10, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 182);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 8188:
            tig_art_item_id_create(2, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 2048);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29702);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8189:
            tig_art_item_id_create(9, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29702);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8190:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 220);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8191:
            tig_art_item_id_create(2, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(2, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 18);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8192:
            tig_art_item_id_create(3, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 550);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8193:
            tig_art_item_id_create(4, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 360);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8194:
            tig_art_item_id_create(7, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_NAME, 2866);
            scr.num = 29016;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            break;
        case 8195:
            tig_art_item_id_create(5, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(5, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29712);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 87);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8196:
            tig_art_item_id_create(6, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29712);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8197:
            tig_art_item_id_create(16, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(16, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(16, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(16, 2, 0, 0, 6, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1300);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 500);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 17);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 40);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1000);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 8198:
            tig_art_item_id_create(9, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 6, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 800);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 850);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
            scr.hdr.counters &= 0xFF8E08FF;
            scr.hdr.counters |= 0x8E0800;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            break;
        case 8199:
            tig_art_item_id_create(9, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 18);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8200:
            tig_art_item_id_create(7, 0, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 8, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 8, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 1);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            scr.num = 30020;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30021;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 18);
            break;
        case 8201:
        case 8202:
            tig_art_item_id_create(12, 0, 0, 0, 1, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 1, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 3);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 275);
            obj_field_int32_set(obj, OBJ_F_NAME, 2849);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            switch (description) {
            case 8202:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(13, 0, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(13, 1, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(13, 2, 0, 0, 1, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8203:
            tig_art_item_id_create(0, 0, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29707);
            scr.hdr.counters &= 0xFFD501FF;
            scr.hdr.counters |= 0xD50100;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 64);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8204:
            tig_art_item_id_create(0, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29702);
            scr.hdr.counters &= 0xFFD804FF;
            scr.hdr.counters |= 0xD80400;
            scr.num = 30293;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30294;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 64);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8205:
            tig_art_item_id_create(3, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29705);
            scr.hdr.counters &= 0xFFD702FF;
            scr.hdr.counters |= 0xD70200;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 64);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8206:
            tig_art_item_id_create(21, 0, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(21, 1, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(21, 2, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29709);
            scr.hdr.counters &= 0xFFD601FF;
            scr.hdr.counters |= 0xD60100;
            scr.num = 28012;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28013;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 64);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8207:
            tig_art_item_id_create(12, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_NAME, 2806);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            break;
        case 8208:
        case 8209:
        case 8210:
            tig_art_item_id_create(12, 0, 0, 0, 2, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 2, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 2, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 170);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 470);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8209:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 160);
                tig_art_item_id_create(13, 0, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(13, 1, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(13, 2, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 14);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            case 8210:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 180);
                tig_art_item_id_create(14, 0, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(14, 1, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(14, 2, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 18);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29719);
                break;
            }
            break;
        case 8211:
        case 8212:
        case 8213:
            tig_art_item_id_create(12, 0, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 2, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 160);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 470);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8212:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
                tig_art_item_id_create(13, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(13, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(13, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 14);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            case 8213:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 180);
                tig_art_item_id_create(14, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(14, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(14, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 18);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29719);
                break;
            }
            break;
        case 8214:
        case 8215:
        case 8216:
            tig_art_item_id_create(15, 0, 0, 0, 2, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 2, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 2, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 470);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8215:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 160);
                tig_art_item_id_create(16, 0, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(16, 1, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(16, 2, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 14);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            case 8216:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 180);
                tig_art_item_id_create(17, 0, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(17, 1, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(17, 2, 0, 0, 2, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 18);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29719);
                break;
            }
            break;
        case 8217:
        case 8218:
        case 8219:
            tig_art_item_id_create(15, 0, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 2, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 2, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 170);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 9);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 470);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            switch (description) {
            case 8218:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 160);
                tig_art_item_id_create(16, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(16, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(16, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 14);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            case 8219:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 180);
                tig_art_item_id_create(17, 0, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(17, 1, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(17, 2, 0, 0, 2, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 18);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29719);
                break;
            }
            break;
        case 8220:
            tig_art_item_id_create(0, 0, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 14);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, -150);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 670);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            break;
        case 8221:
        case 8222:
            tig_art_item_id_create(6, 0, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 520);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 870);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            switch (description) {
            case 8222:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 3, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 10);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8223:
        case 8224:
            tig_art_item_id_create(6, 0, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 3, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 3, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 510);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 870);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            switch (description) {
            case 8224:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 3, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 3, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 3, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 490);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 10);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8225:
        case 8226:
            tig_art_item_id_create(6, 0, 0, 0, 3, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 3, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 3, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 3, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 870);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            switch (description) {
            case 8226:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 3, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 3, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 3, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 490);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 10);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8227:
        case 8228:
            tig_art_item_id_create(6, 0, 0, 0, 3, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 3, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 3, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 3, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 23);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -25);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 870);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            switch (description) {
            case 8228:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 3, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 3, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 3, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 490);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 21);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8229:
        case 8230:
        case 8231:
            tig_art_item_id_create(3, 0, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(3, 1, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(3, 2, 0, 0, 4, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 18);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 45);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, 65);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_WEIGHT_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, -10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, -15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, -10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, -75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 900);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 6);
            switch (description) {
            case 8230:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(4, 0, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(4, 1, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(4, 2, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 40);
                break;
            case 8231:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 4);
                tig_art_item_id_create(5, 0, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(5, 1, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(5, 2, 0, 0, 4, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1400);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 47);
                tig_art_item_id_create(2, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
                break;
            }
            break;
        case 8232:
        case 8233:
            tig_art_item_id_create(4, 0, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8233:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(5, 0, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(5, 1, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(5, 2, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8234:
        case 8235:
            tig_art_item_id_create(4, 0, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8235:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(5, 0, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(5, 1, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(5, 2, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8236:
        case 8237:
            tig_art_item_id_create(4, 0, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 35);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 35);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 35);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 35);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8237:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(5, 0, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(5, 1, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(5, 2, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8238:
        case 8239:
            tig_art_item_id_create(4, 0, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(4, 2, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8239:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(5, 0, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(5, 1, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(5, 2, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8240:
        case 8241:
            tig_art_item_id_create(6, 0, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 2, 0, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8241:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 5, 2, 0, 2, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8242:
        case 8243:
            tig_art_item_id_create(6, 0, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8243:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 5, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8244:
        case 8245:
            tig_art_item_id_create(6, 0, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8245:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 5, 2, 0, 3, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8246:
        case 8247:
            tig_art_item_id_create(6, 0, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29715);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8247:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29722);
                break;
            }
            break;
        case 8248:
        case 8249:
            tig_art_item_id_create(0, 0, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(0, 2, 0, 0, 5, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 7);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 3);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            switch (description) {
            case 8249:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(1, 0, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(1, 1, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(1, 2, 0, 0, 5, 2, 0, 1, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                break;
            }
            break;
        case 8250:
        case 8251:
            tig_art_item_id_create(6, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(6, 2, 0, 0, 6, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 36);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1500);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8251:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(7, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(7, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(7, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 32);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8252:
        case 8253:
            tig_art_item_id_create(8, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 6, 2, 0, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 36);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1500);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8253:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(9, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(9, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(9, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 32);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8254:
        case 8255:
            tig_art_item_id_create(10, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 6, 2, 0, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 36);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1500);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8255:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(11, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(11, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(11, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 32);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8256:
        case 8257:
            tig_art_item_id_create(13, 0, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 6, 2, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 400);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 36);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2, -20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, -80);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 15);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ, 15);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1500);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29696);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 5);
            switch (description) {
            case 8257:
                obj_field_int32_set(obj, OBJ_F_ARMOR_FLAGS, 1);
                tig_art_item_id_create(14, 0, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                tig_art_item_id_create(14, 1, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
                tig_art_item_id_create(14, 2, 0, 0, 6, 2, 0, 0, &art_id);
                obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
                obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 900);
                obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 32);
                obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29716);
                break;
            }
            break;
        case 8258:
            tig_art_item_id_create(8, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 30);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 4, 30);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29712);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8259:
            tig_art_item_id_create(9, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29712);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8260:
            tig_art_item_id_create(10, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29712);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8261:
            tig_art_item_id_create(11, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 85);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 12);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29712);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8262:
            tig_art_item_id_create(12, 0, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(12, 2, 0, 0, 0, 2, 1, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 85);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 180);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1, 2);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 16);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 16);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29712);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 13);
            break;
        case 8263:
            tig_art_item_id_create(14, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 125);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 1024);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29702);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            break;
        case 8264:
            tig_art_item_id_create(15, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(15, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29702);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8265:
            tig_art_item_id_create(16, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(16, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(16, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29702);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8266:
            tig_art_item_id_create(17, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(17, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(17, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 8);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29702);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8267:
            tig_art_item_id_create(18, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(18, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(18, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 12);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29702);
            scr.num = 30291;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30292;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        case 8268:
            tig_art_item_id_create(10, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 2);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 2);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29705);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8269:
            tig_art_item_id_create(10, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 2, 3, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 0, 2, 3, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 3);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 3);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29705);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8270:
            tig_art_item_id_create(10, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 2, 3, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 0, 2, 3, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 4);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29705);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8271:
            tig_art_item_id_create(10, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 2, 3, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 0, 2, 3, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 0);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_field_int32_set(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29705);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 12);
            break;
        case 8272:
            tig_art_item_id_create(8, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 2, 5, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 0, 2, 5, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 5);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29706);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8273:
            tig_art_item_id_create(8, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 10);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29706);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8274:
            tig_art_item_id_create(9, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 75);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 15);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 15);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29706);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8275:
            tig_art_item_id_create(10, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(10, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3, 20);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29706);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8276:
            tig_art_item_id_create(9, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(9, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 4);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE, 2);
            break;
        case 8277:
            tig_art_item_id_create(8, 0, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(8, 2, 0, 0, 0, 2, 3, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 15);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE, 15);
            scr.hdr.counters &= ~0xFF;
            scr.hdr.counters |= 0x02;
            scr.num = 28014;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 28015;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            break;
        case 8278:
            tig_art_item_id_create(14, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(14, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_NAME, 2857);
            break;
        case 8279:
            tig_art_item_id_create(7, 0, 0, 0, 0, 2, 5, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 2, 5, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 0, 2, 5, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8280:
            tig_art_item_id_create(7, 0, 0, 0, 0, 2, 5, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 2, 5, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 0, 2, 5, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_NAME, 5684);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8281:
            tig_art_item_id_create(7, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(7, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 350);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8282:
            tig_art_item_id_create(13, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8283:
            tig_art_item_id_create(11, 0, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(11, 2, 0, 0, 0, 2, 5, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8284:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 5, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 5, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 5, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8285:
            tig_art_item_id_create(1, 0, 0, 0, 0, 2, 5, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 2, 5, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(1, 2, 0, 0, 0, 2, 5, 3, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 9);
            break;
        case 8286:
            tig_art_item_id_create(24, 0, 0, 0, 0, 2, 6, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(24, 1, 0, 0, 0, 2, 6, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(24, 2, 0, 0, 0, 2, 6, 1, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 600);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8287:
            tig_art_item_id_create(24, 0, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(24, 1, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(24, 2, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 900);
            obj_field_int32_set(obj, OBJ_F_NAME, 5684);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8288:
            tig_art_item_id_create(24, 0, 0, 0, 0, 2, 6, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(24, 1, 0, 0, 0, 2, 6, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(24, 2, 0, 0, 0, 2, 6, 2, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8289:
            tig_art_item_id_create(25, 0, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(25, 1, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(25, 2, 0, 0, 0, 2, 6, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1500);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 10);
            break;
        case 8290:
            tig_art_item_id_create(13, 0, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            tig_art_item_id_create(13, 2, 0, 0, 0, 2, 2, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ARMOR_PAPER_DOLL_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_USE_AID_FRAGMENT, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 120);
            obj_field_int32_set(obj, OBJ_F_ARMOR_AC_ADJ, 6);
            obj_arrayfield_int32_set(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ, 0);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 120);
            scr.hdr.counters &= 0xFFE703FF;
            scr.hdr.counters |= 0xE70300;
            scr.num = 30289;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_ON, &scr);
            scr.num = 30290;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_WIELD_OFF, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 8);
            break;
        }
        sub_49B2E0(obj);
        break;
    case OBJ_TYPE_ITEM_GOLD:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        tig_art_item_id_create(0, 0, 0, 0, 0, 3, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        tig_art_item_id_create(0, 1, 0, 0, 0, 3, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
        obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
        break;
    case OBJ_TYPE_ITEM_FOOD:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 0x180);
        switch (description) {
        case 10057:
            tig_art_item_id_create(88, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(88, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10058:
            tig_art_item_id_create(51, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(51, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10059:
            tig_art_item_id_create(11, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(11, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -65);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 35);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10060:
            tig_art_item_id_create(9, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 151);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10061:
            tig_art_item_id_create(12, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(12, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10062:
            tig_art_item_id_create(13, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10063:
            tig_art_item_id_create(5, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10064:
            tig_art_item_id_create(17, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(17, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10065:
            tig_art_item_id_create(4, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 164);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10066:
            tig_art_item_id_create(25, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(25, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10067:
            tig_art_item_id_create(32, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(32, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 173);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10068:
            tig_art_item_id_create(27, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(27, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 170);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10069:
            tig_art_item_id_create(42, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(42, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -56);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 157);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10070:
            tig_art_item_id_create(49, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(49, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -78);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 158);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10071:
            tig_art_item_id_create(48, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(48, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10072:
            tig_art_item_id_create(36, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(36, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10073:
            tig_art_item_id_create(43, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(43, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10074:
            tig_art_item_id_create(79, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(79, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            scr.num = 30282;
            scr.hdr.counters = 0x32002314;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x300000);
            obj_field_int32_set(obj, OBJ_F_ITEM_AI_ACTION, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10075:
            tig_art_item_id_create(29, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(29, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -75);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 171);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10076:
            tig_art_item_id_create(6, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 156);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10077:
            tig_art_item_id_create(7, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 152);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10078:
            tig_art_item_id_create(8, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 153);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10079:
            tig_art_item_id_create(10, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 45);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 154);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10080:
            tig_art_item_id_create(14, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10081:
            tig_art_item_id_create(15, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 3);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10082:
            tig_art_item_id_create(16, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(16, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10083:
            tig_art_item_id_create(18, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(18, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 169);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10084:
            tig_art_item_id_create(19, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(19, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 155);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10085:
            tig_art_item_id_create(20, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(20, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 166);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10086:
            tig_art_item_id_create(21, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(21, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 183);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10087:
            tig_art_item_id_create(22, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(22, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 167);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10088:
            tig_art_item_id_create(23, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(23, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 7);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10089:
            tig_art_item_id_create(24, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(24, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -45);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10090:
            tig_art_item_id_create(26, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(26, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 165);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 384);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10091:
            tig_art_item_id_create(28, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(28, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10092:
            tig_art_item_id_create(30, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(30, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10093:
            tig_art_item_id_create(31, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(31, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 172);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_NAME, 3608);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10094:
            tig_art_item_id_create(33, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(33, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10095:
            tig_art_item_id_create(34, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(34, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10096:
            tig_art_item_id_create(35, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(35, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10097:
            tig_art_item_id_create(37, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(37, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 163);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10098:
            tig_art_item_id_create(38, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(38, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 161);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10099:
            tig_art_item_id_create(39, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(39, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 160);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10100:
            tig_art_item_id_create(40, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(40, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -92);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 90);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 162);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10101:
            tig_art_item_id_create(41, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(41, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -32);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10102:
            tig_art_item_id_create(44, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(44, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10103:
            tig_art_item_id_create(45, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(45, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 159);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10104:
            tig_art_item_id_create(46, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(46, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10105:
            tig_art_item_id_create(47, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(47, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10106:
            tig_art_item_id_create(50, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(50, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10107:
            tig_art_item_id_create(52, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(52, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -46);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10108:
            tig_art_item_id_create(53, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(53, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10109:
            tig_art_item_id_create(54, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(54, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -35);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10110:
            tig_art_item_id_create(55, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(55, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10111:
            tig_art_item_id_create(64, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(64, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -93);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 142);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10112:
            tig_art_item_id_create(65, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(65, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -83);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 141);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10113:
            tig_art_item_id_create(66, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(66, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -83);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 144);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10114:
            tig_art_item_id_create(56, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(56, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 189);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10115:
            tig_art_item_id_create(57, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(57, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -95);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 190);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10116:
            tig_art_item_id_create(58, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(58, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -95);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 192);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10117:
            tig_art_item_id_create(59, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(59, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -74);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 193);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10118:
            tig_art_item_id_create(60, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(60, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -64);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 194);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10119:
            tig_art_item_id_create(61, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(61, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 195);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10120:
            tig_art_item_id_create(62, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(62, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -75);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 196);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            break;
        case 10121:
            tig_art_item_id_create(63, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(63, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 197);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10122:
            tig_art_item_id_create(68, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(68, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10123:
            tig_art_item_id_create(69, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(69, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10124:
            tig_art_item_id_create(70, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(70, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_NAME, 3619);
            scr.num = 2545;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x100000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10125:
            tig_art_item_id_create(67, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(67, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_NAME, 3609);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10126:
            tig_art_item_id_create(71, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(71, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_NAME, 3613);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10127:
            tig_art_item_id_create(72, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(72, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_NAME, 3614);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10128:
            tig_art_item_id_create(73, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(73, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_NAME, 3615);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10129:
            tig_art_item_id_create(74, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(74, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 4);
            obj_field_int32_set(obj, OBJ_F_NAME, 3616);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10130:
            tig_art_item_id_create(75, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(75, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 25);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 3);
            obj_field_int32_set(obj, OBJ_F_NAME, 3617);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10131:
            tig_art_item_id_create(76, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(76, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 3);
            obj_field_int32_set(obj, OBJ_F_NAME, 3618);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 10132:
            tig_art_item_id_create(77, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(77, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 161);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            break;
        case 10133:
            tig_art_item_id_create(78, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(78, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -99);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 206);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            break;
        case 10134:
            tig_art_item_id_create(80, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(80, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            scr.num = 30282;
            scr.hdr.counters = 0x50002550;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x300000);
            obj_field_int32_set(obj, OBJ_F_ITEM_AI_ACTION, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10135:
            tig_art_item_id_create(81, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(81, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            scr.num = 30282;
            scr.hdr.counters = 0x3201271E;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x300000);
            obj_field_int32_set(obj, OBJ_F_ITEM_AI_ACTION, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10136:
            tig_art_item_id_create(82, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(82, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            scr.num = 30282;
            scr.hdr.counters = 0x50012946;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x300000);
            obj_field_int32_set(obj, OBJ_F_ITEM_AI_ACTION, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10137:
            tig_art_item_id_create(83, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(83, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            scr.num = 30282;
            scr.hdr.counters = 0x32022B78;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x300000);
            obj_field_int32_set(obj, OBJ_F_ITEM_AI_ACTION, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10138:
            tig_art_item_id_create(84, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(84, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            scr.num = 30282;
            scr.hdr.counters = 0x50032D78;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x300000);
            obj_field_int32_set(obj, OBJ_F_ITEM_AI_ACTION, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10139:
            tig_art_item_id_create(86, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(86, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 210);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10140:
            tig_art_item_id_create(2, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 211);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10141:
            tig_art_item_id_create(1, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(1, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 212);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10142:
            tig_art_item_id_create(87, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(87, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 213);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10143:
            tig_art_item_id_create(85, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(85, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 214);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            break;
        case 10144:
            tig_art_item_id_create(57, 0, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(57, 1, 0, 0, 0, 4, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -95);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 190);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        }
        break;
    case OBJ_TYPE_ITEM_SCROLL:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 384);
        tig_art_item_id_create(0, 0, 0, 0, 0, 5, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        tig_art_item_id_create(0, 1, 0, 0, 0, 5, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
        obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, 1);
        obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
        obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
        if (description < 11139) {
            int spl = description - 11059;
            int complexity = spl % 5;
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, description);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 4 * (5 * complexity + 5));
            if (complexity != 0) {
                obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750 * complexity);
            } else {
                obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            }
        } else {
            switch (description) {
            case 11139:
                obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 207);
                obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
                obj_field_int32_set(obj, OBJ_F_NAME, 4004);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            case 11140:
                obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 208);
                obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 100);
                obj_field_int32_set(obj, OBJ_F_NAME, 4005);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            case 11141:
                obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 218);
                obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 30);
                obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
                break;
            case 11142:
                obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 191);
                obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
                obj_field_int32_set(obj, OBJ_F_NAME, 4006);
                obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
                break;
            }
        }
        obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
        break;
    case OBJ_TYPE_ITEM_KEY:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
        obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
        obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
        obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
        switch (description) {
        case 12060:
            tig_art_item_id_create(0, 0, 0, 0, 0, 6, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 6, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 12061:
            tig_art_item_id_create(2, 0, 0, 0, 0, 6, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 6, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_KEY_KEY_ID, 62);
            break;
        case 12062:
            tig_art_item_id_create(0, 0, 0, 0, 0, 6, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 6, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_NAME, 4424);
            obj_field_int32_set(obj, OBJ_F_KEY_KEY_ID, 0);
            break;
        }
        break;
    case OBJ_TYPE_ITEM_KEY_RING:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        tig_art_item_id_create(1, 0, 0, 0, 0, 7, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        tig_art_item_id_create(1, 1, 0, 0, 0, 7, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
        obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
        obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
        obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
        obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
        obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
        break;
    case OBJ_TYPE_ITEM_WRITTEN:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 128);
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
        switch (description) {
        case 14062:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            tig_art_item_id_create(2, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14063:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 3);
            tig_art_item_id_create(8, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14064:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            tig_art_item_id_create(6, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14065:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 4);
            tig_art_item_id_create(9, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14066:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 1);
            obj_field_int32_set(obj, OBJ_F_NAME, 5212);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 1030);
            scr.num = 1146;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            scr.num = 1145;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_EXAMINE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            tig_art_item_id_create(8, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14067:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14068:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            tig_art_item_id_create(8, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14069:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            scr.num = 1832;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            tig_art_item_id_create(2, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14070:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 0);
            obj_field_int32_set(obj, OBJ_F_NAME, 5229);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 1020);
            scr.num = 1891;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            tig_art_item_id_create(2, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14071:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 3);
            obj_field_int32_set(obj, OBJ_F_NAME, 5246);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 1050);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            tig_art_item_id_create(9, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14072:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            tig_art_item_id_create(6, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14073:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14074:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4010);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14075:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4020);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14076:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4030);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14077:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14078:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4210);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14079:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4220);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14080:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4230);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14081:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4240);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14082:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4400);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14083:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4410);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14084:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4420);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14085:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4430);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14086:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4440);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14087:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4450);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14088:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4600);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14089:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4610);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14090:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4620);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14091:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4630);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14092:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4640);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14093:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4650);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14094:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4660);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14095:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4670);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14096:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4680);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14097:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4690);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14098:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4700);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14099:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4710);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14100:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4800);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14101:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4820);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14102:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4830);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14103:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4840);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14104:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4850);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14105:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4860);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14106:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4870);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14107:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4880);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14108:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 4890);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14109:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14110:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5010);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14111:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5020);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14112:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5030);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14113:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5040);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14114:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5050);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14115:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5060);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14116:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5070);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14117:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5080);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14118:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5090);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14119:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5200);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14120:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5210);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14121:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5220);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14122:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5230);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14123:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5240);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14124:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5250);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14125:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5260);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14126:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5400);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14127:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5410);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14128:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5420);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        case 14129:
            obj_field_int32_set(obj, OBJ_F_WRITTEN_SUBTYPE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_WRITTEN_TEXT_START_LINE, 5440);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            tig_art_item_id_create(7, 0, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 8, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            break;
        }
        break;
    case OBJ_TYPE_ITEM_GENERIC:
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, description);
        obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS, description);
        tig_art_item_id_create(14, 0, 0, 0, 0, 9, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        tig_art_item_id_create(14, 1, 0, 0, 0, 9, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
        obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
        switch (description) {
        case 15066:
            tig_art_item_id_create(14, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            scr.num = 1022;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_MISS, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 4000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15067:
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 384);
            tig_art_item_id_create(69, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(69, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_NAME, 5609);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15068:
            tig_art_item_id_create(70, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(70, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_NAME, 5610);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15069:
            tig_art_item_id_create(28, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(28, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            break;
        case 15070:
            tig_art_item_id_create(31, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(31, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 4096);
            break;
        case 15071:
            tig_art_item_id_create(32, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(32, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            break;
        case 15072:
            tig_art_item_id_create(52, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(52, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 2048);
            break;
        case 15073:
            tig_art_item_id_create(29, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(29, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            break;
        case 15074:
            tig_art_item_id_create(0, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            break;
        case 15075:
            tig_art_item_id_create(93, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(93, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            break;
        case 15076:
            tig_art_item_id_create(43, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(43, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            break;
        case 15077:
            tig_art_item_id_create(54, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(54, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            break;
        case 15078:
            tig_art_item_id_create(45, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(45, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            break;
        case 15079:
            tig_art_item_id_create(46, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(46, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            break;
        case 15080:
            tig_art_item_id_create(15, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(15, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            scr.num = 30001;
            scr.hdr.counters &= 0xFFFF190F;
            scr.hdr.counters |= 0x190F;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 1048960);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 4);
            break;
        case 15081:
            tig_art_item_id_create(192, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(192, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -35);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_GENERIC_USAGE_BONUS, 10);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x2);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x180);
            break;
        case 15082:
            tig_art_item_id_create(9, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(9, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15083:
            tig_art_item_id_create(13, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(13, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            break;
        case 15084:
            tig_art_item_id_create(10, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_GENERIC_USAGE_BONUS, 5);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x2);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x180);
            break;
        case 15085:
            tig_art_item_id_create(63, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(63, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_NAME, 5622);
            break;
        case 15086:
            tig_art_item_id_create(61, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(61, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 12);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15087:
            tig_art_item_id_create(65, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(65, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15088:
            tig_art_item_id_create(59, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(59, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            break;
        case 15089:
            tig_art_item_id_create(66, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(66, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 1);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            break;
        case 15090:
            tig_art_item_id_create(7, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(7, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            break;
        case 15091:
            tig_art_item_id_create(8, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(8, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 250);
            break;
        case 15092:
            tig_art_item_id_create(14, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            scr.num = 1343;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_MISS, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 4000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15093:
            tig_art_item_id_create(92, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(92, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            break;
        case 15094:
            tig_art_item_id_create(58, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(58, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_NAME, 5638);
            break;
        case 15095:
            tig_art_item_id_create(89, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(89, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            scr.num = 1436;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_EXAMINE, &scr);
            obj_field_int32_set(obj, OBJ_F_NAME, 5639);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            break;
        case 15096:
            tig_art_item_id_create(87, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(87, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            scr.num = 1435;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_EXAMINE, &scr);
            obj_field_int32_set(obj, OBJ_F_NAME, 5641);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            break;
        case 15097:
            tig_art_item_id_create(0, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15098:
            tig_art_item_id_create(2, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15099:
            tig_art_item_id_create(4, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(4, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 6000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 6000);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 3000);
            break;
        case 15100:
            tig_art_item_id_create(5, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(5, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            scr.num = 30001;
            scr.hdr.counters &= 0xFF0000;
            scr.hdr.counters |= 0x8C000A05;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 1048960);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 4);
            break;
        case 15101:
            tig_art_item_id_create(6, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(6, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -65);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            break;
        case 15102:
            tig_art_item_id_create(19, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(19, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 120);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 184);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 384);
            break;
        case 15103:
            tig_art_item_id_create(20, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(20, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -35);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 3);
            break;
        case 15104:
            tig_art_item_id_create(21, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(21, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15105:
            tig_art_item_id_create(22, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(22, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15106:
            tig_art_item_id_create(23, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(23, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 15);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15107:
            tig_art_item_id_create(27, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(27, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 130);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            break;
        case 15108:
            tig_art_item_id_create(30, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(30, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            break;
        case 15109:
            tig_art_item_id_create(35, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(35, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15110:
            tig_art_item_id_create(38, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(38, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            break;
        case 15111:
            tig_art_item_id_create(41, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(41, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -55);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            break;
        case 15112:
            tig_art_item_id_create(42, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(42, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 180);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            break;
        case 15113:
            tig_art_item_id_create(44, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(44, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            break;
        case 15114:
            tig_art_item_id_create(47, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(47, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 80);
            break;
        case 15115:
            tig_art_item_id_create(48, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(48, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15116:
            tig_art_item_id_create(49, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(49, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            scr.num = 29017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_MISS, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x20);
            break;
        case 15117:
            tig_art_item_id_create(50, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(50, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15118:
            tig_art_item_id_create(112, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(112, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            break;
        case 15119:
            tig_art_item_id_create(53, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(53, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            break;
        case 15120:
            tig_art_item_id_create(55, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(55, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            break;
        case 15121:
            tig_art_item_id_create(56, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(56, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15122:
            tig_art_item_id_create(57, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(57, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -25);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            break;
        case 15123:
            tig_art_item_id_create(60, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(60, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15124:
            tig_art_item_id_create(62, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(62, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15125:
            tig_art_item_id_create(64, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(64, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 300);
            break;
        case 15126:
            tig_art_item_id_create(73, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(73, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_NAME, 5614);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 1048960);
            scr.num = 2942;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15127:
            tig_art_item_id_create(82, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(82, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 150);
            obj_field_int32_set(obj, OBJ_F_NAME, 5628);
            scr.num = 2947;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            break;
        case 15128:
            tig_art_item_id_create(83, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(83, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15129:
            tig_art_item_id_create(84, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(84, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15130:
            tig_art_item_id_create(85, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(85, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15131:
            tig_art_item_id_create(86, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(86, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 65);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_NAME, 5684);
            obj_field_int32_set(obj, OBJ_F_NAME, 5684);
            break;
        case 15132:
            tig_art_item_id_create(119, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(119, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -60);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 175);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15133:
            tig_art_item_id_create(96, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(96, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            scr.num = 30008;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_MISS, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 4000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2000);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15134:
            tig_art_item_id_create(25, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(25, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_2, 176);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 384);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 4);
            break;
        case 15135:
            tig_art_item_id_create(98, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(98, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -35);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15136:
            tig_art_item_id_create(99, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(99, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -45);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15137:
            tig_art_item_id_create(55, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(55, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15138:
            tig_art_item_id_create(101, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(101, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            break;
        case 15139:
            tig_art_item_id_create(103, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(103, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            break;
        case 15140:
            tig_art_item_id_create(104, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(104, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -53);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15141:
            tig_art_item_id_create(106, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(106, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15142:
            tig_art_item_id_create(108, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(108, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15143:
            tig_art_item_id_create(109, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(109, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15144:
            tig_art_item_id_create(111, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(111, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15145:
            tig_art_item_id_create(114, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(114, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -23);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15146:
            tig_art_item_id_create(115, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(115, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -38);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15147:
            tig_art_item_id_create(116, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(116, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15148:
            tig_art_item_id_create(117, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(117, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15149:
            tig_art_item_id_create(113, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(113, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15150:
            tig_art_item_id_create(118, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(118, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 30);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 177);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15151:
            tig_art_item_id_create(120, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(120, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 35);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 178);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15152:
            tig_art_item_id_create(121, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(121, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 60);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 179);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15153:
            tig_art_item_id_create(122, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(122, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -45);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 80);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 180);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15154:
            tig_art_item_id_create(117, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(117, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -15);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15155:
            tig_art_item_id_create(64, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(64, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 300);
            break;
        case 15156:
            tig_art_item_id_create(0, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(0, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 384);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15157:
            tig_art_item_id_create(128, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(128, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15158:
            tig_art_item_id_create(129, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(129, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15159:
            tig_art_item_id_create(2, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(2, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_DESCRIPTION_UNKNOWN, 29714);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 2097600);
            scr.num = 28019;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_AI_ACTION, 3);
            break;
        case 15160:
            tig_art_item_id_create(130, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(130, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_NAME, 5693);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15161:
            tig_art_item_id_create(136, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(136, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 198);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15162:
            tig_art_item_id_create(137, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(137, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -37);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 70);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 148);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15163:
            tig_art_item_id_create(138, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(138, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 199);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15164:
            tig_art_item_id_create(140, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(140, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -40);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 201);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15165:
            tig_art_item_id_create(141, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(141, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -45);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 40);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 202);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15166:
            tig_art_item_id_create(142, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(142, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -35);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 30);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 35);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 16);
            break;
        case 15167:
            tig_art_item_id_create(147, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(147, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -22);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            break;
        case 15168:
            tig_art_item_id_create(135, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(135, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            break;
        case 15169:
            tig_art_item_id_create(49, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(49, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            break;
        case 15170:
        case 15171:
            tig_art_item_id_create(49, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(49, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 300);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15172:
            tig_art_item_id_create(96, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(96, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 4500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 350);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2200);
            scr.num = 30008;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_MISS, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15173:
            tig_art_item_id_create(139, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(139, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -87);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_2, 220);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 384);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, 4);
            break;
        case 15174:
            tig_art_item_id_create(143, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(143, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 75);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_NAME, 5699);
            break;
        case 15175:
            tig_art_item_id_create(144, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(144, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 40);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 128);
            scr.num = 29019;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15176:
            tig_art_item_id_create(14, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 4200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1100);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15177:
            tig_art_item_id_create(148, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(148, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 150);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 500);
            break;
        case 15178:
            tig_art_item_id_create(10, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(10, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_GENERIC_USAGE_BONUS, 0);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x2);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x180);
            break;
        case 15179:
            tig_art_item_id_create(176, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(176, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_GENERIC_USAGE_BONUS, 0);
            obj_field_int32_set(obj, OBJ_F_GENERIC_USAGE_COUNT_REMAINING, 10);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x8);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x180);
            break;
        case 15180:
            tig_art_item_id_create(156, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(156, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_NAME, 6698);
            scr.num = 2631;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            break;
        case 15181:
            tig_art_item_id_create(158, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(158, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_NAME, 5682);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            break;
        case 15182:
            tig_art_item_id_create(151, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(151, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 500);
            obj_field_int32_set(obj, OBJ_F_NAME, 5683);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15183:
            tig_art_item_id_create(157, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(157, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_NAME, 5691);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15184:
            tig_art_item_id_create(152, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(152, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 2);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_NAME, 5685);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 8);
            break;
        case 15185:
            tig_art_item_id_create(153, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(153, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 8);
            obj_field_int32_set(obj, OBJ_F_NAME, 5686);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            break;
        case OBJ_F_KEY_KEY_ID:
            tig_art_item_id_create(150, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(150, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 25);
            obj_field_int32_set(obj, OBJ_F_NAME, 5688);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 20);
            break;
        case 15187:
            tig_art_item_id_create(149, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(149, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 0);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 500);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 20);
            obj_field_int32_set(obj, OBJ_F_NAME, 5687);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 3);
            break;
        case 15188:
            tig_art_item_id_create(159, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(159, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            scr.hdr.counters &= 0xFF;
            scr.hdr.counters |= 0x401A900;
            scr.num = 30017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            scr.num = 30018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_REMOVE_ITEM, &scr);
            scr.num = 2914;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15189:
            tig_art_item_id_create(160, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(160, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            scr.hdr.counters &= 0xFF;
            scr.hdr.counters |= 0x401AB00;
            scr.num = 30017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            scr.num = 30018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_REMOVE_ITEM, &scr);
            scr.num = 2916;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15190:
            tig_art_item_id_create(162, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(162, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 0);
            scr.hdr.counters &= 0xFF;
            scr.hdr.counters |= 0x401A800;
            scr.num = 30017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            scr.num = 30018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_REMOVE_ITEM, &scr);
            scr.num = 2913;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15191:
            tig_art_item_id_create(166, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(166, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 2);
            scr.hdr.counters &= 0xFF;
            scr.hdr.counters |= 0x401AA00;
            scr.num = 30017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            scr.num = 30018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_REMOVE_ITEM, &scr);
            scr.num = 2915;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15192:
            tig_art_item_id_create(161, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(161, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 7);
            scr.hdr.counters &= 0xFF;
            scr.hdr.counters |= 0x401AF00;
            scr.num = 30017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            scr.num = 30018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_REMOVE_ITEM, &scr);
            scr.num = 2920;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15193:
            tig_art_item_id_create(165, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(165, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            scr.hdr.counters &= 0xFF;
            scr.hdr.counters |= 0x401AD00;
            scr.num = 30017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            scr.num = 30018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_REMOVE_ITEM, &scr);
            scr.num = 2918;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15194:
            tig_art_item_id_create(163, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(163, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            scr.hdr.counters &= 0xFF;
            scr.hdr.counters |= 0x401AE00;
            scr.num = 30017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            scr.num = 30018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_REMOVE_ITEM, &scr);
            scr.num = 2919;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15195:
            tig_art_item_id_create(164, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(164, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 9);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 750);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 50);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 200);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 4);
            scr.hdr.counters &= 0xFF;
            scr.hdr.counters |= 0x401AC00;
            scr.num = 30017;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            scr.num = 30018;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_REMOVE_ITEM, &scr);
            scr.num = 2917;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            break;
        case 15196:
            tig_art_item_id_create(169, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(169, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 90);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 300);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -90);
            break;
        case 15197:
            tig_art_item_id_create(170, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(170, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 13);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 20);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -85);
            break;
        case 15198:
            tig_art_item_id_create(191, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(191, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 5);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 100);
            obj_field_int32_set(obj, OBJ_F_NAME, 5620);
            scr.num = 30283;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_INSERT_ITEM, &scr);
            break;
        case 15199:
            tig_art_item_id_create(71, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(71, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 50);
            obj_field_int32_set(obj, OBJ_F_NAME, 5613);
            break;
        case 15200:
            tig_art_item_id_create(179, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(179, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 100);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15201:
            tig_art_item_id_create(180, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(180, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 150);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_NAME, 5684);
            break;
        case 15202:
            tig_art_item_id_create(181, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(181, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15203:
            tig_art_item_id_create(178, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(178, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 250);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15204:
            tig_art_item_id_create(182, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(182, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 350);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_NAME, 5684);
            break;
        case 15205:
            tig_art_item_id_create(183, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(183, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 400);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            break;
        case 15206:
            tig_art_item_id_create(177, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(177, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 200);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 90);
            obj_field_int32_set(obj, OBJ_F_GENERIC_USAGE_BONUS, 0);
            obj_field_int32_set(obj, OBJ_F_GENERIC_USAGE_COUNT_REMAINING, 60);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x8);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x180);
            break;
        case 15207:
            tig_art_item_id_create(185, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(185, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -70);
            break;
        case 15208:
            tig_art_item_id_create(OBJ_F_KEY_KEY_ID, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(OBJ_F_KEY_KEY_ID, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 7);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 1);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            break;
        case 15209:
            tig_art_item_id_create(184, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(184, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 6);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 0);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 30);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 3);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -20);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_1, 219);
            obj_field_int32_set(obj, OBJ_F_ITEM_SPELL_MANA_STORE, -1);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, 65664);
            break;
        case 15210:
            tig_art_item_id_create(188, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(188, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 10);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_NAME, 5643);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15211:
            tig_art_item_id_create(187, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(187, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 8);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 50);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 10);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 2);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, 25);
            obj_field_int32_set(obj, OBJ_F_NAME, 5642);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x1);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        case 15212:
            tig_art_item_id_create(14, 0, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            tig_art_item_id_create(14, 1, 0, 0, 0, 9, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_ITEM_INV_AID, art_id);
            scr.num = 30300;
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HIT, &scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_MISS, &scr);
            obj_field_int32_set(obj, OBJ_F_ITEM_DISCIPLINE, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY, -100);
            obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
            obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, 4000);
            obj_field_int32_set(obj, OBJ_F_HP_PTS, 80);
            obj_field_int32_set(obj, OBJ_F_ITEM_WEIGHT, 1000);
            obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS) | 0x80000);
            obj_field_int32_set(obj, OBJ_F_GENERIC_FLAGS, obj_field_int32_get(obj, OBJ_F_GENERIC_FLAGS) | 0x20);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 0);
            break;
        }
        break;
    case OBJ_TYPE_PC:
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
        obj_field_int32_set(obj, OBJ_F_CRITTER_CRIT_HIT_CHART, 0);
        tig_art_light_id_create(6, 0, 0, 1, &rgb);
        obj_field_int32_set(obj, OBJ_F_SHADOW, rgb);
        switch (description) {
        case 16066:
            sub_441C70(obj, 0, 1, 0);
            obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, "none");
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1005);
            object_set_hp_damage(obj, 0);
            break;
        case 16067:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 76);
            sub_441C70(obj, 2, 1, 5);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1017);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 108);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 39);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1039);
            object_set_hp_damage(obj, 0);
            break;
        case 16068:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 77);
            sub_441C70(obj, 2, 1, 4);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1002);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 24);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 40);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1040);
            object_set_hp_damage(obj, 0);
            break;
        case 16069:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 78);
            sub_441C70(obj, 0, 1, 3);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1032);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 20);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 41);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1041);
            object_set_hp_damage(obj, 0);
            break;
        case 16070:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 79);
            sub_441C70(obj, 3, 1, 7);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1027);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 25);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 42);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1042);
            object_set_hp_damage(obj, 0);
            break;
        case 16071:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 80);
            sub_441C70(obj, 4, 1, 2);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1033);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 20);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 43);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1043);
            object_set_hp_damage(obj, 0);
            break;
        case 16072:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 81);
            sub_441C70(obj, 0, 1, 0);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1005);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 22);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 44);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1044);
            object_set_hp_damage(obj, 0);
            break;
        case 16073:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 82);
            sub_441C70(obj, 0, 1, 6);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1030);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 20);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 45);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1045);
            object_set_hp_damage(obj, 0);
            break;
        case 16074:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 83);
            sub_441C70(obj, 1, 1, 1);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1020);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 24);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 46);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1046);
            object_set_hp_damage(obj, 0);
            break;
        case 16075:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 84);
            sub_441C70(obj, 0, 0, 0);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1025);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 23);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 47);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1047);
            object_set_hp_damage(obj, 0);
            break;
        case 16076:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 85);
            sub_441C70(obj, 4, 0, 2);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1024);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 22);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 48);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1048);
            object_set_hp_damage(obj, 0);
            break;
        case 16077:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 86);
            sub_441C70(obj, 0, 0, 6);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1031);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 26);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 49);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1049);
            object_set_hp_damage(obj, 0);
            break;
        case 16078:
            obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, 87);
            sub_441C70(obj, 0, 0, 3);
            str = level_advancement_scheme_get_name(obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME));
            if (str != NULL) {
                obj_field_string_set(obj, OBJ_F_PC_PLAYER_NAME, str);
            }
            obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, 1029);
            obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, 26);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 50);
            obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 1050);
            object_set_hp_damage(obj, 0);
            break;
        default:
            object_set_hp_damage(obj, 0);
            break;
        }
        break;
    case OBJ_TYPE_NPC:
        if (description >= 27309 && description < 27394) {
            sub_49B010(obj, description);
        } else if (description >= 28310 && description < 28472) {
            sub_49B220(obj, description);
        } else {
            sub_49AFF0(obj, description);
        }
        break;
    case OBJ_TYPE_TRAP:
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 5);
        obj_field_int32_set(obj, OBJ_F_HP_PTS, 100);
        obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, 16);
        obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | 0x100);
        switch (description) {
        case 26000:
            tig_art_eye_candy_id_create(0x163, 0, 0, 0, 2, 0, 4, &art_id);
            art_id = tig_art_id_frame_set(art_id, -1);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            scr.num = 30000;
            sub_44BCC0(&scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 26001:
            tig_art_eye_candy_id_create(0x164, 0, 0, 0, 2, 0, 4, &art_id);
            art_id = tig_art_id_frame_set(art_id, -1);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            scr.num = 30001;
            sub_44BCC0(&scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 26002:
            tig_art_eye_candy_id_create(0x15Fu, 0, 0, 0, 2, 0, 4, &art_id);
            art_id = tig_art_id_frame_set(art_id, -1);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            scr.num = 30002;
            sub_44BCC0(&scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 26003:
            tig_art_eye_candy_id_create(0x162, 0, 0, 0, 2, 0, 4, &art_id);
            art_id = tig_art_id_frame_set(art_id, -1);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            scr.num = 30003;
            sub_44BCC0(&scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 26004:
            tig_art_eye_candy_id_create(0x161, 0, 0, 0, 2, 0, 4, &art_id);
            art_id = tig_art_id_frame_set(art_id, -1);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            scr.num = 30004;
            sub_44BCC0(&scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 26005:
            tig_art_eye_candy_id_create(0x160, 0, 0, 0, 2, 0, 4, &art_id);
            art_id = tig_art_id_frame_set(art_id, -1);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            scr.num = 30005;
            sub_44BCC0(&scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 26006:
            tig_art_eye_candy_id_create(0x165, 0, 0, 0, 2, 0, 4, &art_id);
            art_id = tig_art_id_frame_set(art_id, -1);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            scr.num = 30006;
            sub_44BCC0(&scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 1);
            break;
        case 26007:
            tig_art_scenery_id_create(0, 10, 0, 0, 0, &art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
            scr.num = 30007;
            sub_44BCC0(&scr);
            sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_USE, &scr);
            obj_field_int32_set(obj, OBJ_F_CATEGORY, 2);
            break;
        }
        break;
    }
}

// 0x49AFF0
void sub_49AFF0(long long obj, int description)
{
    sub_49B340(obj, description);
}

// 0x49B010
void sub_49B010(long long obj, int description)
{
    tig_art_id_t art_id;
    Script scr;
    int exp;

    obj_field_int32_set(obj, OBJ_F_CRITTER_CRIT_HIT_CHART, 0);

    switch (description) {
    case 27309:
        sub_49AFF0(obj, 17081);
        obj_field_int32_set(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN, 27309);
        tig_art_unique_npc_id_create(11, 0, 0, 4, 0, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        break;
    case 27310:
        sub_49AFF0(obj, 17082);
        obj_field_int32_set(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN, 27310);
        tig_art_unique_npc_id_create(2, 0, 0, 4, 0, 0, 0, &art_id);
        obj_field_int32_set(obj, OBJ_F_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        break;
    case 27311:
        sub_49AFF0(obj, 17082);
        obj_field_int32_set(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN, 27311);
        scr.num = 1498;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_DIALOG, &scr);
        scr.num = 1499;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_FIRST_HEARTBEAT, &scr);
        break;
    case 27319:
        sub_49AFF0(obj, 17082);
        scr.num = 30117;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HEARTBEAT, &scr);
        scr.num = 30118;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_DYING, &scr);
        break;
    case 27320:
        sub_49AFF0(obj, 17082);
        scr.num = 30119;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_HEARTBEAT, &scr);
        scr.num = 30120;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, SAP_DYING, &scr);
        break;
    default:
        sub_49B220(obj, description);
        break;
    }

    exp = sub_45F0B0(obj);
    obj_field_int32_set(obj, OBJ_F_NPC_EXPERIENCE_WORTH, exp);
    obj_field_int32_set(obj, OBJ_F_NPC_EXPERIENCE_POOL, exp);
    object_set_hp_damage(obj, 0);
}

// 0x49B220
void sub_49B220(long long obj, int description)
{
    sub_49B340(obj, description);
}

// 0x49B240
void sub_49B240(long long obj, int f, int percent)
{
    int value;

    value = obj_field_int32_get(obj, f);
    obj_field_int32_set(obj, f, value + value * percent / 100);
}

// 0x49B290
int sub_49B290(long long obj)
{
    long long prototype_handle;
    ObjectID object_id;

    prototype_handle = obj_field_handle_get(obj, OBJ_F_PROTOTYPE_HANDLE);
    object_id = sub_407EF0(prototype_handle);

    return object_id.d.a - 20;
}

// 0x49B2E0
void sub_49B2E0(long long obj)
{
    int complexity;
    int worth;

    complexity = obj_field_int32_get(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
    if (complexity != 0) {
        worth = obj_field_int32_get(obj, OBJ_F_ITEM_WORTH);
        obj_field_int32_set(obj, OBJ_F_ITEM_WORTH, worth + 3 * complexity * complexity / 10);
    }
}

// 0x49B340
void sub_49B340(long long obj, int description)
{
    TigFile* stream;
    int type;
    int v1;
    int exp;
    tig_art_id_t art_id;
    unsigned int rgb;
    int gender;
    int race;

    if (description >= 28310 && description < 28472) {
        // FIXME: Probably wrong, the 28310-28472 range is for UNIQUE NPC.
        stream = tig_file_fopen("rules\\monster.txt", "rt");
        type = 2;
    } else if (description >= 27309 && description < 27394) {
        // FIXME: Probably wrong, the 27309-27394 range is for MONSTER.
        stream = tig_file_fopen("rules\\unique.txt", "rt");
        type = 1;
    } else if (description >= 17067 && description < 17317) {
        stream = tig_file_fopen("rules\\npc.txt", "rt");
        type = 0;
    } else {
        return;
    }

    if (stream == NULL) {
        return;
    }

    v1 = sub_49B5A0(stream, 0, type);
    while (v1 != description && v1 != -1) {
        v1 = sub_49B5A0(stream, 0, type);
    }

    if (v1 != -1) {
        obj_field_int32_set(obj, OBJ_F_DESCRIPTION, description);
        obj_field_int32_set(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN, description);
        sub_49B5A0(stream, obj, type);

        exp = sub_45F0B0(obj);
        obj_field_int32_set(obj, OBJ_F_NPC_EXPERIENCE_WORTH, exp);
        obj_field_int32_set(obj, OBJ_F_NPC_EXPERIENCE_POOL, exp);
        object_set_hp_damage(obj, 0);

        art_id = sub_45FA70(obj, &rgb);
        obj_field_int32_set(obj, OBJ_F_LIGHT_AID, art_id);
        obj_field_int32_set(obj, OBJ_F_LIGHT_COLOR, rgb);

        if (type == 0) {
            gender = stat_get_base(obj, STAT_GENDER);
            race = stat_get_base(obj, STAT_RACE);
            obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 10 * (gender + 2 * race + 1));
            if (race == RACE_ORC) {
                tig_art_monster_id_create(2, 0, 0, 0, 4, 0, 0, 0, &art_id);
            } else {
                tig_art_critter_id_create(gender, dword_5B389C[race], 0, 0, 0, 4, 0, 0, 0, &art_id);
            }
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        }

        if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & 0x1) != 0) {
            art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
            art_id = sub_45EFA0(art_id);
            obj_field_int32_set(obj, OBJ_F_AID, art_id);
            obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
        }
    }

    tig_file_fclose(stream);
}

// 0x49B5A0
int sub_49B5A0(TigFile* stream, long long obj, int type)
{
    tig_art_id_t art_id;
    char str[1000];
    Script scr;
    int fld;
    int v2;
    int v3;
    int v4;

    if (obj != 0) {
        obj_field_int32_set(obj, OBJ_F_CRITTER_CRIT_HIT_CHART, 0);
        obj_field_int32_set(obj, OBJ_F_MATERIAL, 4);
        obj_field_int32_set(obj, OBJ_F_NPC_REACTION_BASE, 50);
        obj_field_int32_set(obj, OBJ_F_NPC_WAIT, 2);
        tig_art_light_id_create(6, 0, 0, 1, &art_id);
        obj_field_int32_set(obj, OBJ_F_SHADOW, art_id);
        obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, 250);
    }

    while (tig_file_fgets(str, sizeof(str), stream) != NULL) {
        if (sub_49BB70(str, &fld, &v2, &v3, &v4)) {
            if (fld == PROTO_F_DESCRIPTION) {
                return v2;
            }

            if (obj != 0) {
                switch (fld) {
                case PROTO_F_INTERNAL_NAME:
                    obj_field_int32_set(obj, OBJ_F_NAME, v2);
                    break;
                case PROTO_F_LEVEL:
                    stat_set_base(obj, STAT_LEVEL, v2);
                    break;
                case PROTO_F_ART_NUMBER_AND_PALETTE:
                    switch (type) {
                    case 1:
                        tig_art_unique_npc_id_create(v2, 0, 0, 4, 0, 0, v3, &art_id);
                        obj_field_int32_set(obj, OBJ_F_AID, art_id);
                        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                        break;
                    case 2:
                        tig_art_monster_id_create(v2, 0, 0, 0, 4, 0, 0, v3, &art_id);
                        obj_field_int32_set(obj, OBJ_F_AID, art_id);
                        obj_field_int32_set(obj, OBJ_F_CURRENT_AID, art_id);
                        break;
                    }
                    break;
                case PROTO_F_SCALE:
                    obj_field_int32_set(obj, OBJ_F_BLIT_SCALE, v2);
                    break;
                case PROTO_F_ALIGNMENT:
                    stat_set_base(obj, STAT_ALIGNMENT, v2);
                    break;
                case PROTO_F_CRITTER_FLAG:
                    obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS, obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) | v2);
                    break;
                case PROTO_F_CRITTER2_FLAG:
                    obj_field_int32_set(obj, OBJ_F_CRITTER_FLAGS2, obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2) | v2);
                    break;
                case PROTO_F_NPC_FLAG:
                    obj_field_int32_set(obj, OBJ_F_NPC_FLAGS, obj_field_int32_get(obj, OBJ_F_NPC_FLAGS) | v2);
                    break;
                case PROTO_F_BLIT_FLAG:
                    obj_field_int32_set(obj, OBJ_F_BLIT_FLAGS, obj_field_int32_get(obj, OBJ_F_BLIT_FLAGS) | v2);
                    break;
                case PROTO_F_SPELL_FLAG:
                    obj_field_int32_set(obj, OBJ_F_SPELL_FLAGS, obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) | v2);
                    break;
                case PROTO_F_HIT_CHART:
                    obj_field_int32_set(obj, OBJ_F_CRITTER_CRIT_HIT_CHART, v2);
                    break;
                case PROTO_F_BASIC_STAT:
                    stat_set_base(obj, v2, v3);
                    break;
                case PROTO_F_SPELL:
                    sub_4B1790(obj, v2, 1);
                    break;
                case PROTO_F_FACTION:
                    obj_field_int32_set(obj, OBJ_F_NPC_FACTION, v2);
                    break;
                case PROTO_F_AI_PACKET:
                    obj_field_int32_set(obj, OBJ_F_NPC_AI_DATA, v2);
                    break;
                case PROTO_F_MATERIAL:
                    obj_field_int32_set(obj, OBJ_F_MATERIAL, v2);
                    break;
                case PROTO_F_HIT_POINTS:
                    sub_49BB40(obj, v2);
                    break;
                case PROTO_F_SCRIPT:
                    scr.num = v3;
                    scr.hdr.counters = v4;
                    sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, v2, &scr);
                    break;
                case PROTO_F_DAMAGE_RESISTANCE:
                    obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 0, v2);
                    break;
                case PROTO_F_FIRE_RESISTANCE:
                    obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 1, v2);
                    break;
                case PROTO_F_ELECTRICAL_RESISTANCE:
                    obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 2, v2);
                    break;
                case PROTO_F_POISON_RESISTANCE:
                    obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 3, v2);
                    break;
                case PROTO_F_MAGIC_RESISTANCE:
                    obj_arrayfield_int32_set(obj, OBJ_F_RESISTANCE_IDX, 4, v2);
                    break;
                case PROTO_F_NORMAL_DAMAGE:
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 0, v2);
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 1, v3);
                    break;
                case PROTO_F_POISON_DAMAGE:
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 2, v2);
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 3, v3);
                    break;
                case PROTO_F_ELECTRICAL_DAMAGE:
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 4, v2);
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 5, v3);
                    break;
                case PROTO_F_FIRE_DAMAGE:
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 6, v2);
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 7, v3);
                    break;
                case PROTO_F_FATIGUE_DAMAGE:
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 8, v2);
                    sub_4074E0(obj, OBJ_F_NPC_DAMAGE_IDX, 9, v3);
                    break;
                case PROTO_F_SOUND_BANK:
                    obj_field_int32_set(obj, OBJ_F_SOUND_EFFECT, v2);
                    break;
                case PROTO_F_INVENTORY_SOURCE:
                    obj_field_int32_set(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, v2);
                    break;
                case PROTO_F_PORTRAIT:
                    obj_field_int32_set(obj, OBJ_F_CRITTER_PORTRAIT, v2);
                    break;
                case PROTO_F_RETAIL_PRICE_MULTIPLIER:
                    obj_field_int32_set(obj, OBJ_F_NPC_RETAIL_PRICE_MULTIPLIER, v2);
                    break;
                case PROTO_F_SOCIAL_CLASS:
                    critter_social_class_set(obj, v2);
                    break;
                case PROTO_F_OBJECT_FLAG:
                    obj_field_int32_set(obj, OBJ_F_FLAGS, obj_field_int32_get(obj, OBJ_F_FLAGS) | v2);
                    break;
                case PROTO_F_AUTO_LEVEL_SCHEME:
                    obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, v2);
                    break;
                case PROTO_F_CATEGORY:
                    obj_field_int32_set(obj, OBJ_F_CATEGORY, v2);
                    break;
                }
            }
        }
    }

    return -1;
}

// 0x49BB40
void sub_49BB40(long long obj, int a2)
{
    object_set_hp_adj(obj, object_get_hp_adj(obj) + a2 - sub_43D5A0(obj));
}

// 0x49BB70
bool sub_49BB70(const char* str, int* fld_ptr, int* a3, int* a4, int* a5)
{
    int fld;
    size_t pos;
    int stat;
    int spl;
    const char* name;
    int v1;
    int v2;
    int v3;
    int v4;

    for (fld = 0; fld < PROTO_F_COUNT; fld++) {
        pos = strlen(off_5B38C8[fld]) + 1;
        if (strnicmp(str, off_5B38C8[fld], pos - 1) == 0) {
            break;
        }
    }

    if (fld == PROTO_F_COUNT) {
        return false;
    }

    *fld_ptr = fld;

    while (str[pos] == ':' || str[pos] == ' ') {
        pos++;
    }

    if (str[pos] == '\0') {
        tig_debug_printf("Proto read: field with no data %s\n", str);
    }

    str += pos;

    switch (fld) {
    case PROTO_F_ART_NUMBER_AND_PALETTE:
    case PROTO_F_NORMAL_DAMAGE:
    case PROTO_F_POISON_DAMAGE:
    case PROTO_F_ELECTRICAL_DAMAGE:
    case PROTO_F_FIRE_DAMAGE:
    case PROTO_F_FATIGUE_DAMAGE:
        sscanf(str, "%d %d", a3, a4);
        return true;
    case PROTO_F_CRITTER_FLAG:
        *a3 = sub_49BF10(str, off_5BA348, 32);
        return true;
    case PROTO_F_CRITTER2_FLAG:
        *a3 = sub_49BF10(str, off_5BA3C8, 27);
        return true;
    case PROTO_F_NPC_FLAG:
        *a3 = sub_49BF10(str, off_5BA44C, 31);
        return true;
    case PROTO_F_BLIT_FLAG:
        *a3 = sub_49BF10(str, off_5B395C, sizeof(off_5B395C) / sizeof(off_5B395C[0]));
        return true;
    case PROTO_F_BASIC_STAT:
        for (stat = STAT_STRENGTH; stat <= STAT_CHARISMA; stat++) {
            name = stat_get_name(stat);
            pos = strlen(name);
            if (strnicmp(str, name, pos) == 0) {
                *a3 = stat;
                *a4 = atoi(str + pos);
                return true;
            }
        }
        for (stat = STAT_MAGICK_POINTS; stat <= STAT_RACE; stat++) {
            name = stat_get_name(stat);
            pos = strlen(name);
            if (strnicmp(str, name, pos) == 0) {
                *a3 = stat;
                *a4 = atoi(str + pos);
                return true;
            }
        }
        tig_debug_printf("Proto read: unknown stat %s\n", str);
        return true;
    case PROTO_F_SPELL:
        for (spl = 0; spl < SPELL_COUNT; spl++) {
            name = spell_get_name(spl);
            pos = strlen(name);
            if (strnicmp(str, name, pos) == 0) {
                *a3 = spl;
                *a4 = atoi(str + pos);
                return true;
            }
        }
        tig_debug_printf("Proto read: unknown spell %s\n", str);
        return true;
    case PROTO_F_SCRIPT:
        sscanf(str, "%d %d %d %d %d %d", a3, a4, &v1, &v2, &v3, &v4);
        *a5 = v1 | (v2 << 8) | (v3 << 16) | (v4 << 24);
        return true;
    case PROTO_F_OBJECT_FLAG:
        *a3 = sub_49BF10(str, off_5BA10C, 31);
        return true;
    default:
        *a3 = atoi(str);
        return true;
    }
}

// 0x49BF10
int sub_49BF10(const char* str, const char** identifiers, int size)
{
    int index;

    for (index = 0; index < size; index++) {
        if (strnicmp(str, identifiers[index], strlen(identifiers[index])) == 0) {
            return 1 << index;
        }
    }

    tig_debug_printf("Proto read: unknown flag %s\n", str);
    return 0;
}

// 0x49C060
void sub_49C060(long long obj, TigFile* stream, int type)
{
    char description[2000];
    int value;
    tig_art_id_t art_id;
    int stat;
    int college;
    int num_spl;
    int spl;
    int index;
    int scr[3];
    int min_damage;
    int max_damage;

    sub_441B60(obj, obj, description);
    tig_file_fprintf(stream, "%s: %d   // %s\n",
        off_5B38C8[PROTO_F_DESCRIPTION],
        obj_field_int32_get(obj, OBJ_F_DESCRIPTION),
        description);

    value = obj_field_int32_get(obj, OBJ_F_NAME);
    if (value != 0) {
        tig_file_fprintf(stream, "%s: %d\n",
            off_5B38C8[PROTO_F_INTERNAL_NAME],
            value);
    }

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_LEVEL],
        stat_level(obj, STAT_LEVEL));

    art_id = obj_field_int32_get(obj, OBJ_F_CURRENT_AID);
    switch (type) {
    case 1:
        tig_file_fprintf(stream, "%s: %d %d\n",
            off_5B38C8[PROTO_F_ART_NUMBER_AND_PALETTE],
            tig_art_num_get(art_id),
            tig_art_id_palette_get(art_id));
        break;
    case 2:
        tig_file_fprintf(stream, "%s: %d %d\n",
            off_5B38C8[PROTO_F_ART_NUMBER_AND_PALETTE],
            sub_503F20(art_id),
            tig_art_id_palette_get(art_id));
        break;
    default:
        tig_file_fprintf(stream, "%s: %s %d\n",
            off_5B38C8[PROTO_F_BASIC_STAT],
            stat_get_name(STAT_GENDER),
            stat_get_base(obj, STAT_GENDER));
        tig_file_fprintf(stream, "%s: %s %d\n",
            off_5B38C8[PROTO_F_BASIC_STAT],
            stat_get_name(STAT_RACE),
            stat_get_base(obj, STAT_RACE));
        break;
    }

    value = obj_field_int32_get(obj, OBJ_F_BLIT_SCALE);
    if (value != 100) {
        tig_file_fprintf(stream, "%s: %d\n",
            off_5B38C8[PROTO_F_SCALE],
            value);
    }

    value = stat_level(obj, STAT_ALIGNMENT);
    if (value != 0) {
        tig_file_fprintf(stream, "%s: %d\n",
            off_5B38C8[PROTO_F_ALIGNMENT],
            value);
    }

    sub_49C610(stream,
        off_5B38C8[PROTO_F_CRITTER_FLAG],
        obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS),
        off_5BA348,
        sizeof(off_5BA348) / sizeof(off_5BA348[0]));

    sub_49C610(stream,
        off_5B38C8[PROTO_F_CRITTER2_FLAG],
        obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2),
        off_5BA3C8,
        sizeof(off_5BA3C8) / sizeof(off_5BA3C8[0]));

    sub_49C610(stream,
        off_5B38C8[PROTO_F_NPC_FLAG],
        obj_field_int32_get(obj, OBJ_F_NPC_FLAGS),
        off_5BA44C,
        sizeof(off_5BA44C) / sizeof(off_5BA44C[0]));

    sub_49C610(stream,
        off_5B38C8[PROTO_F_BLIT_FLAG],
        obj_field_int32_get(obj, OBJ_F_BLIT_FLAGS),
        off_5B395C,
        sizeof(off_5B395C) / sizeof(off_5B395C[0]));

    sub_49C610(stream,
        off_5B38C8[PROTO_F_SPELL_FLAG],
        obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS),
        off_5BA188,
        sizeof(off_5BA188) / sizeof(off_5BA188[0]));

    value = obj_field_int32_get(obj, OBJ_F_CRITTER_CRIT_HIT_CHART);
    if (value != 0) {
        tig_file_fprintf(stream, "%s: %d\n",
            off_5B38C8[PROTO_F_HIT_CHART],
            value);
    }

    for (stat = STAT_STRENGTH; stat <= STAT_CHARISMA; stat++) {
        value = stat_get_base(obj, stat);
        if (value != 8) {
            tig_file_fprintf(stream, "%s: %s %d\n",
                off_5B38C8[PROTO_F_BASIC_STAT],
                stat_get_name(stat),
                value);
        }
    }

    // FIXME: Does not include STAT_MAGICK_POINTS - STAT_RACE range which is
    // parsed.

    for (college = 0; college < COLLEGE_COUNT; college++) {
        num_spl = sub_4B1AB0(obj, college);
        for (spl = 0; spl < num_spl; spl++) {
            tig_file_fprintf(stream, "%s: %s\n",
                off_5B38C8[PROTO_F_SPELL],
                spell_get_name(college * 5 + spl));
        }
    }

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_FACTION],
        obj_field_int32_get(obj, OBJ_F_NPC_FACTION));

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_AI_PACKET],
        obj_field_int32_get(obj, OBJ_F_NPC_AI_DATA));

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_MATERIAL],
        obj_field_int32_get(obj, OBJ_F_MATERIAL));

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_HIT_POINTS],
        sub_43D5A0(obj));

    for (index = 0; index < 36; index++) {
        sub_407840(obj, OBJ_F_SCRIPTS_IDX, index, &scr);
        if (scr[2] != 0) {
            tig_file_fprintf(stream, "%s: %d %d %d %d %d %d\n",
                off_5B38C8[PROTO_F_SCRIPT],
                index,
                scr[2],
                scr[1] & 0xFF,
                (scr[1] >> 8) & 0xFF,
                (scr[1] >> 16) & 0xFF,
                (scr[1] >> 24) & 0xFF);
        }
    }

    for (index = PROTO_F_DAMAGE_RESISTANCE; index <= PROTO_F_MAGIC_RESISTANCE; index++) {
        value = obj_arrayfield_int32_get(obj, OBJ_F_RESISTANCE_IDX, index);
        if (value != 0) {
            tig_file_fprintf(stream, "%s: %d\n",
                off_5B38C8[index],
                value);
        }
    }

    for (index = PROTO_F_NORMAL_DAMAGE; index <= PROTO_F_FATIGUE_DAMAGE; index++) {
        min_damage = sub_407470(obj, OBJ_F_NPC_DAMAGE_IDX, index * 2);
        max_damage = sub_407470(obj, OBJ_F_NPC_DAMAGE_IDX, index * 2 + 1);
        if (min_damage != 0 || max_damage != 0) {
            tig_file_fprintf(stream, "%s: %d %d\n",
                off_5B38C8[index],
                min_damage,
                max_damage);
        }
    }

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_SOUND_BANK],
        obj_field_int32_get(obj, OBJ_F_SOUND_EFFECT));

    value = obj_field_int32_get(obj, OBJ_F_CRITTER_INVENTORY_SOURCE);
    if (value != 0) {
        tig_file_fprintf(stream, "%s: %d\n",
            off_5B38C8[PROTO_F_INVENTORY_SOURCE],
            value);
    }

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_PORTRAIT],
        obj_field_int32_get(obj, OBJ_F_CRITTER_PORTRAIT));

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_RETAIL_PRICE_MULTIPLIER],
        obj_field_int32_get(obj, OBJ_F_NPC_RETAIL_PRICE_MULTIPLIER));

    tig_file_fprintf(stream, "%s: %d\n",
        off_5B38C8[PROTO_F_SOCIAL_CLASS],
        critter_social_class_get(obj));

    sub_49C610(stream,
        off_5B38C8[PROTO_F_OBJECT_FLAG],
        obj_field_int32_get(obj, OBJ_F_FLAGS),
        off_5BA10C,
        sizeof(off_5BA10C) / sizeof(off_5BA10C[0]));

    value = obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME);
    if (value != 0) {
        tig_file_fprintf(stream, "%s: %d\n",
            off_5B38C8[PROTO_F_AUTO_LEVEL_SCHEME],
            value);
    }

    value = obj_field_int32_get(obj, OBJ_F_CATEGORY);
    if (value != 0) {
        tig_file_fprintf(stream, "%s: %d\n",
            off_5B38C8[PROTO_F_CATEGORY],
            value);
    }
}

// 0x49C610
void sub_49C610(TigFile* stream, const char* name, int value, const char** identifiers, int size)
{
    int index;

    if (value == 0) {
        return;
    }

    for (index = 0; index < size; index++) {
        if (((1 << index) & value) != 0) {
            tig_file_fprintf(stream, "%s: %s\n", name, identifiers[index]);
        }
    }
}
