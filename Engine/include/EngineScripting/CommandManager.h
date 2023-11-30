#pragma once

#include "commands/iCommand.h"
#include <vector>

class CommandManager
{
public:
	CommandManager();
	~CommandManager();

	// Commands groups to run every frame until complete
	// Every command here will run every frame until complete then deleted
	void AddCommand(iCommand* pNewCommand);

	// Commands groups that will run forever or until explicitly deleted
	void AddForeverCommand(iCommand* pNewCommand);
	void DeleteForeverCommand(uint16_t UUID);

	// Update all commands in lists
	void Update(double deltaTime);

	// Deletes all commands from all the lists
	void ClearCommands();

private:
	// Serial commands. Run until isDone(), then remove or ignore
	std::vector< iCommand* > m_vecCommands;
	std::vector< iCommand* >::iterator m_itNextGroup;
	void m_UpdateGroups(double deltaTime);

	std::vector< iCommand* > m_vecForeverCommands;
	void m_UpdateForeverGroups(double deltaTime);

};
