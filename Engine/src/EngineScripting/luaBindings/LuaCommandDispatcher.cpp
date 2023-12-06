#include "EngineScripting/LuaBindings/LuaCommandDispatcher.h"
#include "EngineScripting/ScriptingSystem.h"

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

	lua_pop(L, 2);

	lua_pushboolean(L, true);
	lua_pushnumber(L, UUID);

	return 2;
}

int lua_CancelCommand(lua_State* L)
{
	// Command UUID to be canceled
	uint16_t commandUUID = lua_tonumber(L, 1);

	// Send to system
	ScriptingSystem::Get()->DeleteForeverCommand(commandUUID);

	return 0;
}
