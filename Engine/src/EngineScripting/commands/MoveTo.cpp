#include "EngineScripting/commands/MoveTo.h"

MoveTo::MoveTo()
{
}

glm::vec3 MoveTo::m_GetMotionPoint()
{
    return m_pTransform->GetPosition();
}

glm::vec3 MoveTo::m_GetMotionAcceleration()
{
    return m_pForce->GetAcceleration();
}

glm::vec3 MoveTo::m_GetMotionVelocity()
{
    return m_pForce->GetVelocity();
}

void MoveTo::m_SetMotionPoint(glm::vec3 value)
{
    m_pTransform->SetPosition(value);
}

void MoveTo::m_SetMotionAcceleration(glm::vec3 value)
{
    m_pForce->SetAcceleration(value);
}

void MoveTo::m_SetMotionVelocity(glm::vec3 value)
{
    m_pForce->SetVelocity(value);
}
