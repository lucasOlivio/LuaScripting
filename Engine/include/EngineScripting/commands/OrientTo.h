#pragma once

#include "EngineScripting/commands/Motion.h"
#include <glm/vec3.hpp>

class OrientTo : public Motion
{
public:
	OrientTo();
	virtual ~OrientTo() {};

private:
	// Get the current motion vectors, 
	// child class just need to set these to the respective motion it will perform
	virtual glm::vec3 m_GetMotionPoint();
	virtual glm::vec3 m_GetMotionAcceleration();
	virtual glm::vec3 m_GetMotionVelocity();
	// Set the respective motion vector
	virtual void m_SetMotionPoint(glm::vec3 value);
	virtual void m_SetMotionAcceleration(glm::vec3 value);
	virtual void m_SetMotionVelocity(glm::vec3 value);

	// Overrides to get the right orientation velocity based on time
	virtual void m_SetAccTime(glm::vec3 currPoint, glm::vec3 currVelocity);
	// Overrides to get the right orientation velocity
	virtual void m_SetAccMaxVel(glm::vec3 currPoint, glm::vec3 targetPos);
};