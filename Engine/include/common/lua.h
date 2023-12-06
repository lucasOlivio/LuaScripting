#pragma once

extern "C" {
#include <Lua5.4.6/lua.h>
#include <Lua5.4.6/lauxlib.h>
#include <Lua5.4.6/lualib.h>
}

// Debug stack, print stack curr size and all stack objects in console
void lua_StackDump(lua_State* l);