#include "game/settings.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game/obj_private.h"

static SettingsEntry* settings_find(Settings* settings, const char* key);
static void settings_trim(char* str);

// 0x438AE0
void settings_init(Settings* settings, const char* path)
{
    settings->path = path;
    settings->entries = NULL;
}

// 0x438B00
void settings_exit(Settings* settings)
{
    SettingsEntry* curr;
    SettingsEntry* next;

    curr = settings->entries;
    while (curr != NULL) {
        next = curr->next;
        FREE(curr->key);
        FREE(curr->value);
        FREE(curr);
        curr = next;
    }

    settings->path = NULL;
    settings->entries = NULL;
}

// 0x438B50
void settings_load(Settings* settings)
{
    TigFindFileData find_file_data;
    FILE* stream;
    char buffer[256];
    char* sep;

    if (tig_find_first_file(settings->path, &find_file_data)) {
        stream = fopen(settings->path, "rt");
        if (stream != NULL) {
            while (fgets(buffer, sizeof(buffer), stream) != NULL) {
                sep = strchr(buffer, '=');
                if (sep != NULL) {
                    *sep++ = '\0';
                    settings_trim(buffer);
                    settings_trim(sep);
                    if (sep[0] != '\0' && buffer[0] != '\0') {
                        settings_set_str_value(settings, buffer, sep);
                    }
                }
            }
            fclose(stream);
        }
    }
}

// 0x438C20
void settings_save(Settings* settings)
{
    FILE* stream;
    SettingsEntry* curr;

    if (settings->entries != NULL) {
        if ((settings->flags & SETTINGS_CHANGED) != 0) {
            stream = fopen(settings->path, "wt");
            if (stream != NULL) {
                curr = settings->entries;
                while (curr != NULL) {
                    fprintf(stream, "%s=%s\n", curr->key, curr->value);
                    curr = curr->next;
                }
                fclose(stream);

                // FIXME: Should probably unset `SETTINGS_CHANGED` flag.
            }
        }
    }
}

// 0x438C80
void settings_add(Settings* settings, const char* key, const char* default_value, SettingsValueChangedFunc* value_changed_func)
{
    SettingsEntry* entry;

    entry = settings_find(settings, key);
    if (entry != NULL) {
        entry->value_changed_func = value_changed_func;
    } else {
        entry = (SettingsEntry*)malloc(sizeof(*entry));
        entry->key = _strdup(key);
        entry->value = _strdup(default_value);
        entry->value_changed_func = value_changed_func;
        entry->next = settings->entries;
        settings->entries = entry;
    }
}

// 0x438CE0
void settings_set_value(Settings* settings, const char* key, int value)
{
    char buffer[48];

    _itoa(value, buffer, 10);
    settings_set_str_value(settings, key, buffer);
}

// 0x438D10
int settings_get_value(Settings* settings, const char* key)
{
    const char* str;

    str = settings_get_str_value(settings, key);
    if (str != NULL) {
        return atoi(str);
    } else {
        return 0;
    }
}

// 0x438D40
void settings_set_obj_value(Settings* settings, const char* key, ObjectID oid)
{
    char str[40];

    objid_id_to_str(str, oid);
    settings_set_str_value(settings, key, str);
}

// 0x438DA0
ObjectID settings_get_obj_value(Settings* settings, const char* key)
{
    const char* str;
    ObjectID oid;

    str = settings_get_str_value(settings, key);
    if (str != NULL) {
        objid_id_from_str(&oid, str);
    } else {
        oid.type = 0;
    }

    return oid;
}

// 0x438DF0
void settings_set_str_value(Settings* settings, const char* key, const char* value)
{
    SettingsEntry* entry;

    settings->flags |= SETTINGS_CHANGED;

    entry = settings_find(settings, key);
    if (entry == NULL) {
        settings_add(settings, key, value, NULL);
    } else {
        if (strlen(value) < strlen(entry->value)) {
            free(entry->value);
            entry->value = _strdup(value);
        } else {
            memcpy(entry->value, value, strlen(value) + 1);
        }

        if (entry->value_changed_func != NULL) {
            entry->value_changed_func();
        }
    }
}

// 0x438E90
const char* settings_get_str_value(Settings* settings, const char* key)
{
    SettingsEntry* entry;

    entry = settings_find(settings, key);
    if (entry != NULL) {
        return entry->value;
    } else {
        return NULL;
    }
}

// 0x438EB0
SettingsEntry* settings_find(Settings* settings, const char* key)
{
    SettingsEntry* curr;

    curr = settings->entries;
    while (curr != NULL) {
        if (_strcmpi(curr->key, key) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// 0x438EF0
void settings_trim(char* str)
{
    char* curr;

    curr = str;
    while (isspace(*curr)) {
        curr++;
    }

    size_t len = strlen(curr);
    // FIXME: Should be `memmove`.
    memcpy(str, curr, len + 1);

    if (len > 0) {
        curr = str + len;
        while (isspace(*curr)) {
            *curr-- = '\0';
        }
    }
}
