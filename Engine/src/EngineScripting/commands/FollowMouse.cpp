#include "EngineScripting/commands/FollowMouse.h"
#include "common/ParserJSON.h"
#include "scene/SceneView.h"
#include "common/Input.h"
#include "Engine.h"

FollowMouse::FollowMouse()
{
}

bool FollowMouse::Initialize(rapidjson::Value& document)
{
    using namespace rapidjson;

    float entity;
    bool isValid = true;

    // Initialize default command variables
    isValid &= this->Command::Initialize();
    m_name = "FollowMouse";

    ParserJSON parser = ParserJSON();

    Value& objEntt = document["entity"];
    isValid &= parser.GetFloat(objEntt, entity);
    Value& objSense = document["sensitivity"];
    isValid &= parser.GetFloat(objSense, m_sensitivity);

    if (!isValid)
    {
        // Invalid arguments
        return false;
    }

    m_pTransform = SceneView::Get()->GetComponent<TransformComponent>(entity, "transform");

    return true;
}

bool FollowMouse::Update(double deltaTime)
{
    using namespace glm;

    vec2 mouse = Input::MousePosition();

    // Calculate offset to new mouse position
    float xoffset = mouse.x - m_lastX;
    float yoffset = mouse.y - m_lastY;
    m_lastX = mouse.x;
    m_lastY = mouse.y;
    xoffset *= m_sensitivity * deltaTime;
    yoffset *= m_sensitivity * deltaTime;

    m_pTransform->AdjustOrientation(vec2(xoffset, yoffset));

    return false;
}

bool FollowMouse::IsDone(void)
{
    // Follows mouse indefenitely
    return false;
}

bool FollowMouse::PreStart(void)
{
    using namespace glm;

    Engine::Get()->CaptureMouse();

    vec2 mousePos = Input::MousePosition();

    m_lastX = mousePos.x;
    m_lastY = mousePos.y;

    return true;
}

bool FollowMouse::PostEnd(void)
{
    Engine::Get()->ReleaseMouse();
    return false;
}
