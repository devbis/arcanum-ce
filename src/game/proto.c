#include "game/lib/object.h"
#include "game/lib/spell.h"
#include "game/lib/stat.h"

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
static ObjectId* dword_5E882C;

// 0x5E8830
static bool initialized;

// 0x4681B0
int proto_init(GameInitInfo* init_info)
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

    dword_5E882C = CALLOC(OBJ_TYPE_COUNT, sizeof(ObjectId));

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
    ObjectId object_id;
    char* name = NULL;
    char path[TIG_MAX_PATH];
    TigFile* stream;

    object_id = sub_407EF0(obj);
    obj_f_get_string(obj, OBJ_F_NAME, &name);

    if (object_id.type != 1) {
        return false;
    }

    sprintf(path, "proto\\%06d - %s.pro", object_id.d.a.field_8, name);
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
    ObjectId object_id;
    const char* name;
    char path[TIG_MAX_PATH];

    tig_debug_printf("Reading prototypes...\n");

    for (object_type = 0; object_type < 20; object_type++) {
        for (description = dword_5B37FC[object_type]; description < dword_5B384C[object_type]; description++) {
            object_id = sub_468860(description);
            name = sub_4E7400(sub_468720(object_type));
            sprintf(path, "proto\\%06d - %s.pro", object_id.d.index, name);
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
ObjectId sub_468860(int a1)
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
    obj_f_set_int32(obj, OBJ_F_NAME, sub_468720(object_type));
    sub_408020(obj, description + 20);
    sub_468930(obj, description);
    in_proto_save = true;
    proto_save(obj);
    in_proto_save = false;
    sub_405BF0(obj);
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
    int scr[3];
    int exp;

    obj_f_set_int32(obj, OBJ_F_CRITTER_CRIT_HIT_CHART, 0);

    switch (description) {
    case 27309:
        sub_49AFF0(obj, 17081);
        obj_f_set_int32(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN, 27309);
        tig_art_unique_npc_id_create(11, 0, 0, 4, 0, 0, 0, &art_id);
        obj_f_set_int32(obj, OBJ_F_AID, art_id);
        obj_f_set_int32(obj, OBJ_F_CURRENT_AID, art_id);
        break;
    case 27310:
        sub_49AFF0(obj, 17082);
        obj_f_set_int32(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN, 27310);
        tig_art_unique_npc_id_create(2, 0, 0, 4, 0, 0, 0, &art_id);
        obj_f_set_int32(obj, OBJ_F_AID, art_id);
        obj_f_set_int32(obj, OBJ_F_CURRENT_AID, art_id);
        break;
    case 27311:
        sub_49AFF0(obj, 17082);
        obj_f_set_int32(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN, 27311);
        scr[2] = 1498;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, 9, scr);
        scr[2] = 1499;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, 10, scr);
        break;
    case 27319:
        sub_49AFF0(obj, 17082);
        scr[2] = 30117;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, 19, scr);
        scr[2] = 30118;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, 12, scr);
        break;
    case 27320:
        sub_49AFF0(obj, 17082);
        scr[2] = 30119;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, 19, scr);
        scr[2] = 30120;
        sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, 12, scr);
        break;
    default:
        sub_49B220(obj, description);
        break;
    }

    exp = sub_45F0B0(obj);
    obj_f_set_int32(obj, OBJ_F_NPC_EXPERIENCE_WORTH, exp);
    obj_f_set_int32(obj, OBJ_F_NPC_EXPERIENCE_POOL, exp);
    sub_43D530(obj, 0);
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

    value = obj_f_get_int32(obj, f);
    obj_f_set_int32(obj, f, value + value * percent / 100);
}

// 0x49B290
int sub_49B290(long long obj)
{
    long long prototype_handle;
    ObjectId object_id;

    prototype_handle = obj_f_get_handle(obj, OBJ_F_PROTOTYPE_HANDLE);
    object_id = sub_407EF0(prototype_handle);

    return object_id.d.a.field_8 - 20;
}

// 0x49B2E0
void sub_49B2E0(long long obj)
{
    int complexity;
    int worth;

    complexity = obj_f_get_int32(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);
    if (complexity != 0) {
        worth = obj_f_get_int32(obj, OBJ_F_ITEM_WORTH);
        obj_f_set_int32(obj, OBJ_F_ITEM_WORTH, worth + 3 * complexity * complexity / 10);
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
    int rgb;
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
        obj_f_set_int32(obj, OBJ_F_DESCRIPTION, description);
        obj_f_set_int32(obj, OBJ_F_CRITTER_DESCRIPTION_UNKNOWN, description);
        sub_49B5A0(stream, obj, type);

        exp = sub_45F0B0(obj);
        obj_f_set_int32(obj, OBJ_F_NPC_EXPERIENCE_WORTH, exp);
        obj_f_set_int32(obj, OBJ_F_NPC_EXPERIENCE_POOL, exp);
        sub_43D530(obj, 0);

        art_id = sub_45FA70(obj, &rgb);
        obj_f_set_int32(obj, OBJ_F_LIGHT_AID, art_id);
        obj_f_set_int32(obj, OBJ_F_LIGHT_COLOR, rgb);

        if (type == 0) {
            gender = stat_get_base(obj, STAT_GENDER);
            race = stat_get_base(obj, STAT_RACE);
            obj_f_set_int32(obj, OBJ_F_SOUND_EFFECT, 10 * (gender + 2 * race + 1));
            if (race == RACE_ORC) {
                tig_art_monster_id_create(2, 0, 0, 0, 4, 0, 0, 0, &art_id);
            } else {
                tig_art_critter_id_create(gender, dword_5B389C[race], 0, 0, 0, 4, 0, 0, 0, &art_id);
            }
            obj_f_set_int32(obj, OBJ_F_AID, art_id);
            obj_f_set_int32(obj, OBJ_F_CURRENT_AID, art_id);
        }

        if ((obj_f_get_int32(obj, OBJ_F_CRITTER_FLAGS) & 0x1) != 0) {
            art_id = obj_f_get_int32(obj, OBJ_F_CURRENT_AID);
            art_id = sub_45EFA0(art_id);
            obj_f_set_int32(obj, OBJ_F_AID, art_id);
            obj_f_set_int32(obj, OBJ_F_CURRENT_AID, art_id);
        }
    }

    tig_file_fclose(stream);
}

// 0x49B5A0
int sub_49B5A0(TigFile* stream, long long obj, int type)
{
    tig_art_id_t art_id;
    char str[1000];
    int scr[3];
    int fld;
    int v2;
    int v3;
    int v4;

    if (obj != 0) {
        obj_f_set_int32(obj, OBJ_F_CRITTER_CRIT_HIT_CHART, 0);
        obj_f_set_int32(obj, OBJ_F_MATERIAL, 4);
        obj_f_set_int32(obj, OBJ_F_NPC_REACTION_BASE, 50);
        obj_f_set_int32(obj, OBJ_F_NPC_WAIT, 2);
        tig_art_light_id_create_(6u, 0, 0, 1, &art_id);
        obj_f_set_int32(obj, OBJ_F_SHADOW, art_id);
        obj_f_set_int32(obj, OBJ_F_SOUND_EFFECT, 250);
    }

    while (tig_file_fgets(str, sizeof(str), stream) != NULL) {
        if (sub_49BB70(str, &fld, &v2, &v3, &v4)) {
            if (fld == PROTO_F_DESCRIPTION) {
                return v2;
            }

            if (obj != 0) {
                switch (fld) {
                case PROTO_F_INTERNAL_NAME:
                    obj_f_set_int32(obj, OBJ_F_NAME, v2);
                    break;
                case PROTO_F_LEVEL:
                    stat_set_base(obj, STAT_LEVEL, v2);
                    break;
                case PROTO_F_ART_NUMBER_AND_PALETTE:
                    switch (type) {
                    case 1:
                        tig_art_unique_npc_id_create(v2, 0, 0, 4, 0, 0, v3, &art_id);
                        obj_f_set_int32(obj, OBJ_F_AID, art_id);
                        obj_f_set_int32(obj, OBJ_F_CURRENT_AID, art_id);
                        break;
                    case 2:
                        tig_art_monster_id_create(v2, 0, 0, 0, 4, 0, 0, v3, &art_id);
                        obj_f_set_int32(obj, OBJ_F_AID, art_id);
                        obj_f_set_int32(obj, OBJ_F_CURRENT_AID, art_id);
                        break;
                    }
                    break;
                case PROTO_F_SCALE:
                    obj_f_set_int32(obj, OBJ_F_BLIT_SCALE, v2);
                    break;
                case PROTO_F_ALIGNMENT:
                    stat_set_base(obj, STAT_ALIGNMENT, v2);
                    break;
                case PROTO_F_CRITTER_FLAG:
                    obj_f_set_int32(obj, OBJ_F_CRITTER_FLAGS, obj_f_get_int32(obj, OBJ_F_CRITTER_FLAGS) | v2);
                    break;
                case PROTO_F_CRITTER2_FLAG:
                    obj_f_set_int32(obj, OBJ_F_CRITTER_FLAGS2, obj_f_get_int32(obj, OBJ_F_CRITTER_FLAGS2) | v2);
                    break;
                case PROTO_F_NPC_FLAG:
                    obj_f_set_int32(obj, OBJ_F_NPC_FLAGS, obj_f_get_int32(obj, OBJ_F_NPC_FLAGS) | v2);
                    break;
                case PROTO_F_BLIT_FLAG:
                    obj_f_set_int32(obj, OBJ_F_BLIT_FLAGS, obj_f_get_int32(obj, OBJ_F_BLIT_FLAGS) | v2);
                    break;
                case PROTO_F_SPELL_FLAG:
                    obj_f_set_int32(obj, OBJ_F_SPELL_FLAGS, obj_f_get_int32(obj, OBJ_F_SPELL_FLAGS) | v2);
                    break;
                case PROTO_F_HIT_CHART:
                    obj_f_set_int32(obj, OBJ_F_CRITTER_CRIT_HIT_CHART, v2);
                    break;
                case PROTO_F_BASIC_STAT:
                    stat_set_base(obj, v2, v3);
                    break;
                case PROTO_F_SPELL:
                    sub_4B1790(obj, v2, 1);
                    break;
                case PROTO_F_FACTION:
                    obj_f_set_int32(obj, OBJ_F_NPC_FACTION, v2);
                    break;
                case PROTO_F_AI_PACKET:
                    obj_f_set_int32(obj, OBJ_F_NPC_AI_DATA, v2);
                    break;
                case PROTO_F_MATERIAL:
                    obj_f_set_int32(obj, OBJ_F_MATERIAL, v2);
                    break;
                case PROTO_F_HIT_POINTS:
                    sub_49BB40(obj, v2);
                    break;
                case PROTO_F_SCRIPT:
                    scr[2] = v3;
                    scr[1] = v4;
                    sub_4078A0(obj, OBJ_F_SCRIPTS_IDX, v2, scr);
                    break;
                case PROTO_F_DAMAGE_RESISTANCE:
                    obj_f_set_int32_idx(obj, OBJ_F_RESISTANCE_IDX, 0, v2);
                    break;
                case PROTO_F_FIRE_RESISTANCE:
                    obj_f_set_int32_idx(obj, OBJ_F_RESISTANCE_IDX, 1, v2);
                    break;
                case PROTO_F_ELECTRICAL_RESISTANCE:
                    obj_f_set_int32_idx(obj, OBJ_F_RESISTANCE_IDX, 2, v2);
                    break;
                case PROTO_F_POISON_RESISTANCE:
                    obj_f_set_int32_idx(obj, OBJ_F_RESISTANCE_IDX, 3, v2);
                    break;
                case PROTO_F_MAGIC_RESISTANCE:
                    obj_f_set_int32_idx(obj, OBJ_F_RESISTANCE_IDX, 4, v2);
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
                    obj_f_set_int32(obj, OBJ_F_SOUND_EFFECT, v2);
                    break;
                case PROTO_F_INVENTORY_SOURCE:
                    obj_f_set_int32(obj, OBJ_F_CRITTER_INVENTORY_SOURCE, v2);
                    break;
                case PROTO_F_PORTRAIT:
                    obj_f_set_int32(obj, OBJ_F_CRITTER_PORTRAIT, v2);
                    break;
                case PROTO_F_RETAIL_PRICE_MULTIPLIER:
                    obj_f_set_int32(obj, OBJ_F_NPC_RETAIL_PRICE_MULTIPLIER, v2);
                    break;
                case PROTO_F_SOCIAL_CLASS:
                    sub_45D030(obj, v2);
                    break;
                case PROTO_F_OBJECT_FLAG:
                    obj_f_set_int32(obj, OBJ_F_FLAGS, obj_f_get_int32(obj, OBJ_F_FLAGS) | v2);
                    break;
                case PROTO_F_AUTO_LEVEL_SCHEME:
                    obj_f_set_int32(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, v2);
                    break;
                case PROTO_F_CATEGORY:
                    obj_f_set_int32(obj, OBJ_F_CATEGORY, v2);
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
    sub_43D4E0(obj, sub_43D4C0(obj) + a2 - sub_43D5A0(obj));
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
                *a3 = stat;
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
