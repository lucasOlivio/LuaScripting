#include "EngineScripting/commands/DestroyEntity.h"
#include "common/ParserJSON.h"
#include "components/Transform.h"

DestroyEntity::DestroyEntity()
{
}

bool DestroyEntity::Initialize(SceneView* pScene, rapidjson::Value& document)
{
    using namespace rapidjson;

    m_pScene = pScene;

    std::string entity;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "DestroyEntity";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetString(objEntt, entity);

    m_entityID = pScene->GetEntityByTag(entity);

    return true;
}

bool DestroyEntity::Update(double deltaTime)
{
    // Delete entity
    m_pScene->DeleteEntity(m_entityID);

    return true;
}

bool DestroyEntity::IsDone(void)
{
    // Only needs to run once
    return true;
}

bool DestroyEntity::PreStart(void)
{
    return true;
}

bool DestroyEntity::PostEnd(void)
{
    return true;
}
