#pragma once

#include <string>
#include <rapidjson/document.h>

class iCommand
{
public:
	virtual ~iCommand() {};

	// TODO: Should not depend on rapidjson, 
	// should be able to receive n args of variable types without this coupling
	// 
	// Get all variables needed for the command from the json object
	virtual bool Initialize(rapidjson::Value& document) = 0;

	/**
	 * Called every frame/step
	 *
	 * @return true when command is done
	 */
	virtual bool Update(double deltaTime) = 0;

	virtual bool IsDone(void) = 0;

	// Called before the 1st Update
	virtual bool PreStart() = 0;
	// Called after IsDone() returns true
	virtual bool PostEnd(void) = 0;

	virtual uint16_t GetUUID() = 0;
	// UUID of the command should be the same as the root
	virtual void SetUUID(uint16_t value) = 0;
};

