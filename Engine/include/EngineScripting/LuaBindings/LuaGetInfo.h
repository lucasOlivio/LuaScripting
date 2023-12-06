#pragma once

#include "common/lua.h"

/**
* Get the normalized vector that represents the direction the object is facing
*
* @param const char. The entity tag to get the info from
*
* @return Returns x, y, z for the direction obj is facing
*/
int lua_GetFrontDirection(lua_State* L);

/**
* Get the transform info from an entity
*
* @param const char. The entity tag to get the info from
*
* @return Returns all the info about the transform (position, orientation, scale).
*/
int lua_GetTransform(lua_State* L);

/**
* Check key state
*
* @param int. Key to retrieve
*
* @return If the key is pressed or not.
*/
int lua_GetKey(lua_State* L);
