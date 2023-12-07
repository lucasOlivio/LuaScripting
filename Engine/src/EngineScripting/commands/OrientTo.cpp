#include "EngineScripting/commands/OrientTo.h"
#include "EngineDebug/DebugSystem.h"
#include "common/constants.h"
#include "common/utilsMat.h"

OrientTo::OrientTo()
{
}

glm::vec3 OrientTo::m_GetMotionPoint()
{
    return m_pTransform->GetPosition() +  m_pTransform->GetForwardVector();
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
    // TODO: How to set the orientation from the offset from origin to target?
    // m_pTransform->SetOrientation(value);
}

void OrientTo::m_SetMotionAcceleration(glm::vec3 value)
{
    m_pForce->SetCentrifugalAcceleration(value);
}

void OrientTo::m_SetMotionVelocity(glm::vec3 value)
{
    m_pForce->SetCentrifugalVelocity(-value);
}

void OrientTo::m_SetAccTime(glm::vec3 currPoint, glm::vec3 currVelocity)
{
    using namespace glm;
    using namespace myutils;


    vec3 normal = GetNormal(m_endxyz, currPoint);
    vec3 forward = m_pTransform->GetForwardVector();

    // TODO: Merge with motion and calculate easyin and out
    vec3 velocity = myutils::CalculateVelocity(forward, normal,
                                               m_easyInTime,
                                               m_time - m_easyOutTime);
    // Sensibility adjustment, this should come from parameters
    velocity = -velocity * 0.07f;

    m_currPhase = ePhase::CONSTANT;
    m_SetMotionAcceleration(vec3(0));
    m_SetMotionVelocity(velocity);
}

void OrientTo::m_SetAccMaxVel(glm::vec3 currPoint, glm::vec3 targetPos)
{
    using namespace glm;
    using namespace myutils;

    vec3 normal = GetNormal(targetPos, currPoint);
    vec3 forward = m_pTransform->GetForwardVector();

    float xoffset = forward.x - normal.x;
    float yoffset = normal.y - forward.y;
    glm::vec3 targetOrientation = glm::vec3(xoffset, yoffset, 0.0f);

    m_SetMotionVelocity(m_maxVelocity * targetOrientation);
}