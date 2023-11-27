#pragma once

#include "Component.h"
#include <map>

class ScriptComponent : public Component
{
public:
	std::string scriptName;

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);
};
