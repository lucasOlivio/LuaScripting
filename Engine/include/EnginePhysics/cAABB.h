#pragma once

#include "common/types.h"
#include "PhysicsProperties.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <math.h>
#include <unordered_set>

class cAABB
{
public:
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;

	// Collision type: sMeshOfTriangles_Indirect
	// Index to mesh triangles inside this AABB
	std::vector<sTriangle> vecIdxTriangles;

	// Any other collision types go here
	std::unordered_set<EntityID> vecEntities;

	glm::vec3 GetCentreXYZ(void);

	glm::vec3 GetExtentsXYZ(void);

	void RemoveEntity(EntityID entityID);

	int GetNumEntities();
	int GetNumChildren();
};


