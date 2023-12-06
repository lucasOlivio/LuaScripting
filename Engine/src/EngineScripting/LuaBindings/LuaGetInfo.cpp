#include "EngineScripting/LuaBindings/LuaGetInfo.h"
#include "scene/SceneView.h"
#include "components.h"
#include "common/Input.h"
#include "common/utilsMat.h"
#include <glm/vec3.hpp>

int lua_GetFrontDirection(lua_State* L)
{
	EntityID entity = lua_tonumber(L, 1);

	TransformComponent* pTransform = SceneView::Get()->GetComponent<TransformComponent>(entity, "transform");

	glm::vec3 direction = pTransform->GetForwardVector();

	lua_pushnumber(L, direction.x);
	lua_pushnumber(L, direction.y);
	lua_pushnumber(L, direction.z);

	return 3;
}

int lua_GetTransform(lua_State* L)
{
	EntityID entity = lua_tonumber(L, 1);

	TransformComponent* pTransform = SceneView::Get()->GetComponent<TransformComponent>(entity, "transform");

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

int lua_GetKey(lua_State* L)
{
	int key = lua_tonumber(L, 1);
	
	bool isKey = Input::IsKeyPressed(key);

	lua_pushboolean(L, isKey);

	return 1;
}
