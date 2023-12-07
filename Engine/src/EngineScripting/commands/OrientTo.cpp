#include "EngineScripting/commands/OrientTo.h"
#include "common/ParserJSON.h"
#include "scene/SceneView.h"
#include "common/Input.h"
#include "Engine.h"

OrientTo::OrientTo()
{
}

bool OrientTo::Initialize(rapidjson::Value& document)
{
    using namespace rapidjson;

    float entity;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "OrientTo";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);
    Value& objEnd = document["endxyz"];
    isValid &= parser.GetVec3(objEnd, m_endxyz);

    if (!isValid)
    {
        // Invalid arguments
        return false;
    }

    m_pTransform = SceneView::Get()->GetComponent<TransformComponent>(entity, "transform");

    return true;
}

void OrientTo::Initialize(TransformComponent* pTransform, glm::vec3 endxyz)
{
    m_endxyz = endxyz;

    m_pTransform = pTransform;
}

bool OrientTo::Update(double deltaTime)
{
    using namespace glm;

    vec2 mouse = Input::MousePosition();

    // Calculate offset to new position
    float xoffset = mouse.x - m_lastX;
    float yoffset = mouse.y - m_lastY;
    m_lastX = mouse.x;
    m_lastY = mouse.y;
    xoffset *= deltaTime * deltaTime;
    yoffset *= deltaTime * deltaTime;

    // Update the new rotation based on current rotation
    quat transfQuat = m_pTransform->GetQuatOrientation();
    quat yaw = angleAxis(-xoffset, vec3(UP_VECTOR));
    quat pitch = angleAxis(-yoffset, vec3(RIGHT_VECTOR));

    transfQuat = yaw * transfQuat;
    transfQuat = transfQuat * pitch;

    m_pTransform->SetOrientation(transfQuat);

    return true;
}

bool OrientTo::IsDone(void)
{
    return true;
}

bool OrientTo::PreStart(void)
{
    using namespace glm;

    m_lastX = m_endxyz.x;
    m_lastY = m_endxyz.y;

    return true;
}

bool OrientTo::PostEnd(void)
{
    return false;
}
