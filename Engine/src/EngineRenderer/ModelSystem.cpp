#include "EngineRenderer/ModelSystem.h"
#include "components/iComponent.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "common/opengl.h"
#include "common/utilsMat.h"
#include "scene/SceneView.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr\

ModelSystem::ModelSystem(iShaderInfo* pShaderInfo)
{
    m_pShaderInfo = pShaderInfo;
    m_pVAOManager = new VAOManager(m_pShaderInfo);
}

ModelSystem::~ModelSystem()
{
    delete m_pVAOManager;
}

bool ModelSystem::LoadModels(int shaderID)
{
    for (SceneView::Get()->First("model"); !SceneView::Get()->IsDone(); SceneView::Get()->Next())
    {
        ModelComponent* pModel = SceneView::Get()->CurrentValue<ModelComponent>();
        // For now only debug objects will need to be dynamic
        for (int i = 0; i < pModel->models.size(); i++)
        {
            sMesh* pMesh = m_pVAOManager->LoadModelIntoVAO(pModel->models[i], shaderID, false);
            if (!pMesh)
            {
                return false;
            }
            pModel->SetMesh(i, pMesh);
        }

        if (pModel->collisionName == "")
        {
            continue;
        }

        pModel->pCollisionMesh = m_pVAOManager->LoadModelIntoVAO(pModel->collisionName, shaderID, false);
        if (!pModel->pCollisionMesh)
        {
            return false;
        }
    }

    return true;
}

bool ModelSystem::LoadModels(std::string basePath, int shaderID)
{
    printf("Loading models...\n");
    m_pVAOManager->SetBasePath(basePath);

    bool isLoaded = LoadModels(shaderID);
    if (isLoaded)
    {
        return true;
    }
    return false;
}

void ModelSystem::Destroy()
{
    for (SceneView::Get()->First("model"); !SceneView::Get()->IsDone(); SceneView::Get()->Next())
    {
        ModelComponent* pModel = SceneView::Get()->CurrentValue<ModelComponent>();
        
        for (std::string model : pModel->models)
        {
            m_pVAOManager->DestroyVBO(model);
        }
    }
}

