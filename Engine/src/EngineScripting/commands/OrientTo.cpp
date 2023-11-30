#include "EngineScripting/commands/OrientTo.h"
#include "common/utils.h"
#include "common/ParserJSON.h"

OrientTo::OrientTo() :
    m_elapsedTime(0), m_currPhase(ePhase::STARTUP)
{
}

bool OrientTo::Initialize(SceneView* pScene, rapidjson::Value& document)
{
    using namespace rapidjson;
    using namespace myutils;

    std::string entity;
    float easyIn;
    float easyOut;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "OrientTo";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetString(objEntt, entity);
    Value& objLoc = document["orientation"];
    isValid &= parser.GetVec3(objLoc, m_orientation);
    Value& objTime = document["time"];
    isValid &= parser.GetFloat(objTime, m_time);
    Value& objIn = document["easyIn"];
    isValid &= parser.GetFloat(objIn, easyIn);
    Value& objOut = document["easyOut"];
    isValid &= parser.GetFloat(objOut, easyOut);
    Value& objStop = document["stopAtEnd"];
    isValid &= parser.GetBool(objStop, m_stopAtEnd);

    if (!isValid)
    {
        // Invalid arguments
        return false;
    }

    m_pTransform = pScene->GetComponentByTag<TransformComponent>(entity, "transform");
    m_pForce = pScene->GetComponentByTag<ForceComponent>(entity, "force");

    // Calculate time phases
    m_easyInTime = easyIn * m_time;
    m_easyOutTime = easyOut * m_time;
    m_constantTime = m_time - m_easyInTime - m_easyOutTime;

    // Calculate orientation phases
    m_easyInPos = CalculateVector(m_pTransform->GetOrientation(),
                                  m_orientation, easyIn);          // Orientation it will stop accelerating
    m_easyOutPos = CalculateVector(m_pTransform->GetPosition(),
                                   m_orientation, 1.0f - easyOut); // Orientation it will start decelerating

    return true;
}

bool OrientTo::Update(double deltaTime)
{
    using namespace glm;

    m_elapsedTime += deltaTime;
    vec3 currOrientation = m_pTransform->GetOrientation();
    vec3 currCentrVelocity = m_pForce->GetCentrifugalVelocity();

    // Check which phase of the velocity curve its in
    if (m_elapsedTime <= m_easyInTime &&
        m_currPhase == ePhase::STARTUP) {
        // Acceleration phase
        m_currPhase = ePhase::EASYIN;
        vec3 acceleration = myutils::CalculateAcceleration(currOrientation, m_easyInPos,
                                                           m_elapsedTime, m_easyInTime);

        m_pForce->SetCentrifugalVelocity(vec3(0));
        m_pForce->SetCentrifugalAcceleration(acceleration);
    }
    else if (m_elapsedTime >= m_easyInTime &&
        m_elapsedTime <= m_easyInTime + m_constantTime &&
        m_currPhase != ePhase::CONSTANT) {
        // Constant speed phase
        m_currPhase = ePhase::CONSTANT;
        // Setting velocity again for cases when easyin = 0
        vec3 velocity = myutils::CalculateVelocity(currOrientation, m_easyOutPos,
                                                   m_easyInTime,
                                                   m_time - m_easyOutTime);

        m_pForce->SetCentrifugalAcceleration(vec3(0));
        m_pForce->SetCentrifugalVelocity(velocity);
    }
    else if (m_elapsedTime >= m_easyInTime + m_constantTime &&
        m_elapsedTime <= m_time &&
        m_currPhase != ePhase::EASYOUT) {
        // Deceleration phase
        m_currPhase = ePhase::EASYOUT;
        vec3 acceleration = myutils::CalculateAcceleration(currOrientation, m_orientation,
                                                           currCentrVelocity,
                                                           m_elapsedTime, m_time);

        m_pForce->SetCentrifugalAcceleration(-acceleration);
    }

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
