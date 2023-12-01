#pragma once

#include "common/types.h"
#include "EngineScripting/commands/Command.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class MoveTo : public Command
{
public:
	MoveTo();
	virtual ~MoveTo() {};

	virtual bool Initialize(rapidjson::Value& document);
	// For subcommands initialization, avoids searching scene every time
	void Initialize(TransformComponent* pTransform, ForceComponent* pForce, glm::vec3 location, float time);
	void Initialize(TransformComponent* pTransform, ForceComponent* pForce, glm::vec3 location, glm::vec3 maxSpeed);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	enum ePhase
	{
		STARTUP,
		EASYIN,
		CONSTANT,
		EASYOUT
	};
	ePhase m_currPhase;

	glm::vec3 m_location;   // End location to move to
	glm::vec3 m_easyInPos;  // Position it will stop accelerating
	glm::vec3 m_easyOutPos; // Position it will start decelerating
	bool m_stopAtEnd;       // When arrive at location should stop velocity 
						    // and acceleration or just keep going?

	glm::vec3 m_maxVelocity;	  // Max velocity the object can reach

	float m_time;		  // Max time that should take to move
	float m_easyInTime;   // Time it will accelerate
	float m_easyOutTime;  // Time it will decelerate
	float m_constantTime; // Time the velocity will be constant

	float m_elapsedTime;

	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;

	// Update the acceleration based on a fixed time
	void m_SetAccTime(glm::vec3 currPosition, glm::vec3 currVelocity);
	// Update the acceleration based on a fixed max velocity
	void m_SetAccMaxVel(glm::vec3 currPosition, glm::vec3 currVelocity);
};
