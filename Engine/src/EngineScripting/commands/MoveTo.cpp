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
    float accRatio;
    float deaccRatio;
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
    rapidjson::Value& objAcc = document["accRatio"];
    isValid &= parser.GetFloat(objAcc, accRatio);
    rapidjson::Value& objDeacc = document["deaccRatio"];
    isValid &= parser.GetFloat(objDeacc, deaccRatio);
    rapidjson::Value& objStop = document["stopAtEnd"];
    isValid &= parser.GetBool(objStop, m_stopAtEnd);

    m_accelerationTime = accRatio * m_time;
    m_decelerationTime = deaccRatio * m_time;
    m_constantTime = m_time - m_accelerationTime - m_decelerationTime;

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
    using namespace glm;

    m_elapsedTime += deltaTime;
    vec3 currPosition = m_pTransform->GetPosition();
    vec3 currVelocity = m_pForce->GetVelocity();

    glm::vec3 acceleration = glm::vec3(0.0f);

    // Check which phase of the velocity curve its in
    if (m_elapsedTime <= m_accelerationTime) {
        // Acceleration phase
        float accRatio = m_accelerationTime / m_time;
        acceleration = myutils::CalculateAcceleration(currPosition, m_location, 
                                                      currVelocity, accRatio, 
                                                      m_elapsedTime, m_accelerationTime);
    }
    else if (m_elapsedTime <= m_accelerationTime + m_constantTime) {
        // Constant speed phase
        acceleration = glm::vec3(0.0f);
    }
    else if (m_elapsedTime <= m_time) {
        // Deceleration phase
        acceleration = myutils::CalculateDeceleration(currPosition, m_location, 
                                                      currVelocity, m_elapsedTime,
                                                      m_time);
    }

    m_pForce->SetAcceleration(acceleration);

    bool isDone = IsDone();

    return isDone;
}

bool MoveTo::IsDone(void)
{
    float distanceToFinal = glm::length(m_pTransform->GetPosition() - m_location);

    // Reached destination
    if (distanceToFinal <= 0.1)
    {
        // Reached or passed destination, so stop object and clamp to right position
        m_pTransform->SetPosition(m_location);
        return true;
    }

    m_lastDistance = distanceToFinal;

    if (m_elapsedTime >= m_time)
    {
        // Did not reach destination but ran out of time so probably can't reach it
        return true;
    }

    return false;
}

bool MoveTo::PreStart(void)
{
    float distanceToFinal = glm::length(m_pTransform->GetPosition() - m_location);
    m_lastDistance = distanceToFinal;

    return true;
}

bool MoveTo::PostEnd(void)
{
    if (m_stopAtEnd)
    {
        m_pForce->SetAcceleration(glm::vec3(0));
        m_pForce->SetVelocity(glm::vec3(0));
    }

    return true;
}
