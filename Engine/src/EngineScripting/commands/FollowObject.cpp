#include "EngineScripting/commands/FollowObject.h"
#include "EngineScripting/commands/MoveTo.h"
#include "EngineScripting/CommandGroup.h"
#include "common/utils.h"
#include "common/ParserJSON.h"

FollowObject::FollowObject() : m_elapsedTime(0)
{
}

bool FollowObject::Initialize(SceneView* pScene, rapidjson::Value& document)
{
    using namespace rapidjson;

    std::string targetTag;
    float entity;
    float easyIn;
    float easyOut;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "FollowObject";

    ParserJSON parser = ParserJSON();

    Value& objTarget = document["targetTag"];
    isValid &= parser.GetString(objTarget, targetTag);
    Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);
    Value& objStep = document["timeStep"];
    isValid &= parser.GetFloat(objStep, m_timeStep);
    Value& objSpeed = document["maxSpeed"];
    isValid &= parser.GetFloat(objSpeed, m_maxSpeed);
    Value& objForever = document["followForever"];
    isValid &= parser.GetBool(objForever, m_followForever);
    Value& objEasyIn = document["easyIn"];
    isValid &= parser.GetFloat(objEasyIn, easyIn);
    Value& objEasyOut = document["easyOut"];
    isValid &= parser.GetFloat(objEasyOut, easyOut);

    if (!isValid)
    {
        // Invalid arguments
        return false;
    }

    m_pTransformTarget = pScene->GetComponentByTag<TransformComponent>(targetTag, "transform");
    m_pTransform = pScene->GetComponent<TransformComponent>(entity, "transform");
    m_pForce = pScene->GetComponent<ForceComponent>(entity, "force");

    m_lastTargetPosition = m_pTransformTarget->GetPosition();

    return true;
}

bool FollowObject::Update(double deltaTime)
{
    m_elapsedTime += deltaTime;

    this->CommandGroup::Update(deltaTime);

    bool isDone = IsDone();
    if (isDone)
    {
        return true;
    }

    // Check last move status
    bool isAtTargetPosition = this->m_IsAtSamePosition();
    if (m_elapsedTime > m_timeStep)
    {
        m_elapsedTime = 0;
        ClearCommands();
        m_GenerateSubCommands();
    }

    return false;
}

bool FollowObject::IsDone(void)
{
    if (m_followForever)
    {
        return false;
    }

    bool isDone = this->CommandGroup::IsDone();
    // This command will only generate the movement commands to follow the curve
    return isDone && m_IsAtSamePosition();
}

bool FollowObject::PreStart(void)
{
    bool isStarted = this->CommandGroup::PreStart();

    return isStarted;
}

bool FollowObject::PostEnd(void)
{
    bool isEnded = this->CommandGroup::PostEnd();

    m_pForce->SetVelocity(glm::vec3(0));
    m_pForce->SetAcceleration(glm::vec3(0));

    return isEnded;
}

void FollowObject::m_GenerateSubCommands()
{
    // Create a MoveTo command for the calculated position with the given max speed
    MoveTo* pMove = new MoveTo();

    pMove->Initialize(m_pTransform, m_pForce, m_lastTargetPosition, glm::vec3(m_maxSpeed));
    pMove->PreStart();

    // Set motion commands
    this->CommandGroup::AddSerialCommand(pMove);
}

bool FollowObject::m_IsAtSamePosition()
{
    using namespace glm;

    m_lastTargetPosition = m_pTransformTarget->GetPosition();

    float distance = length(m_lastTargetPosition - m_pTransform->GetPosition());

    if (distance <= 0.1)
    {
        return true;
    }

    return false;
}
