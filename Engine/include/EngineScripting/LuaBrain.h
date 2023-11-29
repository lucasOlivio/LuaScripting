#ifndef _LuaBrain_HG_
#define _LuaBrain_HG_

#include "scene/SceneView.h"

extern "C" {
#include <Lua5.4.6/lua.h>
#include <Lua5.4.6/lauxlib.h>
#include <Lua5.4.6/lualib.h>
}

#include <string>
#include <vector>
#include <map>

class LuaBrain
{
public:
	~LuaBrain();

	static LuaBrain* Get();

	bool Initialize(std::string baseScriptsPath, SceneView* pScene);
	void Destroy();

	bool LoadScene();

	bool LoadScript(EntityID entityID, std::string scriptName);
	void DeleteScript(EntityID entityID);

	// Call all the active scripts that are loaded
	void Update(float deltaTime);

	// Call all onstart for each script
	void OnStart();

	// Runs a script, but doesn't save it (originally used to set the ObjectID)
	void RunScriptImmediately(std::string script);

private:
	// Singleton
	LuaBrain() {};
	static LuaBrain* m_pInstance;

	std::string m_baseScriptsPath;
	SceneView* m_pScene;

	std::map< EntityID,
		      std::string /*scriptSource*/ > m_mapScripts;

	lua_State* m_pLuaState;

	std::string m_DecodeLuaErrorToString(int error);

	std::string m_ReadLuaScriptFile(std::string scriptName);
};

#endif
