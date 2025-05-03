#ifndef ARCANUM_GAME_SFX_H_
#define ARCANUM_GAME_SFX_H_

#include "game/context.h"

typedef enum ContainerSoundType {
    CONTAINER_SOUND_OPEN,
    CONTAINER_SOUND_CLOSE,
    CONTAINER_SOUND_LOCKED,
} ContainerSoundType;

typedef enum PortalSoundType {
    PORTAL_SOUND_OPEN,
    PORTAL_SOUND_CLOSE,
    PORTAL_SOUND_LOCKED,
} PortalSoundType;

typedef enum MiscSoundType {
    MISC_SOUND_BUSTING,
    MISC_SOUND_DESTROYING,
    MISC_SOUND_ANIMATING,
} MiscSoundType;

int sub_4F0BF0(int64_t item_obj, int64_t parent_obj, int64_t target_obj, int type);
int sub_4F0ED0(int64_t obj, int a2);
int sfx_container_sound(int64_t obj, ContainerSoundType type);
int sfx_portal_sound(int64_t obj, PortalSoundType type);
int sfx_misc_sound(int64_t obj, MiscSoundType type);

#endif /* ARCANUM_GAME_SFX_H_ */
