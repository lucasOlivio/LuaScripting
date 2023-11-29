#pragma once

extern "C" {
#include <Lua5.4.6/lua.h>
#include <Lua5.4.6/lauxlib.h>
#include <Lua5.4.6/lualib.h>
}

/**
* Passes json string with command batch to execute.
* 
* @param const char. A valid json batch serialized (See docs).
* @param bool. If the command should run forever or not.
* 
* @return if was a valid command batch and the command id.
*/
int SendCommands(lua_State* L);
