#include "EngineScripting/commands/Motion.h"
#include "common/utils.h"
#include "common/utilsMat.h"
#include "common/ParserJSON.h"
#include "scene/SceneView.h"

Motion::Motion() :
    m_elapsedTime(0), m_currPhase(ePhase::STARTUP)
{
}

bool Motion::Initialize(rapidjson::Value& document)
{
    using namespace rapidjson;
    using namespace myutils;

    float entity;
    float easyIn;
    float easyOut;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);

    Value& objEnd = document["endxyz"];
    isValid &= parser.GetVec3(objEnd, m_endxyz);

    // Calculations are based in time OR max velocity, so must have one
    if (document.HasMember("time"))
    {
        Value& objTime = document["time"];
        isValid &= parser.GetFloat(objTime, m_time);
    }
    else
    {
        Value& objMaxVel = document["maxVelocity"];
        isValid &= parser.GetVec3(objMaxVel, m_maxVelocity);
    }

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

    m_pTransform = SceneView::Get()->GetComponent<TransformComponent>(entity, "transform");
    m_pForce = SceneView::Get()->GetComponent<ForceComponent>(entity, "force");

    // Calculate time phases
    m_easyInTime = easyIn * m_time;
    m_easyOutTime = easyOut * m_time;

    // TODO: Fix this whole easyin/easyout, for now just constant speed to avoid these problems
    //m_constantTime = m_time - m_easyInTime - m_easyOutTime;
    m_constantTime = m_time;

    return true;
}

void Motion::Initialize(TransformComponent* pTransform, ForceComponent* pForce, glm::vec3 endXYZ, float time)
{
    m_endxyz = endXYZ;
    m_time = time;
    m_maxVelocity = glm::vec3(0);

    m_stopAtEnd = false;

    m_easyInTime = 0.0;
    m_easyOutTime = 0.0;
    m_constantTime = m_time;

    m_pTransform = pTransform;
    m_pForce = pForce;
}

void Motion::Initialize(TransformComponent* pTransform, ForceComponent* pForce, glm::vec3 endXYZ, glm::vec3 maxVelocity)
{
    m_endxyz = endXYZ;
    m_time = 0;
    m_maxVelocity = maxVelocity;

    m_stopAtEnd = false;

    m_easyInTime = 0.0;
    m_easyOutTime = 0.0;
    m_constantTime = m_time;

    m_pTransform = pTransform;
    m_pForce = pForce;
}

bool Motion::Update(double deltaTime)
{
    using namespace glm;

    m_elapsedTime += deltaTime;
    vec3 currPosition = m_GetMotionPoint();
    vec3 currVelocity = m_GetMotionVelocity();

    if (m_time > 0)
    {
        m_SetAccTime(currPosition, currVelocity);
    }
    else
    {
        m_SetAccMaxVel(currPosition, m_endxyz);
    }

    bool isDone = IsDone();

    return isDone;
}

bool Motion::IsDone(void)
{
    float distanceToFinal = glm::length(m_GetMotionPoint() - m_endxyz);

    // Reached destination
    if (distanceToFinal <= 0.1)
    {
        // Reached or passed destination, so stop object and clamp to right point
        m_SetMotionPoint(m_endxyz);
        return true;
    }

    if (m_elapsedTime >= m_time)
    {
        // Did not reach destination but ran out of time so probably can't reach it
        return true;
    }

    return false;
}

bool Motion::PreStart(void)
{
    using namespace myutils;

    // Calculate positions phases
    float easyIn = m_easyInTime / m_time;
    m_easyInPoint = CalculateVector(m_GetMotionPoint(),
                                    m_endxyz, easyIn);           // Point it will stop accelerating
    float easyOut = m_easyOutTime / m_time;
    m_easyOutPoint = CalculateVector(m_GetMotionPoint(),
                                     m_endxyz, 1.0f - easyOut);  // Point it will start decelerating

    return true;
}

bool Motion::PostEnd(void)
{
    if (m_stopAtEnd)
    {
        m_SetMotionAcceleration(glm::vec3(0));
        m_SetMotionVelocity(glm::vec3(0));
    }

    return true;
}

void Motion::m_SetAccTime(glm::vec3 currPoint, glm::vec3 currVelocity)
{
    using namespace glm;

    // Check which phase of the velocity curve its in
    if (m_elapsedTime <= m_easyInTime &&
        m_currPhase == ePhase::STARTUP) 
    {
        // Acceleration phase
        vec3 acceleration = myutils::CalculateAcceleration(currPoint, m_easyInPoint,
                                                           m_elapsedTime, m_easyInTime);

        m_currPhase = ePhase::EASYIN;
        m_SetMotionVelocity(vec3(0));
        m_SetMotionAcceleration(acceleration);
    }
    else if (m_elapsedTime >= m_easyInTime &&
             m_elapsedTime <= m_easyInTime + m_constantTime &&
             m_currPhase != ePhase::CONSTANT) 
    {
        // Constant speed phase
        vec3 velocity = myutils::CalculateVelocity(currPoint, m_easyOutPoint,
                                                   m_easyInTime,
                                                   m_time - m_easyOutTime);
     
        m_currPhase = ePhase::CONSTANT;
        m_SetMotionAcceleration(vec3(0));
        m_SetMotionVelocity(velocity);
    }
    else if (m_elapsedTime >= m_easyInTime + m_constantTime &&
             m_elapsedTime <= m_time &&
             m_currPhase != ePhase::EASYOUT) 
    {
        // Deceleration phase
        vec3 acceleration = myutils::CalculateAcceleration(currPoint, m_endxyz,
                                                           currVelocity,
                                                           m_elapsedTime, m_time);

        m_currPhase = ePhase::EASYOUT;
        m_SetMotionAcceleration(-acceleration);
    }
}

void Motion::m_SetAccMaxVel(glm::vec3 currPoint, glm::vec3 targetPos)
{
    using namespace glm;
    using namespace myutils;

    vec3 normal = GetNormal(targetPos, currPoint);

    m_pForce->SetVelocity(m_maxVelocity * normal);
}
