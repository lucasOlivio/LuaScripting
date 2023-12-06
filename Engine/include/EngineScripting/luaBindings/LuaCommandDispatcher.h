#pragma once

#include "common/lua.h"

/**
* Passes json string with command batch to execute.
* 
* @param const char. A valid json batch serialized (See docs).
* @param bool. If the command should run forever or not.
* 
* @return if was a valid command batch and the command id.
*/
int lua_SendCommands(lua_State* L);

/**
* Cancel a forever command from the command manager
*
* @param int. UUID of the forever command.
*/
int lua_CancelCommand(lua_State* L);
