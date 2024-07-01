#include "game/a_name.h"

static bool build_roof_file_name(int index, char* buffer);
static bool load_roof_data();

// 0x603B6C
static bool light_initialized;

// 0x603B70
static int light_mes_file;

// 0x603B74
static char **roof_file_names;

// 0x603B78
static int roofname_mes_file;

// 0x603B7C
static int num_roof_file_names;

// 0x603B80
static int roof_initialized;

// 0x4ED1E0
bool a_name_light_init()
{
    if (light_initialized) {
        return true;
    }

    if (!mes_load("art\\light\\light.mes", &light_mes_file)) {
        return false;
    }

    return light_initialized;
}

// 0x4ED220
void a_name_light_exit()
{
    if (light_initialized) {
        mes_unload(light_mes_file);
        light_initialized = false;
    }
}

// 0x4ED250
bool a_name_light_aid_to_fname(tig_art_id_t aid, char* fname)
{
    MesFileEntry mes_file_entry;

    if (!light_initialized) {
        return false;
    }

    mes_file_entry.key = tig_art_num(aid);
    if (!mes_search(light_mes_file, &mes_file_entry)) {
        return false;
    }

    if (sub_504790(aid)) {
        sprintf(fname, "art\\light\\%s_s%d.art", mes_file_entry.text, sub_504700(aid) / 8);
    } else {
        sprintf(fname, "art\\light\\%s.art", mes_file_entry.text);
    }

    return true;
}

// 0x4ED2F0
bool a_name_roof_init()
{
    if (!load_roof_data()) {
        return false;
    }

    roof_initialized = true;

    return true;
}

// 0x4ED310
void a_name_roof_exit()
{
    if (roof_initialized) {
        mes_unload(roofname_mes_file);
        FREE(roof_file_names);
        roof_initialized = false;
    }
}

// 0x4ED370
bool a_name_roof_aid_to_fname(tig_art_id_t aid, char* fname)
{
    if (tig_art_type(aid) != TIG_ART_TYPE_ROOF) {
        return false;
    }

    return build_roof_file_name(tig_art_num(aid), fname);
}

// 0x4ED3A0
bool build_roof_file_name(int index, char* fname)
{
    if (index >= num_roof_file_names) {
        return false;
    }

    sprintf(fname, "art\\roof\\%s.art", roof_file_names[index]);

    return true;
}

// 0x4ED3E0
bool a_name_roof_fname_to_aid(const char* fname, tig_art_id_t* aid_ptr)
{
    int index;

    for (index = 0; index < num_roof_file_names; index++) {
        if (strcmpi(fname, roof_file_names[index]) == 0) {
            tig_art_roof_id_create(index, 0, 0, 0, aid_ptr);
            return true;
        }
    }

    return false;
}

// 0x4ED440
bool load_roof_data()
{
    MesFileEntry mes_file_entry;
    int index;

    num_roof_file_names = 0;
    roof_file_names = NULL;

    if (!mes_load("art\\roof\\roofname.mes", &roofname_mes_file)) {
        return false;
    }

    num_roof_file_names = mes_entries_count(roofname_mes_file);
    if (num_roof_file_names == 0) {
        mes_unload(roofname_mes_file);
        return false;
    }

    mes_file_entry.num = 0;
    if (!mes_search(roofname_mes_file, &mes_file_entry)) {
        mes_unload(roofname_mes_file);
        return false;
    }

    roof_file_names = MALLOC(sizeof(char*) * num_roof_file_names);
    do {
        roof_file_names[index] = mes_file_entry.text;
    } while (mes_find_next(roofname_mes_file, &mes_file_entry));

    return true;
}
