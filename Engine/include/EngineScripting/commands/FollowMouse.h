#pragma once

#include "common/types.h"
#include "EngineScripting/commands/Command.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class FollowMouse : public Command
{
public:
	FollowMouse();
	virtual ~FollowMouse() {};

	virtual bool Initialize(rapidjson::Value& document);

	// Follows mouse indefenitely
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	// Lock the mouse to the screen and reset position to center
	virtual bool PreStart(void);

	// Releases the mouse
	virtual bool PostEnd(void);
private:
	float m_sensitivity;
	float m_lastX;
	float m_lastY;

	TransformComponent* m_pTransform;
};
