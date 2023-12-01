#pragma once

#include "iShaderInfo.h"
#include <glm/mat4x4.hpp>

class LightSystem
{
private:
	iShaderInfo* m_pShaderInfo;

public:
	// ctors & dtors
	LightSystem(iShaderInfo* pShaderInfo);
	~LightSystem();

	bool Initialize(int shaderID);

	void Update(double deltatime);
};