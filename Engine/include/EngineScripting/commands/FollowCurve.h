#pragma once

#include "common/types.h"
#include "EngineScripting/CommandGroup.h"
#include "scene/SceneView.h"
#include "components/Transform.h"
#include "components/Force.h"
#include <glm/vec3.hpp>

class FollowCurve : public CommandGroup
{
public:
	FollowCurve();
	virtual ~FollowCurve() {};

	virtual bool Initialize(SceneView* pScene, rapidjson::Value& document);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	std::vector<glm::vec3> m_controlPoints; // Bezier curve control points 
											 // (current position inserted in first index)

	float m_timeStep;     // Steps to calculate the curve
	float m_time;		  // Max time that should take to move
	bool m_stopAtEnd;     // When arrive at location should stop?

	float m_accelerationTime; // % of the time it will accelerate
	float m_decelerationTime; // % of the time it will decelerate
	float m_constantTime;     // % of the time the velocity will be constant

	TransformComponent* m_pTransform;
	ForceComponent* m_pForce;

	void m_GenerateSubCommands(SceneView* pScene);
};
