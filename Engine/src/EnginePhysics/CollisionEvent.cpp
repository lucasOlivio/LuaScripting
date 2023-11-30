#include "events/CollisionEvent.h"
#include "events/iCollisionListener.h"
#include "EngineScripting/ScriptingSystem.h"

CollisionEvent::CollisionEvent()
{
}

CollisionEvent::~CollisionEvent()
{
}

void CollisionEvent::TriggerCollisions(std::vector<sCollisionData*> pFrameCollisions)
{
	// Notify only listeners that collided with their collision
	for (int i = 0; i < pFrameCollisions.size(); i++)
	{
		sCollisionData* pCollision = pFrameCollisions[i];

		// Notify entities that collided
		for (int j = 0; j < this->m_pListeners.size(); j++)
		{
			iCollisionListener* pCollListener = (iCollisionListener*)this->m_pListeners[j];
			EntityID entityID = pCollListener->GetEntityID();

			if (entityID == pCollision->entityA || entityID == pCollision->entityB)
			{
				pCollListener->Notify(this, pCollision);
			}
		}

		// Notify Scripting engine
		ScriptingSystem::Get()->OnCollision(pCollision);
	}
}
