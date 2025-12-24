//
// Created by liad on 24/12/2025.
//

#include "RegenXLua.h"

// Lua MUST come first
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


#include <iostream>
#include <string>


// Lua RegenXIO port
#include "../RegenXIO.h"
static int l_file_exists(lua_State* L) {
    const char* file_path = luaL_checkstring(L, 1);
    lua_pushboolean(L, file_exists(file_path));
    return 1;
}

static int l_file_delete(lua_State* L) {
    const char* file_path = luaL_checkstring(L, 1);
    lua_pushboolean(L, file_delete(file_path));
    return 1;
}

static int l_file_create(lua_State* L) {
    const char* file_path = luaL_checkstring(L, 1);
    const char* content   = luaL_optstring(L, 2, "");
    lua_pushboolean(L, file_create(file_path, content));
    return 1;
}

static int l_file_read_line(lua_State* L) {
    const char* file_path = luaL_checkstring(L, 1);
    int line = luaL_checkinteger(L, 2);

    std::string s = file_read_line(file_path, line);
    lua_pushlstring(L, s.data(), s.size());
    return 1;
}

static int l_file_read(lua_State* L) {
    const char* file_path = luaL_checkstring(L, 1);

    std::string s = file_read(file_path);
    lua_pushlstring(L, s.data(), s.size());
    return 1;
}

static int l_file_overwrite(lua_State* L) {
    const char* file_path = luaL_checkstring(L, 1);
    const char* content   = luaL_checkstring(L, 2);
    lua_pushboolean(L, file_overwrite(file_path, content));
    return 1;
}

static int l_file_write(lua_State* L) {
    const char* file_path = luaL_checkstring(L, 1);
    const char* content   = luaL_checkstring(L, 2);
    lua_pushboolean(L, file_write(file_path, content));
    return 1;
}

// initalize
void regenx_io(lua_State* L) {
    lua_newtable(L);

    static const luaL_Reg file_funcs[] = {
        {"exists",    l_file_exists},
        {"delete",    l_file_delete},
        {"create",    l_file_create},
        {"read_line", l_file_read_line},
        {"read",      l_file_read},
        {"overwrite", l_file_overwrite},
        {"write",     l_file_write},
        {nullptr, nullptr}
    };

    for (const luaL_Reg* f = file_funcs; f->name != nullptr; ++f) {
        lua_pushcfunction(L, f->func);      // stack: [ table, function ]
        lua_setfield(L, -2, f->name);       // table[f->name] = function
    }

    // now set the populated table as global "file"
    lua_setglobal(L, "file");
}

void initalize_lua_funtions() {
    const std::string script = file_read("/home/liad/CLionProjects/RegenX/src/lua-test/io_test.lua");

    if (script.empty()) {
        std::cerr << "Lua script failed to load or is empty\n";
        return;
    }

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    regenx_io(L);

    if (luaL_loadbuffer(L, script.data(), script.size(), "lua-test.lua") != LUA_OK ||
        lua_pcall(L, 0, 0, 0) != LUA_OK) {

        std::cerr << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        }

    lua_close(L);
}

void execute_lua_file() {
    initalize_lua_funtions();
}