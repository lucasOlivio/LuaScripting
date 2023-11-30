#pragma once

#include "common/types.h"
#include "EngineScripting/commands/Command.h"
#include "scene/SceneView.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

// TODO: MoveTo and OrientTo (All movements commands) should have a common class
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
	enum ePhase
	{
		STARTUP,
		EASYIN,
		CONSTANT,
		EASYOUT
	};
	ePhase m_currPhase;

	glm::vec3 m_orientation; // End orientation to rotate to
	glm::vec3 m_easyInPos;   // Orientation it will stop accelerating
	glm::vec3 m_easyOutPos;  // Orientation it will start decelerating
	bool m_stopAtEnd;        // When arrive at orientation should stop rotating?

	float m_time;		  // Max time that should take to move
	float m_easyInTime;   // Time it will accelerate
	float m_easyOutTime;  // Time it will decelerate
	float m_constantTime; // Time the velocity will be constant

	float m_elapsedTime;

	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;
};
