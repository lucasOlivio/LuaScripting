#include "EnginePhysics/cAABB.h"

typedef std::unordered_set<EntityID>::iterator itEntities;

glm::vec3 cAABB::GetCentreXYZ(void)
{
    return (minXYZ + maxXYZ) * 0.5f;
}

glm::vec3 cAABB::GetExtentsXYZ(void)
{
	return maxXYZ - minXYZ;
}

void cAABB::RemoveEntity(EntityID entityID)
{
    for (itEntities it = vecEntities.begin();
         it != vecEntities.end();
         it++)
    {
        if (*it == entityID)
        {
            vecEntities.erase(it);

            return;
        }
    }
}

int cAABB::GetNumEntities()
{
    return vecEntities.size();
}

int cAABB::GetNumChildren()
{
    return vecEntities.size() + vecIdxTriangles.size();
}
