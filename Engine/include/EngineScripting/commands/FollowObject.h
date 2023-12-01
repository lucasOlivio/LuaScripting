#pragma once

#include "common/types.h"
#include "EngineScripting/CommandGroup.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class FollowObject : public CommandGroup
{
public:
	FollowObject();
	virtual ~FollowObject() {};

	virtual bool Initialize(rapidjson::Value& document);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	float m_timeStep;       // Frequency to update target location in seconds
	float m_maxSpeed;       // Max speed to reach 
	float m_followDistance; // Max it will get to the object
	glm::vec3 m_offset;     // Offset from target position

	bool m_followForever; // When reach object position should end command?

	float m_elapsedTime;

	glm::vec3 m_lastTargetPosition;

	TransformComponent* m_pTransformTarget;
	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;

	void m_GenerateSubCommands();

	// Update the target position based on distance and offset
	void m_UpdateTargetPosition();

	bool m_IsAtSamePosition();
};
