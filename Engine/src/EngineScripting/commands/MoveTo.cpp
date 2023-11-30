#include "EngineScripting/commands/MoveTo.h"
#include "common/utils.h"
#include "common/ParserJSON.h"

MoveTo::MoveTo() :
    m_elapsedTime(0), m_currPhase(ePhase::STARTUP)
{
}

bool MoveTo::Initialize(SceneView* pScene, rapidjson::Value& document)
{
    using namespace rapidjson;
    using namespace myutils;

    float entity;
    float easyIn;
    float easyOut;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "MoveTo";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);
    Value& objLoc = document["location"];
    isValid &= parser.GetVec3(objLoc, m_location);
    Value& objTime = document["time"];
    isValid &= parser.GetFloat(objTime, m_time);
    Value& objIn = document["easyIn"];
    isValid &= parser.GetFloat(objIn, easyIn);
    Value& objDeacc = document["easyOut"];
    isValid &= parser.GetFloat(objDeacc, easyOut);
    Value& objStop = document["stopAtEnd"];
    isValid &= parser.GetBool(objStop, m_stopAtEnd);

    if (!isValid)
    {
        // Invalid arguments
        return false;
    }

    m_pTransform = pScene->GetComponent<TransformComponent>(entity, "transform");
    m_pForce = pScene->GetComponent<ForceComponent>(entity, "force");

    // Calculate time phases
    m_easyInTime = easyIn * m_time;
    m_easyOutTime = easyOut * m_time;
    m_constantTime = m_time - m_easyInTime - m_easyOutTime;

    // Calculate positions phases
    m_easyInPos = CalculateVector(m_pTransform->GetPosition(),
        m_location, easyIn);  // Position it will stop accelerating
    m_easyOutPos = CalculateVector(m_pTransform->GetPosition(),
        m_location, 1.0f - easyOut);  // Position it will start decelerating

    return true;
}

void MoveTo::Initialize(SceneView* pScene, TransformComponent* pTransform, 
                        ForceComponent* pForce, glm::vec3 location, float time)
{
    m_location = location;
    m_time = time;

    m_stopAtEnd = false;

    m_easyInTime = 0.0;
    m_easyOutTime = 0.0;
    m_constantTime = m_time;

    m_pTransform = pTransform;
    m_pForce = pForce;
}

#include "EngineDebug/DebugSystem.h"
#include "common/constants.h"

bool MoveTo::Update(double deltaTime)
{
    using namespace glm;

    DebugSystem::Get()->AddLine(m_pTransform->GetPosition(), m_location, GREEN);

    m_elapsedTime += deltaTime;
    vec3 currPosition = m_pTransform->GetPosition();
    vec3 currVelocity = m_pForce->GetVelocity();

    // Check which phase of the velocity curve its in
    if (m_elapsedTime <= m_easyInTime && 
        m_currPhase == ePhase::STARTUP) {
        // Acceleration phase
        m_currPhase = ePhase::EASYIN;
        vec3 acceleration = myutils::CalculateAcceleration(currPosition, m_easyInPos,
                                                           m_elapsedTime, m_easyInTime);

        m_pForce->SetVelocity(vec3(0));
        m_pForce->SetAcceleration(acceleration);
    }
    else if (m_elapsedTime >= m_easyInTime &&
             m_elapsedTime <= m_easyInTime + m_constantTime &&
             m_currPhase   != ePhase::CONSTANT) {
        // Constant speed phase
        m_currPhase = ePhase::CONSTANT;
        // Setting velocity again for cases when easyin = 0
        vec3 velocity = myutils::CalculateVelocity(currPosition, m_easyOutPos, 
                                                   m_easyInTime, 
                                                   m_time - m_easyOutTime);

        m_pForce->SetAcceleration(vec3(0));
        m_pForce->SetVelocity(velocity);       
    }
    else if (m_elapsedTime >= m_easyInTime + m_constantTime &&
             m_elapsedTime <= m_time &&
             m_currPhase != ePhase::EASYOUT) {
        // Deceleration phase
        m_currPhase = ePhase::EASYOUT;
        vec3 acceleration = myutils::CalculateAcceleration(currPosition, m_location,
                                                           currVelocity,
                                                           m_elapsedTime, m_time);

        m_pForce->SetAcceleration(-acceleration);
    }

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

    if (m_elapsedTime >= m_time)
    {
        // Did not reach destination but ran out of time so probably can't reach it
        return true;
    }

    return false;
}

bool MoveTo::PreStart(void)
{
    using namespace myutils;

    // Calculate positions phases
    float easyIn = m_easyInTime / m_time;
    m_easyInPos = CalculateVector(m_pTransform->GetPosition(),
                                  m_location, easyIn);  // Position it will stop accelerating
    float easyOut = m_easyOutTime / m_time;
    m_easyOutPos = CalculateVector(m_pTransform->GetPosition(),
                                   m_location, 1.0f - easyOut);  // Position it will start decelerating

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
