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
	void Initialize(SceneView* pScene, TransformComponent* pTransform, 
					ForceComponent* pForce, glm::vec3 location, float time);

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

	float m_time;		  // Max time that should take to move
	float m_easyInTime;   // Time it will accelerate
	float m_easyOutTime;  // Time it will decelerate
	float m_constantTime; // Time the velocity will be constant

	float m_elapsedTime;

	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;
};
