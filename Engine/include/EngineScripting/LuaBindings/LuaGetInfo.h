#pragma once

#include "common/lua.h"

/**
* Get the transform info from an entity
*
* @param const char. The entity tag to get the info from
*
* @return Returns all the info about the transform (position, orientation, scale).
*/
int lua_GetTransform(lua_State* L);
