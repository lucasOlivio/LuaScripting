#pragma once

#include "iCommand.h"

// Implementation of common functions for commands and command groups
class Command : public iCommand
{
public:
	virtual ~Command() {};

	virtual bool Initialize();

	// Unique command ID to retrieve state
	virtual uint64_t GetUUID();

	// Friendly name for command or group
	virtual std::string GetName();

protected:
	std::string m_name;
	uint64_t m_UUID;
};
