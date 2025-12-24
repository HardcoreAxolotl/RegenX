extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <iostream>

int l_engine_log(lua_State* L) {
    const char* msg = luaL_checkstring(L, 1);
    std::cout << "[Lua] " << msg << std::endl;
    return 0;
}

int l_engine_add(lua_State* L) {
    int a = luaL_checkinteger(L, 1);
    int b = luaL_checkinteger(L, 2);
    lua_pushinteger(L, a + b);
    return 1;
}

int main() {
    std::string script = R"(Engine.log("Hello")
print(Engine.add(2, 3)))";

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    static const luaL_Reg engine_funcs[] = {
        {"log", l_engine_log},
        {"add", l_engine_add},
        {nullptr, nullptr}
    };

    luaL_newlib(L, engine_funcs);
    lua_setglobal(L, "Engine");

    if (luaL_dostring(L, script.c_str()) != LUA_OK) {
        std::cerr << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    lua_close(L);
    return 0;
}
