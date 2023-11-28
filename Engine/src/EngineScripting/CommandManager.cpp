#include "EngineScripting/CommandManager.h"

CommandManager::CommandManager()
{
	m_itNextGroup = m_vecCommandGroups.begin();
}

CommandManager::~CommandManager()
{
	// Command manager responsible for deleting all these command groups
	for (CommandGroup* pCommand : m_vecCommandGroups)
	{
		delete pCommand;
	}
	for (CommandGroup* pCommand : m_vecForeverCommands)
	{
		delete pCommand;
	}
}

void CommandManager::AddSerialCommandGroup(CommandGroup* pNewCommand)
{
	pNewCommand->PreStart();
	m_vecCommandGroups.push_back(pNewCommand);
	return;
}

void CommandManager::AddForeverCommand(CommandGroup* pNewCommand)
{
	pNewCommand->PreStart();
	m_vecForeverCommands.push_back(pNewCommand);
	return;
}

void CommandManager::DeleteForeverCommand(CommandGroup* pNewCommand)
{
	for (std::vector< CommandGroup* >::iterator itCurCommand = m_vecForeverCommands.begin();
		itCurCommand != m_vecForeverCommands.end();
		itCurCommand++)
	{
		CommandGroup* pGroup = *itCurCommand;
		if (pNewCommand != pGroup)
		{
			continue;
		}

		// Found the command, delete pointer, rm from vector
		delete pGroup;

		itCurCommand = m_vecForeverCommands.erase(itCurCommand);

		return;
	}
}

void CommandManager::Update(double deltaTime)
{
	m_UpdateGroups(deltaTime);
	m_UpdateForeverGroups(deltaTime);

	return;
}

void CommandManager::m_UpdateGroups(double deltaTime)
{
	for (std::vector< CommandGroup* >::iterator itCurCommand = m_vecCommandGroups.begin();
		itCurCommand != m_vecCommandGroups.end();)
	{

		CommandGroup* pGroup = *itCurCommand;

		bool isDone = pGroup->Update(deltaTime);

		if (isDone)
		{
			// Found the command, delete pointer, rm from vector
			delete pGroup;

			itCurCommand = m_vecCommandGroups.erase(itCurCommand);
		}
		else
		{
			// Only increase if last command not erased
			itCurCommand++;
		}
	}

	return;
}

void CommandManager::m_UpdateForeverGroups(double deltaTime)
{
	for (std::vector< CommandGroup* >::iterator itCurCommand = m_vecForeverCommands.begin();
		itCurCommand != m_vecForeverCommands.end();
		itCurCommand++)
	{
		// No need to check for done in forever commands
		CommandGroup* pGroup = *itCurCommand;

		pGroup->Update(deltaTime);
	}

	return;
}
