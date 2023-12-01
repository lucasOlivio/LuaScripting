#include "EngineScripting/LuaBindings/LuaGetInfo.h"
#include "scene/SceneView.h"
#include "components.h"
#include "common/Input.h"
#include <glm/vec3.hpp>

int lua_GetTransform(lua_State* L)
{
	std::string entityTag = lua_tostring(L, 1);

	TransformComponent* pTransform = SceneView::Get()->GetComponentByTag<TransformComponent>(entityTag, "transform");

	glm::vec3 position = pTransform->GetPosition();
	glm::vec3 orientation = pTransform->GetOrientation();
	float scale = pTransform->GetScale();

	lua_pushnumber( L, position.x );
	lua_pushnumber( L, position.y );		
	lua_pushnumber( L, position.z );		
	lua_pushnumber( L, orientation.x );		
	lua_pushnumber( L, orientation.y );		
	lua_pushnumber( L, orientation.z );
	lua_pushnumber( L, scale );
	
	return 7;
}

int lua_Action(lua_State* L)
{
	const char* action = lua_tostring(L, 1);
	
	bool isAction = Input::IsActionKeyPressed(action);

	lua_pushboolean(L, isAction);

	return 1;
}
