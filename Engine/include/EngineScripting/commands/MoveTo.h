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

	// Calculate the velocity it should reach
	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	glm::vec3 m_location; // End location to move to
	float m_time;		  // Max time that should take to move

	float m_elapsedTime;
	float m_lastDistance;

	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;
};
