#pragma once

namespace key_values
{
    inline void* key_values()
    {
        using key_values_fn = void* (__fastcall*)(unsigned int);

        static key_values_fn key_values = (key_values_fn)memory::pattern_scanner(xorstr("client.dll"), xorstr("40 53 48 83 EC 20 48 8B D9 FF 15 ? ? ? ? 8B D3"));
        if (!key_values)
            throw;

        return key_values(0x48);
    }

    inline void* initialize(void* kv, const char* name)
    {
        using init_fn = void* (__fastcall*)(void*, const char*);

        static init_fn initialize = (init_fn)memory::pattern_scanner(xorstr("client.dll"), xorstr("48 89 5C 24 ? 57 48 83 EC 20 81 21 00 00 00 FF"));
        if (!initialize)
            throw;

        return initialize(kv, name);
    }

    inline bool load_from_buffer(void* kv, char const* resource_name, const char* buffer, void* file_system = NULL, const char* path_id = NULL)
    {
        using load_from_buffer_fn = bool(__fastcall*)(void*, char const*, const char*, void*, const char*, void*);

        static load_from_buffer_fn load_from_buffer = (load_from_buffer_fn)memory::pattern_scanner(xorstr("client.dll"), xorstr("48 89 6C 24 ? 56 41 56 41 57 48 83 EC 70 49 8B E9"));
        if (!load_from_buffer)
            throw;

        return load_from_buffer(kv, resource_name, buffer, file_system, path_id, 0);
    }
}