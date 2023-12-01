#include "EngineRenderer/CameraSystem.h"
#include "common/opengl.h"
#include "scene/SceneView.h"
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp>

CameraSystem::CameraSystem() : 
    m_pCamera(nullptr), m_pTransform(nullptr)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::Initialize()
{
    m_pCamera = SceneView::Get()->GetComponentByTag<CameraComponent>("camera", "camera");
    m_pTransform = SceneView::Get()->GetComponentByTag<TransformComponent>("camera", "transform");
}

glm::mat4 CameraSystem::GetViewMat()
{
    using namespace glm;

    // Calculating camera view
    vec3 cameraPosition = this->m_pTransform->GetPosition();
    vec3 cameraRotation = this->m_pTransform->GetOrientation();
    vec3 cameraUpVector = this->m_pCamera->upVector;

    vec3 cameraTarget = this->m_pCamera->GetCameraTarget(cameraPosition, cameraRotation);

    return lookAt(cameraPosition,
                  cameraTarget,
                  cameraUpVector);
}

glm::vec3 CameraSystem::GetCameraPosition()
{
    return m_pTransform->GetPosition();
}

