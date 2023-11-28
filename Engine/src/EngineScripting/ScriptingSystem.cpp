#include "EngineScripting/ScriptingSystem.h"
#include "EngineScripting/CommandFactory.h"
#include "common/utils.h"

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

bool ScriptingSystem::Initialize(std::string baseScriptsPath, SceneView* pScene)
{
    m_pScene = pScene;

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

    iCommand* pComm = CommandFactory::CreateCommand("MoveTo", 5, 0, 0.0, 0.0, 0.0, 0.0);

    return true;
}

bool ScriptingSystem::LoadScene()
{
    bool isLoaded = m_pLuaBrain->LoadScene();
    if (!isLoaded)
    {
        CheckEngineError("lua brain loading error\n\n");
        return false;
    }

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

void ScriptingSystem::AddCommand(std::string command)
{
}
