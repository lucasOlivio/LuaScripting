#include "EnginePhysics/collisionTests.h"
#include "common/utilsMat.h"

bool SphereTriangle(glm::vec3 sphereCenter, float radius, 
					glm::vec3 vertices[3])
{
	using namespace glm;
	using namespace myutils;

	// Getting closest point in triangle
	vec3 closestPoint = ClosestPtPointTriangle(sphereCenter,
											   vertices[0], vertices[1],
											   vertices[2]);

	// Is this the closest so far
	float distanceToThisTriangle = distance(closestPoint, sphereCenter);
	if (distanceToThisTriangle > radius)
	{
		return false;
	}

	return true;
}

const int MAX_TRIES = 5;
bool GetValidPos(float radius, glm::vec3 vertices[3],
				 glm::vec3 oldPos, glm::vec3& newPosOut,
				 int counter)
{
	using namespace glm;

	bool isCollision = SphereTriangle(newPosOut, radius, vertices);

	if (!isCollision)
	{
		// New position is a valid position
		return true;
	}

	if (counter > MAX_TRIES)
	{
		// Too much attempts
		return false;
	}
	counter += 1;

	// Validate the new position recursively by going halfway backwards
	newPosOut = 0.5f * (oldPos + newPosOut);

	// Check if already check all line
	float dist = distance(oldPos, newPosOut);
	if (dist < 0.001f)
	{
		// No valid position found
		return false;
	}

	return GetValidPos(radius, vertices, oldPos, newPosOut, counter);
}
