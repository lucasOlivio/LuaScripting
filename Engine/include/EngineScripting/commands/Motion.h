#pragma once

#include "common/types.h"
#include "EngineScripting/commands/Command.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

// Parent class for any motion (movement or rotation) that uses acceleration AND velocity
class Motion : public Command
{
public:
	Motion();
	virtual ~Motion() {};

	virtual bool Initialize(rapidjson::Value& document);
	// For subcommands initialization, avoids searching scene every time
	virtual void Initialize(TransformComponent* pTransform, ForceComponent* pForce, 
							glm::vec3 endXYZ, float time);
	virtual void Initialize(TransformComponent* pTransform, ForceComponent* pForce, 
							glm::vec3 direction, glm::vec3 maxSpeed);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
protected:
	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;
private:
	enum ePhase
	{
		STARTUP,
		EASYIN,
		CONSTANT,
		EASYOUT
	};
	ePhase m_currPhase;

	glm::vec3 m_endxyz;       // End motion point to reach
	glm::vec3 m_easyInPoint;  // Point in motion it will stop accelerating
	glm::vec3 m_easyOutPoint; // Point in motion it will start decelerating
	bool m_stopAtEnd;         // When arrive at location should stop?

	glm::vec3 m_maxVelocity;	  // Max velocity the object can reach

	float m_time;		  // Max time that should take to finish
	float m_easyInTime;   // Time it will accelerate
	float m_easyOutTime;  // Time it will decelerate
	float m_constantTime; // Time the velocity will be constant

	float m_elapsedTime;

	// Update the acceleration based on a fixed time
	void m_SetAccTime(glm::vec3 currPoint, glm::vec3 currVelocity);
	// Update the acceleration based on a fixed max velocity
	void m_SetAccMaxVel(glm::vec3 currPoint, glm::vec3 targetPos);

	// Get the current motion vectors, 
	// child class just need to set these to the respective motion it will perform
	virtual glm::vec3 m_GetMotionPoint() = 0;
	virtual glm::vec3 m_GetMotionAcceleration() = 0;
	virtual glm::vec3 m_GetMotionVelocity() = 0;
	// Set the respective motion vector
	virtual void m_SetMotionPoint(glm::vec3 value) = 0;
	virtual void m_SetMotionAcceleration(glm::vec3 value) = 0;
	virtual void m_SetMotionVelocity(glm::vec3 value) = 0;
};
