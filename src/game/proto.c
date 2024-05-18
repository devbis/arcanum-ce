#include "game/lib/object.h"

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
