#pragma once

#include "common/types.h"
#include "EngineScripting/commands/Command.h"
#include "scene/SceneView.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class OrientTo : public Command
{
public:
	OrientTo();
	virtual ~OrientTo() {};

	virtual bool Initialize(SceneView* pScene, rapidjson::Value& document);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	// Calculate the velocity it should reach
	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	glm::vec3 m_orientation; // End orientation to rotate object
	float m_time;		     // Max time that should take to rotate
	bool m_stopAtEnd;        // When reach orientation should stop centrifugal force?

	float m_accelerationTime; // % of the time it will accelerate
	float m_decelerationTime; // % of the time it will decelerate
	float m_constantTime;     // % of the time the velocity will be constant

	float m_elapsedTime;

	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;
};
