#include "components/Script.h"

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
