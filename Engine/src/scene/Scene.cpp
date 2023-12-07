#include "scene/Scene.h"
#include "components/Model.h"
#include "components/Tag.h"
#include "components/Script.h"
#include "components/ComponentBuilder.h"
#include "EngineScripting/LuaBrain.h"

Scene* Scene::m_pInstance = nullptr;

Scene::Scene()
{
    m_isPlaying = false;
    m_numEntities = 0;
}

Scene::~Scene()
{
    m_components.clear();
}

Scene* Scene::Get()
{
    if (Scene::m_pInstance == nullptr)
    {
        Scene::m_pInstance = new Scene();
    }
    return Scene::m_pInstance;
}

void Scene::Clear()
{
    for (auto& pairComponent : m_components) {
        for (auto& pairEntityComp : pairComponent.second) {
            iComponent* pComp = pairEntityComp.second;
            pComp->SetDeleted(true);
            m_compToDestroy.push_back(pairEntityComp.second);
        }
        pairComponent.second.clear();
    }
    m_components.clear();
    m_numEntities = 0;
}

void Scene::ClearDeleted()
{
    for (iComponent* pComp : m_compToDestroy)
    {
        delete pComp;
    }
    m_compToDestroy.clear();
}

EntityID Scene::GetNumEntities()
{
    return m_numEntities;
}

EntityID Scene::CreateEntity()
{
    EntityID newEntityID = m_numEntities;
    m_numEntities++;
    return newEntityID;
}

EntityID Scene::CreateEntity(EntityID entityID, bool createAndActivate)
{
    EntityID newEntityID = CreateEntity();
    
    std::vector<sComponentInfo> componentsInfo = GetComponentsInfo(entityID);

    // Go for each component and copy it to the new one
    ComponentBuilder compBuilder = ComponentBuilder();
    for (sComponentInfo sCompInfo : componentsInfo)
    {
        iComponent* pNewComp = compBuilder.BuildComponent(sCompInfo, newEntityID);

        pNewComp->SetActive(createAndActivate);

        // Copy meshes info for models and animations too
        if (sCompInfo.componentName == "model")
        {
            ModelComponent* pNewModelComp = (ModelComponent*)pNewComp;
            ModelComponent* pModelComp = (ModelComponent*)GetComponent(entityID, "model");
            pNewModelComp->SetMeshes(pModelComp->GetMeshes());
        }
    }

    // Initializes script after because the script can uses other components
    iComponent* pComp = GetComponent(newEntityID, "script");
    if (pComp)
    {
        ScriptComponent* pNewScript = (ScriptComponent*)pComp;

        LuaBrain* pLua = LuaBrain::Get();
        // Initialize script
        pLua->LoadScript(newEntityID, pNewScript);
        pLua->OnStart(newEntityID);
    }

    return newEntityID;
}

EntityID Scene::CreateEntity(EntityID entityID)
{
    EntityID newEntityID = CreateEntity(entityID, true);

    return newEntityID;
}

void Scene::DeleteEntity(EntityID entityID)
{
    for (auto& pairComponent : m_components) {
        auto& innerMap = pairComponent.second;
        auto entityIter = innerMap.find(entityID);
        if (entityIter != innerMap.end()) {
            iComponent* pComp = entityIter->second;
            pComp->SetDeleted(true);
            m_compToDestroy.push_back(pComp);
            innerMap.erase(entityIter);
        }
    }
}

bool Scene::GetMapComponents(std::string componentName, std::map<EntityID, iComponent*>& componentsOut)
{
    std::map<std::string,
        std::map<EntityID, iComponent*>>::iterator it;
    it = m_components.find(componentName);

    if (it == m_components.end())
    {
        // No components of this type
        return false;
    }

    componentsOut = it->second;
    return true;
}

iComponent* Scene::GetComponent(EntityID entityID, std::string componentName)
{
    std::map<EntityID, iComponent*> mapComponents;

    GetMapComponents(componentName, mapComponents);

    std::map<EntityID, iComponent*>::iterator it;
    it = mapComponents.find(entityID);

    if (it == mapComponents.end())
    {
        // Entity doesn't have this kind of component
        return nullptr;
    }

    return it->second;
}

void Scene::SetComponent(EntityID entityID, std::string componentName, iComponent* componentIn)
{
    // Check the entity already have this component
    iComponent* compToDelete = GetComponent(entityID, componentName);

    // If already have, delete it first
    if (compToDelete)
    {
        compToDelete->SetDeleted(true);
        m_compToDestroy.push_back(compToDelete);
    }

    // Now replace with the new component
    m_components[componentName][entityID] = componentIn;

    return;
}

std::vector<sComponentInfo> Scene::GetComponentsInfo(EntityID entityID)
{
    std::vector<sComponentInfo> componentsInfo;

    // Go through each component type in the scene
    for (const auto& pairComponents : m_components)
    {
        // Check if user has this component
        const auto& itComponents = pairComponents.second.find(entityID);
        if (itComponents == pairComponents.second.end())
        {
            continue;
        }

        // Get info from component and attach to vector
        sComponentInfo componentInfo;
        itComponents->second->GetInfo(componentInfo);

        componentsInfo.push_back(componentInfo);
    }

    return componentsInfo;
}

std::map<std::string, iComponent*> Scene::GetComponents(EntityID entityID)
{
    std::map<std::string, iComponent*> components;

    // Go through each component type in the scene
    for (const auto& pairComponents : m_components)
    {
        // Check if user has this component
        const auto& itComponents = pairComponents.second.find(entityID);
        if (itComponents == pairComponents.second.end())
        {
            continue;
        }

        // Get info from component and attach to vector
        components[pairComponents.first] = itComponents->second;
    }

    return components;
}

bool Scene::IsPlaying()
{
    if (m_isPlaying)
    {
        return true;
    } 
    else
    {
        return false;
    }
}

void Scene::SetPlaying(bool isPlaying)
{
    m_isPlaying = isPlaying;
}
