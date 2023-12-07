#pragma once

#include "cAABB.h"
#include <map>

typedef std::map< uint /*index*/, cAABB* > mapIdxAABB;
typedef std::map< uint /*index*/, cAABB* >::iterator itIdxAABB;
typedef std::pair< uint /*index*/, cAABB* > pairIdxAABB;
typedef std::unordered_set< EntityID >::iterator itEntities;

class BroadPhaseCollision
{
public:
	BroadPhaseCollision();
	~BroadPhaseCollision();

	void LoadScene();
	void ClearAABBs();

	// Calculate in which AABB index this points is
	uint LocatePoint(glm::vec3 point);

	glm::vec3 ReverseLocatePoint(uint theIndex);

	// Returns the relative AABB to this location
	cAABB* GetAABB(glm::vec3 point);
	// Returns the relative AABB to this index
	cAABB* GetAABB(uint idxAABB);

	// TODO: This update could happen when the new position is calculated,
	// this way we avoid having to iterate twice.
	// 
	// Update aabb location for all non-static entities
	void UpdateDynamicEntities();

	// Remove entity from AABB using index
	void RemoveEntityAABB(EntityID entityID, uint index);
	// Remove entity from AABB using its iterator position
	itEntities RemoveEntityAABB(itEntities it, uint index);

	// Remove aabb from active map
	size_t RemoveActiveAABB(uint idxAABB);

	// Remove aabb from main map
	size_t RemoveAABB(uint idxAABB);

	// Add entity to aabb and activate aabb
	void InsertEntityAABB(EntityID entityID, uint index);

	// Returns the active aabb map as value to allow removing 
	// and updating the aabb the entity is on
	mapIdxAABB GetActivesAABBs();
private:
	// All AABBs and its index localization
	mapIdxAABB m_mapAABBs;
	// Only AABBs that have non-static entities
	// Used for collision system to iterate updating the AABB the entity is now
	mapIdxAABB m_mapActiveAABBs;

	glm::vec3 m_lengthPerBox;

	// If aabb for this location is not on map then creates one
	cAABB* m_GetOrCreateAABB(uint idxAABB);

	// Go through each mesh triangle to insert into AABB
	// Only should use when collision type is mesh triangles indirect
	void m_InsertMeshTrianglesIndirect(EntityID entityID);

	// Check on all directions in which aabb the sphere intersects and inserts into
	void m_InsertSphere(EntityID entityID, float radius);

	// Calculate triangle location index and insert in cAABB
	// Insert triangle in every aabb thta collided
	void m_InsertTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

	// TODO: Temporarily to debug broadphase creation
	friend class DebugSystem;
};