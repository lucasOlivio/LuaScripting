#pragma once

#include "PhysicsProperties.hpp"
#include <glm/vec3.hpp>

// TODO: All collision tests should be replaced by this, to decouple and returns the normal

// Sphere x Triangle collision
// Collision normal to sphere
bool SphereTriangle(glm::vec3 sphereCenter, float radius, 
					glm::vec3 vertices[3]);


// Validate non-colliding position by recursively going halfway backwards
// newPosOut will be the new valid position it found 
// collPointOut will be the point where is colliding
// Returns false if doesnt found any valid position

// Sphere x triangle validation
bool GetValidPos(float radius, glm::vec3 vertices[3], 
				 glm::vec3 oldPos, glm::vec3& newPosOut,
				 int counter);
