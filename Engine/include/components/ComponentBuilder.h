#pragma once

#include "iComponent.h"
#include <string>

class ComponentBuilder
{
public:
	ComponentBuilder();
	~ComponentBuilder() {};

	iComponent* BuildComponent(sComponentInfo& componentInfo, EntityID entityID);
};