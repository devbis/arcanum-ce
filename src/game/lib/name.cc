#include "game/lib/name.h"

#include <stdio.h>

#include "game/lib/message.h"
#include "tig/art.h"
#include "tig/color.h"
#include "tig/file.h"
#include "tig/window.h"

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
static int name_monster_msg_file;

// 0x5D55F8
static int name_eye_candy_msg_file;

// 0x5D55FC
static int name_unique_npc_msg_file;

// 0x5D5600
static int dword_5D5600;

// 0x5D5604
static int name_container_msg_file;

// 0x5D5608
static int* dword_5D5608;

// 0x5D560C
static int* dword_5D560C;

// 0x5D5610
static int* dword_5D5610;

// 0x5D5614
static int name_interface_msg_file;

// 0x5D5618
static int dword_5D5618;

// 0x5D561C
static int name_scenery_msg_file;

// 0x5D5620
static bool dword_5D5620;

// 0x5D5624
static bool dword_5D5624;

// 0x739E48
static int dword_739E48;

// 0x739E54
static int dword_739E54;

// 0x41CA40
bool name_init(GameContext* ctx)
{
    if (dword_5D5620) {
        return true;
    }

    if (!message_load("art\\scenery\\scenery.mes", &name_scenery_msg_file)) {
        return false;
    }

    if (!message_load("art\\interface\\interface.mes", &name_interface_msg_file)) {
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!message_load("art\\unique_npc\\unique_npc.mes", &name_unique_npc_msg_file)) {
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!message_load("art\\monster\\monster.mes", &name_monster_msg_file)) {
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!message_load("art\\eye_candy\\eye_candy.mes", &name_eye_candy_msg_file)) {
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!message_load("art\\container\\container.mes", &name_container_msg_file)) {
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!sub_4ED1E0()) {
        message_unload(name_container_msg_file);
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!sub_4EAC80()) {
        sub_4ED220();
        message_unload(name_container_msg_file);
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!sub_4ED2F0()) {
        sub_4EAD50();
        sub_4ED220();
        message_unload(name_container_msg_file);
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!sub_4EC9B0()) {
        sub_4ED310();
        sub_4EAD50();
        sub_4ED220();
        message_unload(name_container_msg_file);
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!sub_4EC5A0()) {
        sub_4EC9E0();
        sub_4ED310();
        sub_4EAD50();
        sub_4ED220();
        message_unload(name_container_msg_file);
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!sub_4EC190()) {
        sub_4EC610();
        sub_4EC9E0();
        sub_4ED310();
        sub_4EAD50();
        sub_4ED220();
        message_unload(name_container_msg_file);
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    if (!sub_4EC370()) {
        sub_4EC250();
        sub_4EC610();
        sub_4EC9E0();
        sub_4ED310();
        sub_4EAD50();
        sub_4ED220();
        message_unload(name_container_msg_file);
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);
        return false;
    }

    dword_5D5620 = true;

    sub_4EBC40();
    sub_41CE60();

    return true;
}

// 0x41CDA0
void name_exit()
{
    if (dword_5D5620) {
        free(dword_5D560C);
        free(dword_5D5608);
        free(dword_5D5610);

        sub_4EC390();
        sub_4EC250();
        sub_4EC610();
        sub_4EC9E0();
        sub_4ED310();
        sub_4EAD50();
        sub_4ED220();

        message_unload(name_container_msg_file);
        message_unload(name_eye_candy_msg_file);
        message_unload(name_monster_msg_file);
        message_unload(name_unique_npc_msg_file);
        message_unload(name_interface_msg_file);
        message_unload(name_scenery_msg_file);

        dword_5D5620 = false;
        dword_5D5624 = false;
    }
}

// 0x41D1F0
bool sub_41D1F0()
{
    TigFile* stream = tig_file_fopen(off_5A1068, "rb");
    if (stream == NULL) {
        return false;
    }

    int expected_count = sub_41D390();

    int count;
    if (tig_file_fread(&count, sizeof(count), 1, stream) != 1) {
        tig_file_fclose(stream);
        return false;
    }

    if (expected_count != count) {
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
}

// 0x41D2C0
bool sub_41D2C0()
{
    int count = sub_41D390();
    sub_52DFE0(off_5A1064);

    TigFile* stream = tig_file_fopen(off_5A1068, "wb");
    if (stream == NULL) {
        return false;
    }

    if (tig_file_fwrite(&count, sizeof(count), 1, stream) != 1) {
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
    int count = 0;
    TigFileList directory_list;
    TigFileList file_list;
    char path[MAX_PATH];

    tig_file_list_create(&directory_list, "art\\critter\\*.*");
    for (int index = 0; index < directory_list.count; index++) {
        if (directory_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\critter\\%s\\*.art", directory_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            count += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&directory_list);

    tig_file_list_create(&directory_list, "art\\monster\\*.*");
    for (int index = 0; index < directory_list.count; index++) {
        if (directory_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\monster\\%s\\*.art", directory_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            count += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&directory_list);

    tig_file_list_create(&directory_list, "art\\unique_npc\\*.*");
    for (int index = 0; index < directory_list.count; index++) {
        if (directory_list.entries[index].path[0] != '.') {
            sprintf(path, "art\\unique_npc\\%s\\*.art", directory_list.entries[index].path);
            tig_file_list_create(&file_list, path);
            count += file_list.count;
            tig_file_list_destroy(&file_list);
        }
    }
    tig_file_list_destroy(&directory_list);

    return count;
}

// 0x41F240
void sub_41F240()
{
    TigRect dst_rect;
    TigRect src_rect;
    TigArtFrameData art_frame_data;
    TigArtBlitSpec art_blit_spec;

    dst_rect.x = 10;
    dst_rect.y = 10;
    dst_rect.width = 80;
    dst_rect.height = 80;

    tig_window_fill(dword_739E48, &dst_rect, tig_color_make(0, 0, 60));

    if (dword_739E54 != -1) {
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

            art_blit_spec.field_0 = 4;
            art_blit_spec.src_art_id = dword_739E54;
            art_blit_spec.src_rect = &src_rect;
            art_blit_spec.dst_rect = &dst_rect;
            tig_window_blit_art(dword_739E48, &art_blit_spec);
        }
    }
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
