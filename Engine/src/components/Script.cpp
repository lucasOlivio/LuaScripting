#include "components/Script.h"

void ScriptComponent::GetInfo(sComponentInfo& compInfoOut)
{
    compInfoOut.componentName = "script";
    compInfoOut.componentParameters.clear();

    this->AddCompParInfo("scriptName", "string", this->scriptName, compInfoOut);

    this->Component::GetInfo(compInfoOut);
}

void ScriptComponent::SetParameter(sParameterInfo& parameterIn)
{
    this->Component::SetParameter(parameterIn);

    if (parameterIn.parameterName == "scriptName") {
        this->scriptName = parameterIn.parameterStrValue;
    }

    return;
}
