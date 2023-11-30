#pragma once

#include "iCommand.h"

// Implementation of common functions for commands and command groups
class Command : public iCommand
{
public:
	virtual ~Command() {};

	virtual bool Initialize();

	// Unique command ID to retrieve state
	virtual uint16_t GetUUID();
	// UUID of the command should be the same as the root
	virtual void SetUUID(uint16_t value);

	// Friendly name for command or group
	virtual std::string GetName();

protected:
	std::string m_name;
	uint16_t m_UUID;
};
