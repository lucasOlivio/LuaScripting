#include "components/ComponentBuilder.h"
#include "components.h"
#include "scene/Scene.h"

ComponentBuilder::ComponentBuilder()
{
}

iComponent* ComponentBuilder::BuildComponent(sComponentInfo& componentInfo, EntityID entityID)
{
	iComponent* newComponent = nullptr;

	if (componentInfo.componentName == "model")
	{
		newComponent = new ModelComponent();
	}
	else if (componentInfo.componentName == "transform")
	{
		newComponent = new TransformComponent();
	}
	else if (componentInfo.componentName == "tag")
	{
		newComponent = new TagComponent();
	}
	else if (componentInfo.componentName == "camera")
	{
		newComponent = new CameraComponent();
	}
	else if (componentInfo.componentName == "force")
	{
		newComponent = new ForceComponent();
	}
	else if (componentInfo.componentName == "collision")
	{
		newComponent = new CollisionComponent();
	}
	else if (componentInfo.componentName == "script")
	{
		newComponent = new ScriptComponent();
	}
	else if (componentInfo.componentName == "texture")
	{
		newComponent = new TextureComponent();
	}
	else if (componentInfo.componentName == "material")
	{
		newComponent = new MaterialComponent();
	}
	else if (componentInfo.componentName == "tiling")
	{
		newComponent = new TilingComponent();
	}
	else if (componentInfo.componentName == "light")
	{
		newComponent = new LightComponent();
	}
	else if (componentInfo.componentName == "audio")
	{
		newComponent = new AudioComponent();
	}
	else if (componentInfo.componentName == "channel")
	{
		newComponent = new ChannelComponent();
	}
	else if (componentInfo.componentName == "audioOcclusion")
	{
		newComponent = new AudioOcclusionComponent();
	}
	else
	{
		printf("Error: Invalid component '%s'\n", componentInfo.componentName.c_str());
		return nullptr;
	}

	// Every created component we already set the entityID it belongs and sends to scene
	newComponent->SetEntityID(entityID);
	for (sParameterInfo parameter : componentInfo.componentParameters)
	{
		newComponent->SetParameter(parameter);
	}
	Scene::Get()->SetComponent(entityID, componentInfo.componentName, newComponent);

	return newComponent;
}
