#include "EngineScripting/ScriptingSystem.h"
#include "common/utils.h"
#include <cstdarg>

ScriptingSystem* ScriptingSystem::m_pInstance = nullptr;

ScriptingSystem::ScriptingSystem()
{

}

ScriptingSystem::~ScriptingSystem()
{
}

ScriptingSystem* ScriptingSystem::Get()
{
    if (ScriptingSystem::m_pInstance == nullptr)
    {
        ScriptingSystem::m_pInstance = new ScriptingSystem();
    }
    return ScriptingSystem::m_pInstance;
}

#include <chrono>
#include <iostream>
bool ScriptingSystem::Initialize(std::string baseScriptsPath, SceneView* pScene)
{
    m_pScene = pScene;
    m_pCommandFactory = new CommandFactory(m_pScene);

    // Initializes lua settings 
    m_pLuaBrain = LuaBrain::Get();

    bool isLuaInit = m_pLuaBrain->Initialize(baseScriptsPath, m_pScene);
    if (!isLuaInit)
    {
        CheckEngineError("Engine lua brain initialization");
        return false;
    }

    // Initializes command manager
    m_pCommandManager = new CommandManager();

    return true;
}

bool ScriptingSystem::LoadScene()
{
    m_pCommandManager->ClearCommands();

    bool isLoaded = m_pLuaBrain->LoadScene();
    if (!isLoaded)
    {
        CheckEngineError("lua brain loading error\n\n");
        return false;
    }

    const char* test = "{\"entity\": 0, \"location\": [-200.0, 0.0, 0.0], \"time\": 3}";
    AddCommand("MoveTo", false, test);

    return true;
}

void ScriptingSystem::Destroy()
{
    m_pLuaBrain->Destroy();
    delete m_pLuaBrain;
    delete m_pCommandManager;
}

void ScriptingSystem::Update(double deltaTime)
{
    m_pLuaBrain->Update(deltaTime);
    m_pCommandManager->Update(deltaTime);
}

void ScriptingSystem::AddCommand(const char* command, bool isForever, const char* args)
{
    using namespace std;

    iCommand* pComm = m_pCommandFactory->CreateCommand(command, args);

    if (pComm == nullptr)
    {
        // command not found or arguments wrong
        CheckEngineError(m_pCommandFactory->GetError().c_str());
        return;
    }

    // Add command to the command manager
    if (isForever)
    {
        m_pCommandManager->AddForeverCommand(pComm);
    }
    else
    {
        m_pCommandManager->AddCommand(pComm);
    }
}