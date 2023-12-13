#include "EnginePhysics/Physics.h"
#include "common/utilsMat.h"
#include "common/utils.h"
#include "components/Tag.h"
#include "scene/SceneView.h"
#include "EngineDebug/DebugSystem.h"
#include "common/constants.h"
#include "EnginePhysics/collisionTests.h"
#include <glm/gtx/string_cast.hpp>

typedef std::map<EntityID, sCollisions*>::iterator itColls;
typedef std::pair<EntityID, sCollisions*> pairColls;

Physics::Physics(CollisionEvent* pCollisionEvent)
{
	m_isRunning = false;
	m_pCollisionEvent = pCollisionEvent;
	m_pBroadPhaseCollision = new BroadPhaseCollision();
}

Physics::~Physics()
{
	delete m_pBroadPhaseCollision;
}

bool Physics::LoadScene()
{
	m_pBroadPhaseCollision->LoadScene();

	return true;
}

void Physics::NewFrame()
{
	for (sCollisionData* pCollision : m_vecCollided)
	{
		delete pCollision;
	}

	m_vecCollided.clear();
	m_vecCollVisited.clear();
}

void Physics::Update(double deltaTime)
{
	if (!m_isRunning)
	{
		return;
	}

	SceneView* pScene = SceneView::Get();

	// Check all non-static entities for collision
	m_CheckCollisions();

	// Return transform to non-collision point
	m_RepositionCollision();

	// Apply respective response for each collision types
	for (sCollisionData* pCollision : m_vecCollided)
	{
		TransformComponent* pTransformA = pScene->GetComponent<TransformComponent>(pCollision->entityA, "transform");
		TransformComponent* pTransformB = pScene->GetComponent<TransformComponent>(pCollision->entityB, "transform");

		// Static bodies won`t have force
		ForceComponent* pForceA = nullptr;
		if (pCollision->bodyTypeA != eBodyType::STATIC)
		{
			iComponent* pComp = pScene->GetComponent(pCollision->entityA, "force");
			if (pComp)
			{
				pForceA = (ForceComponent*)pComp;
			}
		}
		ForceComponent* pForceB = nullptr;
		if (pCollision->bodyTypeB != eBodyType::STATIC)
		{
			iComponent* pComp = pScene->GetComponent(pCollision->entityB, "force");
			if (pComp)
			{
				pForceB = (ForceComponent*)pComp;
			}
		}

		m_ResolveCollision(pCollision, pTransformA, pTransformB, pForceA, pForceB);
	}

	for (pairColls pcoll : m_mapCollisions)
	{
		delete pcoll.second;
	}
	m_mapCollisions.clear();

	// Trigger collision event for objects that collided
	m_pCollisionEvent->TriggerCollisions(m_vecCollided);

	// Change position based on the acceleration and velocity
	for (pScene->First("force"); !pScene->IsDone(); pScene->Next())
	{
		EntityID entityID = pScene->CurrentKey();
		ForceComponent* pForce = pScene->CurrentValue<ForceComponent>();

		if (!pForce->IsActive())
		{
			continue;
		}

		TransformComponent* pTransform = pScene->GetComponent<TransformComponent>(entityID, "transform");

		m_ApplyForce(pForce, pTransform, deltaTime);
	}
}

bool Physics::IsRunning()
{
	return m_isRunning;
}

void Physics::SetRunning(bool isRunning)
{
	m_isRunning = isRunning;

	if (!isRunning)
	{
		m_pBroadPhaseCollision->ClearAABBs();
	}
}

bool Physics::AABBAABB_Test(sAABB* aabbA, glm::mat4 matTransfA,
	sAABB* aabbB, glm::mat4 matTransfB,
	glm::vec3& contactPointA, glm::vec3& contactPointB,
	glm::vec3& collisionNormalA, glm::vec3& collisionNormalB)
{
	// Transform A in world space
	glm::vec4 AminWorld = (matTransfA * glm::vec4(aabbA->minXYZ, 1.0f));
	glm::vec4 AmaxWorld = (matTransfA * glm::vec4(aabbA->maxXYZ, 1.0f));

	// Transform B in world space
	glm::vec4 BminWorld = (matTransfB * glm::vec4(aabbB->minXYZ, 1.0f));
	glm::vec4 BmaxWorld = (matTransfB * glm::vec4(aabbB->maxXYZ, 1.0f));

	// Check if objects collide
	if (AmaxWorld[0] < BminWorld[0])
	{
		return false;
	}

	if (AminWorld[0] > BmaxWorld[0])
	{
		return false;
	}

	if (AmaxWorld[1] < BminWorld[1])
	{
		return false;
	}

	if (AminWorld[1] > BmaxWorld[1])
	{
		return false;
	}

	if (AmaxWorld[2] < BminWorld[2])
	{
		return false;
	}

	if (AminWorld[2] > BmaxWorld[2])
	{
		return false;
	}

	// Determine the collision side by calculating the minimum overlap in each direction
	float xOverlap = glm::min(AmaxWorld[0], BmaxWorld[0]) - glm::max(AminWorld[0], BminWorld[0]);
	float yOverlap = glm::min(AmaxWorld[1], BmaxWorld[1]) - glm::max(AminWorld[1], BminWorld[1]);
	float zOverlap = glm::min(AmaxWorld[2], BmaxWorld[2]) - glm::max(AminWorld[2], BminWorld[2]);

	// Determine the normal direction based on the collision side
	if (xOverlap < yOverlap && xOverlap < zOverlap) {
		// Horizontal collision
		if (AmaxWorld[0] < BmaxWorld[0]) {
			collisionNormalA = glm::vec3(-1.0f, 0.0f, 0.0f);
			collisionNormalB = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else {
			collisionNormalA = glm::vec3(1.0f, 0.0f, 0.0f);
			collisionNormalB = glm::vec3(-1.0f, 0.0f, 0.0f);
		}
	}
	else if (yOverlap < xOverlap && yOverlap < zOverlap) {
		// Vertical collision
		if (AmaxWorld[1] < BmaxWorld[1]) {
			collisionNormalA = glm::vec3(0.0f, -1.0f, 0.0f);
			collisionNormalB = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else {
			collisionNormalA = glm::vec3(0.0f, 1.0f, 0.0f);
			collisionNormalB = glm::vec3(0.0f, -1.0f, 0.0f);
		}
	}
	else {
		// Depth collision (Z-axis)
		if (AmaxWorld[2] < BmaxWorld[2]) {
			collisionNormalA = glm::vec3(0.0f, 0.0f, -1.0f);
			collisionNormalB = glm::vec3(0.0f, 0.0f, 1.0f);
		}
		else {
			collisionNormalA = glm::vec3(0.0f, 0.0f, 1.0f);
			collisionNormalB = glm::vec3(0.0f, 0.0f, -1.0f);
		}
	}

	// Calculate the contact points
	contactPointA = AminWorld;
	contactPointB = BminWorld;

	return true; // Collision detected
}

bool Physics::AABBAABB2D_Test(sAABB2D* aabb2dA, glm::mat4 matTransfA,
	sAABB2D* aabb2dB, glm::mat4 matTransfB,
	glm::vec3& contactPointA, glm::vec3& contactPointB,
	glm::vec3& collisionNormalA, glm::vec3& collisionNormalB)
{
	// Transform A in world space
	glm::vec4 AminWorld = (matTransfA * glm::vec4(aabb2dA->minXY, 1.0f, 1.0f));
	glm::vec4 AmaxWorld = (matTransfA * glm::vec4(aabb2dA->maxXY, 1.0f, 1.0f));

	// Transform B in world space
	glm::vec4 BminWorld = (matTransfB * glm::vec4(aabb2dB->minXY, 1.0f, 1.0f));
	glm::vec4 BmaxWorld = (matTransfB * glm::vec4(aabb2dB->maxXY, 1.0f, 1.0f));

	// Check if objects collide
	if (AmaxWorld[0] < BminWorld[0])
	{
		return false;
	}

	if (AminWorld[0] > BmaxWorld[0])
	{
		return false;
	}

	if (AmaxWorld[1] < BminWorld[1])
	{
		return false;
	}

	if (AminWorld[1] > BmaxWorld[1])
	{
		return false;
	}

	// Determine the collision side by calculating the minimum overlap in each direction
	float xOverlap = glm::min(AmaxWorld[0], BmaxWorld[0]) - glm::max(AminWorld[0], BminWorld[0]);
	float yOverlap = glm::min(AmaxWorld[1], BmaxWorld[1]) - glm::max(AminWorld[1], BminWorld[1]);

	// Determine the normal direction based on the collision side
	if (xOverlap < yOverlap) {
		// Horizontal collision
		if (AmaxWorld[0] < BmaxWorld[0]) {
			collisionNormalA = glm::vec3(-1.0f, 0.0f, 0.0f);
			collisionNormalB = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else {
			collisionNormalA = glm::vec3(1.0f, 0.0f, 0.0f);
			collisionNormalB = glm::vec3(-1.0f, 0.0f, 0.0f);
		}
	}
	else {
		// Vertical collision
		if (AmaxWorld[1] < BmaxWorld[1]) {
			collisionNormalA = glm::vec3(0.0f, -1.0f, 0.0f);
			collisionNormalB = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else {
			collisionNormalA = glm::vec3(0.0f, 1.0f, 0.0f);
			collisionNormalB = glm::vec3(0.0f, -1.0f, 0.0f);
		}
	}

	// Calculate the contact points
	contactPointA = glm::vec3(AminWorld[0], AminWorld[1], 0.0f);
	contactPointB = glm::vec3(BminWorld[0], BminWorld[1], 0.0f);

	return true; // Collision detected
}

bool Physics::SphereMeshTriangles_Test(sSphere* sphereA, glm::vec3 sphereAPosition,
	sMesh* meshB, glm::mat4 matTransfB,
	glm::vec3& contactPointA, glm::vec3& contactPointB,
	glm::vec3& collisionNormalA, glm::vec3& collisionNormalB)
{
	using namespace glm;
	using namespace myutils;

	float closestDistanceSoFar = FLT_MAX;
	vec3 closestTriangleVertices[3] = { vec3(0.0f), vec3(0.0f), vec3(0.0f) };
	vec3 closestContactPoint = vec3(0.0f);
	unsigned int indexOfClosestTriangle = INT_MAX;

	// Which triangle is closest to this sphere (right now)
	for (unsigned int index = 0; index != meshB->numberOfIndices; index += 3)
	{
		vec3 verts[3];

		// Get triangle vertices
		verts[0].x = meshB->pVertices[meshB->pIndices[index]].x;
		verts[0].y = meshB->pVertices[meshB->pIndices[index]].y;
		verts[0].z = meshB->pVertices[meshB->pIndices[index]].z;

		verts[1].x = meshB->pVertices[meshB->pIndices[index + 1]].x;
		verts[1].y = meshB->pVertices[meshB->pIndices[index + 1]].y;
		verts[1].z = meshB->pVertices[meshB->pIndices[index + 1]].z;

		verts[2].x = meshB->pVertices[meshB->pIndices[index + 2]].x;
		verts[2].y = meshB->pVertices[meshB->pIndices[index + 2]].y;
		verts[2].z = meshB->pVertices[meshB->pIndices[index + 2]].z;

		vec4 vertsWorld[3];
		vertsWorld[0] = (matTransfB * vec4(verts[0], 1.0f));
		vertsWorld[1] = (matTransfB * vec4(verts[1], 1.0f));
		vertsWorld[2] = (matTransfB * vec4(verts[2], 1.0f));

		// Getting closest point in triangle
		vec3 thisTriangleClosestPoint = ClosestPtPointTriangle(sphereAPosition,
			vertsWorld[0], vertsWorld[1], vertsWorld[2]);

		// Is this the closest so far
		float distanceToThisTriangle = distance(thisTriangleClosestPoint, sphereAPosition);
		if (distanceToThisTriangle > closestDistanceSoFar)
		{
			continue;
		}

		// this one is closer
		closestDistanceSoFar = distanceToThisTriangle;
		// Make note of the triangle index
		indexOfClosestTriangle = index;
		// 
		closestTriangleVertices[0] = vertsWorld[0];
		closestTriangleVertices[1] = vertsWorld[1];
		closestTriangleVertices[2] = vertsWorld[2];

		closestContactPoint = thisTriangleClosestPoint;

	} //for ( unsigned int index...

	// Hit the triangle?
	if (closestDistanceSoFar > sphereA->radius)
	{
		// no
		return false;
	}

	collisionNormalB = GetNormal(closestTriangleVertices);
	collisionNormalA = collisionNormalB;

	// Calculate the contact points
	contactPointA = closestContactPoint;
	contactPointB = closestContactPoint;

	return true;
}

bool Physics::SphereTriangle_Test(sSphere* sphereA, glm::vec3 sphereAPosition,
								  sTriangle triangleB,
								  glm::vec3& contactPointA, glm::vec3& contactPointB,
								  glm::vec3& collisionNormalA, glm::vec3& collisionNormalB)
{
	using namespace glm;
	using namespace myutils;

	// Getting closest point in triangle
	vec3 closestPoint = ClosestPtPointTriangle(sphereAPosition,
		triangleB.vertices[0], triangleB.vertices[1],
		triangleB.vertices[2]);

	// Is this the closest so far
	float distanceToThisTriangle = distance(closestPoint, sphereAPosition);
	if (distanceToThisTriangle > sphereA->radius)
	{
		return false;
	}

	// Is collision then get info
	contactPointA = closestPoint;
	contactPointB = closestPoint;
	collisionNormalB = triangleB.normal;
	collisionNormalA = -collisionNormalB;

	return true;
}

bool Physics::SphereSphere_Test(sSphere* pSphereA, glm::vec3 sphereAPosition,
	sSphere* pSphereB, glm::vec3 sphereBPosition,
	glm::vec3& contactPointA, glm::vec3& contactPointB,
	glm::vec3& collisionNormalA, glm::vec3& collisionNormalB)
{
	// Calculate squared distance between centers
	glm::vec3 d = sphereAPosition - sphereBPosition;
	float dist2 = glm::dot(d, d);
	// Spheres intersect if squared distance is less than squared sum of radii
	float radiusSum = pSphereA->radius + pSphereB->radius;
	if (dist2 > radiusSum * radiusSum)
	{
		// Not hit
		return false;
	}

	contactPointA = myutils::GetSpheresContactPont(sphereAPosition, pSphereA->radius,
		sphereBPosition, pSphereB->radius);
	contactPointB = myutils::GetSpheresContactPont(sphereBPosition, pSphereB->radius,
		sphereAPosition, pSphereA->radius);
	collisionNormalA = myutils::GetNormal(contactPointB, sphereBPosition);
	collisionNormalB = myutils::GetNormal(contactPointA, sphereAPosition);

	return true;
}

void Physics::m_CreateOrAddCollision(EntityID entityId, EntityID entityCollided, 
									 sTriangle triangle, glm::vec3 normal)
{
	using namespace glm;

	itColls it = m_mapCollisions.find(entityId);
	sCollisions* pColl;
	if (it == m_mapCollisions.end())
	{
		pColl = new sCollisions();
		pColl->mergedNormal = glm::vec3(0);

		m_mapCollisions[entityId] = pColl;
	}
	else
	{
		pColl = it->second;
	}

	if (entityCollided > 0)
	{
		pColl->collidedEntities.push_back(entityCollided);
	}
	else
	{
		pColl->collidedTriangles.push_back(triangle);
	}

	// Get addition of vectors
	glm::vec3 summed = (pColl->mergedNormal + normal);
	// When getting normal of 0 glm set to NaN
	if (summed != glm::vec3(0))
	{
		summed = normalize(summed);
	}
	pColl->mergedNormal = summed;
}

void Physics::m_ApplyForce(ForceComponent* pForce, TransformComponent* pTransform, double deltaTime)
{
	using namespace glm;

	// Check if object have mass
	if (pForce->GetInverseMass() <= 0.0f)
	{
		return;
	}

	// Explicit forward Euler "integration step"
	//	NewVelocity = LastVel + (Accel * DeltaTime)
	//	NewPosition = LastPos + (Vel * DeltaTime)	

	// Calculate new velocity this frame based on 
	// delta time, acceleration and current velocity
	vec3 velThisFrame = myutils::IncreaseVelocity(pForce->GetVelocity(),
		pForce->GetAcceleration(),
		pForce->GetDrag(),
		(float)deltaTime);
	pForce->SetVelocity(velThisFrame);

	// New object position
	vec3 deltaPosition = velThisFrame * (float)deltaTime;

	pTransform->Move(deltaPosition);

	// Apply centrifugal forces
	// Same principle with movement velocity but applying adjusts to rotation
	vec3 rotationVel = myutils::IncreaseVelocity(pForce->GetCentrifugalVelocity(),
												 pForce->GetCentrifugalAcceleration(),
												 pForce->GetCentrifugalDrag(),
												 (float)deltaTime);
	pForce->SetCentrifugalVelocity(rotationVel);
	// New object position
	vec3 deltaRotation = rotationVel * (float)deltaTime;

	pTransform->AdjustOrientation(deltaRotation);

	return;
}

void Physics::m_CheckCollisions()
{
	m_pBroadPhaseCollision->UpdateDynamicEntities();

	SceneView* pScene = SceneView::Get();
	// Get aabbs that have dynamic entities
	mapIdxAABB mapAABBs = m_pBroadPhaseCollision->GetActivesAABBs();
	for (pairIdxAABB pairAABB : mapAABBs)
	{
		uint idxaabb = pairAABB.first;
		cAABB* pAABB = pairAABB.second;

		DebugSystem* pDebug = DebugSystem::Get();
		if (pDebug->IsModesOn(eDebugMode::BROADPHASE_TRIANGLES))
		{
			for (const sTriangle& triangle : pAABB->vecIdxTriangles)
			{
				pDebug->AddTriangle(triangle.vertices[0],
					triangle.vertices[1],
					triangle.vertices[2],
					BLUE);
			}
		}

		// Check collisions between entities inside aabbs
		for (EntityID entityA : pAABB->vecEntities)
		{
			std::vector<sTriangle> vecTriangles;

			// Get all inside this aabb
			m_CheckBroadPhaseCollision(idxaabb,
				vecTriangles);

			// Check the detailed collisions
			m_CheckNarrowPhaseCollision(entityA,
				vecTriangles);
		}

		// Clear collision comparisons for next aabb
		m_vecCollVisited.clear();
	}

	return;
}

void Physics::m_RepositionCollision()
{
	using namespace glm;

	for (pairColls pairColl : m_mapCollisions)
	{
		EntityID entityId = pairColl.first;
		sCollisions* pCollisions = pairColl.second;

		SceneView* pScene = SceneView::Get();
		TransformComponent* pTransform = pScene->GetComponent<TransformComponent>(entityId, "transform");
		CollisionComponent* pCollision = pScene->GetComponent<CollisionComponent>(entityId, "collision");

		vec3 oldPos	= pTransform->GetOldPosition();
		vec3 newPos	= pTransform->GetPosition();
		vec3 collPoint = vec3(0);

		// TODO: Use strategy pattern to merge all collisions tests to avoid all these repetitions and iterations
		// Get Which type is the entity
		bool validPos = true;
		if (pCollision->Get_eShape() == eShape::SPHERE)
		{
			sSphere* pSphere = pCollision->GetShape<sSphere>();

			// TODO: This should be a complete test to avoid hitting something on the way back that wasnt on the list
			// Now we must validate the position for all the entities and triangles that it collided
			for (sTriangle triangle : pCollisions->collidedTriangles)
			{
				validPos &= GetValidPos(pSphere->radius, triangle.vertices, oldPos, newPos, 0);
			}
		}

		pTransform->SetPosition(newPos);
	}
}

void Physics::m_CheckBroadPhaseCollision(uint idxaabb,
	std::vector<sTriangle>& vecTrianglesOut)
{
	cAABB* pAABB = m_pBroadPhaseCollision->GetAABB(idxaabb);
	if (pAABB)
	{
		// There is an aabb so append the triangles to the vector to test
		vecTrianglesOut.insert(vecTrianglesOut.end(),
			pAABB->vecIdxTriangles.begin(),
			pAABB->vecIdxTriangles.end());
	}

	return;
}

void Physics::m_CheckNarrowPhaseCollision(EntityID entityA,
										  std::vector<sTriangle>& vecTrianglesIn)
{
	using namespace std;
	using namespace glm;

	bool isIntersecting = false;
	vec3 collisionNormalA = vec3(0);
	vec3 collisionNormalB = vec3(0);

	SceneView* pScene = SceneView::Get();
	CollisionComponent* pCollA = pScene->GetComponent<CollisionComponent>(entityA, "collision");
	TransformComponent* pTransformA = pScene->GetComponent<TransformComponent>(entityA, "transform");

	if (!pCollA->IsActive())
	{
		return;
	}

	// TODO: Narrow phase should go through only non-static entities
	if (pCollA->Get_eBodyType() == eBodyType::STATIC)
	{
		// Test collision only against already visited to avoid repeated checks
		m_vecCollVisited.push_back(entityA);
		return;
	}

	mat4 transformMatA = pTransformA->GetTransformNoRotation();
	vec3 contactPointA(0);
	vec3 contactPointB(0);

	// Go through list of already visited components, this way we are sure we'll only test 1 time each
	for (EntityID entityB : m_vecCollVisited)
	{
		bool isCollision = false;
		CollisionComponent* pCollB = pScene->GetComponent<CollisionComponent>(entityB, "collision");
		TransformComponent* pTransformB = pScene->GetComponent<TransformComponent>(entityB, "transform");
		mat4 transformMatB = pTransformB->GetTransformNoRotation();

		if (pCollA->Get_eShape() == eShape::AABB2D && pCollB->Get_eShape() == eShape::AABB2D)
		{
			sAABB2D* pAABB2D_A = pCollA->GetShape<sAABB2D>();
			sAABB2D* pAABB2D_B = pCollB->GetShape<sAABB2D>();

			isCollision = AABBAABB2D_Test(pAABB2D_A, transformMatA, pAABB2D_B, transformMatB,
				contactPointA, contactPointB, collisionNormalA,
				collisionNormalB);
		}
		else if (pCollA->Get_eShape() == eShape::AABB && pCollB->Get_eShape() == eShape::AABB)
		{
			sAABB* pAABB_A = pCollA->GetShape<sAABB>();
			sAABB* pAABB_B = pCollB->GetShape<sAABB>();

			isCollision = AABBAABB_Test(pAABB_A, transformMatA, pAABB_B, transformMatB,
				contactPointA, contactPointB, collisionNormalA,
				collisionNormalB);
		}
		else if (pCollA->Get_eShape() == eShape::SPHERE && pCollB->Get_eShape() == eShape::SPHERE)
		{
			sSphere* pSphereA = pCollA->GetShape<sSphere>();
			sSphere* pSphereB = pCollB->GetShape<sSphere>();

			isCollision = SphereSphere_Test(pSphereA, pTransformA->GetPosition(),
				pSphereB, pTransformB->GetPosition(),
				contactPointA, contactPointB,
				collisionNormalA, collisionNormalB);
		}
		else
		{
			// Collision test not implemented yet
			continue;
		}

		if (!isCollision)
		{
			continue;
		}

		TagComponent* tagA = pScene->GetComponent<TagComponent>(entityA, "tag");
		TagComponent* tagB = pScene->GetComponent<TagComponent>(entityB, "tag");

		// Set all collision data needed for others to handle it
		sCollisionData* pCollision = new sCollisionData();
		pCollision->entityA = entityA;
		pCollision->entityB = entityB;
		pCollision->tagA = tagA->name;
		pCollision->tagB = tagB->name;
		pCollision->bodyTypeA = pCollA->Get_eBodyType();
		pCollision->bodyTypeB = pCollB->Get_eBodyType();
		pCollision->collisionNormalA = collisionNormalA;
		pCollision->collisionNormalB = collisionNormalB;
		pCollision->contactPointA = contactPointA;
		pCollision->contactPointB = contactPointB;
		pCollision->positionA = pTransformA->GetPosition();
		pCollision->positionB = pTransformB->GetPosition();

		// Add to vector that will be later sent as notification
		m_vecCollided.push_back(pCollision);

		// TODO: we can only handle 1 collision right now, find a way to accumulate the opposite force
		return;
	}

	// Test collision only against already visited to avoid repeated checks
	m_vecCollVisited.push_back(entityA);

	// Now test all triangles that can be colliding
	for (sTriangle triangle : vecTrianglesIn)
	{
		sSphere* pSphere_A = pCollA->GetShape<sSphere>();

		bool isCollision = SphereTriangle_Test(pSphere_A, pTransformA->GetPosition(),
			triangle,
			contactPointA, contactPointB,
			collisionNormalA, collisionNormalB);

		if (!isCollision)
		{
			continue;
		}

		TagComponent* tagA = pScene->GetComponent<TagComponent>(entityA, "tag");

		// TODO: This collision data structure is not good, and should be able to get the triangles entityid too 
		// Set all collision data needed for others to handle it
		sCollisionData* pCollision = new sCollisionData();
		pCollision->entityA = entityA;
		pCollision->entityB = 0;
		pCollision->tagA = tagA->name;
		pCollision->tagB = "";
		pCollision->bodyTypeA = pCollA->Get_eBodyType();
		pCollision->bodyTypeB = eBodyType::STATIC;
		pCollision->collisionNormalA = collisionNormalA;
		pCollision->collisionNormalB = collisionNormalB;
		pCollision->contactPointA = contactPointA;
		pCollision->contactPointB = contactPointB;
		pCollision->positionA = pTransformA->GetPosition();
		pCollision->positionB = triangle.getCentre();

		// Add to vector that will be later sent as notification
		m_vecCollided.push_back(pCollision);
		m_CreateOrAddCollision(entityA, 0, triangle, collisionNormalB);
	}
}

// TODO: Entity A will never be static body here right? because we just jump them
void Physics::m_ResolveCollision(sCollisionData* pCollisionEvent,
								 TransformComponent* pTransformA, TransformComponent* pTransformB,
								 ForceComponent* pForceA, ForceComponent* pForceB)
{
	using namespace glm;

	vec3 accelerationA = vec3(0);
	vec3 accelerationB = vec3(0);

	vec3 velocityA = vec3(0);
	vec3 velocityB = vec3(0);

	float inverseMassA = 0;
	float inverseMassB = 0;

	float restitutionA = 0;
	float restitutionB = 0;

	if (pForceA)
	{
		accelerationA = pForceA->GetAcceleration();
		velocityA = pForceA->GetVelocity();
		inverseMassA = pForceA->GetInverseMass();
		restitutionA = pForceA->GetRestitution();
	}

	if (pForceB)
	{
		accelerationB = pForceB->GetAcceleration();
		velocityB = pForceB->GetVelocity();
		inverseMassB = pForceB->GetInverseMass();
		restitutionB = pForceB->GetRestitution();
	}

	DebugSystem* pDebug = DebugSystem::Get();
	pDebug->AddSphere(pCollisionEvent->contactPointA, 0.05f, RED);

	// Velocity and acceleration 0 in collision normal direction
	//if (pCollisionEvent->bodyTypeA == eBodyType::DYNAMIC &&
	//	pCollisionEvent->bodyTypeB == eBodyType::STATIC)
	//{
	//	myutils::CalculateProjectedDirection(pCollisionEvent->collisionNormalB, accelerationA);

	//	pForceA->SetAcceleration(accelerationA);

	//	myutils::CalculateProjectedDirection(pCollisionEvent->collisionNormalB, velocityA);

	//	pForceA->SetVelocity(velocityA);
	//}

	//if (pCollisionEvent->bodyTypeB == eBodyType::DYNAMIC &&
	//	pCollisionEvent->bodyTypeA == eBodyType::STATIC)
	//{
	//	myutils::CalculateProjectedDirection(pCollisionEvent->collisionNormalB, accelerationB);

	//	pForceB->SetAcceleration(accelerationB);

	//	myutils::CalculateProjectedDirection(pCollisionEvent->collisionNormalB, velocityB);

	//	pForceB->SetVelocity(velocityB);
	//}

	vec3 contactNormal = glm::vec3(0);
	itColls it = m_mapCollisions.find(pCollisionEvent->entityA);
	if (it == m_mapCollisions.end())
	{
		it = m_mapCollisions.find(pCollisionEvent->entityB);
		if (it == m_mapCollisions.end())
		{
			return;
		}
		else
		{
			contactNormal = it->second->mergedNormal;
		}
	}
	else
	{
		contactNormal = it->second->mergedNormal;
	}

	if (pCollisionEvent->bodyTypeA == eBodyType::DYNAMIC && pForceA)
	{
		myutils::ResolveVelocity(velocityA, velocityB, contactNormal,
			pForceA->GetRestitution(), pForceA->GetInverseMass(), 0.0f);

		pForceA->SetAcceleration(glm::vec3(0));
		pForceA->SetVelocity(velocityA);
	}

	if (pCollisionEvent->bodyTypeB == eBodyType::DYNAMIC && pForceB)
	{
		myutils::ResolveVelocity(velocityB, velocityA, contactNormal,
			pForceB->GetRestitution(), pForceB->GetInverseMass(), 0.0f);

		pForceB->SetAcceleration(glm::vec3(0));
		pForceB->SetVelocity(velocityB);
	}
}
