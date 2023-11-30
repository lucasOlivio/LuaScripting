#include "components/Tag.h"
#include "common/utils.h"
#include <glm/gtx/string_cast.hpp>

void TagComponent::GetInfo(sComponentInfo& compInfoOut)
{
    using namespace myutils;

    compInfoOut.componentName = "tag";
    compInfoOut.componentParameters.clear();

    this->AddCompParInfo("name", "string", this->name, compInfoOut);

    this->Component::GetInfo(compInfoOut);
}

void TagComponent::SetParameter(sParameterInfo& parameterIn)
{
    using namespace myutils;

    this->Component::SetParameter(parameterIn);

    if (parameterIn.parameterName == "name") {
        this->name = parameterIn.parameterStrValue;
    }

    return;
}
