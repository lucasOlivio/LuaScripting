#pragma once

#include "EngineScripting/commands/Command.h"

class DestroyEntity : public Command
{
public:
	DestroyEntity();
	virtual ~DestroyEntity() {};

	virtual bool Initialize(SceneView* pScene, rapidjson::Value& document);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	SceneView* m_pScene;
	EntityID m_entityID;        // Entity to destroy

	bool m_isReplace;
	EntityID m_entityToReplace; // Replace entity by this one
};
