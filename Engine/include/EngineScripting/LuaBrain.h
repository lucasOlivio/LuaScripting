#ifndef _LuaBrain_HG_
#define _LuaBrain_HG_

#include "common/lua.h"
#include "common/types.h"
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

	bool LoadScript(EntityID entityID, std::string scriptName);
	void DeleteScript(EntityID entityID);

	// Call all onstart for each script
	void OnStart();

	// Call all the active scripts that are loaded
	void Update(float deltaTime);

	// Call collision function only for this entity
	void OnCollision(EntityID entityId, std::string tagCollided);

	// Runs a script, but doesn't save it (originally used to set the ObjectID)
	void RunScriptImmediately(std::string script);

private:
	// Singleton
	LuaBrain() {};
	static LuaBrain* m_pInstance;

	std::string m_baseScriptsPath;

	std::map< EntityID,
		      std::string /*scriptSource*/ > m_mapScripts;

	lua_State* m_pLuaState;

	std::string m_DecodeLuaErrorToString(int error);

	std::string m_ReadLuaScriptFile(std::string scriptName);
};

#endif
