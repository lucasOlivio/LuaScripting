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
    std::string tagToReplace;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "DestroyEntity";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetString(objEntt, entity);
    Value& objReplace = document["isReplace"];
    isValid &= parser.GetBool(objReplace, m_isReplace);
    Value& objNewTag = document["tagToReplace"];
    isValid &= parser.GetString(objNewTag, tagToReplace);

    m_entityID = pScene->GetEntityByTag(entity);
    
    if (m_isReplace)
    {
        m_entityToReplace = pScene->GetEntityByTag(tagToReplace);
    }
    
    return true;
}

bool DestroyEntity::Update(double deltaTime)
{
    if (m_isReplace && m_entityToReplace < m_pScene->GetNumEntities())
    {
        // Create new entity to replace
        EntityID newEntity = m_pScene->CreateEntity(m_entityToReplace);

        // Set new entity to same position as the old
        TransformComponent* pOldTransform = m_pScene->GetComponent<TransformComponent>(m_entityID, "transform");
        TransformComponent* pTransform = m_pScene->GetComponent<TransformComponent>(newEntity, "transform");

        pTransform->SetPosition(pOldTransform->GetPosition());
    }

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
