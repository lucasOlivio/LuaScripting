#include "EngineScripting/commands/MoveTo.h"
#include "common/utils.h"

MoveTo::MoveTo(SceneView* pScene, EntityID entity, glm::vec3 location, float time) :
    m_finalPos(location), m_maxTime(time), m_elapsedTime(0), m_lastDistance(0)
{
    m_pTransform = pScene->GetComponent<TransformComponent>(entity, "transform");
    m_pForce = pScene->GetComponent<ForceComponent>(entity, "force");
}

bool MoveTo::Update(double deltaTime)
{
    m_elapsedTime += deltaTime;

    bool isDone = IsDone();

    return isDone;
}

bool MoveTo::IsDone(void)
{
    float distanceToFinal = glm::length(m_pTransform->GetPosition() - m_finalPos);
    printf("distance: %.2f\n", distanceToFinal);
    printf("time: %.2f\n", m_elapsedTime);

    // Doubel check to see if reached or passed destination
    if (distanceToFinal <= 0.1 || 
        m_lastDistance < distanceToFinal)
    {
        // Reached or passed destination, so stop object and clamp to right position
        m_pForce->SetVelocity(glm::vec3(0));
        m_pTransform->SetPosition(m_finalPos);
        return true;
    }

    m_lastDistance = distanceToFinal;

    if (m_elapsedTime >= m_maxTime)
    {
        // Did not reach destination but ran out of time so probably can't reach it
        m_pForce->SetVelocity(glm::vec3(0));
        return true;
    }

    return false;
}

bool MoveTo::PreStart(void)
{
    m_lastDistance = glm::length(m_pTransform->GetPosition() - m_finalPos);
    glm::vec3 newVelocity = myutils::CalculateVelocity(m_pTransform->GetPosition(), 
                                                       m_finalPos, 
                                                       m_maxTime);

    m_pForce->SetVelocity(newVelocity);

    return true;
}

bool MoveTo::PostEnd(void)
{
    return true;
}
