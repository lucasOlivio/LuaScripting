#include "EngineScripting/commands/OrientTo.h"
#include "common/utils.h"
#include "common/ParserJSON.h"

OrientTo::OrientTo() :
    m_elapsedTime(0)
{
}

bool OrientTo::Initialize(SceneView* pScene, rapidjson::Value& document)
{
    float entity;
    float accRatio;
    float deaccRatio;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "OrientTo";

    ParserJSON parser = ParserJSON();

    rapidjson::Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);
    rapidjson::Value& objLoc = document["orientation"];
    isValid &= parser.GetVec3(objLoc, m_orientation);
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

bool OrientTo::Update(double deltaTime)
{
    using namespace glm;

    m_elapsedTime += deltaTime;
    vec3 currOrientation = m_pTransform->GetOrientation();
    vec3 currVCentrVelocity = m_pForce->GetCentrifugalVelocity();

    glm::vec3 acceleration = glm::vec3(0.0f);

    // Check which phase of the velocity curve its in
    if (m_elapsedTime <= m_accelerationTime) {
        // Acceleration phase
        float accRatio = m_accelerationTime / m_time;
        acceleration = myutils::CalculateAcceleration(currOrientation, m_orientation,
                                                      currVCentrVelocity, accRatio,
                                                      m_elapsedTime, m_accelerationTime);
    }
    else if (m_elapsedTime <= m_accelerationTime + m_constantTime) {
        // Constant speed phase
        acceleration = glm::vec3(0.0f);
    }
    else if (m_elapsedTime <= m_time) {
        // Deceleration phase
        acceleration = myutils::CalculateDeceleration(currOrientation, m_orientation,
                                                      currVCentrVelocity, m_elapsedTime,
                                                      m_time);
    }

    m_pForce->SetCentrifugalAcceleration(acceleration);

    bool isDone = IsDone();

    return isDone;
}

bool OrientTo::IsDone(void)
{
    float distanceToFinal = glm::length(m_pTransform->GetOrientation() - m_orientation);

    // Reached destination
    if (distanceToFinal <= 0.1)
    {
        // Reached or passed destination, so stop object and clamp to right orientation
        m_pTransform->SetOrientation(m_orientation);
        return true;
    }

    if (m_elapsedTime >= m_time)
    {
        // Did not reach destination but ran out of time so probably can't reach it
        return true;
    }

    return false;
}

bool OrientTo::PreStart(void)
{
    return true;
}

bool OrientTo::PostEnd(void)
{
    if (m_stopAtEnd)
    {
        m_pForce->SetCentrifugalAcceleration(glm::vec3(0));
        m_pForce->SetCentrifugalVelocity(glm::vec3(0));
    }

    return true;
}