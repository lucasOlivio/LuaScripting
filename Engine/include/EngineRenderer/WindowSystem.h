#pragma once

#include "iShaderInfo.h"
#include "common/types.h"
#include "common/opengl.h"
#include <glm/mat4x4.hpp>

class WindowSystem
{
public:
	// ctors & dtors
	WindowSystem(iShaderInfo* pShaderInfo);
	~WindowSystem();

	// Where we really start/destroy everything that could go wrong
	bool Initialize(uint width, uint height, const std::string& windowName, 
					GLFWkeyfun KeyCallback, GLFWmousebuttonfun MousePressCallback, GLFWcursorposfun MousePosCallback);
	void Destroy();

	glm::mat4 GetProjection();

	// Clear all and bind opengl parameters
	void NewFrame(int shaderID);
	void UpdateUL(int shaderID);
	// Swap frames and buffers
	void EndFrame();

	bool WindowShouldClose();

	GLFWwindow* GetWindow();

	void GetWindowSize(int& windowHeight, int& windowWidth);

	float GetCurrentWindowRatio();

	void CaptureMouse();
	void ReleaseMouse();

private:
	GLFWwindow* m_pWindow;
	iShaderInfo* m_pShaderInfo;

	int m_width;
	int m_height;
	float m_ratio;
};