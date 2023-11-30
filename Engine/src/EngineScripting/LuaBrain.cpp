#include "EngineScripting/LuaBrain.h"
#include "components/Script.h"
#include "common/utils.h"
#include "EngineScripting/luaBindings/LuaCommandDispatcher.h"
#include <iostream>
#include <fstream>
#include <sstream>

LuaBrain* LuaBrain::m_pInstance = nullptr;

LuaBrain::~LuaBrain()
{
	return;
}

LuaBrain* LuaBrain::Get()
{
	if (LuaBrain::m_pInstance == nullptr)
	{
		LuaBrain::m_pInstance = new LuaBrain();
	}
	return LuaBrain::m_pInstance;
}

bool LuaBrain::Initialize(std::string baseScriptsPath, SceneView* pScene)
{
	m_baseScriptsPath = baseScriptsPath;
	m_pScene = pScene;

	// Create new Lua state.
	m_pLuaState = luaL_newstate();

	luaL_openlibs(m_pLuaState);

	// Bind Lua integration functions
	lua_pushcfunction(m_pLuaState, lua_SendCommands);
	lua_setglobal(m_pLuaState, "SendCommands");

	return true;
}

void LuaBrain::Destroy()
{
	lua_close(m_pLuaState);
	return;
}

bool LuaBrain::LoadScene()
{
	m_mapScripts.clear();
	printf("Loading scripts...\n");
	
	// Lua functions bindings
	for (m_pScene->First("script"); !m_pScene->IsDone(); m_pScene->Next())
	{
		EntityID entityId = m_pScene->CurrentKey();
		ScriptComponent* pScript = m_pScene->CurrentValue<ScriptComponent>();

		bool isLoaded = LoadScript(entityId, pScript->scriptName);
		if (!isLoaded)
		{
			CheckEngineError(("Not able to load " + pScript->scriptName).c_str());
			return false;
		}
	}

	return true;
}


// Saves (and overwrites) any script
// scriptName is just so we can delete them later
bool LuaBrain::LoadScript(EntityID entityID, std::string scriptName)
{
	std::string scriptSource = m_ReadLuaScriptFile(scriptName);
	if (scriptSource == "")
	{
		return false;
	}

	m_mapScripts[entityID] = scriptSource;
	return true;
}


void LuaBrain::DeleteScript(EntityID entityID)
{
	m_mapScripts.erase(entityID);
	return;
}

void LuaBrain::OnStart()
{
	for (std::map< EntityID, std::string /*source*/>::iterator itScript =
		m_mapScripts.begin(); itScript != m_mapScripts.end(); itScript++)
	{
		EntityID entityID = itScript->first;
		std::string curLuaScript = itScript->second;

		// Run script to load functions
		RunScriptImmediately(curLuaScript);

		// Call the onstart function for each object
		lua_getglobal(m_pLuaState, "onstart");
		lua_pushnumber(m_pLuaState, entityID);
		int result = lua_pcall(m_pLuaState, 1, 0, 0);
		if (result != LUA_OK) {
			std::string luaError;
			// Get error information from top of stack (-1 is top)
			luaError.append(lua_tostring(m_pLuaState, -1));

			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running onstart: ";
			std::cout << "Entity: " << entityID << std::endl;
			std::cout << luaError << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			lua_pop(m_pLuaState, 1); // Pop the function from the stack
		}

	}

	return;
}

// Call all the active scripts that are loaded
void LuaBrain::Update(float deltaTime)
{
	for (std::map< EntityID, std::string /*source*/>::iterator itScript =
		m_mapScripts.begin(); itScript != m_mapScripts.end(); itScript++)
	{
		EntityID entityID = itScript->first;
		std::string curLuaScript = itScript->second;

		// Run script to load functions
		RunScriptImmediately(curLuaScript);

		// Call the update function for each object
		lua_getglobal(m_pLuaState, "update");
		lua_pushnumber(m_pLuaState, deltaTime);
		int result = lua_pcall(m_pLuaState, 1, 0, 0);
		if (result != LUA_OK) {
			std::string luaError;
			// Get error information from top of stack (-1 is top)
			luaError.append(lua_tostring(m_pLuaState, -1));

			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running update: ";
			std::cout << "Entity: " << entityID << std::endl;
			std::cout << luaError << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			lua_pop(m_pLuaState, 1); // Pop the function from the stack
		}

	}

	return;
}

// Runs a script, but doesn't save it (originally used to set the ObjectID)
void LuaBrain::RunScriptImmediately(std::string script)
{
	int error = luaL_loadstring(m_pLuaState,
		script.c_str());

	if (error != 0 /*no error*/)
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << m_DecodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		return;
	}

	// execute funtion in "protected mode", where problems are 
	//  caught and placed on the stack for investigation
	error = lua_pcall(m_pLuaState,	/* lua state */
	                  0,	/* nargs: number of arguments pushed onto the lua stack */
	                  0,	/* nresults: number of results that should be on stack at end*/
	                  0);	/* errfunc: location, in stack, of error function.
								if 0, results are on top of stack. */
	if (error != 0 /*no error*/)
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << m_DecodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append(lua_tostring(m_pLuaState, -1));

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(m_pLuaState, 1);  /* pop error message from the stack */
	}

	return;
}

std::string LuaBrain::m_DecodeLuaErrorToString(int error)
{
	switch (error)
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_YIELD:
		return "Lua: Lua yield";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error";
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}//switch ( error )

	// Who knows what this error is?
	return "Lua: UNKNOWN error";
}

std::string LuaBrain::m_ReadLuaScriptFile(std::string scriptName)
{
	// Open the file
	std::ifstream file(m_baseScriptsPath + scriptName);

	// Check if the file is opened successfully
	if (!file.is_open()) {
		CheckEngineError(("Error opening file: " + scriptName).c_str());
		return std::string(); // Return an empty string to indicate an error
	}

	// Read the contents of the file into a string
	std::stringstream buffer;
	buffer << file.rdbuf();

	// Close the file
	file.close();

	// Return the contents as a string
	return buffer.str();
}
