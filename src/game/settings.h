#ifndef ARCANUM_GAME_SETTINGS_H_
#define ARCANUM_GAME_SETTINGS_H_

#include "game/obj.h"

#define SETTINGS_CHANGED 0x1

typedef void(SettingsValueChangedFunc)();

typedef struct SettingsEntry {
    char* key;
    char* value;
    SettingsValueChangedFunc* value_changed_func;
    struct SettingsEntry* next;
} SettingsEntry;

// See 0x438C80.
static_assert(sizeof(SettingsEntry) == 0x10, "wrong size");

typedef struct Settings {
    const char* path;
    SettingsEntry* entries;
    unsigned int flags;
} Settings;

static_assert(sizeof(Settings) == 0xC, "wrong size");

void settings_init(Settings* settings, const char* path);
void settings_exit(Settings* settings);
void settings_load(Settings* settings);
void settings_save(Settings* settings);
void settings_add(Settings* settings, const char* key, const char* default_value, SettingsValueChangedFunc* value_changed_func);
void settings_set_value(Settings* settings, const char* key, int value);
int settings_get_value(Settings* settings, const char* key);
void settings_set_obj_value(Settings* settings, const char* key, ObjectID oid);
ObjectID settings_get_obj_value(Settings* settings, const char* key);
void settings_set_str_value(Settings* settings, const char* key, const char* value);
const char* settings_get_str_value(Settings* settings, const char* key);

#endif /* ARCANUM_GAME_SETTINGS_H_ */
