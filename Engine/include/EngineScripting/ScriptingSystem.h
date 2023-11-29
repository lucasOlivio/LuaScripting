#pragma once

#include "scene/SceneView.h"
#include "CommandFactory.h"
#include "CommandManager.h"
#include "LuaBrain.h"
#include <glm/mat4x4.hpp>

class ScriptingSystem
{
public:
	~ScriptingSystem();

	static ScriptingSystem* Get();

	// Where we really start/destroy everything that could go wrong
	bool Initialize(std::string baseScriptsPath, SceneView* pScene);
	bool LoadScene();
	void Destroy();

	void Update(double deltaTime);

	/**
	 * Creeates and send the new command to the command manager.
	 *
	 * @param json. A serialized batch of commands to be executed (See docs).
	 * @param isForever. If this command should run forever without a check
	 * 
	 * @return If batch was valid and added
	 */
	bool AddCommand(const char* json, bool isForever, uint64_t UUIDOut);

private:
	LuaBrain* m_pLuaBrain;
	CommandManager* m_pCommandManager;
	SceneView* m_pScene;

	CommandFactory* m_pCommandFactory;

	//Singleton
	ScriptingSystem();

	static ScriptingSystem* m_pInstance;
	
	
};