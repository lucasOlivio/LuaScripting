#pragma once

#include "CommandGroup.h"
#include <vector>

class CommandManager
{
public:
	CommandManager();

	// Commands groups to run every frame until complete
	// Every command here will run every frame until complete then deleted
	void AddSerialCommandGroup(CommandGroup* pNewCommand);

	// Commands groups that will run forever or until explicitly deleted
	void AddForeverCommand(CommandGroup* pNewCommand);
	void DeleteForeverCommand(CommandGroup* pNewCommand);

	// ONLY does serial command group.
	// The parallel is handled by the Command Group
	void Update(double deltaTime);

private:
	// Serial commands. Run until isDone(), then remove or ignore
	std::vector< CommandGroup* > m_vecCommandGroups;
	std::vector< CommandGroup* >::iterator m_itNextGroup;
	void m_UpdateGroups(double deltaTime);

	std::vector< CommandGroup* > m_vecForeverCommands;
	void m_UpdateForeverGroups(double deltaTime);

};
