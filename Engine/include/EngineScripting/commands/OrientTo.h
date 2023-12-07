#pragma once

#include "common/types.h"
#include "EngineScripting/commands/Command.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class OrientTo : public Command
{
public:
	OrientTo();
	virtual ~OrientTo() {};

	virtual bool Initialize(rapidjson::Value& document);
	// For subcommands initialization, avoids searching scene every time
	virtual void Initialize(TransformComponent* pTransform, glm::vec3 endxyz);

	// Orient towards the end point
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	glm::vec3 m_endxyz;

	float m_lastX;
	float m_lastY;

	TransformComponent* m_pTransform;
};
