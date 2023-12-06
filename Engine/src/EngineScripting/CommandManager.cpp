#include "EngineScripting/CommandManager.h"

CommandManager::CommandManager()
{
	m_vecForeverCommands = {};
	m_vecCommands = {};
	m_itNextGroup = m_vecCommands.begin();
}

CommandManager::~CommandManager()
{
	// Command manager responsible for deleting all these command groups
	ClearCommands();
}

void CommandManager::AddCommand(iCommand* pNewCommand)
{
	pNewCommand->PreStart();
	m_vecCommands.push_back(pNewCommand);
	return;
}

void CommandManager::AddForeverCommand(iCommand* pNewCommand)
{
	pNewCommand->PreStart();
	m_vecForeverCommands.push_back(pNewCommand);
	return;
}

void CommandManager::DeleteForeverCommand(uint16_t UUID)
{
	for (std::vector< iCommand* >::iterator itCurCommand = m_vecForeverCommands.begin();
		itCurCommand != m_vecForeverCommands.end();
		itCurCommand++)
	{
		iCommand* pGroup = *itCurCommand;
		if (UUID != pGroup->GetUUID())
		{
			continue;
		}

		pGroup->PostEnd();

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

void CommandManager::ClearCommands()
{
	for (int i = 0; i < m_vecCommands.size(); i++)
	{
		delete m_vecCommands[i];
	}
	for (int i = 0; i < m_vecForeverCommands.size(); i++)
	{
		delete m_vecForeverCommands[i];
	}

	m_vecCommands.clear();
	m_vecForeverCommands.clear();
}

void CommandManager::m_UpdateGroups(double deltaTime)
{
	for (std::vector< iCommand* >::iterator itCurCommand = m_vecCommands.begin();
		itCurCommand != m_vecCommands.end();)
	{

		iCommand* pGroup = *itCurCommand;

		bool isDone = pGroup->Update(deltaTime);

		if (isDone)
		{
			// Found the command, delete pointer, rm from vector
			delete pGroup;

			itCurCommand = m_vecCommands.erase(itCurCommand);
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
	for (std::vector< iCommand* >::iterator itCurCommand = m_vecForeverCommands.begin();
		itCurCommand != m_vecForeverCommands.end();
		itCurCommand++)
	{
		// No need to check for done in forever commands
		iCommand* pGroup = *itCurCommand;

		pGroup->Update(deltaTime);
	}

	return;
}
