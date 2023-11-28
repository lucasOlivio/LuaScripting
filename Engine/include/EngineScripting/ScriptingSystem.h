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
	 * @param command. Name of the command to be created.
	 * @param isForever. If this command should run forever without a check
	 * @param nArgs. Number of parameters been passed.
     * @param args... variable number of parameters for the command.
	 */
	void AddCommand(const char* command, bool isForever, const char* args);

private:
	LuaBrain* m_pLuaBrain;
	CommandManager* m_pCommandManager;
	SceneView* m_pScene;

	CommandFactory* m_pCommandFactory;

	//Singleton
	ScriptingSystem();

	static ScriptingSystem* m_pInstance;
	
	
};