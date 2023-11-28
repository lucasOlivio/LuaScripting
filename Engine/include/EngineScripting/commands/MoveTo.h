#pragma once

#include "common/types.h"
#include "EngineScripting/iCommand.h"
#include "scene/SceneView.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class MoveTo : public iCommand
{
public:
	MoveTo(SceneView* pScene, EntityID entity, glm::vec3 location, float time);
	virtual ~MoveTo() {};

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	// Calculate the velocity it should reach
	virtual bool PreStart(void);

	virtual bool PostEnd(void);

private:
	glm::vec3 m_finalPos;
	float m_lastDistance;

	float m_elapsedTime;
	float m_maxTime;

	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;
};
