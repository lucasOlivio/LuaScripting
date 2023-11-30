#include "EngineScripting/commands/FollowCurve.h"
#include "EngineScripting/commands/MoveTo.h"
#include "EngineScripting/CommandGroup.h"
#include "common/utils.h"
#include "common/ParserJSON.h"

FollowCurve::FollowCurve()
{
}

bool FollowCurve::Initialize(SceneView* pScene, rapidjson::Value& document)
{
    using namespace rapidjson;

    std::string entity;
    float accRatio;
    float deaccRatio;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "FollowCurve";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetString(objEntt, entity);
    Value& objLoc = document["controlPoints"];
    isValid &= parser.GetVecVec3(objLoc, m_controlPoints);
    Value& objTime = document["time"];
    isValid &= parser.GetFloat(objTime, m_time);
    Value& objStep = document["timeStep"];
    isValid &= parser.GetFloat(objStep, m_timeStep);
    Value& objAcc = document["accRatio"];
    isValid &= parser.GetFloat(objAcc, accRatio);
    Value& objDeacc = document["deaccRatio"];
    isValid &= parser.GetFloat(objDeacc, deaccRatio);
    Value& objStop = document["stopAtEnd"];
    isValid &= parser.GetBool(objStop, m_stopAtEnd);

    m_accelerationTime = accRatio * m_time;
    m_decelerationTime = deaccRatio * m_time;
    m_constantTime = m_time - m_accelerationTime - m_decelerationTime;

    if (!isValid)
    {
        // Invalid arguments
        return false;
    }

    m_pTransform = pScene->GetComponentByTag<TransformComponent>(entity, "transform");
    m_pForce = pScene->GetComponentByTag<ForceComponent>(entity, "force");

    // First point should aways be the current position for simplicity
    m_controlPoints.insert(m_controlPoints.begin(), m_pTransform->GetPosition());

    m_GenerateSubCommands();

    return true;
}

bool FollowCurve::Update(double deltaTime)
{
    this->CommandGroup::Update(deltaTime);

    bool isDone = IsDone();

    return isDone;
}

bool FollowCurve::IsDone(void)
{
    bool isDone = this->CommandGroup::IsDone();
    // This command will only generate the movement commands to follow the curve
    return isDone;
}

bool FollowCurve::PreStart(void)
{
    bool isStarted = this->CommandGroup::PreStart();

    return isStarted;
}

bool FollowCurve::PostEnd(void)
{
    if (m_stopAtEnd)
    {
        m_pForce->SetAcceleration(glm::vec3(0));
        m_pForce->SetVelocity(glm::vec3(0));
    }

    bool isEnded = this->CommandGroup::PostEnd();

    return isEnded;
}

void FollowCurve::m_GenerateSubCommands()
{
    // Calculate the number of steps based on the time
    float step = m_time / m_timeStep;

    CommandGroup* pMoveGroup = new CommandGroup();

    // Generate MoveTo commands to follow the Bezier curve
    // Starting from 2 position (first position is current)
    for (float i = step; i <= m_time; i += step) {
        float ratioTime = i / m_time;
        glm::vec3 position = myutils::CalculateBezierPoint(m_controlPoints, ratioTime);

        // Create a MoveTo command for the calculated position in "step" time
        MoveTo* pMove = new MoveTo();

        pMove->Initialize(m_pTransform, m_pForce, position, step);

        // Add the MoveTo command to the list of commands
        pMoveGroup->AddSerialCommand(pMove);
    }


    // Setup new groups
    pMoveGroup->Initialize(m_name);
    pMoveGroup->PreStart();

    if (pMoveGroup->IsDone())
    {
        // No waypoints created
        return;
    }

    // Set motion commands
    this->CommandGroup::AddParallelCommand(pMoveGroup);
}
