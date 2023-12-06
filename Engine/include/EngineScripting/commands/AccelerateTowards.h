#pragma once

#include "common/types.h"
#include "EngineScripting/commands/Command.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class AccelerateTowards : public Command
{
public:
	AccelerateTowards();
	virtual ~AccelerateTowards() {};

	virtual bool Initialize(rapidjson::Value& document);

	// Infinite command.
	// Accelerates until command is deleted
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;

	glm::vec3 m_direction; // Relative direction normalized to accelerate towards:
						   // x = 1  - Relative right to the object
						   // y = 1  - Relative up of the object
						   // z = -1 - Relative forward of the object

	float m_acceleration;  // Acceleration value to apply
	float m_maxSpeed;      // Max speed the object can reach, if 0 then no limit for speed

	glm::vec3 m_vecAcc;
	glm::vec3 m_vecMaxVel;

	// Update the acceleration towards parameter direction
	void m_Accelerate();
};
