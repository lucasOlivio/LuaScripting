#pragma once

#include "scene/SceneView.h"
#include "commands/CommandManager.h"
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

	void AddCommand(std::string command);

private:
	LuaBrain* m_pLuaBrain;
	CommandManager* m_pCommandManager;
	SceneView* m_pScene;

	//Singleton
	ScriptingSystem();

	static ScriptingSystem* m_pInstance;
};