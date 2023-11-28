#include "EngineScripting/commands/MoveTo.h"

MoveTo::MoveTo(EntityID entity, float x, float y, float z, float sec)
{
    m_finalPos = glm::vec3(x, y, z);
}

bool MoveTo::Update(double deltaTime)
{
    return false;
}

bool MoveTo::isDone(void)
{
    return false;
}

bool MoveTo::PreStart(void)
{
    return false;
}

bool MoveTo::PostEnd(void)
{
    return false;
}
