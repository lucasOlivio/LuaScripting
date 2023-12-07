#ifndef _LuaBrain_HG_
#define _LuaBrain_HG_

#include "common/lua.h"
#include "common/types.h"
#include "events/InputProperties.h"
#include "components/Script.h"
#include <string>
#include <vector>
#include <map>

class LuaBrain
{
public:
	~LuaBrain();

	static LuaBrain* Get();

	bool Initialize(std::string baseScriptsPath);
	void Destroy();

	bool LoadScene();

	bool LoadScript(EntityID entityId);
	bool LoadScript(EntityID entityId, ScriptComponent* pScript);
	void DeleteScript(EntityID entityID);

	// Call all onstart for each script
	void OnStart();
	// Call all onstart for this entity
	void OnStart(EntityID entityId);

	// Call all onupdate for each script
	void Update(float deltaTime);

	// Call oncollision function only for this entity
	void OnCollision(EntityID entityId, std::string tagCollided);

	// Call onkeyinput for every script listening
	void OnKeyInput(sKeyInfo keyInfo);

	// Runs a script, but doesn't save it
	void RunScriptImmediately(std::string script);

private:
	lua_State* m_pLuaState;

	std::string m_DecodeLuaErrorToString(int error);

	std::string m_ReadLuaScriptFile(std::string scriptName);

	// Loads a new table into registry and return the table index from lua registry
	int m_CreateTableRegistry();

	// Load the given object into the given table registry
	// (The object must already be loaded from script)
	// Returns the object index into the table registry
	int m_LoadObjectRegistry(int tableIdx, const char* funcName);

	// Retrieve the table from the registry to the stack
	void m_GetTableRegistry(int tableIdx);

	// Retrieve the object from the registry to the stack
	void m_GetObjectRegistry(int tableIdx, int objIdx);

	// Calls the next function on the stack
	// Returns the result of the call
	int m_CallFunction(int numParameters, int numReturns);

	// Setup all global variables and load function into stack
	bool m_PreFunctionCall(int tbIdx, int entityIdx, int globalsIdx, int fncIdx);

	// Remove globals from stack and update globals on registry if function is OK
	void m_PosFunctionCall(int result);

	// Save the scripts global variables from table "tbGlobals" into component
	void m_SaveGlobals(int tableIdx, char* tbGlobals, ScriptComponent* pScript);

	// Singleton
	LuaBrain() {};
	static LuaBrain* m_pInstance;

	std::string m_baseScriptsPath;
};

#endif
