#include "EngineScripting/commands/MoveTo.h"
#include "common/utils.h"
#include "common/ParserJSON.h"

MoveTo::MoveTo() :
    m_elapsedTime(0), m_lastDistance(0)
{
}

bool MoveTo::Initialize(SceneView* pScene, rapidjson::Value& document)
{
    float entity;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "MoveTo";

    ParserJSON parser = ParserJSON();

    rapidjson::Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);
    rapidjson::Value& objLoc = document["location"];
    isValid &= parser.GetVec3(objLoc, m_location);
    rapidjson::Value& objTime = document["time"];
    isValid &= parser.GetFloat(objTime, m_time);

    if (!isValid)
    {
        // Invalid arguments
        return false;
    }

    m_pTransform = pScene->GetComponent<TransformComponent>(entity, "transform");
    m_pForce = pScene->GetComponent<ForceComponent>(entity, "force");

    return true;
}

bool MoveTo::Update(double deltaTime)
{
    m_elapsedTime += deltaTime;

    bool isDone = IsDone();

    return isDone;
}

bool MoveTo::IsDone(void)
{
    float distanceToFinal = glm::length(m_pTransform->GetPosition() - m_location);

    // Doubel check to see if reached or passed destination
    if (distanceToFinal <= 0.1 ||
        m_lastDistance < distanceToFinal)
    {
        // Reached or passed destination, so stop object and clamp to right position
        m_pForce->SetVelocity(glm::vec3(0));
        m_pTransform->SetPosition(m_location);
        return true;
    }

    m_lastDistance = distanceToFinal;

    if (m_elapsedTime >= m_time)
    {
        // Did not reach destination but ran out of time so probably can't reach it
        m_pForce->SetVelocity(glm::vec3(0));
        return true;
    }

    return false;
}

bool MoveTo::PreStart(void)
{
    m_lastDistance = glm::length(m_pTransform->GetPosition() - m_location);
    glm::vec3 newVelocity = myutils::CalculateVelocity(m_pTransform->GetPosition(),
        m_location,
        m_time);

    m_pForce->SetVelocity(newVelocity);

    return true;
}

bool MoveTo::PostEnd(void)
{
    return true;
}
