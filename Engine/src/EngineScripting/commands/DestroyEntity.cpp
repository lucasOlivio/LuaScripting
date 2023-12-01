#include "EngineScripting/commands/DestroyEntity.h"
#include "common/ParserJSON.h"
#include "components/Transform.h"
#include "scene/Scene.h"
#include "scene/SceneView.h"

DestroyEntity::DestroyEntity()
{
}

bool DestroyEntity::Initialize(rapidjson::Value& document)
{
    using namespace rapidjson;

    std::string entity;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "DestroyEntity";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetString(objEntt, entity);

    m_entityID = SceneView::Get()->GetEntityByTag(entity);

    return true;
}

bool DestroyEntity::Update(double deltaTime)
{
    // Delete entity
    Scene::Get()->DeleteEntity(m_entityID);

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
