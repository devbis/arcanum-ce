#ifndef ARCANUM_TIG_ART_H_
#define ARCANUM_TIG_ART_H_

#include "tig/types.h"

#define TIG_ART_ID_INVALID ((tig_art_id_t)-1)

typedef enum TigArtType {
    TIG_ART_TYPE_TILE,
    TIG_ART_TYPE_WALL,
    TIG_ART_TYPE_CRITTER,
    TIG_ART_TYPE_PORTAL,
    TIG_ART_TYPE_SCENERY,
    TIG_ART_TYPE_INTERFACE,
    TIG_ART_TYPE_ITEM,
    TIG_ART_TYPE_CONTAINER,
    TIG_ART_TYPE_MISC,
    TIG_ART_TYPE_LIGHT,
    TIG_ART_TYPE_ROOF,
    TIG_ART_TYPE_FACADE,
    TIG_ART_TYPE_MONSTER,
    TIG_ART_TYPE_UNIQUE_NPC,
    TIG_ART_TYPE_EYE_CANDY,
} TigArtType;

typedef enum TigArtType6Subtype {
    TIG_ART_TYPE_6_SUBTYPE_0,
    TIG_ART_TYPE_6_SUBTYPE_1,
    TIG_ART_TYPE_6_SUBTYPE_2,
} TigArtType6Subtype;

typedef struct TigArtData {
    int unk_0;
    int unk_4;
    int bpp;
    int field_C;
    int frames;
    unsigned int color_key;
    void* palette1;
    void* palette2;
} TigArtData;

static_assert(sizeof(TigArtData) == 0x20, "wrong size");

typedef struct TigArtFrameData {
    /* 0000 */ int width;
    /* 0004 */ int height;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
} TigArtFrameData;

static_assert(sizeof(TigArtFrameData) == 0x18, "wrong size");

typedef enum TigArtBltFlags {
    TIG_ART_BLT_MIRROR_0x1 = 1 << 0,
    TIG_ART_BLT_MIRROR_0x2 = 1 << 1,

    /// Forces to use base art palette.
    ///
    /// Mutually exclusive with `TIG_ART_BLT_PALETTE_CUSTOM`.
    TIG_ART_BLT_PALETTE_NORMAL = 1 << 2,

    /// Forces to use custom palette specified in `custom_palette`.
    ///
    /// Mutually exclusive with `TIG_ART_BLT_PALETTE_NORMAL`.
    TIG_ART_BLT_PALETTE_CUSTOM = 1 << 3,

    TIG_ART_BLT_0x10 = 1 << 4,

    TIG_ART_BLT_BLEND_MODE_0x20 = 1 << 5,

    TIG_ART_BLT_BLEND_MODE_0x40 = 1 << 6,

    TIG_ART_BLT_BLEND_MODE_0x80 = 1 << 7,

    TIG_ART_BLT_BLEND_MODE_0x100 = 1 << 8,

    TIG_ART_BLT_BLEND_MODE_0x200 = 1 << 9,

    TIG_ART_BLT_BLEND_MODE_GRADIENT_HORIZONTAL = 1 << 10,

    TIG_ART_BLT_BLEND_MODE_GRADIENT_VERTICAL = 1 << 11,

    TIG_ART_BLT_BLEND_MODE_GRADIENT_DIAGONAL = 1 << 12,

    TIG_ART_BLT_0x2000 = 1 << 13,

    TIG_ART_BLT_0x4000 = 1 << 14,

    TIG_ART_BLT_0x8000 = 1 << 15,

    TIG_ART_BLT_0x10000 = 1 << 16,

    TIG_ART_BLT_0x20000 = 1 << 17,

    TIG_ART_BLT_MIRROR_ANY = TIG_ART_BLT_MIRROR_0x1 | TIG_ART_BLT_MIRROR_0x2,

    TIG_ART_BLT_BLEND_MODE_GRADIENT_ANY = TIG_ART_BLT_BLEND_MODE_GRADIENT_HORIZONTAL
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_VERTICAL
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_DIAGONAL,

    TIG_ART_BLT_0x26000 = TIG_ART_BLT_0x20000
        | TIG_ART_BLT_0x4000
        | TIG_ART_BLT_0x2000,

    TIG_ART_BLT_0x19F80 = TIG_ART_BLT_0x10000
        | TIG_ART_BLT_0x8000
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_DIAGONAL
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_VERTICAL
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_HORIZONTAL
        | TIG_ART_BLT_BLEND_MODE_0x200
        | TIG_ART_BLT_BLEND_MODE_0x100
        | TIG_ART_BLT_BLEND_MODE_0x80,
} TigArtBltFlags;

typedef struct TigArtBlitSpec {
    /* 0000 */ unsigned int flags;
    /* 0004 */ unsigned int art_id;
    /* 0008 */ TigRect* src_rect;
    /* 000C */ TigPalette field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int* field_14;
    /* 0018 */ TigRect* field_18;
    /* 001C */ unsigned int field_1C;
    /* 0020 */ TigVideoBuffer* dst_video_buffer;
    /* 0024 */ TigRect* dst_rect;
    /* 0028 */ TigVideoBuffer* scratch_video_buffer;
} TigArtBlitSpec;

static_assert(sizeof(TigArtBlitSpec) == 0x2C, "wrong size");

typedef bool(TigArtBlitPaletteAdjustCallback)(tig_art_id_t art_id, TigPaletteAdjustDesc* palette_adjust_desc);

int tig_art_init(TigContext* ctx);
void tig_art_exit();
void tig_art_ping();
int sub_5006E0(tig_art_id_t art_id, TigPalette palette);
int tig_art_misc_id_create(unsigned int a1, unsigned int palette, unsigned int* art_id);
int sub_501E10(tig_art_id_t art_id, int a2);
int sub_501E60(tig_art_id_t art_id, short a2);
int sub_501EB0(tig_art_id_t art_id, const char* filename);
int sub_501F60(const char* filename, uint32_t* new_palette_entries, int new_palette_index);
void tig_art_flush();
int tig_art_exists(tig_art_id_t art_id);
int sub_502270(tig_art_id_t art_id, char* path);
int sub_502290(tig_art_id_t art_id);
void sub_5022B0(TigArtBlitPaletteAdjustCallback* callback);
TigArtBlitPaletteAdjustCallback* sub_5022C0();
void sub_5022D0();
int tig_art_blit(TigArtBlitSpec* blit_spec);
int tig_art_type(unsigned int art_id);
int tig_art_num(unsigned int art_id);
unsigned int sub_502780(unsigned int art_id, unsigned int value);
unsigned int sub_502830(unsigned int art_id);
int tig_art_id_rotation_get(tig_art_id_t art_id);
tig_art_id_t tig_art_id_rotation_inc(tig_art_id_t art_id);
tig_art_id_t tig_art_id_rotation_set(tig_art_id_t, int rotation);
int tig_art_id_frame_get(unsigned int art_id);
unsigned int tig_art_id_frame_inc(unsigned int art_id);
unsigned int tig_art_id_frame_dec(unsigned int art_id);
tig_art_id_t tig_art_id_frame_set(tig_art_id_t art_id, int value);
unsigned int sub_502D30(unsigned int art_id, int value);
int tig_art_palette(unsigned int art_id);
unsigned int tig_art_set_palette(unsigned int art_id, int value);
int sub_502E00(tig_art_id_t art_id);
int sub_502E50(tig_art_id_t art_id, int x, int y, unsigned int* color_ptr);
int sub_502FD0(tig_art_id_t art_id, int x, int y);
int tig_art_data(unsigned int art_id, TigArtData* data);
int tig_art_frame_data(unsigned int art_id, TigArtFrameData* data);
int sub_503340(tig_art_id_t art_id, uint8_t* dst, int pitch);
int sub_5033E0(tig_art_id_t art_id, int a2, int a3);
int tig_art_size(tig_art_id_t art_id, int* width_ptr, int* height_ptr);
int sub_5036B0(unsigned int art_id);
int sub_5036E0(unsigned int art_id);
int sub_503700(unsigned int art_id);
int sub_5037B0(unsigned int art_id);
int sub_5038C0(unsigned int art_id);
int sub_503900(unsigned int art_id);
int sub_503950(unsigned int art_id);
int sub_503990(unsigned int art_id);
int tig_art_wall_id_create(unsigned int a1, int a2, int a3, int rotation, unsigned int a5, int a6, tig_art_id_t* art_id_ptr);
int sub_503A60(unsigned int art_id);
unsigned int sub_503A90(unsigned int art_id, int value);
int sub_503AD0(unsigned int art_id);
int sub_503B00(unsigned int art_id);
unsigned int sub_503B30(unsigned int art_id, int value);
int sub_503B70(unsigned int art_id);
unsigned int sub_503BB0(unsigned int art_id, int value);
int tig_art_critter_id_create(unsigned int a1, int a2, int a3, unsigned int a4, unsigned int a5, int rotation, int a7, int a8, unsigned int a9, tig_art_id_t* art_id_ptr);
int tig_art_monster_id_create(int a1, int a2, unsigned int a3, unsigned int a4, int rotation, int a6, int a7, unsigned int a8, tig_art_id_t* art_id_ptr);
int tig_art_unique_npc_id_create(int a1, unsigned int a2, unsigned int a3, int a4, int a5, int a6, unsigned int a7, tig_art_id_t* art_id_ptr);
int sub_503E20(unsigned int art_id);
unsigned int sub_503E50(unsigned int art_id, int value);
int sub_503EA0(unsigned int art_id);
unsigned int sub_503ED0(unsigned int art_id, int value);
int sub_503F20(unsigned int art_id);
int sub_503F50(int a1);
int sub_503F60(unsigned int art_id);
int sub_503FB0(unsigned int art_id);
unsigned int sub_503FE0(unsigned int art_id, int value);
int sub_504030(unsigned int art_id);
unsigned int sub_504060(unsigned int art_id, int value);
int sub_5040D0(unsigned int art_id);
unsigned int sub_504100(unsigned int art_id, int value);
int sub_504150(unsigned int art_id);
unsigned int sub_504180(unsigned int art_id, int value);
int tig_art_portal_id_create(unsigned int a1, int a2, int a3, unsigned int a4, int a5, unsigned int a6, tig_art_id_t* art_id_ptr);
int sub_504260(unsigned int art_id);
int tig_art_scenery_id_create(unsigned int a1, int a2, unsigned int a3, int a4, unsigned int a5, tig_art_id_t* art_id_ptr);
int sub_504300(unsigned int art_id);
int tig_art_interface_id_create(unsigned int a1, unsigned int a2, unsigned char a3, unsigned int a4, unsigned int* art_id);
int sub_504390(unsigned int art_id);
int sub_504490(unsigned int art_id);
unsigned int sub_5044C0(unsigned int art_id, int value);
int sub_5044F0(unsigned int art_id);
tig_art_id_t sub_504520(tig_art_id_t art_id, int value);
int sub_504550(unsigned int art_id);
int sub_504570(unsigned int art_id);
int sub_5045A0(unsigned int art_id);
unsigned int sub_5045C0(unsigned int art_id, int value);
int tig_art_container_id_create(unsigned int a1, int a2, unsigned int a3, int rotation, unsigned int a5, tig_art_id_t* art_id_ptr);
int sub_504660(unsigned int art_id);
int tig_art_light_id_create(unsigned int a1, unsigned int a2, unsigned int a3, int a4, tig_art_id_t* art_id);
int sub_504700(unsigned int art_id);
int sub_504790(unsigned int art_id);
int tig_art_roof_id_create(unsigned int a1, int a2, unsigned int a3, unsigned int a4, tig_art_id_t* art_id_ptr);
int sub_504840(unsigned int art_id);
tig_art_id_t sub_504880(tig_art_id_t art_id, int frame);
int sub_5048D0(unsigned int art_id);
unsigned int sub_504900(unsigned int art_id, unsigned int value);
int sub_504940(unsigned int art_id);
unsigned int sub_504970(unsigned int art_id, unsigned int value);
int tig_art_facade_id_create(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, tig_art_id_t* art_id_ptr);
int sub_504A60(unsigned int art_id);
int sub_504A90(tig_art_id_t art_id);
int sub_504AC0(unsigned int art_id);
int tig_art_eye_candy_id_create(unsigned int a1, unsigned int a2, int a3, int translucency, int type, unsigned int palette, int scale, tig_art_id_t* art_id);
int tig_art_eye_candy_id_type_get(tig_art_id_t art_id);
tig_art_id_t tig_art_eye_candy_id_type_set(tig_art_id_t art_id, int value);
int tig_art_eye_candy_id_translucency_get(tig_art_id_t art_id);
tig_art_id_t tig_art_eye_candy_id_translucency_set(tig_art_id_t art_id, int value);
int tig_art_eye_candy_id_scale_get(tig_art_id_t art_id);
tig_art_id_t tig_art_eye_candy_id_scale_set(tig_art_id_t art_id, int value);
int sub_504FD0(unsigned int art_id);
void sub_505000(tig_art_id_t art_id, TigPalette src_palette, TigPalette dst_palette);
void sub_51AC00(int a1);
tig_art_id_t sub_51B0A0(tig_art_id_t art_id);

#endif /* ARCANUM_TIG_ART_H_ */
