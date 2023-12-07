#include "components/Force.h"

ForceComponent::ForceComponent()
{
    m_centrifugalAcceleration = glm::vec3(0);
    m_centrifugalVelocity = glm::vec3(0);
}

ForceComponent::~ForceComponent()
{
}

void ForceComponent::SetInverseMass(float value)
{
    m_inverseMass = value;
}

void ForceComponent::SetRestitution(float value)
{
    m_restitution = value;
}

void ForceComponent::SetDrag(float value)
{
    m_drag = value;
}

void ForceComponent::SetAcceleration(glm::vec3 value)
{
    m_acceleration = value;
}

void ForceComponent::SetVelocity(glm::vec3 value)
{
    m_velocity = value;
}

void ForceComponent::SetActive(bool isActive)
{
    m_isActive = isActive;
}

void ForceComponent::SetCentrifugalAcceleration(glm::vec3 value)
{
    m_centrifugalAcceleration = value;
}

void ForceComponent::SetCentrifugalVelocity(glm::vec3 value)
{
    m_centrifugalVelocity = value;
}

void ForceComponent::SetCentrifugalDrag(float value)
{
    m_centrifugalDrag = value;
}

float ForceComponent::GetInverseMass()
{
    return m_inverseMass;
}

float ForceComponent::GetRestitution()
{
    return m_restitution;
}

float ForceComponent::GetDrag()
{
    return m_drag;
}

glm::vec3 ForceComponent::GetAcceleration()
{
    return m_acceleration;
}

glm::vec3 ForceComponent::GetVelocity()
{
    return m_velocity;
}

float ForceComponent::GetCentrifugalDrag()
{
    return m_centrifugalDrag;
}

bool ForceComponent::IsActive()
{
    if (m_isActive)
    {
        return true;
    }
    else
    {
        return false;
    }
}

glm::vec3 ForceComponent::GetCentrifugalAcceleration()
{
    return m_centrifugalAcceleration;
}

glm::vec3 ForceComponent::GetCentrifugalVelocity()
{
    return m_centrifugalVelocity;
}

void ForceComponent::GetInfo(sComponentInfo& compInfoOut)
{
    compInfoOut.componentName = "force";
    compInfoOut.componentParameters.clear();

    AddCompParInfo("inverseMass", "float", GetInverseMass(), compInfoOut);
    AddCompParInfo("restitution", "float", GetRestitution(), compInfoOut);
    AddCompParInfo("drag", "float", GetDrag(), compInfoOut);
    AddCompParInfo("centrifugalDrag", "float", GetCentrifugalDrag(), compInfoOut);
    AddCompParInfo("acceleration", "vec3", GetAcceleration(), compInfoOut);
    AddCompParInfo("velocity", "vec3", GetVelocity(), compInfoOut);
    AddCompParInfo("centrifugalAcceleration", "vec3", GetCentrifugalAcceleration(), compInfoOut);
    AddCompParInfo("centrifugalVelocity", "vec3", GetCentrifugalVelocity(), compInfoOut);

    Component::GetInfo(compInfoOut);

    return;
}

void ForceComponent::SetParameter(sParameterInfo& parameterIn)
{
    Component::SetParameter(parameterIn);

    if (parameterIn.parameterName == "inverseMass") {
        SetInverseMass(parameterIn.parameterFloatValue);
    }
    else if (parameterIn.parameterName == "restitution") {
        SetRestitution(parameterIn.parameterFloatValue);
    }
    else if (parameterIn.parameterName == "drag") {
        SetDrag(parameterIn.parameterFloatValue);
    }
    else if (parameterIn.parameterName == "centrifugalDrag") {
        SetCentrifugalDrag(parameterIn.parameterFloatValue);
    }
    else if (parameterIn.parameterName == "acceleration") {
        SetAcceleration(parameterIn.parameterVec3Value);
    }
    else if (parameterIn.parameterName == "velocity") {
        SetVelocity(parameterIn.parameterVec3Value);
    }
    else if (parameterIn.parameterName == "centrifugalAcceleration") {
        SetCentrifugalAcceleration(parameterIn.parameterVec3Value);
    }
    else if (parameterIn.parameterName == "centrifugalVelocity") {
        SetCentrifugalVelocity(parameterIn.parameterVec3Value);
    }

    return;
}
