#include "EngineScripting/LuaBrain.h"
#include "common/utils.h"
#include "EngineScripting/luaBindings/LuaCommandDispatcher.h"
#include "EngineScripting/luaBindings/LuaGetInfo.h"
#include "scene/SceneView.h"
#include <iostream>
#include <fstream>
#include <sstream>

LuaBrain* LuaBrain::m_pInstance = nullptr;

// Default variables and functions names from lua script
const char* ON_START_NAME     = "onstart";
const char* ON_UPDATE_NAME    = "onupdate";
const char* ON_COLLISION_NAME = "oncollision";
const char* ON_KEY_INPUT_NAME = "onkeyinput";
const char* GLOBALS_NAME      = "tbGlobals";
const char* ENTITY_NAME       = "entity";

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

bool LuaBrain::Initialize(std::string baseScriptsPath)
{
	m_baseScriptsPath = baseScriptsPath;

	// Create new Lua state.
	m_pLuaState = luaL_newstate();

	luaL_openlibs(m_pLuaState);

	// Bind Lua integration functions
	lua_pushcfunction(m_pLuaState, lua_SendCommands);
	lua_setglobal(m_pLuaState, "SendCommands");
	lua_pushcfunction(m_pLuaState, lua_CancelCommand);
	lua_setglobal(m_pLuaState, "CancelCommand");
	lua_pushcfunction(m_pLuaState, lua_GetFrontDirection);
	lua_setglobal(m_pLuaState, "GetFrontDirection");
	lua_pushcfunction(m_pLuaState, lua_GetTransform);
	lua_setglobal(m_pLuaState, "GetTransform");
	lua_pushcfunction(m_pLuaState, lua_GetKey);
	lua_setglobal(m_pLuaState, "GetKey");

	return true;
}

void LuaBrain::Destroy()
{
	lua_close(m_pLuaState);
	return;
}

bool LuaBrain::LoadScene()
{
	printf("Loading scripts...\n");
	
	// Lua functions bindings
	for (SceneView::Get()->First("script"); !SceneView::Get()->IsDone(); SceneView::Get()->Next())
	{
		EntityID entityId = SceneView::Get()->CurrentKey();
		ScriptComponent* pScript = SceneView::Get()->CurrentValue<ScriptComponent>();

		if (!pScript->IsActive())
		{
			continue;
		}

		bool isLoaded = LoadScript(entityId, pScript);
		if (!isLoaded)
		{
			CheckEngineError(("Not able to load " + pScript->scriptName).c_str());
			return false;
		}
	}

	return true;
}


bool LuaBrain::LoadScript(EntityID entityId)
{
	ScriptComponent* pScript = SceneView::Get()->GetComponent<ScriptComponent>(entityId, "script");

	return LoadScript(entityId, pScript);
}

// Saves (and overwrites) any script
// scriptName is just so we can delete them later
bool LuaBrain::LoadScript(EntityID entityId, ScriptComponent* pScript)
{
	std::string scriptSource = m_ReadLuaScriptFile(pScript->scriptName);
	if (scriptSource == "")
	{
		return false;
	}

	// Run script to load functions
	RunScriptImmediately(scriptSource);

	// Each script have its own table on registry to keep its objects
	int tbIdx = m_CreateTableRegistry();
	pScript->SetTableRegistry(tbIdx);

	// Set entity value in globals for script
	lua_pushnumber(m_pLuaState, entityId);
	lua_setglobal(m_pLuaState, ENTITY_NAME);
	int entityIdx = m_LoadObjectRegistry(tbIdx, ENTITY_NAME);
	pScript->SetLuaObject(ENTITY_NAME, entityIdx);

	// Load global variables into registry
	int globalsIdx = m_LoadObjectRegistry(tbIdx, GLOBALS_NAME);
	pScript->SetLuaObject(GLOBALS_NAME, globalsIdx);

	// Load all functions into registry
	int onstartIdx = m_LoadObjectRegistry(tbIdx, ON_START_NAME);
	int onupdateIdx = m_LoadObjectRegistry(tbIdx, ON_UPDATE_NAME);
	int oncollisionIdx = m_LoadObjectRegistry(tbIdx, ON_COLLISION_NAME);
	int onkeyinputIdx = m_LoadObjectRegistry(tbIdx, ON_KEY_INPUT_NAME);

	pScript->SetLuaObject(ON_START_NAME, onstartIdx);
	pScript->SetLuaObject(ON_UPDATE_NAME, onupdateIdx);
	pScript->SetLuaObject(ON_COLLISION_NAME, oncollisionIdx);
	pScript->SetLuaObject(ON_KEY_INPUT_NAME, onkeyinputIdx);

	return true;
}


void LuaBrain::DeleteScript(EntityID entityID)
{
	// Remove scripts functions loaded from registry
	ScriptComponent* pScript = SceneView::Get()->GetComponent<ScriptComponent>(entityID, "script");
	luaL_unref(m_pLuaState, LUA_REGISTRYINDEX, pScript->GetTableRegistry());

	return;
}

void LuaBrain::OnStart()
{
	SceneView* pScene = SceneView::Get();
	for (pScene->First("script"); !pScene->IsDone(); pScene->Next())
	{
		EntityID entityId = pScene->CurrentKey();
		ScriptComponent* pScript = pScene->CurrentValue<ScriptComponent>();
		int tbIdx = pScript->GetTableRegistry();
		int fncIdx = pScript->GetLuaObject(ON_START_NAME);
		int globalsIdx = pScript->GetLuaObject(GLOBALS_NAME);
		int entityIdx = pScript->GetLuaObject(ENTITY_NAME);

		if (fncIdx == LUA_REFNIL)
		{
			// Script doesn`t have this function
			continue;
		}

		// For now we do this here separately because we haven't found a way
		// to load the function then load the parameters in a generic way
		m_PreFunctionCall(tbIdx, entityIdx, globalsIdx, fncIdx);

		// Call the onstart function for each object
		int result = m_CallFunction(0, 0);

		m_PosFunctionCall(result);
	}

	return;
}

// Call all the active scripts that are loaded
void LuaBrain::Update(float deltaTime)
{
	SceneView* pScene = SceneView::Get();
	for (pScene->First("script"); !pScene->IsDone(); pScene->Next())
	{
		EntityID entityId = pScene->CurrentKey();
		ScriptComponent* pScript = pScene->CurrentValue<ScriptComponent>();
		int tbIdx = pScript->GetTableRegistry();
		int fncIdx = pScript->GetLuaObject(ON_UPDATE_NAME);
		int globalsIdx = pScript->GetLuaObject(GLOBALS_NAME);
		int entityIdx = pScript->GetLuaObject(ENTITY_NAME);

		if (fncIdx == LUA_REFNIL)
		{
			// Script doesn`t have this function
			continue;
		}

		// For now we do this here separately because we haven't found a way
		// to load the function then load the parameters in a generic way
		m_PreFunctionCall(tbIdx, entityIdx, globalsIdx, fncIdx);

		lua_pushnumber(m_pLuaState, deltaTime);

		int result = m_CallFunction(1, 0);

		m_PosFunctionCall(result);
	}

	return;
}

void LuaBrain::OnCollision(EntityID entityID, std::string tagCollided)
{
	SceneView* pScene = SceneView::Get();
	for (pScene->First("script"); !pScene->IsDone(); pScene->Next())
	{
		EntityID entityId = pScene->CurrentKey();
		ScriptComponent* pScript = pScene->CurrentValue<ScriptComponent>();
		int tbIdx = pScript->GetTableRegistry();
		int fncIdx = pScript->GetLuaObject(ON_COLLISION_NAME);
		int globalsIdx = pScript->GetLuaObject(GLOBALS_NAME);
		int entityIdx = pScript->GetLuaObject(ENTITY_NAME);

		if (fncIdx == LUA_REFNIL)
		{
			// Script doesn`t have this function
			continue;
		}

		// For now we do this here separately because we haven't found a way
		// to load the function then load the parameters in a generic way
		m_PreFunctionCall(tbIdx, entityIdx, globalsIdx, fncIdx);

		lua_pushstring(m_pLuaState, tagCollided.c_str());

		int result = m_CallFunction(1, 0);

		m_PosFunctionCall(result);
	}

	return;
}

void LuaBrain::OnKeyInput(sKeyInfo keyInfo)
{
	SceneView* pScene = SceneView::Get();
	for (pScene->First("script"); !pScene->IsDone(); pScene->Next())
	{
		EntityID entityId = pScene->CurrentKey();
		ScriptComponent* pScript = pScene->CurrentValue<ScriptComponent>();
		int tbIdx = pScript->GetTableRegistry();
		int fncIdx = pScript->GetLuaObject(ON_KEY_INPUT_NAME);
		int globalsIdx = pScript->GetLuaObject(GLOBALS_NAME);
		int entityIdx = pScript->GetLuaObject(ENTITY_NAME);

		if (fncIdx == LUA_REFNIL)
		{
			// Script doesn`t have this function
			continue;
		}

		// For now we do this here separately because we haven't found a way
		// to load the function then load the parameters in a generic way
		m_PreFunctionCall(tbIdx, entityIdx, globalsIdx, fncIdx);

		lua_pushnumber(m_pLuaState, keyInfo.pressedKey);
		lua_pushnumber(m_pLuaState, keyInfo.action);
		lua_pushnumber(m_pLuaState, keyInfo.mods);
		lua_pushnumber(m_pLuaState, keyInfo.scanCode);

		int result = m_CallFunction(4, 0);

		m_PosFunctionCall(result);
	}

	return;
}

// Runs a script, but doesn't save it
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

int LuaBrain::m_CreateTableRegistry()
{
	lua_newtable(m_pLuaState);
	return luaL_ref(m_pLuaState, LUA_REGISTRYINDEX);
}

int LuaBrain::m_LoadObjectRegistry(int tableIdx, const char* objName)
{
	// Retrieve table registry to store object
	lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, tableIdx);
	// Retrieve object named "objName" to store
	lua_getglobal(m_pLuaState, objName);
	// store object in the object table
	// (-2 table is 2 places up the current stack)
	int funcIdx = luaL_ref(m_pLuaState, -2); 

	// Done with the object table, so pop it
	lua_pop(m_pLuaState, 1); 

	return funcIdx;
}

void LuaBrain::m_GetTableRegistry(int tableIdx)
{
	lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, tableIdx); // retrieve function table

	return;
}

void LuaBrain::m_GetObjectRegistry(int tableIdx, int objIdx)
{
	// Table should aways be at the top
	m_GetTableRegistry(tableIdx);
	lua_rawgeti(m_pLuaState, -1, objIdx);				   // retrieve object

	return;
}

int LuaBrain::m_CallFunction(int numParameters, int numReturns)
{
	using namespace std;

	// Call the top stack function
	int result = lua_pcall(m_pLuaState, numParameters, numReturns, 0);
	if (result != LUA_OK) 
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << m_DecodeLuaErrorToString(result) << std::endl;

		string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append(lua_tostring(m_pLuaState, -1));

		cout << "-------------------------------------------------------" << endl;
		cout << "Error running function: ";
		cout << luaError << endl;
		cout << "-------------------------------------------------------" << endl;
		lua_pop(m_pLuaState, 1);
	}

	return result;
}

bool LuaBrain::m_PreFunctionCall(int tbIdx, int entityIdx, int globalsIdx, int fncIdx)
{
	// Load entity id into stack
	m_GetObjectRegistry(tbIdx, entityIdx);
	lua_setglobal(m_pLuaState, ENTITY_NAME);

	// Load stored globals into stack
	m_GetObjectRegistry(tbIdx, globalsIdx);
	lua_setglobal(m_pLuaState, GLOBALS_NAME);

	// Load function into stack
	m_GetObjectRegistry(tbIdx, fncIdx);

	return true;
}

void LuaBrain::m_PosFunctionCall(int result)
{
	lua_settop(m_pLuaState, 0);
}
