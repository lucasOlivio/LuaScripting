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
    float entity;
    float accRatio;
    float deaccRatio;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "FollowCurve";

    ParserJSON parser = ParserJSON();

    rapidjson::Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);
    rapidjson::Value& objLoc = document["controlPoints"];
    isValid &= parser.GetVecVec3(objLoc, m_controlPoints);
    rapidjson::Value& objTime = document["time"];
    isValid &= parser.GetFloat(objTime, m_time);
    rapidjson::Value& objStep = document["timeStep"];
    isValid &= parser.GetInt(objStep, m_timeStep);
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

    // First point should aways be the current position for simplicity
    m_controlPoints.insert(m_controlPoints.begin(), m_pTransform->GetPosition());

    m_GenerateSubCommands(pScene);

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
    bool isEnded = this->CommandGroup::PostEnd();

    return isEnded;
}

void FollowCurve::m_GenerateSubCommands(SceneView* pScene)
{
    // Calculate the number of steps based on the time
    float step = m_time / m_timeStep;

    CommandGroup* pMoveGroup = new CommandGroup();

    // Generate MoveTo commands to follow the Bezier curve
    // Starting from 2 position (first position is current)
    for (float i = step; i <= m_time; i += step) {
        glm::vec3 position = myutils::CalculateBezierPoint(m_controlPoints, i);

        // Create a MoveTo command for the calculated position
        MoveTo* pMove = new MoveTo();

        pMove->Initialize(pScene, m_pTransform, m_pForce, position, i);

        // Add the MoveTo command to the list of commands
        pMoveGroup->AddSerialCommand(pMove);
    }


    // Setup new groups
    pMoveGroup->Initialize(pScene, m_name);
    pMoveGroup->PreStart();

    if (pMoveGroup->IsDone())
    {
        // No waypoints created
        return;
    }

    // Set motion commands
    this->CommandGroup::AddParallelCommand(pMoveGroup);
}
