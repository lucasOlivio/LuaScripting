#include "EnginePhysics/BroadPhaseCollision.h"
#include "scene/SceneView.h"
#include "components/Collision.h"
#include "components/Transform.h"
#include "components/Model.h"
#include <math.h>

typedef std::pair< uint, cAABB* > pairAABBs;
typedef std::map< uint, cAABB* >::iterator itAABBs;
typedef std::unordered_set< EntityID >::iterator itEntities;

BroadPhaseCollision::BroadPhaseCollision()
{
	// TODO: For non-static grid aabbs this should be dynamic, comming from component size or map
	m_lengthPerBox = glm::vec3(5'000, 5'000, 5'000);
}

BroadPhaseCollision::~BroadPhaseCollision()
{
	ClearAABBs();
}

void BroadPhaseCollision::LoadScene()
{
	ClearAABBs();

	SceneView* pScene = SceneView::Get();

	for (pScene->First("collision"); !pScene->IsDone(); pScene->Next())
	{
		EntityID entityID = pScene->CurrentKey();
		CollisionComponent* pCollision = pScene->CurrentValue<CollisionComponent>();

		if (!pCollision->IsActive())
		{
			continue;
		}

		if (pCollision->Get_eShape() == eShape::MESH_OF_TRIANGLES_INDIRECT)
		{
			m_InsertMeshTrianglesIndirect(entityID);
		}
		else if (pCollision->Get_eShape() == eShape::SPHERE)
		{
			m_InsertSphere(entityID, pCollision->GetShape<sSphere>()->radius);
		}
	}
}

void BroadPhaseCollision::ClearAABBs()
{
	for (pairAABBs pairAABB : m_mapAABBs)
	{
		delete pairAABB.second;
	}

	m_mapAABBs.clear();
}

uint BroadPhaseCollision::LocatePoint(glm::vec3 point)
{
	unsigned int theX = (unsigned int)floor(point.x / m_lengthPerBox.x);
	unsigned int theY = (unsigned int)floor(point.y / m_lengthPerBox.y);
	unsigned int theZ = (unsigned int)floor(point.z / m_lengthPerBox.z);

	// Shift the numbers into the columns we want:
	// xxx,yyy,zzz
	// For 32 bits, we'll multiply by 1000
	unsigned int theIndex = (theX * 1000 * 1000) +
							       (theY * 1000) +
							              (theZ);

	return theIndex;
}

glm::vec3 BroadPhaseCollision::ReverseLocatePoint(uint theIndex)
{
	unsigned int theZ = theIndex % 1000;
	unsigned int theY = (theIndex / 1000) % 1000;
	unsigned int theX = theIndex / (1000 * 1000);

	glm::vec3 minPoint;
	minPoint.x = (float)theX * m_lengthPerBox.x;
	minPoint.y = (float)theY * m_lengthPerBox.y;
	minPoint.z = (float)theZ * m_lengthPerBox.z;

	return minPoint;
}

cAABB* BroadPhaseCollision::GetAABB(uint idxAABB)
{
	itAABBs itAABB = m_mapAABBs.find(idxAABB);
	if (itAABB == m_mapAABBs.end())
	{
		return nullptr;
	}

	return itAABB->second;
}

void BroadPhaseCollision::UpdateDynamicEntities()
{
	SceneView* pScene = SceneView::Get();
	// Get aabbs that have dynamic entities
	mapIdxAABB mapAABBs = GetActivesAABBs();
	for (pairIdxAABB pairAABB : mapAABBs)
	{
		uint oldIdx = pairAABB.first;
		cAABB* pAABB = pairAABB.second;

		// First update all entities aabb location
		for (itEntities it = pAABB->vecEntities.begin(); 
			 it != pAABB->vecEntities.end();)
		{
			EntityID entityID = *it;

			TransformComponent* pTransf = pScene->GetComponent<TransformComponent>(entityID, "transform");
			CollisionComponent* pCollision = pScene->GetComponent<CollisionComponent>(entityID, "collision");

			uint newIdx = LocatePoint(pTransf->GetPosition());

			if (newIdx == oldIdx)
			{
				it++;
				continue;
			}

			// Entity changed aabb location, so update maps
			RemoveEntityAABB(entityID, oldIdx);

			InsertEntityAABB(entityID, newIdx);

			// TODO: Avoid deleting inside the vector iteration
			if (pAABB->vecEntities.size() == 0)
			{
				break;
			}
		}

		// and delete AABB if no more children
		if (pAABB->GetNumChildren() == 0)
		{
			m_mapAABBs.erase(oldIdx);
			delete pAABB;
			break;
		}
	}
}

void BroadPhaseCollision::RemoveEntityAABB(EntityID entityID, uint index)
{
	cAABB* pAABB = GetAABB(index);
	if (!pAABB)
	{
		return;
	}

	pAABB->RemoveEntity(entityID);

	// remove AABB from active if no entities left
	if (pAABB->GetNumEntities() == 0)
	{
		m_mapActiveAABBs.erase(index);
	}
}

void BroadPhaseCollision::InsertEntityAABB(EntityID entityID, uint index)
{
	cAABB* pAABB = m_GetOrCreateAABB(index);

	pAABB->vecEntities.insert(entityID);

	// If first entity inserted then we can set aabb to actives
	if (pAABB->GetNumEntities() == 1)
	{
		m_mapActiveAABBs[index] = pAABB;
	}
}

cAABB* BroadPhaseCollision::GetAABB(glm::vec3 point)
{
	uint idxAABB = LocatePoint(point);
	
	return GetAABB(idxAABB);
}

mapIdxAABB BroadPhaseCollision::GetActivesAABBs()
{
	return m_mapActiveAABBs;
}

cAABB* BroadPhaseCollision::m_GetOrCreateAABB(uint idxAABB)
{
	cAABB* pAABB = GetAABB(idxAABB);
	if (pAABB)
	{
		return pAABB;
	}

	pAABB = new cAABB();
	pAABB->minXYZ = ReverseLocatePoint(idxAABB);
	pAABB->maxXYZ = pAABB->minXYZ + m_lengthPerBox;
	
	pAABB->vecEntities = {};
	pAABB->vecIdxTriangles = {};

	m_mapAABBs[idxAABB] = pAABB;

	return pAABB;
}

void BroadPhaseCollision::m_InsertTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	sTriangle triangle = sTriangle(v1, v2, v3);
	triangle.calcNormal();

	// Locate each vertex
	uint idxV1 = LocatePoint(v1);
	uint idxV2 = LocatePoint(v2);
	uint idxV3 = LocatePoint(v3);

	// TODO: When a triangle edge passes 3 or more aabbs it should be in those aabbs too
	// Insert triangle into all AABBs that it intersects
	cAABB* pAABB = m_GetOrCreateAABB(idxV1);
	pAABB->vecIdxTriangles.push_back(triangle);

	if (idxV2 != idxV1)
	{
		cAABB* pAABB2 = m_GetOrCreateAABB(idxV2);
		pAABB->vecIdxTriangles.push_back(triangle);
	}

	if (idxV3 != idxV1 && idxV3 != idxV2)
	{
		cAABB* pAABB3 = m_GetOrCreateAABB(idxV3);
		pAABB->vecIdxTriangles.push_back(triangle);
	}

	return;
}

void BroadPhaseCollision::m_InsertMeshTrianglesIndirect(EntityID entityID)
{
	SceneView* pScene = SceneView::Get();

	TransformComponent* pTransform = pScene->GetComponent<TransformComponent>(entityID, "transform");
	// TODO: Should get for each mesh? Then would have to update on animation change
	// Get first mesh only from array
	ModelComponent* pModel = pScene->GetComponent<ModelComponent>(entityID, "model");
	
	sMesh* pMesh       = pModel->GetMesh(0);
	glm::mat4 matModel = pTransform->GetTransform();

	// for every triangle in the mesh insert into aabb
	for (uint i = 0; i < pMesh->numberOfTriangles; i++)
	{
		// Get vertices converting to worldposition
		glm::vec3 v1 = (matModel * glm::vec4(pMesh->pTriangles[i].vertices[0], 1.0f));
		glm::vec3 v2 = (matModel * glm::vec4(pMesh->pTriangles[i].vertices[1], 1.0f));
		glm::vec3 v3 = (matModel * glm::vec4(pMesh->pTriangles[i].vertices[2], 1.0f));

		m_InsertTriangle(v1, v2, v3);
	}
}

void BroadPhaseCollision::m_InsertSphere(EntityID entityID, float radius)
{
	using namespace glm;

	SceneView* pScene = SceneView::Get();
	TransformComponent* pTransform = pScene->GetComponent<TransformComponent>(entityID, "transform");

	vec3 position = pTransform->GetPosition();
	uint idxpos = LocatePoint(position);
	InsertEntityAABB(entityID, idxpos);

	// TODO: Way to set the entity in all aabb that it intersects that makes easy updating every frame
}

