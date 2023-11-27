#include "components/Script.h"

void ScriptComponent::GetInfo(sComponentInfo& compInfoOut)
{
    compInfoOut.componentName = "script";
    compInfoOut.componentParameters.clear();

    this->AddCompParInfo("scriptName", "string", this->scriptName, compInfoOut);
}

void ScriptComponent::SetParameter(sParameterInfo& parameterIn)
{
    if (parameterIn.parameterName == "scriptName") {
        this->scriptName = parameterIn.parameterStrValue;
    }

    return;
}
