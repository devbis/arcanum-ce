#ifndef ARCANUM_TIG_ART_H_
#define ARCANUM_TIG_ART_H_

#include "tig/types.h"

typedef unsigned int art_id_t;

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
};

typedef enum TigArtType6Subtype {
    TIG_ART_TYPE_6_SUBTYPE_0,
    TIG_ART_TYPE_6_SUBTYPE_1,
    TIG_ART_TYPE_6_SUBTYPE_2,
};

typedef struct TigArtData {
    int field_0;
    int field_4;
    int bpp;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
};

static_assert(sizeof(TigArtData) == 0x20, "wrong size");

typedef struct TigArtFrameData {
    int width;
    int height;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
};

static_assert(sizeof(TigArtFrameData) == 0x18, "wrong size");

typedef struct TigArtBlitSpec {
    int field_0;
    unsigned int src_art_id;
    TigRect* src_rect;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    TigVideoBuffer* dst_video_buffer;
    TigRect* dst_rect;
    int field_28;
};

static_assert(sizeof(TigArtBlitSpec) == 0x2C, "wrong size");

int tig_art_init(TigContext* ctx);
void tig_art_exit();
void tig_art_ping();
int tig_art_misc_id_create(unsigned int a1, unsigned int palette, unsigned int* art_id);
void tig_art_flush();
int tig_art_exists(art_id_t art_id);
int tig_art_blit(TigArtBlitSpec* blit_spec);
int tig_art_type(unsigned int art_id);
int tig_art_num(unsigned int art_id);
int tig_art_data(unsigned int art_id, TigArtData* art_data);
int tig_art_frame_data(unsigned int art_id, TigArtFrameData* art_frame_data);
int tig_art_interface_id_create(unsigned int a1, unsigned int a2, unsigned char a3, unsigned int a4, unsigned int* art_id);
int tig_art_light_id_create(unsigned int a1, unsigned int a2, unsigned int a3, int a4, art_id_t* art_id);
int tig_art_eye_candy_id_create(unsigned int a1, unsigned int a2, int a3, int translucency, int type, unsigned int palette, int scale, art_id_t* art_id);
int tig_art_eye_candy_id_type_get(art_id_t art_id);
art_id_t tig_art_eye_candy_id_type_set(art_id_t art_id, int value);
int tig_art_eye_candy_id_translucency_get(art_id_t art_id);
art_id_t tig_art_eye_candy_id_translucency_set(art_id_t art_id, int value);
int tig_art_eye_candy_id_scale_get(art_id_t art_id);
art_id_t tig_art_eye_candy_id_scale_set(art_id_t art_id, int value);

#endif /* ARCANUM_TIG_ART_H_ */
