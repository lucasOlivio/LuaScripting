#pragma once

#include "Component.h"

class ForceComponent : public Component
{
private:
	bool m_isActive;
	float m_inverseMass;
	float m_restitution;

	// Movement forces
	glm::vec3 m_acceleration;
	glm::vec3 m_velocity;

	// Rotation forces (In degrees)
	glm::vec3 m_centrifugalAcceleration;
	glm::vec3 m_centrifugalVelocity;
public:
	ForceComponent();
	~ForceComponent();

	// Setters
	void SetInverseMass(float value);
	void SetRestitution(float value);
	void SetAcceleration(glm::vec3 value);
	void SetVelocity(glm::vec3 value);
	void SetActive(bool isActive);
	void SetCentrifugalAcceleration(glm::vec3 value);
	void SetCentrifugalVelocity(glm::vec3 value);

	// Getters
	float GetInverseMass();
	float GetRestitution();
	glm::vec3 GetAcceleration();
	glm::vec3 GetVelocity();
	bool IsActive();
	glm::vec3 GetCentrifugalAcceleration();
	glm::vec3 GetCentrifugalVelocity();

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};

