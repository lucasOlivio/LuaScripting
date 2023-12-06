#pragma once

#include "drawInfo.h"
#include "EngineRenderer/ShaderManager.h"
#include "EngineRenderer/VAOManager.h"
#include "EnginePhysics/Physics.h"

enum class eDebugMode
{
	COLLISION = 1 << 0,
	NORMAL = 1 << 1,
	BROADPHASE = 1 << 2,
	BROADPHASE_TRIANGLES = 1 << 3
};

class DebugSystem
{
public:
	~DebugSystem() {};

	static DebugSystem* Get();

	bool Initialize(ShaderManager* pShaderManager, std::string baseModelsPath, 
					Physics* pPhysics);

	void ResetDebugObjects();

	// Debug objects
	void AddLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec4 RGBA);
	void AddGizmo(glm::vec3 position, int size);
	void AddRectangle(glm::vec3 minXYZ, glm::vec3 maxXYZ, glm::vec4 RGBA, bool isStatic);
	void AddTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec4 RGBA);
	void AddSphere(glm::vec3 position, float radius, glm::vec4 color);

	void Update(double deltaTime, glm::mat4 matView, glm::mat4 matProjection);

	//https://dietertack.medium.com/using-bit-flags-in-c-d39ec6e30f08
	//Sets the debug mode value from true to false and vice versa
	void ToggleMode(eDebugMode dMode);

	//Check whether multiple modes are set to true
	bool IsModesOn(eDebugMode dModes);

private:
	// Singleton
	DebugSystem() {};
	static DebugSystem* m_pInstance;

	uint8_t m_debugMode;

	std::string m_debugShaderName;
	uint m_debugShaderID;
	uint m_debugVAO;
	uint m_debugVBO;
	ShaderManager* m_pShaderManager;
	ShaderManager::ShaderProgram* m_pShaderProgram;

	Physics* m_pPhysics;
	VAOManager* m_pVAOManager;
	sMesh* m_pSphereMesh;

	struct sDebugRectangle
	{
		glm::vec4 color;

		// Bottom face
		glm::vec3 vb1;
		glm::vec3 vb2;
		glm::vec3 vb3;
		glm::vec3 vb4;

		// Top face
		glm::vec3 vt1;
		glm::vec3 vt2;
		glm::vec3 vt3;
		glm::vec3 vt4;

		// Vertical edges
		glm::vec3 ve1;
		glm::vec3 ve2;
		glm::vec3 ve3;
		glm::vec3 ve4;
	};

	struct sDebugTriangle
	{
		glm::vec4 color;

		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 v3;
	};

	struct sDebugSphere
	{
		glm::vec3 position;
		glm::vec4 color;
		float radius;
	};

	struct sLineVertex
	{
		float x, y, z, w;
		float r, g, b, a;
	};

	std::vector<sDebugSphere*> m_vecSpheresToDraw;
	std::vector<sDebugRectangle*> m_vecRectanglesToDraw;
	std::vector<sDebugRectangle*> m_vecStaticRectanglesToDraw;
	std::vector<sDebugTriangle*> m_vecTrianglesToDraw;

	bool m_isBroadPhaseLoaded;

	int m_sizeOfLineVBO;
	sLineVertex* m_pLinesVerticesToDraw;

	void m_InitializeLineVertex();

	void m_AddCollisions();
	void m_AddBroadphase();
	void m_AddNormals();

	void m_DrawLines();
	void m_DrawSpheres();
	void m_DrawRectangles();
	void m_DrawARectangle(glm::mat4 matModel, glm::mat4 matModelIT, sDebugRectangle* pRect);
	void m_DrawTriangles();
	void m_DrawATriangle(glm::mat4 matModel, glm::mat4 matModelIT, sDebugTriangle* pTri);
};
