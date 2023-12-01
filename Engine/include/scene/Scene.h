#pragma once

#include "components/iComponent.h"
#include "events/iEvent.h"
#include "common/types.h"
#include <map>
#include <string>

class Scene
{
public:
	~Scene();

	static Scene* Get();

	// Delete all components pointers and clear maps
	virtual void Clear();

	// Delete components that were marked
	void ClearDeleted();

	virtual EntityID GetNumEntities();

	virtual EntityID CreateEntity();
	// Create a new entity copy of the "entityID" passed
	virtual EntityID CreateEntity(EntityID entityID);
	virtual EntityID CreateEntity(EntityID entityID, bool createAndActivate);

	virtual void DeleteEntity(EntityID entityID);

	// Get all parameters info from all the components
	// this entity has attached to it
	virtual std::vector<sComponentInfo> GetComponentsInfo(EntityID entityID);
	virtual std::map<std::string, iComponent*> GetComponents(EntityID entityID);

	// Get the vector for the respective component
	virtual bool GetMapComponents(std::string componentName, std::map<EntityID, iComponent*>& componentsOut);

	// Get a component from the scene by its entity id and component name
	virtual iComponent* GetComponent(EntityID entityID, std::string componentName);
	// Set a component to the entity by its entity id and component name
	virtual void SetComponent(EntityID entityID, std::string componentName, iComponent* componentIn);

	virtual bool IsPlaying();
	void SetPlaying(bool isPlaying);

private:
	EntityID m_numEntities;

	// TODO: For now keep as a map for simplicity, 
	// but later transform into a vector of entities
	// and each entity with a bitmask of components
	std::map<std::string /* component name */,
		std::map<EntityID, iComponent*>> m_components;

	bool m_isPlaying;

	// Components and listeners that were already deleted and are waiting end of frame to 
	// be completely removed
	std::vector<iComponent*> m_compToDestroy;
	std::vector<iListener*> m_listenerToDestroy;

	//Singleton
	Scene();

	static Scene* m_pInstance;
};