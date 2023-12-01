#include "EngineRenderer/LightSystem.h"
#include "components/Light.h"
#include "components/Transform.h"
#include "common/utilsMat.h"
#include "scene/SceneView.h"
#include <glm/gtc/quaternion.hpp>

LightSystem::LightSystem(iShaderInfo* pShaderInfo)
{
    m_pShaderInfo = pShaderInfo;
}

LightSystem::~LightSystem()
{
}

bool LightSystem::Initialize(int shaderID)
{
    printf("Setting up lights...\n");
    // Go over each lights setting the ULs and initializing them
    int lightIndex = 0;
    for (SceneView::Get()->First("light"); !SceneView::Get()->IsDone(); SceneView::Get()->Next())
    {
        std::string ulBasePath = "theLights[" + std::to_string(lightIndex) + "].";

        LightComponent* pLight = SceneView::Get()->CurrentValue<LightComponent>();
        pLight->SetupLight(shaderID, ulBasePath);
        lightIndex++;
    }

    return true;
}

void LightSystem::Update(double deltatime)
{
    using namespace glm;

    // Set lights to transform position of entity
    for (SceneView::Get()->First("light"); !SceneView::Get()->IsDone(); SceneView::Get()->Next())
    {
        LightComponent* pLight = SceneView::Get()->CurrentValue<LightComponent>();
        TransformComponent* pTransform = SceneView::Get()->GetComponent<TransformComponent>(pLight->GetEntityID(),
                                                                                       "transform");

        // Light direction temporary just relative to the angle the transform is facing
        // TODO: confirm this is the best way to do this and how it works properly
        glm::vec3 degrees = pTransform->GetOrientation();
        glm::vec3 direction = myutils::GetDirectionFromDegrees(degrees);

        pLight->SetDirection(vec4(direction, 1.0));
        pLight->SetPosition(vec4(pTransform->GetPosition(), 1.0));
    }
}
