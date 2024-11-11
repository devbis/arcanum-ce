#include "game/name.h"

#include <stdio.h>

#include "game/a_name.h"
#include "game/mes.h"

static void sub_41CE60();
static bool sub_41D1F0();
static bool sub_41D2C0();
static int sub_41D390();
static void sub_41F240();

// 0x5A1064
static const char* off_5A1064 = "art";

// 0x5A1068
static const char* off_5A1068 = "art\\missing.dat";

// 0x5A15C0
static int dword_5A15C0 = 10;

// 0x5A15C4
static int dword_5A15C4 = 250;

// 0x5D55F0
static int dword_5D55F0;

// 0x5D55F4
static mes_file_handle_t name_monster_mes_file;

// 0x5D55F8
static mes_file_handle_t name_eye_candy_mes_file;

// 0x5D55FC
static mes_file_handle_t name_unique_npc_mes_file;

// 0x5D5600
static int dword_5D5600;

// 0x5D5604
static mes_file_handle_t name_container_mes_file;

// 0x5D5608
static int* dword_5D5608;

// 0x5D560C
static int* dword_5D560C;

// 0x5D5610
static int* dword_5D5610;

// 0x5D5614
static mes_file_handle_t name_interface_mes_file;

// 0x5D5618
static int dword_5D5618;

// 0x5D561C
static mes_file_handle_t name_scenery_mes_file;

// 0x5D5620
static bool name_initialized;

// 0x5D5624
static bool dword_5D5624;

// 0x739E48
static int dword_739E48;

// 0x739E54
static int dword_739E54;

// 0x41CA40
bool name_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (name_initialized) {
        return true;
    }

    if (!mes_load("art\\scenery\\scenery.mes", &name_scenery_mes_file)) {
        return false;
    }

    if (!mes_load("art\\interface\\interface.mes", &name_interface_mes_file)) {
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!mes_load("art\\unique_npc\\unique_npc.mes", &name_unique_npc_mes_file)) {
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!mes_load("art\\monster\\monster.mes", &name_monster_mes_file)) {
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!mes_load("art\\eye_candy\\eye_candy.mes", &name_eye_candy_mes_file)) {
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!mes_load("art\\container\\container.mes", &name_container_mes_file)) {
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_light_init()) {
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_tile_init()) {
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_roof_init()) {
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_wall_init()) {
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_portal_init()) {
        a_name_wall_exit();
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_item_init()) {
        a_name_portal_exit();
        a_name_wall_exit();
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    if (!a_name_facade_init()) {
        a_name_item_exit();
        a_name_portal_exit();
        a_name_wall_exit();
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();
        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);
        return false;
    }

    name_initialized = true;

    sub_4EBC40();
    sub_41CE60();

    return true;
}

// 0x41CDA0
void name_exit()
{
    if (name_initialized) {
        free(dword_5D560C);
        free(dword_5D5608);
        free(dword_5D5610);

        a_name_facade_exit();
        a_name_item_exit();
        a_name_portal_exit();
        a_name_wall_exit();
        a_name_roof_exit();
        a_name_tile_exit();
        a_name_light_exit();

        mes_unload(name_container_mes_file);
        mes_unload(name_eye_candy_mes_file);
        mes_unload(name_monster_mes_file);
        mes_unload(name_unique_npc_mes_file);
        mes_unload(name_interface_mes_file);
        mes_unload(name_scenery_mes_file);

        name_initialized = false;
        dword_5D5624 = false;
    }
}

// 0x41CE60
void sub_41CE60()
{
    // TODO: Incomplete.
}

// 0x41D1F0
bool sub_41D1F0()
{
    TigFile* stream;
    int expected_cnt;
    int actual_cnt;

    stream = tig_file_fopen(off_5A1068, "rb");
    if (stream == NULL) {
        return false;
    }

    expected_cnt = sub_41D390();

    if (tig_file_fread(&actual_cnt, sizeof(actual_cnt), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    if (expected_cnt != actual_cnt) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fread(dword_5D560C, sizeof(*dword_5D560C), dword_5D55F0, stream) < dword_5D55F0) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fread(dword_5D5608, sizeof(*dword_5D5608), dword_5D5618, stream) < dword_5D5618) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fread(dword_5D5610, sizeof(*dword_5D5610), dword_5D5600, stream) < dword_5D5618) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);

    return true;
}

// 0x41D2C0
bool sub_41D2C0()
{
    int cnt;
    TigFile* stream;

    cnt = sub_41D390();
    tig_file_mkdir(off_5A1064);

    stream = tig_file_fopen(off_5A1068, "wb");
    if (stream == NULL) {
        return false;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fwrite(dword_5D560C, sizeof(*dword_5D560C), dword_5D55F0, stream) != dword_5D55F0) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fwrite(dword_5D5608, sizeof(*dword_5D5608), dword_5D5618, stream) != dword_5D5618) {
        tig_file_fclose(stream);
        return false;
    }

    if (tig_file_fwrite(dword_5D5610, sizeof(*dword_5D5610), dword_5D5600, stream) != dword_5D5600) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);

    return true;
}

// 0x41D390
int sub_41D390()
{
    int cnt = 0;
    TigFileList dir_list;
    TigFileList file_list;
    char path[MAX_PATH];
    unsigned int index;

    tig_file_list_create(&dir_list, "art\\critter\\*.*");
    for (index = 0; index < dir_list.count; index++) {
        if (dir_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\critter\\%s\\*.art", dir_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            cnt += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&dir_list);

    tig_file_list_create(&dir_list, "art\\monster\\*.*");
    for (index = 0; index < dir_list.count; index++) {
        if (dir_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\monster\\%s\\*.art", dir_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            cnt += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&dir_list);

    tig_file_list_create(&dir_list, "art\\unique_npc\\*.*");
    for (index = 0; index < dir_list.count; index++) {
        if (dir_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\unique_npc\\%s\\*.art", dir_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            cnt += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&dir_list);

    return cnt;
}

// 0x41D510
tig_art_id_t sub_41D510(tig_art_id_t aid)
{
    // TODO: Incomplete.
}

// 0x41DA40
void sub_41DA40()
{
    // TODO: Incomplete.
}

// 0x41DAE0
void sub_41DAE0()
{
    // TODO: Incomplete.
}

// 0x41DFC0
void sub_41DFC0()
{
    // TODO: Incomplete.
}

// 0x41E200
void sub_41E200()
{
    // TODO: Incomplete.
}

// 0x41E960
void sub_41E960()
{
    // TODO: Incomplete.
}

// 0x41F090
void sub_41F090()
{
    // TODO: Incomplete.
}

// 0x41F190
void sub_41F190()
{
    // TODO: Incomplete.
}

// 0x41F240
void sub_41F240()
{
    TigRect dst_rect;
    TigRect src_rect;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_spec;

    dst_rect.x = 10;
    dst_rect.y = 10;
    dst_rect.width = 80;
    dst_rect.height = 80;

    tig_window_fill(dword_739E48, &dst_rect, tig_color_make(0, 0, 60));

    if (dword_739E54 != TIG_ART_ID_INVALID) {
        if (tig_art_frame_data(dword_739E54, &art_frame_data) == TIG_OK) {
            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.height;

            if (art_frame_data.width > 80 || art_frame_data.height > 80) {
                if (80.0 / (double)art_frame_data.height < 80.0 / (double)art_frame_data.width) {
                    art_frame_data.width = (int)((double)art_frame_data.width * (80.0 / (double)art_frame_data.height));
                } else {
                    art_frame_data.height = (int)((double)art_frame_data.height * (80.0 / (double)art_frame_data.width));
                }
            } else {
                dst_rect.width = art_frame_data.width;
                dst_rect.height = art_frame_data.height;
            }

            dst_rect.x += (80 - dst_rect.width) / 2;
            dst_rect.y += (80 - dst_rect.height) / 2;

            art_blit_spec.flags = TIG_ART_BLT_PALETTE_ORIGINAL;
            art_blit_spec.art_id = dword_739E54;
            art_blit_spec.src_rect = &src_rect;
            art_blit_spec.dst_rect = &dst_rect;
            tig_window_blit_art(dword_739E48, &art_blit_spec);
        }
    }
}

// 0x41F3C0
bool sub_41F3C0(PathCreateInfo* path_create_info)
{
    // TODO: Incomplete.
}

// 0x41F570
void sub_41F570()
{
    // TODO: Incomplete.
}

// 0x41F6C0
void sub_41F6C0()
{
    // TODO: Incomplete.
}

// 0x41F840
void sub_41F840()
{
    // TODO: Incomplete.
}

// 0x41F9F0
void sub_41F9F0()
{
    // TODO: Incomplete.
}

// 0x4200C0
void sub_4200C0()
{
    // TODO: Incomplete.
}

// 0x420110
void sub_420110()
{
    // TODO: Incomplete.
}

// 0x4201C0
int sub_4201C0(int64_t a1, int64_t a2, void* a3)
{
    // TODO: Incomplete.
}

// 0x420330
void sub_420330()
{
    // TODO: Incomplete.
}

// 0x4203B0
void sub_4203B0()
{
    // TODO: Incomplete.
}

// 0x420660
void sub_420660()
{
    // TODO: Incomplete.
}

// 0x4207D0
void sub_4207D0()
{
    // TODO: Incomplete.
}

// 0x420900
void sub_420900()
{
    // TODO: Incomplete.
}

// 0x4209C0
void sub_4209C0()
{
    // TODO: Incomplete.
}

// 0x420E30
void sub_420E30()
{
    // TODO: Incomplete.
}

// 0x421AC0
bool sub_421AC0(int a1)
{
    if (a1 <= 0 || a1 >= 35) {
        return false;
    }

    dword_5A15C0 = a1;
    return true;
}

// 0x421AE0
bool sub_421AE0(int a1)
{
    if (a1 <= 0 || a1 >= 1000) {
        return false;
    }

    dword_5A15C4 = a1;
    return true;
}
