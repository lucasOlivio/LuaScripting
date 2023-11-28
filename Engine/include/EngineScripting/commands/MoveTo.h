#pragma once

#include "common/types.h"
#include "EngineScripting/iCommand.h"
#include <glm/vec3.hpp>

class MoveTo : public iCommand
{
public:
	MoveTo(EntityID entity, float x, float y, float z, float sec);
	virtual ~MoveTo() {};

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool isDone(void);

	// Optional: Called before the 1st Update
	virtual bool PreStart(void);
	// Optional: Called after isDone() returns true
	virtual bool PostEnd(void);

private:
	glm::vec3 m_finalPos;
};
