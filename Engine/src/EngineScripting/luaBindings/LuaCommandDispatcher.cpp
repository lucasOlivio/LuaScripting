#include "EngineScripting/LuaBindings/LuaCommandDispatcher.h"
#include "EngineScripting/ScriptingSystem.h"
#include <iostream>

int lua_SendCommands(lua_State* L)
{
	// Batch serialized
	const char* json = lua_tostring(L, 1);
	bool isForever = lua_toboolean(L, 2);

	// Send to system
	uint16_t UUID = 0;
	bool isValid = ScriptingSystem::Get()->AddCommand(json, isForever, UUID);

	if (!isValid)
	{
		// Invalid batch of commands
		lua_pushboolean(L, false);

		return 1;
	}

	std::cout << "UUID C++: " << UUID << std::endl;
	lua_pushboolean(L, true);
	lua_pushnumber(L, UUID);

	return 2;
}
