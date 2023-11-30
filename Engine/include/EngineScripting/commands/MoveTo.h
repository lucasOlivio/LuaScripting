#pragma once

#include "common/types.h"
#include "EngineScripting/commands/Command.h"
#include "scene/SceneView.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class MoveTo : public Command
{
public:
	MoveTo();
	virtual ~MoveTo() {};

	virtual bool Initialize(SceneView* pScene, rapidjson::Value& document);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	glm::vec3 m_location; // End location to move to
	float m_time;		  // Max time that should take to move
	bool m_stopAtEnd;       // When arrive at location should stop velocity 
						  // and acceleration or just keep going?

	float m_accelerationTime; // % of the time it will accelerate
	float m_decelerationTime; // % of the time it will decelerate
	float m_constantTime;     // % of the time the velocity will be constant

	float m_elapsedTime;

	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;
};
