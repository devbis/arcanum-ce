#include "game/obj_file.h"

#include "game/obj_private.h"
#include "game/obj.h"

static bool append_handle_fname(int64_t handle, char* path);
static TigFile* open_solitary_for_write(int64_t handle, const char* dir, const char* ext);
static bool handle_from_fname(int64_t* handle_ptr, const char* path);

// 0x4E6B60
bool objf_solitary_write(int64_t handle, const char* dir, const char* ext)
{
    TigFile* stream;

    stream = open_solitary_for_write(handle, dir, ext);
    if (stream == NULL) {
        return false;
    }

    if (!obj_write(stream, handle)) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}

// 0x4E6BC0
bool objf_solitary_read(int64_t* handle_ptr, const char* path)
{
    TigFile* stream;

    stream = tig_file_fopen(path, "rb");
    if (stream == NULL) {
        tig_debug_printf("Unable to open [%s] in ObjFile, objf_solitary_read\n", path);
        return false;
    }

    if (!obj_read(stream, handle_ptr)) {
        tig_file_fclose(stream);
        return false;
    }

    tig_file_fclose(stream);
    return true;
}

// 0x4E6D10
void objf_solitary_delete(int64_t handle, const char* dir, const char* ext)
{
    char path[TIG_MAX_PATH];
    int length;
    char* dot;
    TigFile* stream;

    strcpy(path, dir);

    if (!append_handle_fname(handle, path)) {
        tig_debug_println("Can't generate filename in ObjFile, objf_solitary_delete");
        return;
    }

    strcat(path, ext);

    length = (int)strlen(path);
    if (length > TIG_MAX_PATH) {
        tig_debug_println("Filename too long in ObjFile, objf_solitary_delete.");
        return;
    }

    if (tig_file_exists(path, NULL) && tig_file_remove(path) != 0) {
        tig_debug_printf("Unable to delete [%s] in ObjFile, objf_solitary_delete\n", path);
        return;
    }

    dot = strchr(path, '.');
    if (dot == NULL) {
        tig_debug_printf("Unable to find the dot in [%s] in ObjFile, objf_solitary_delete\n", path);
        return;
    }

    strcpy(dot, ".del");

    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        tig_debug_printf("Unable to open [%s] in ObjFile, objf_solitary_delete\n", path);
        return;
    }

    if (!objf_write(&length, sizeof(length), stream)) {
        tig_debug_println("Write failed in ObjFile, objf_solitary_delete");
    }

    tig_file_fclose(stream);
}

// 0x4E7010
bool objf_write(const void* buffer, size_t size, TigFile* stream)
{
    return tig_file_fwrite(buffer, size, 1, stream) == 1;
}

// 0x4E7030
bool objf_read(void* buffer, size_t size, TigFile* stream)
{
    return tig_file_fread(buffer, size, 1, stream) == 1;
}

// 0x4E7050
bool append_handle_fname(int64_t handle, char* path)
{
    size_t length;

    length = strlen(path);
    if (length != 0 && path[length - 1] != '\\') {
        path[length - 1] = '\\';
        path[length] = '\0';
    }

    objid_id_to_str(path, obj_get_id(handle));

    return true;
}

// 0x4E70C0
TigFile* open_solitary_for_write(int64_t handle, const char* dir, const char* ext)
{
    char path[TIG_MAX_PATH];
    int length;
    TigFile* stream;

    strcpy(path, dir);

    if (!append_handle_fname(handle, path)) {
        return NULL;
    }

    strcat(path, ext);

    length = (int)strlen(path);
    if (length > TIG_MAX_PATH) {
        tig_debug_println("Filename too long in ObjFile, open_solitary_for_write.");
        return NULL;
    }

    stream = tig_file_fopen(path, "wb");
    if (stream == NULL) {
        tig_debug_printf("Unable to open [%s] in ObjFile, open_solitary_for_write\n", path);
        return NULL;
    }

    return stream;
}

// 0x4E71B0
bool handle_from_fname(int64_t* handle_ptr, const char* path)
{
    char fname[TIG_MAX_PATH];
    ObjectID oid;

    compat_splitpath(path, NULL, NULL, fname, NULL);

    if (fname[0] == '\0') {
        tig_debug_printf("handle_from_fname found empty base_name infilename [%s]", path);
        return false;
    }

    if (!objid_id_from_str(&oid, fname)) {
        tig_debug_printf("Unable to extract id from filename [%s]\n", path);
        return false;
    }

    if (oid.type <= 0 || oid.type > 3) {
        tig_debug_printf("Wrong id type in filename [%s]\n", path);
        return false;
    }

    *handle_ptr = objp_perm_lookup(oid);
    if (*handle_ptr == OBJ_HANDLE_NULL) {
        tig_debug_printf("ID not loaded, can't convert to handle in ObjFile,handle_from_fname: [%s]", fname);
        return false;
    }

    return true;
}
