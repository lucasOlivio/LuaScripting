#include "EngineScripting/commands/CreateEntity.h"
#include "common/ParserJSON.h"
#include "components/Transform.h"

CreateEntity::CreateEntity()
{
}

bool CreateEntity::Initialize(SceneView* pScene, rapidjson::Value& document)
{
    using namespace rapidjson;

    m_pScene = pScene;

    std::string entity;
    glm::vec3 m_position;
    glm::vec3 m_orientation;

    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "CreateEntity";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetString(objEntt, entity);
    Value& objPos = document["position"];
    isValid &= parser.GetVec3(objPos, m_position);
    Value& objOrient = document["orientation"];
    isValid &= parser.GetVec3(objOrient, m_orientation);

    m_entityID = pScene->GetEntityByTag(entity);

    return true;
}

bool CreateEntity::Update(double deltaTime)
{
    // Create the entity
    EntityID newEntity = m_pScene->CreateEntity(m_entityID);

    // Set transform parameters
    TransformComponent* pTransform = m_pScene->GetComponent<TransformComponent>(newEntity, "transform");

    pTransform->SetPosition(m_position);
    pTransform->SetOrientation(m_orientation);

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
