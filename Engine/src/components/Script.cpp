#include "components/Script.h"

typedef std::map<const char*, int>::iterator itObjIdx;

ScriptComponent::ScriptComponent()
{
    m_tbRegistry = -1;
}

void ScriptComponent::GetInfo(sComponentInfo& compInfoOut)
{
    compInfoOut.componentName = "script";
    compInfoOut.componentParameters.clear();

    AddCompParInfo("scriptName", "string", scriptName, compInfoOut);

    Component::GetInfo(compInfoOut);
}

void ScriptComponent::SetParameter(sParameterInfo& parameterIn)
{
    Component::SetParameter(parameterIn);

    if (parameterIn.parameterName == "scriptName") {
        scriptName = parameterIn.parameterStrValue;
    }

    return;
}

void ScriptComponent::SetTableRegistry(int tbIdx)
{
    m_tbRegistry = tbIdx;
}

void ScriptComponent::SetLuaObject(const char* objName, int objIdx)
{
    m_mapLuaObjects[objName] = objIdx;
}

int ScriptComponent::GetTableRegistry()
{
    return m_tbRegistry;
}

int ScriptComponent::GetLuaObject(const char* objName)
{
    itObjIdx it = m_mapLuaObjects.find(objName);
    if (it == m_mapLuaObjects.end())
    {
        return -1;
    }
    return it->second;
}

