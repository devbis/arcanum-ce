#include "ui/wmap_ui.h"

#include <tig/tig.h>

// 0x55F8D0
void wmap_ui_init()
{
    // TODO: Incomplete.
}

// 0x55FF80
void wmap_ui_exit()
{
    // TODO: Incomplete.
}

// 0x560010
void sub_560010()
{
    wmap_ui_town_notes_save();
    dword_5C991C = 0;
    if (dword_5C9928 != NULL) {
        FREE(dword_5C9928);
        dword_5C9928 = NULL;
    }
}

// 0x560040
void wmap_ui_town_notes_save()
{
    // TODO: Incomplete.
}

// 0x560150
void sub_560150()
{
    // TODO: Incomplete.
}

// 0x560200
void wmap_ui_reset()
{
    // TODO: Incomplete.
}

// 0x560280
void wmap_ui_save()
{
    // TODO: Incomplete.
}

// 0x560350
void sub_560350()
{
    // TODO: Incomplete.
}

// 0x560440
void sub_560440()
{
    // TODO: Incomplete.
}

// 0x5604B0
void wmap_ui_load()
{
    // TODO: Incomplete.
}

// 0x5605C0
void sub_5605C0()
{
    // TODO: Incomplete.
}

// 0x5606B0
void sub_5606B0()
{
    // TODO: Incomplete.
}

// 0x560720
void sub_560720()
{
    dword_66D9C4 = 1;
    sub_57D370(13, 0, 4000);
}

// 0x560740
int sub_560740()
{
    return dword_66D9C4;
}

// 0x560750
void sub_560750()
{
    dword_66D9C4 = 0;
}

// 0x560760
void sub_560760()
{
    dword_5C9220 = -1;
    dword_66D9C8 = 0;
    qword_66D888 = 0;
    sub_5607E0(0);
}

// 0x560790
void sub_560790()
{
    // TODO: Incomplete.
}

// 0x5607E0
void sub_5607E0()
{
    // TODO: Incomplete.
}

// 0x560AA0
void wmap_load_worldmap_info()
{
    // TODO: Incomplete.
}

// 0x560EE0
void sub_560EE0()
{
}

// 0x560EF0
void sub_560EF0()
{
    dword_66D880 = 0;
    dword_66D874 = sub_4BE380(sub_4CFC50(obj_field_int64_get(player_get_pc_obj(), OBJ_F_LOCATION)));
    if (sub_40FF40() == dword_66D87C) {
        if (dword_66D874 == 0) {
            dword_66D880 = 1;
        }
    }
}

// 0x560F40
void sub_560F40()
{
    // TODO: Incomplete.
}

// 0x560F90
int sub_560F90()
{
    return dword_66D864;
}

// 0x560FA0
void wmap_ui_create()
{
    // TODO: Incomplete.
}

// 0x561430
void sub_561430(long long location)
{
    int coords[2];
    int v1;
    int v2;

    sub_561490(location, coords);

    if (coords[0] > 99999 || coords[0] == 0) {
        v1 = 200;
    }

    if (coords[1] > 99999 || coords[1] == 0) {
        v2 = 180;
    }

    sub_5614C0(v1, v2)
}

// 0x561490
void sub_561490(long long location, int* coords)
{
    coords[0] = dword_66D6F8 - ((location >> 6) & 0x3FFFFFF);
    coords[1] = (location >> 32) >> 6;
}

// 0x5614C0
void sub_5614C0(int a1, int a2)
{
    stru_5C9228[dword_66D868].field_3C = a1;
    stru_5C9228[dword_66D868].field_40 = a2;
}

// 0x5614F0
bool sub_5614F0()
{
    if (dword_66D868) {
        return false;
    }

    if (sub_40FF40() == sub_40FF50(1)) {
        return false;
    }

    return true;
}

// 0x5615D0
void sub_5615D0()
{
    // TODO: Incomplete.
}

// 0x561800
void sub_561800()
{
    // TODO: Incomplete.
}

// 0x561860
void sub_561860(long long a1)
{
    TeleportData teleport_data;

    sector_flush(0);

    teleport_data.flags = 0x20;
    teleport_data.field_8 = player_get_pc_obj();
    teleport_data.field_10 = a1;
    teleport_data.field_18 = sub_40FF50(1)
    sub_4D3380(&teleport_data);
}

// 0x5618B0
void sub_5618B0()
{
    // TODO: Incomplete.
}

// 0x5627B0
bool sub_5627B0(int a1)
{
    long long v1;

    if (dword_66D868 == 2) {
        return 1;
    }

    sub_561800(a1, &v1);
    return sub_5627F0(v1);
}

// 0x5627F0
bool sub_5627F0(long long a1)
{
    return true;
}

// 0x562800
void sub_562800()
{
    // TODO: Incomplete.
}

// 0x562880
void sub_562880()
{
    // TODO: Incomplete.
}

// 0x562A20
void sub_562A20()
{
    // TODO: Incomplete.
}

// 0x562AF0
void sub_562AF0()
{
    // TODO: Incomplete.
}

// 0x562B70
void sub_562B70()
{
    // TODO: Incomplete.
}

// 0x562D80
void wmap_load_townmap_info()
{
    // TODO: Incomplete.
}

// 0x562F90
void sub_562F90()
{
    // TODO: Incomplete.
}

// 0x562FA0
void sub_562FA0(int a1)
{
    wmTileArtLockMode(dword_66D868, a1);
}

// 0x562FC0
void sub_562FC0(int a1)
{
    sub_563200(dword_66D868, a1);
}

// 0x562FE0
void sub_562FE0(int a1)
{
    sub_563210(dword_66D868, a1);
}

// 0x563000
bool wmTileArtLockMode(int a1, int a2)
{
    char path[TIG_MAX_PATH];

    if ((stru_5C9228[a1].field_180[a2].flags & 0x1) == 0) {
        if (a1 == 2) {
            snprintf(path, sizeof(path),
                "townmap\\%s\\%s%06d.bmp",
                stru_5C9228[a1].field_68,
                stru_5C9228[a1].field_68,
                a2);
            if (!tig_file_exists(path, NULL)) {
                snprintf(path, sizeof(path),
                    "townmap\\%s\\%s%06d.bmp",
                    stru_5C9228[a1].field_68,
                    stru_5C9228[a1].field_68,
                    0);
            }
        } else {
            snprintf(path, sizeof(path),
                "%s%03d",
                stru_5C9228[a1].field_68,
                a2 + 1);
        }

        if (sub_5630F0(path, &(stru_5C9228[a1].field_180[a2].video_buffer), &(stru_5C9228[a1].field_180[a2].field_8))) {
            stru_5C9228[a1].field_180[a2].flags |= 0x1;
            stru_5C9228[a1].field_184++;
        } else {
            tig_debug_printf("WMapUI: Blit: ERROR: Bmp Load Failed!\n");
        }
    }

    return true;
}

// 0x5630F0
bool sub_5630F0(const char* path, TigVideoBuffer** video_buffer_ptr, TigRect* rect)
{
    TigBmp bmp;
    TigVideoBufferData video_buffer_data;

    if (dword_66D868 != 2) {
        snprintf(bmp.path, sizeof(bmp.path), "WorldMap\\%s.bmp", path);
        path = bmp.path;
    }

    if (tig_video_buffer_load_from_bmp(path, video_buffer_ptr, 0x1) != TIG_OK) {
        tig_debug_printf("WMapUI: Pic [%s] FAILED to load!\n", path);
        return false;
    }

    if (tig_video_buffer_data(*video_buffer_ptr, &video_buffer_data) != TIG_OK) {
        return false;
    }

    rect->x = 0;
    rect->y = 0;
    rect->width = video_buffer_data.width;
    rect->height = video_buffer_data.height;

    return true;
}

// 0x563200
bool sub_563200(int a1, int a2)
{
    return true;
}

// 0x563210
void sub_563210(int a1, int a2)
{
    if ((stru_5C9228[a1].field_180[a2].flags & 0x1) != 0) {
        if (tig_video_buffer_destroy(stru_5C9228[a1].field_180[a2].video_buffer) != TIG_OK) {
            tig_debug_printf("WMapUI: Destroy: ERROR: Video Buffer Destroy FAILED!\n");
            return;
        }

        stru_5C9228[a1].field_180[a2].flags &= ~0x1;
        stru_5C9228[a1].field_184--;
    }
}

// 0x563270
void sub_563270()
{
    if (stru_5C9228[dword_66D868].field_4C != NULL) {
        stru_5C9228[dword_66D868].field_4C(&(stru_5C9228[dword_66D868].field_4));
    }
}

// 0x5632A0
void sub_5632A0(int direction)
{
    switch (direction) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (stru_5C9228[dword_66D868].field_4C != NULL) {
            stru_5C9228[dword_66D868].field_4C(&(stru_5C9228[dword_66D868].field_4));
        }
        break;
    default:
        tig_debug_printf("WMapUI: Error: Refresh Scroll Direction out of bounds!\n");
        break;
    }
}

// 0x563300
void sub_563300()
{
    // TODO: Incomplete.
}

// 0x563590
void sub_563590()
{
    // TODO: Incomplete.
}

// 0x563610
void sub_563610()
{
    // TODO: Incomplete.
}

// 0x563750
void sub_563750(int a1)
{
    sub_563790(a1, 4);
}

// 0x563770
void sub_563770(int a1)
{
    if (dword_66D8AC != 2) {
        sub_563790(a1, 8);
    }
}

// 0x563790
void sub_563790()
{
    // TODO: Incomplete.
}

// 0x563AC0
void sub_563AC0(int x, int y, int* coords)
{
    coords[0] = x + stru_5C9228[dword_66D868].field_34 - stru_5C9228[dword_66D868].field_4.x;
    coords[1] = y + stru_5C9228[dword_66D868].field_38 - stru_5C9228[dword_66D868].field_4.y - stru_5C9B08.y;
}

// 0x563B10
void sub_563B10()
{
    // TODO: Incomplete.
}

// 0x563C00
void sub_563C00(int x, int y, int* coords)
{
    coords[0] = x + stru_5C9228[dword_66D868].field_34 - stru_5C9228[dword_66D868].field_4.x;
    coords[1] = y + stru_5C9228[dword_66D868].field_38 - stru_5C9228[dword_66D868].field_4.y - stru_5C9B08.y;
}

// 0x563C60
void sub_563C60()
{
    // TODO: Incomplete.
}

// 0x563D50
void sub_563D50(WmapNote* note)
{
    note->field_18.x = note->x;
    note->field_18.y = note->y;
    note->field_18.width = dword_66D890 + 203;
    note->field_18.height = 50;
}

// 0x563D80
void sub_563D80(int a1, int a2)
{
}

// 0x563D90
WmapNote* sub_563D90(int id)
{
    int index;

    for (index = 0; index < *stru_5C9228[dword_66D868].num_notes; index++) {
        if (stru_5C9228[dword_66D868].notes[index].id == id) {
            return &(stru_5C9228[dword_66D868].notes[index]);
        }
    }

    return NULL;
}

// 0x563DE0
void sub_563DE0()
{
    // TODO: Incomplete.
}

// 0x563E00
void sub_563E00()
{
    // TODO: Incomplete.
}

// 0x563F00
void sub_563F00()
{
    // TODO: Incomplete.
}

// 0x563F90
void sub_563F90(int* coords)
{
    stru_66D718.str[0] = '\0';
    stru_66D718.field_28 = dword_66D89C;
    stru_66D718.x = coords[0];
    stru_66D718.y = coords[1];
    sub_563D50(&stru_66D718);
    stru_66D718.field_10 = 0;
    stru_66D718.field_14 = 0;
    sub_563F00(&stru_66D718.x, &stru_66D718.field_10);
    sub_564030(&stru_66D718);
}

// 0x564000
void sub_564000(int a1)
{
    int v1[3];
    v1[0] = 0;
    v1[1] = 1;
    v1[2] = 2;
    dword_66D89C = v1[a1];
}

// 0x564030
void sub_564030(WmapNote* note)
{
    sub_5506C0(6);
    dword_66D9D4 = note;
    stru_5C9B20.buffer = note->str;
    textedit_ui_focus(&stru_5C9B20);
    dword_66D898 = 1;
    sub_552160(note->str, stru_5C9160[0].font);
}

// 0x564070
void sub_564070(bool a1)
{
    dword_66D9D4 = 0;
    stru_66D718.str[0] = '\0';
    stru_66D718.field_0 = -1;
    dword_66D898 = 0;
    textedit_ui_unfocus();
    stru_5C9B20.buffer = NULL;
    if (a1) {
        sub_552160(" ", stru_5C9160[0].font);
    }
}

// 0x5640C0
void sub_5640C0(TextEdit* textedit)
{
    bool v1;

    if (*textedit->buffer != '\0' && sub_5643C0(textedit->buffer)) {
        if (textedit->buffer == stru_66D718.str) {
            dword_66D9D4->field_4 = 0;
            v1 = sub_564140(dword_66D9D4);
        } else {
            v1 = true;
        }
    } else {
        v1 = sub_564210(dword_66D9D4);
    }

    sub_564070(1);

    if (v1) {
        stru_5C9228[dword_66D868].field_48();
    }
}

// 0x564140
bool sub_564140(WmapNote* note)
{
    return sub_564160(a1, dword_66D868);;
}

// 0x564160
bool sub_564160(WmapNote* note, int a2)
{
    int note_index;

    if (stru_5C9228[a2].notes == NULL
        || *stru_5C9228[a2].num_notes >= 199) {
        return false;
    }

    note_index = *stru_5C9228[a2].num_notes;

    note->field_0 = stru_5C9228[a2].field_194++;
    note->field_4 &= ~0x4;
    stru_5C9228[a2].notes[note_index] = *note;
    sub_563D50(&(stru_5C9228[a2].notes[note_index]));
    (*stru_5C9228[a2].num_notes)++;
    sub_563D80(stru_5C9228[a2].notes[note_index].field_0, a2);

    return true;
}

// 0x564210
void sub_564210()
{
    // TODO: Incomplete.
}

// 0x5642E0
void sub_5642E0()
{
}

// 0x5642F0
void sub_5642F0(WmapNote* note)
{
    if ((note->field_4 & 0x4) != 0) {
        tig_video_buffer_destroy(note->video_buffer);
        note->field_4 &= ~0x4;
    }
}

// 0x564320
void sub_564320(TextEdit* textedit)
{
    if (*textedit->buffer != '\0') {
        sub_552160(textedit->buffer, dword_5C9170);
    } else {
        sub_552160(" ", dword_5C9170);
    }
}

// 0x564360
void sub_564360()
{
    // TODO: Incomplete.
}

// 0x5643C0
void sub_5643C0()
{
    // TODO: Incomplete.
}

// 0x5643E0
void sub_5643E0()
{
    // TODO: Incomplete.
}

// 0x564780
void sub_564780()
{
    // TODO: Incomplete.
}

// 0x564830
void sub_564830()
{
}

// 0x564840
void sub_564840()
{
    // TODO: Incomplete.
}

// 0x5648E0
void sub_5648E0()
{
    // TODO: Incomplete.
}

// 0x564940
void sub_564940()
{
    // TODO: Incomplete.
}

// 0x564970
void sub_564970()
{
    // TODO: Incomplete.
}

// 0x5649C0
void sub_5649C0()
{
}

// 0x5649D0
void sub_5649D0()
{
}

// 0x5649F0
void sub_5649F0()
{
    // TODO: Incomplete.
}

// 0x564A70
void sub_564A70(long long a1, long long a2)
{
    int v1;

    v1 = sub_4CB4D0(a2, 0);
    if (v1 > 0) {
        if (!sub_4CAF50(a1, v1)) {
            if (sub_4CB100(a1) == v1) {
                sub_4CB160(a1);
            }
            sub_4CAFD0(a1, v1);
            sub_562800(v1);
        }

        if (sub_4CB100(a1) == v1) {
            sub_4CB160(a1);
        }
    }
}

// 0x564AF0
void sub_564AF0()
{
    // TODO: Incomplete.
}

// 0x564C20
void wmap_ui_bkg_process_callback()
{
    // TODO: Incomplete.
}

// 0x564E30
void sub_564E30()
{
    // TODO: Incomplete.
}

// 0x564EE0
void sub_564EE0()
{
    // TODO: Incomplete.
}

// 0x564F60
void sub_564F60(long long a1, long long a2)
{
    dword_66D874 = sub_4BE380(a2);
    if (dword_66D874) {
        sub_564A70(a1, sub_4CFC90(a2));
        wmap_ui_town_notes_load();
        sub_4607B0(2);
    } else {
        sub_5650C0();
        sub_4607B0(3);
    }
    sub_560EF0();
}

// 0x564FD0
void wmap_ui_town_notes_load()
{
    // TODO: Incomplete.
}

// 0x5650C0
void sub_5650C0()
{
    if (dword_66D878) {
        wmap_ui_town_notes_save();
    }
    dword_66D878 = 0;
    dword_66D870 = 0;
}

// 0x5650E0
void sub_5650E0()
{
    // TODO: Incomplete.
}

// 0x565130
void sub_565130()
{
}

// 0x565140
bool sub_565140()
{
    if (!sub_424070(player_get_pc_obj()), 3, 0, 1) {
        return false;
    }

    sub_457450(player_get_pc_obj());

    return true;
}

// 0x565170
void sub_565170()
{
    // TODO: Incomplete.
}

// 0x565230
void sub_565230()
{
    // TODO: Incomplete.
}

// 0x5656B0
void sub_5656B0()
{
    // TODO: Incomplete.
}

// 0x5657A0
void sub_5657A0()
{
    // TODO: Incomplete.
}

// 0x565CF0
bool sub_565CF0(WmapNote* note)
{
    return true;
}

// 0x565D00
void sub_565D00()
{
    // TODO: Incomplete.
}

// 0x565E40
void wmap_note_vbid_lock(WmapNote* note)
{
    TigVideoBufferCreateInfo vb_create_info;
    TigRect dirty_rect;

    if ((note->field_4 & 0x4) == 0) {
        vb_create_info.flags = TIG_VIDEO_BUFFER_LOCKED | TIG_VIDEO_BUFFER_VIDEO_MEMORY;
        vb_create_info.width = dword_66D890 + 203;
        vb_create_info.height = 50;
        vb_create_info.background_color = dword_64E03C;
        vb_create_info.color_key = dword_64E03C;
        if (tig_video_buffer_create(&vb_create_info, &(note->video_buffer)) == TIG_OK) {
            tig_debug_printf("wmap_note_vbid_lock: ERROR: tig_video_buffer_create failed!\n");
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
        }

        note->field_4 |= 0x4;

        sub_565F00(note->video_buffer, &(note->field_18));
        tig_font_push(stru_5C9160[note->field_28].font);
        tig_font_write(note->video_buffer, note->str, &stru_5C9AC8, &dirty_rect);
        tig_font_pop();
    }
}

// 0x565F00
void sub_565F00()
{
    // TODO: Incomplete.
}

// 0x566130
void wmap_town_refresh_rect()
{
    // TODO: Incomplete.
}

// 0x566A80
void sub_566A80()
{
    // TODO: Incomplete.
}

// 0x566CC0
void sub_566CC0()
{
    // TODO: Incomplete.
}

// 0x566D10
void sub_566D10()
{
    // TODO: Incomplete.
}
