#pragma once

#include "EngineScripting/commands/Command.h"
#include "common/types.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class CreateEntity : public Command
{
public:
	CreateEntity();
	virtual ~CreateEntity() {};

	bool Initialize(rapidjson::Value& document);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	EntityID m_childEntity;        // Entity copy to create from
	float m_parentEntity;

	glm::vec3 m_position;
	glm::vec3 m_orientation;
};
