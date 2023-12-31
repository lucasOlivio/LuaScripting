#pragma once

#include "scene/Scene.h"
#include "scene/SceneView.h"
#include "scene/iSceneDirector.h"
#include "EngineRenderer/Renderer.h"
#include "EngineEditor/Editor.h"
#include "EnginePhysics/Physics.h"
#include "EngineMedia/MediaPlayer.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/CollisionEvent.h"
#include "common/Input.h"
#include "EngineDebug/DebugSystem.h"
#include "EngineScripting/ScriptingSystem.h"

class Engine :
	public iSceneDirector
{
public:
	// ctors & dtors
	virtual ~Engine();

	static Engine* Get();

	// Initializes all the systems, if anything go wrong we should stop here
	virtual bool Initialize(const std::string& sceneName);

	// Run app and update all systems every frame
	void Run();
	virtual void Update(double fixedDeltaTime);

	bool IsRunning();

	double GetDeltaTime();

	// Close app and destroy all
	void Exit();
	void Exit(std::string errorMsg);

	// Window and camera info
	GLFWwindow* GetWindow();
	glm::mat4 GetWindowProjectionMat();
	void GetWindowSize(int& windowHeight, int& windowWidth);
	void CaptureMouse();
	void ReleaseMouse();

	glm::mat4 GetViewMat();

	virtual void SetRunning(bool isRunning);
	// Change between edit mode and play mode
	virtual void ChangeMode();

	// Save all components data to file
	virtual void SaveScene(std::string filePath);
	virtual void SaveScene();

	// Load all components data from file
	virtual bool LoadScene(std::string filePath);
	virtual bool LoadScene();

protected:
	bool m_isRunning;
	bool m_isInitialized;
	double m_lastTime;
	std::vector<double> m_frameTimes;

	// TODO: All this should be "systems" with an interface and be 
	// able to attach itself to the engine to be started and updated
	Renderer* m_pRenderer;
	Editor* m_pEditor;
	Physics* m_pPhysics;
	MediaPlayer* m_pMediaPlayer;
	ScriptingSystem* m_pScriptingSystem;
	DebugSystem* m_pDebugSystem;
	WindowSystem* m_pWindowSystem;
	ShaderManager* m_pShaderManager;
	uint m_currShaderID;

	KeyEvent* m_pKeyEvent;
	MouseEvent* m_pMouseEvent;
	CollisionEvent* m_pCollisionEvent;

	Input* m_pInput;

	Scene* m_pScene;
	SceneView* m_pSceneView;

	// Singleton
	Engine();
	static Engine* m_pInstance;
};