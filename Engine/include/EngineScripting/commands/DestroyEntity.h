#pragma once

#include "EngineScripting/commands/Command.h"
#include "common/types.h"

class DestroyEntity : public Command
{
public:
	DestroyEntity();
	virtual ~DestroyEntity() {};

	virtual bool Initialize(rapidjson::Value& document);

	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime);

	virtual bool IsDone(void);

	virtual bool PreStart(void);

	virtual bool PostEnd(void);
private:
	EntityID m_entityID;        // Entity to destroy
};
