#pragma once

#include "Component.h"

class ForceComponent : public Component
{
public:
	ForceComponent();
	~ForceComponent();

	// Setters
	void SetInverseMass(float value);
	void SetRestitution(float value);
	void SetDrag(float value);
	void SetAcceleration(glm::vec3 value);
	void SetVelocity(glm::vec3 value);
	void SetActive(bool isActive);
	void SetCentrifugalAcceleration(glm::vec3 value);
	void SetCentrifugalVelocity(glm::vec3 value);
	void SetCentrifugalDrag(float value);

	// Getters
	float GetInverseMass();
	float GetRestitution();
	float GetDrag();
	glm::vec3 GetAcceleration();
	glm::vec3 GetVelocity();
	bool IsActive();
	glm::vec3 GetCentrifugalAcceleration();
	glm::vec3 GetCentrifugalVelocity();
	float GetCentrifugalDrag();

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);

private:
	float m_inverseMass;
	float m_restitution;

	// Decceleration
	float m_drag;
	float m_centrifugalDrag;

	// Movement forces
	glm::vec3 m_acceleration;
	glm::vec3 m_velocity;

	// Rotation forces (In degrees)
	glm::vec3 m_centrifugalAcceleration;
	glm::vec3 m_centrifugalVelocity;
};

