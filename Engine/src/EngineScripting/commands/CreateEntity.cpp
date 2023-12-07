#include "EngineScripting/commands/CreateEntity.h"
#include "common/ParserJSON.h"
#include "components/Transform.h"
#include "scene/Scene.h"
#include "scene/SceneView.h"
#include "common/utilsMat.h"

CreateEntity::CreateEntity()
{
}

bool CreateEntity::Initialize(rapidjson::Value& document)
{
    using namespace rapidjson;

    std::string spawnEntity;
    SceneView* pScene = SceneView::Get();

    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "CreateEntity";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, m_parentEntity);
    Value& objSpawnEntt = document["spawnentity"];
    isValid &= parser.GetString(objSpawnEntt, spawnEntity);
    Value& objPos = document["position"];
    isValid &= parser.GetVec3(objPos, m_position);
    Value& objOrient = document["direction"];
    isValid &= parser.GetVec3(objOrient, m_orientation);

    m_childEntity = SceneView::Get()->GetEntityByTag(spawnEntity);

    TransformComponent* pTransfParent = pScene->GetComponent<TransformComponent>(m_parentEntity, "transform");

    m_orientation = pTransfParent->GetRelativeVector(m_orientation);

    return isValid;
}

bool CreateEntity::Update(double deltaTime)
{
    using namespace glm;
    using namespace myutils;

    SceneView* pScene = SceneView::Get();

    // Create the entity
    EntityID newEntity = Scene::Get()->CreateEntity(m_childEntity);

    // Convert to forward of orientation
    TransformComponent* pTransfChild = pScene->GetComponent<TransformComponent>(newEntity, "transform");

    // Set transform parameters
    pTransfChild->SetPosition(m_position);
    pTransfChild->AdjustOrientation(m_orientation);

    return true;
}

bool CreateEntity::IsDone(void)
{
    // Only needs to run once
    return true;
}

bool CreateEntity::PreStart(void)
{
    return true;
}

bool CreateEntity::PostEnd(void)
{
    return true;
}
