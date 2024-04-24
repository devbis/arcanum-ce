#ifndef ARCANUM_TIG_ART_H_
#define ARCANUM_TIG_ART_H_

#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TIG_ART_ID_INVALID ((tig_art_id_t)-1)

#define TIG_ART_SYSTEM_MOUSE 0
#define TIG_ART_SYSTEM_BUTTON 1
#define TIG_ART_SYSTEM_UP 2
#define TIG_ART_SYSTEM_DOWN 3
#define TIG_ART_SYSTEM_CANCEL 4
#define TIG_ART_SYSTEM_LENS 5
#define TIG_ART_SYSTEM_X 6
#define TIG_ART_SYSTEM_PLUS 7
#define TIG_ART_SYSTEM_MINUS 8
#define TIG_ART_SYSTEM_BLANK 9
#define TIG_ART_SYSTEM_FONT 10

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

typedef enum TigArtFlags {
    TIG_ART_0x01 = 0x01,
    TIG_ART_0x02 = 0x02,
    TIG_ART_0x04 = 0x04,
    TIG_ART_0x08 = 0x08,
    TIG_ART_0x10 = 0x10,
    TIG_ART_0x20 = 0x20,
} TigArtFlags;

typedef struct TigArtData {
    unsigned int flags;
    int fps;
    int bpp;
    int action_frame;
    int num_frames;
    unsigned int color_key;
    void* palette1;
    void* palette2;
} TigArtData;

static_assert(sizeof(TigArtData) == 0x20, "wrong size");

typedef struct TigArtFrameData {
    /* 0000 */ int width;
    /* 0004 */ int height;
    /* 0008 */ int hot_x;
    /* 000C */ int hot_y;
    /* 0010 */ int offset_x;
    /* 0014 */ int offset_y;
} TigArtFrameData;

static_assert(sizeof(TigArtFrameData) == 0x18, "wrong size");

typedef enum TigArtBltFlags {
    TIG_ART_BLT_MIRROR_LEFT_RIGHT = 0x00000001,
    TIG_ART_BLT_MIRROR_UP_DOWN = 0x00000002,

    /// Forces to use base art palette.
    ///
    /// Mutually exclusive with `TIG_ART_BLT_PALETTE_CUSTOM`.
    TIG_ART_BLT_PALETTE_NORMAL = 0x00000004,

    /// Forces to use custom palette specified in `custom_palette`.
    ///
    /// Mutually exclusive with `TIG_ART_BLT_PALETTE_NORMAL`.
    TIG_ART_BLT_PALETTE_CUSTOM = 0x00000008,

    TIG_ART_BLT_0x10 = 0x00000010,

    TIG_ART_BLT_BLEND_MODE_0x20 = 0x00000020,

    TIG_ART_BLT_BLEND_MODE_0x40 = 0x00000040,

    TIG_ART_BLT_BLEND_MODE_0x80 = 0x00000080,

    TIG_ART_BLT_BLEND_MODE_0x100 = 0x00000100,

    TIG_ART_BLT_BLEND_MODE_0x200 = 0x00000200,

    // Blends dst with src applying left-to-right gradient mask.
    //
    // Use `opacity[0]` to specify start opacity (left edge) and `opacity[1]`
    // to specify end opacity (right edge) where `255` is fully opaque (results
    // in dst color) and `0` is fully transparent (result in src).
    TIG_ART_BLT_BLEND_MODE_GRADIENT_HORIZONTAL = 0x00000400,

    // Blends dst with src applying top-to-bottom gradient mask.
    //
    // Use `opacity[0]` to specify start opacity (top edge) and `opacity[3]`
    // to specify end opacity (bottom edge) where `255` is fully opaque and `0`
    // is fully transparent.
    TIG_ART_BLT_BLEND_MODE_GRADIENT_VERTICAL = 0x00000800,

    // Blends dst with src applying gradient mask as follows:
    //  - `opacity[0]` - top-left corner opacity,
    //  - `opacity[1]` - top-right corner opacity,
    //  - `opacity[2]` - bottom-right corner opacity,
    //  - `opacity[3]` - bottom-left corner opacity.
    //
    // As with other gradient masks `255` is fully opaque and `0` is fully
    // transparent.
    TIG_ART_BLT_BLEND_MODE_GRADIENT_CORNERS = 0x00001000,

    TIG_ART_BLT_0x2000 = 0x00002000,

    TIG_ART_BLT_0x4000 = 0x00004000,

    TIG_ART_BLT_BLEND_MODE_CHECKERBOARD_SRC = 0x00008000,

    TIG_ART_BLT_BLEND_MODE_CHECKERBOARD_DST = 0x00010000,

    TIG_ART_BLT_0x20000 = 0x00020000,

    TIG_ART_BLT_0x01000000 = 0x01000000,

    TIG_ART_BLT_MIRROR_ANY = TIG_ART_BLT_MIRROR_LEFT_RIGHT | TIG_ART_BLT_MIRROR_UP_DOWN,

    TIG_ART_BLT_BLEND_MODE_GRADIENT_ANY = TIG_ART_BLT_BLEND_MODE_GRADIENT_HORIZONTAL
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_VERTICAL
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_CORNERS,

    TIG_ART_BLT_0x26000 = TIG_ART_BLT_0x20000
        | TIG_ART_BLT_0x4000
        | TIG_ART_BLT_0x2000,

    TIG_ART_BLT_0x19F80 = TIG_ART_BLT_BLEND_MODE_CHECKERBOARD_DST
        | TIG_ART_BLT_BLEND_MODE_CHECKERBOARD_SRC
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_CORNERS
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_VERTICAL
        | TIG_ART_BLT_BLEND_MODE_GRADIENT_HORIZONTAL
        | TIG_ART_BLT_BLEND_MODE_0x200
        | TIG_ART_BLT_BLEND_MODE_0x100
        | TIG_ART_BLT_BLEND_MODE_0x80,

    TIG_ART_BLT_0x00019FF0 = TIG_ART_BLT_0x19F80
        | TIG_ART_BLT_BLEND_MODE_0x40
        | TIG_ART_BLT_BLEND_MODE_0x20
        | TIG_ART_BLT_0x10,

    TIG_ART_BLT_0x0003FFF0 = TIG_ART_BLT_0x26000 | TIG_ART_BLT_0x00019FF0,
} TigArtBltFlags;

typedef struct TigArtBlitSpec {
    /* 0000 */ unsigned int flags;
    /* 0004 */ tig_art_id_t art_id;
    /* 0008 */ TigRect* src_rect;
    /* 000C */ TigPalette field_C;
    /* 0010 */ uint32_t field_10;
    /* 0014 */ uint32_t* field_14;
    /* 0018 */ TigRect* field_18;
    /* 001C */ uint8_t opacity[4];
    /* 0020 */ TigVideoBuffer* dst_video_buffer;
    /* 0024 */ TigRect* dst_rect;
    /* 0028 */ TigVideoBuffer* scratch_video_buffer;
} TigArtBlitSpec;

static_assert(sizeof(TigArtBlitSpec) == 0x2C, "wrong size");

typedef bool(TigArtBlitPaletteAdjustCallback)(tig_art_id_t art_id, TigPaletteModifyInfo* modify_info);

typedef struct TigArtPackInfo {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int fps;
    /* 0008 */ int field_8;
    /* 000C */ int action_frame;
} TigArtPackInfo;

int tig_art_init(TigInitializeInfo* init_info);
void tig_art_exit();
void tig_art_ping();
int sub_5006E0(tig_art_id_t art_id, TigPalette palette);
int tig_art_misc_id_create(unsigned int a1, unsigned int palette, tig_art_id_t* art_id);
int tig_art_set_fps(tig_art_id_t art_id, int fps);
int tig_art_set_action_frame(tig_art_id_t art_id, short action_frame);
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
int tig_art_type(tig_art_id_t art_id);
int tig_art_num(tig_art_id_t art_id);
tig_art_id_t sub_502780(tig_art_id_t art_id, unsigned int value);
unsigned int sub_502830(tig_art_id_t art_id);
int tig_art_id_rotation_get(tig_art_id_t art_id);
tig_art_id_t tig_art_id_rotation_inc(tig_art_id_t art_id);
tig_art_id_t tig_art_id_rotation_set(tig_art_id_t, int rotation);
int tig_art_id_frame_get(tig_art_id_t art_id);
tig_art_id_t tig_art_id_frame_inc(tig_art_id_t art_id);
tig_art_id_t tig_art_id_frame_dec(tig_art_id_t art_id);
tig_art_id_t tig_art_id_frame_set(tig_art_id_t art_id, int value);
tig_art_id_t sub_502D30(tig_art_id_t art_id, int value);
int tig_art_palette(tig_art_id_t art_id);
tig_art_id_t tig_art_set_palette(tig_art_id_t art_id, int value);
int sub_502E00(tig_art_id_t art_id);
int sub_502E50(tig_art_id_t art_id, int x, int y, unsigned int* color_ptr);
int sub_502FD0(tig_art_id_t art_id, int x, int y);
int tig_art_data(tig_art_id_t art_id, TigArtData* data);
int tig_art_frame_data(tig_art_id_t art_id, TigArtFrameData* data);
int sub_503340(tig_art_id_t art_id, uint8_t* dst, int pitch);
int sub_5033E0(tig_art_id_t art_id, int a2, int a3);
int tig_art_size(tig_art_id_t art_id, int* width_ptr, int* height_ptr);
int tig_art_tile_id_create(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8, tig_art_id_t* art_id_ptr);
int sub_5036B0(tig_art_id_t art_id);
int sub_5036E0(tig_art_id_t art_id);
int sub_503700(tig_art_id_t art_id);
tig_art_id_t sub_503740(tig_art_id_t art_id, int value);
int sub_5037B0(tig_art_id_t art_id);
tig_art_id_t sub_503800(tig_art_id_t art_id, int value);
int sub_5038C0(tig_art_id_t art_id);
int sub_503900(tig_art_id_t art_id);
int sub_503950(tig_art_id_t art_id);
int sub_503990(tig_art_id_t art_id);
int tig_art_wall_id_create(unsigned int a1, int a2, int a3, int rotation, unsigned int a5, int a6, tig_art_id_t* art_id_ptr);
int sub_503A60(tig_art_id_t art_id);
tig_art_id_t sub_503A90(tig_art_id_t art_id, int value);
int sub_503AD0(tig_art_id_t art_id);
int sub_503B00(tig_art_id_t art_id);
tig_art_id_t sub_503B30(tig_art_id_t art_id, int value);
int sub_503B70(tig_art_id_t art_id);
tig_art_id_t sub_503BB0(tig_art_id_t art_id, int value);
int tig_art_critter_id_create(unsigned int a1, int a2, int a3, unsigned int a4, unsigned int a5, int rotation, int a7, int a8, unsigned int a9, tig_art_id_t* art_id_ptr);
int tig_art_monster_id_create(int a1, int a2, unsigned int a3, unsigned int a4, int rotation, int a6, int a7, unsigned int a8, tig_art_id_t* art_id_ptr);
int tig_art_unique_npc_id_create(int a1, unsigned int a2, unsigned int a3, int a4, int a5, int a6, unsigned int a7, tig_art_id_t* art_id_ptr);
int sub_503E20(tig_art_id_t art_id);
tig_art_id_t sub_503E50(tig_art_id_t art_id, int value);
int sub_503EA0(tig_art_id_t art_id);
tig_art_id_t sub_503ED0(tig_art_id_t art_id, int value);
int sub_503F20(tig_art_id_t art_id);
int sub_503F50(int a1);
int sub_503F60(tig_art_id_t art_id);
int sub_503FB0(tig_art_id_t art_id);
tig_art_id_t sub_503FE0(tig_art_id_t art_id, int value);
int sub_504030(tig_art_id_t art_id);
tig_art_id_t sub_504060(tig_art_id_t art_id, int value);
int sub_5040D0(tig_art_id_t art_id);
tig_art_id_t sub_504100(tig_art_id_t art_id, int value);
int sub_504150(tig_art_id_t art_id);
tig_art_id_t sub_504180(tig_art_id_t art_id, int value);
int tig_art_portal_id_create(unsigned int a1, int a2, int a3, unsigned int a4, int a5, unsigned int a6, tig_art_id_t* art_id_ptr);
int sub_504260(tig_art_id_t art_id);
int tig_art_scenery_id_create(unsigned int a1, int a2, unsigned int a3, int a4, unsigned int a5, tig_art_id_t* art_id_ptr);
int sub_504300(tig_art_id_t art_id);
int tig_art_interface_id_create(unsigned int a1, unsigned int a2, unsigned char a3, unsigned int a4, tig_art_id_t* art_id_ptr);
int sub_504390(tig_art_id_t art_id);
int tig_art_item_id_create(int a1, int a2, int a3, int a4, int a5, int a6, int a7, unsigned int a8, tig_art_id_t* art_id_ptr);
int sub_504490(tig_art_id_t art_id);
tig_art_id_t sub_5044C0(tig_art_id_t art_id, int value);
int sub_5044F0(tig_art_id_t art_id);
tig_art_id_t sub_504520(tig_art_id_t art_id, int value);
int sub_504550(tig_art_id_t art_id);
int sub_504570(tig_art_id_t art_id);
int sub_5045A0(tig_art_id_t art_id);
tig_art_id_t sub_5045C0(tig_art_id_t art_id, int value);
int tig_art_container_id_create(unsigned int a1, int a2, unsigned int a3, int rotation, unsigned int a5, tig_art_id_t* art_id_ptr);
int sub_504660(tig_art_id_t art_id);
int tig_art_light_id_create(unsigned int a1, unsigned int a2, unsigned int rotation, int a4, tig_art_id_t* art_id_ptr);
int sub_504700(tig_art_id_t art_id);
tig_art_id_t sub_504730(tig_art_id_t art_id, int rotation);
int sub_504790(tig_art_id_t art_id);
int tig_art_roof_id_create(unsigned int a1, int a2, unsigned int a3, unsigned int a4, tig_art_id_t* art_id_ptr);
int sub_504840(tig_art_id_t art_id);
tig_art_id_t sub_504880(tig_art_id_t art_id, int frame);
int sub_5048D0(tig_art_id_t art_id);
tig_art_id_t sub_504900(tig_art_id_t art_id, unsigned int value);
int sub_504940(tig_art_id_t art_id);
tig_art_id_t sub_504970(tig_art_id_t art_id, unsigned int value);
int tig_art_facade_id_create(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, tig_art_id_t* art_id_ptr);
int sub_504A60(tig_art_id_t art_id);
int sub_504A90(tig_art_id_t art_id);
int sub_504AC0(tig_art_id_t art_id);
int tig_art_eye_candy_id_create(unsigned int a1, unsigned int a2, int a3, int translucency, int type, unsigned int palette, int scale, tig_art_id_t* art_id_ptr);
int tig_art_eye_candy_id_type_get(tig_art_id_t art_id);
tig_art_id_t tig_art_eye_candy_id_type_set(tig_art_id_t art_id, int value);
int tig_art_eye_candy_id_translucency_get(tig_art_id_t art_id);
tig_art_id_t tig_art_eye_candy_id_translucency_set(tig_art_id_t art_id, int value);
int tig_art_eye_candy_id_scale_get(tig_art_id_t art_id);
tig_art_id_t tig_art_eye_candy_id_scale_set(tig_art_id_t art_id, int value);
bool sub_504CC0(const char* name);
int sub_504FD0(tig_art_id_t art_id);
void sub_505000(tig_art_id_t art_id, TigPalette src_palette, TigPalette dst_palette);
void tig_art_cache_set_video_memory_fullness(int fullness);
tig_art_id_t sub_51B0A0(tig_art_id_t art_id);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_ART_H_ */
