#pragma once

#include "events/CollisionEvent.h"
#include "components/Transform.h"
#include "components/Model.h"
#include "components/Force.h"
#include "components/Collision.h"
#include "BroadPhaseCollision.h"

class DebugSystem;

class Physics
{
public:
	Physics(CollisionEvent* pCollisionEvent);
	~Physics();

	// Load static collisions to AABBs broadphase
	bool LoadScene();

	// Resets collisions
	void NewFrame();

	// Physics update per frame
	void Update(double deltaTime);

	bool IsRunning();
	void SetRunning(bool isRunning);

	// Collision tests
	// Returns the collision points for each body

	bool AABBAABB_Test(sAABB* aabbA, glm::mat4 matTransfA,
		sAABB* aabbB, glm::mat4 matTransfB,
		glm::vec3& contactPointA, glm::vec3& contactPointB,
		glm::vec3& collisionNormalA, glm::vec3& collisionNormalB);

	bool AABBAABB2D_Test(sAABB2D* aabb2dA, glm::mat4 matTransfA,
		sAABB2D* aabb2dB, glm::mat4 matTransfB,
		glm::vec3& contactPointA, glm::vec3& contactPointB,
		glm::vec3& collisionNormalA, glm::vec3& collisionNormalB);

	bool SphereMeshTriangles_Test(sSphere* sphereA, glm::vec3 sphereAPosition,
		sMesh* meshB, glm::mat4 matTransfB,
		glm::vec3& contactPointA, glm::vec3& contactPointB,
		glm::vec3& collisionNormalA, glm::vec3& collisionNormalB);

	bool SphereTriangle_Test(sSphere* sphereA, glm::vec3 sphereAPosition,
		sTriangle triangle,
		glm::vec3& contactPointA, glm::vec3& contactPointB,
		glm::vec3& collisionNormalA, glm::vec3& collisionNormalB);

	bool SphereSphere_Test(sSphere* pSphereA, glm::vec3 sphereAPosition,
		sSphere* pSphereB, glm::vec3 sphereBPosition,
		glm::vec3& contactPointA, glm::vec3& contactPointB,
		glm::vec3& collisionNormalA, glm::vec3& collisionNormalB);
private:
	bool m_isRunning;

	CollisionEvent* m_pCollisionEvent;

	BroadPhaseCollision* m_pBroadPhaseCollision;

	// All the entities that we already tested in the frame (true or false)
	std::vector<EntityID> m_vecCollVisited;

	// Collisions that happenned in the frame
	std::vector<sCollisionData*> m_vecCollided;
	std::map<EntityID, sCollisions*> m_mapCollisions;

	// Merge to alread existing collision for entity or create new collision for entity in vector
	void m_CreateOrAddCollision(EntityID entityId, EntityID entityCollided, 
								sTriangle triangle, glm::vec3 normal);

	// Update object transform based on velocity and acceleration
	void m_ApplyForce(ForceComponent* pForce, TransformComponent* pTransform, double deltaTime);

	// Check every object in scene for collision between the collisionShapes
	// Add collision to map cache and send collision events
	void m_CheckCollisions();

	// Get entity back to point of no collision
	void m_RepositionCollision();

	// Check which aabbs contains the entity and returns the triangles in it to be tested
	void m_CheckBroadPhaseCollision(uint idxaabb,
		std::vector<sTriangle>& vecTrianglesOut);

	// Given the limited objects, now we can compare in more details the collision
	void m_CheckNarrowPhaseCollision(EntityID entityA,
		std::vector<sTriangle>& vecTrianglesIn);

	// Given the collision, calculates the new positions and velocities
	void m_ResolveCollision(sCollisionData* pCollisionEvent,
		TransformComponent* pTransformA, TransformComponent* pTransformB,
		ForceComponent* pForceA, ForceComponent* pForceB);

	// TODO: Temporarily to debug broadphase creation
	friend class DebugSystem;

};
