#pragma once

#include "CommandFactory.h"
#include "CommandManager.h"
#include "LuaBrain.h"
#include "events/CollisionProperties.h"
#include "events/InputProperties.h"
#include <glm/mat4x4.hpp>

class ScriptingSystem
{
public:
	~ScriptingSystem();

	static ScriptingSystem* Get();

	// Where we really start/destroy everything that could go wrong
	bool Initialize(std::string baseScriptsPath);
	bool LoadScene(); 
	void Destroy();
	void LoadScript(EntityID entityID);

	// Lua commands
	void OnStart();
	void Update(double deltaTime);
	void OnCollision(sCollisionData* pCollision);
	void OnKeyInput(sKeyInfo keyInfo);

	/**
	 * Creates and send the new command to the command manager.
	 *
	 * @param json. A serialized batch of commands to be executed (See docs).
	 * @param isForever. If this command should run forever without a check
	 * 
	 * @return If batch was valid and added
	 */
	bool AddCommand(const char* json, bool isForever, uint16_t& UUIDOut);

	// Delete commands from forever list
	void DeleteForeverCommand(uint16_t UUID);

private:
	LuaBrain* m_pLuaBrain;
	CommandManager* m_pCommandManager;

	CommandFactory* m_pCommandFactory;

	//Singleton
	ScriptingSystem();

	static ScriptingSystem* m_pInstance;
};