#include "EngineScripting/commands/OrientTo.h"

OrientTo::OrientTo()
{
}

glm::vec3 OrientTo::m_GetMotionPoint()
{
    return m_pTransform->GetOrientation();
}

glm::vec3 OrientTo::m_GetMotionAcceleration()
{
    return m_pForce->GetCentrifugalAcceleration();
}

glm::vec3 OrientTo::m_GetMotionVelocity()
{
    return m_pForce->GetCentrifugalVelocity();
}

void OrientTo::m_SetMotionPoint(glm::vec3 value)
{

    m_pTransform->SetOrientation(value);
}

void OrientTo::m_SetMotionAcceleration(glm::vec3 value)
{
    m_pForce->SetCentrifugalAcceleration(value);
}

void OrientTo::m_SetMotionVelocity(glm::vec3 value)
{
    m_pForce->SetCentrifugalVelocity(value);
}
