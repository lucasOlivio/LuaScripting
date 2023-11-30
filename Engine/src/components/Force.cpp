#include "components/Force.h"

ForceComponent::ForceComponent()
{
    this->m_centrifugalAcceleration = glm::vec3(0);
    this->m_centrifugalVelocity = glm::vec3(0);
}

ForceComponent::~ForceComponent()
{
}

void ForceComponent::SetInverseMass(float value)
{
    this->m_inverseMass = value;
}

void ForceComponent::SetRestitution(float value)
{
    this->m_restitution = value;
}

void ForceComponent::SetAcceleration(glm::vec3 value)
{
    this->m_acceleration = value;
}

void ForceComponent::SetVelocity(glm::vec3 value)
{
    this->m_velocity = value;
}

void ForceComponent::SetActive(bool isActive)
{
    this->m_isActive = isActive;
}

void ForceComponent::SetCentrifugalAcceleration(glm::vec3 value)
{
    this->m_centrifugalAcceleration = value;
}

void ForceComponent::SetCentrifugalVelocity(glm::vec3 value)
{
    this->m_centrifugalVelocity = value;
}

float ForceComponent::GetInverseMass()
{
    return this->m_inverseMass;
}

float ForceComponent::GetRestitution()
{
    return this->m_restitution;
}

glm::vec3 ForceComponent::GetAcceleration()
{
    return this->m_acceleration;
}

glm::vec3 ForceComponent::GetVelocity()
{
    return this->m_velocity;
}

bool ForceComponent::IsActive()
{
    if (this->m_isActive)
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
    return this->m_centrifugalAcceleration;
}

glm::vec3 ForceComponent::GetCentrifugalVelocity()
{
    return this->m_centrifugalVelocity;
}

void ForceComponent::GetInfo(sComponentInfo& compInfoOut)
{
    compInfoOut.componentName = "force";
    compInfoOut.componentParameters.clear();

    this->AddCompParInfo("inverseMass", "float", this->GetInverseMass(), compInfoOut);
    this->AddCompParInfo("restitution", "float", this->GetRestitution(), compInfoOut);
    this->AddCompParInfo("acceleration", "vec3", this->GetAcceleration(), compInfoOut);
    this->AddCompParInfo("velocity", "vec3", this->GetVelocity(), compInfoOut);
    this->AddCompParInfo("isActive", "bool", this->m_isActive, compInfoOut);
    this->AddCompParInfo("centrifugalAcceleration", "vec3", this->GetCentrifugalAcceleration(), compInfoOut);
    this->AddCompParInfo("centrifugalVelocity", "vec3", this->GetCentrifugalVelocity(), compInfoOut);

    return;
}

void ForceComponent::SetParameter(sParameterInfo& parameterIn)
{
    if (parameterIn.parameterName == "inverseMass") {
        this->SetInverseMass(parameterIn.parameterFloatValue);
    }
    else if (parameterIn.parameterName == "restitution") {
        this->SetRestitution(parameterIn.parameterFloatValue);
    }
    else if (parameterIn.parameterName == "acceleration") {
        this->SetAcceleration(parameterIn.parameterVec3Value);
    }
    else if (parameterIn.parameterName == "velocity") {
        this->SetVelocity(parameterIn.parameterVec3Value);
    }
    else if (parameterIn.parameterName == "isActive") {
        this->m_isActive = parameterIn.parameterBoolValue;
    }
    else if (parameterIn.parameterName == "centrifugalAcceleration") {
        this->SetCentrifugalAcceleration(parameterIn.parameterVec3Value);
    }
    else if (parameterIn.parameterName == "centrifugalVelocity") {
        this->SetCentrifugalVelocity(parameterIn.parameterVec3Value);
    }

    return;
}
