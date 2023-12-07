#include "EngineScripting/commands/AccelerateTowards.h"
#include "common/ParserJSON.h"
#include "scene/SceneView.h"

AccelerateTowards::AccelerateTowards()
{
}

bool AccelerateTowards::Initialize(rapidjson::Value& document)
{
    using namespace rapidjson;

    float entity;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);

    Value& objDir = document["direction"];
    isValid &= parser.GetVec3(objDir, m_direction);

    Value& objAcceleration = document["acceleration"];
    isValid &= parser.GetFloat(objAcceleration, m_acceleration);

    Value& objMaxVel = document["maxSpeed"];
    isValid &= parser.GetFloat(objMaxVel, m_maxSpeed);

    if (!isValid)
    {
        // Invalid arguments
        return false;
    }

    SceneView* pScene = SceneView::Get();
    m_pTransform = pScene->GetComponent<TransformComponent>(entity, "transform");
    m_pForce = pScene->GetComponent<ForceComponent>(entity, "force");

    return true;
}

bool AccelerateTowards::Update(double deltaTime)
{
    using namespace glm;

    // Update direction based on rotation
    vec3 curDirection = m_pTransform->GetRelativeVector(m_direction);
    curDirection = glm::normalize(curDirection);

    m_vecAcc = curDirection * m_acceleration;
    m_vecMaxVel = curDirection * m_maxSpeed;

    // Here we set the acceleration force
    m_pForce->SetAcceleration(m_vecAcc);

    // Here we check when max speed is reached
    glm::vec3 currVel = m_pForce->GetVelocity();

    if (m_maxSpeed > 0 &&
        glm::length(currVel) > m_maxSpeed)
    {
        m_pForce->SetVelocity(m_vecMaxVel);
    }

    return false;
}

bool AccelerateTowards::IsDone(void)
{
    // Accelerates indefinitly
    return false;
}

bool AccelerateTowards::PreStart(void)
{
    return true;
}

bool AccelerateTowards::PostEnd(void)
{
    // Reset acceleration
    m_pForce->SetAcceleration(glm::vec3(0));

    return true;
}

void AccelerateTowards::m_Accelerate()
{
}
