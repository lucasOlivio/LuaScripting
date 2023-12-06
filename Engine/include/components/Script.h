#pragma once

#include "Component.h"
#include "common/lua.h"
#include <map>

class ScriptComponent : public Component
{
public:
	ScriptComponent();
	virtual ~ScriptComponent() {};

	std::string scriptName;

	virtual void GetInfo(sComponentInfo& compInfoOut);
	virtual void SetParameter(sParameterInfo& parameterIn);

	void SetTableRegistry(int tbIdx);
	void SetLuaObject(const char* objName, int objIdx);

	int GetTableRegistry();
	// Return object index from registry table, -1 if not found
	int GetLuaObject(const char* objName);

private:
	int m_tbRegistry; // Table with all objects for this script
	std::map<const char* /*lua object name*/, int /*lua registry id*/> m_mapLuaObjects;
};
