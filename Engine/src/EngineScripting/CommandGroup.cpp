#include "EngineScripting/CommandGroup.h"
#include "common/ParserJSON.h"
#include "common/utils.h"

CommandGroup::CommandGroup()
{
}

CommandGroup::~CommandGroup()
{
	// Command groups should be responsible for deleting their commands
	for (iCommand* pCommand : m_vecSerialCommands)
	{
		delete pCommand;
	}
	for (iCommand* pCommand : m_vecParallelCommands)
	{
		delete pCommand;
	}
}

bool CommandGroup::IsDone(void)
{
	if (!m_IsDoneParallel())
	{
		return false;
	}
	if (!m_IsDoneSerial())
	{
		return false;
	}
	// Both ARE done
	return true;
}

void CommandGroup::AddSerialCommand(iCommand* pTheCommand)
{
	m_vecSerialCommands.push_back(pTheCommand);
	return;
}

void CommandGroup::AddParallelCommand(iCommand* pTheCommand)
{
	m_vecParallelCommands.push_back(pTheCommand);
	return;
}

bool CommandGroup::Initialize(SceneView* pScene, rapidjson::Value& document)
{
	this->Command::Initialize();

	ParserJSON parser = ParserJSON();

	rapidjson::Value& objName = document["name"];
	parser.GetString(objName, m_name);

	return true;
}

bool CommandGroup::PreStart()
{
	m_itNextSerialCommand = m_vecSerialCommands.begin();

	bool isValid = true;

	isValid &= m_PreStartSerial();
	isValid &= m_PreStartParallel();

	return isValid;
}

bool CommandGroup::PostEnd(void)
{
	bool isValid = true;
	isValid &= m_PostEndSerial();
	isValid &= m_PostEndParallel();

	return isValid;
}

bool CommandGroup::Update(double deltaTime)
{
	bool isDone = true;
	isDone &= m_UpdateSerial(deltaTime);
	isDone &= m_UpdateParallel(deltaTime);

	return isDone;
}

bool CommandGroup::m_PreStartParallel()
{
	bool isValid = true;

	for (std::vector< iCommand* >::iterator itCurCommand = m_vecParallelCommands.begin();
		itCurCommand != m_vecParallelCommands.end();
		itCurCommand++)
	{

		iCommand* pThisCommand = *itCurCommand;

		isValid &= pThisCommand->PreStart();
	}

	return isValid;
}

bool CommandGroup::m_UpdateParallel(double deltaTime)
{
	// Assume they are all done
	bool isDone = true;

	for (std::vector< iCommand* >::iterator itCurCommand = m_vecParallelCommands.begin();
		itCurCommand != m_vecParallelCommands.end();
		itCurCommand++)
	{

		iCommand* pThisCommand = *itCurCommand;

		// Is this done? 
		if (pThisCommand->IsDone())
		{
			continue;
		}

		pThisCommand->Update(deltaTime);
		// This isn't done
		isDone = false;
	}//for ( std::vector< iCommand* >::iterator

	return isDone;
}

bool CommandGroup::m_IsDoneParallel(void)
{
	// Assume they are all done
	bool isDone = true;

	for (std::vector< iCommand* >::iterator itCurCommand = m_vecParallelCommands.begin();
		itCurCommand != m_vecParallelCommands.end();
		itCurCommand++)
	{
		iCommand* pThisCommand = *itCurCommand;

		// Is this done? 
		if (!pThisCommand->IsDone())
		{
			// This isn't done
			isDone = false;
		}
	}//for (std::vector< iCommand* >::iterator

	return isDone;
}

bool CommandGroup::m_PostEndParallel()
{
	bool isValid = true;

	for (std::vector< iCommand* >::iterator itCurCommand = m_vecParallelCommands.begin();
		itCurCommand != m_vecParallelCommands.end();
		itCurCommand++)
	{
		iCommand* pThisCommand = *itCurCommand;

		isValid &= pThisCommand->PostEnd();
	}

	return isValid;
}

bool CommandGroup::m_IsDoneSerial(void)
{
	if (m_itNextSerialCommand == m_vecSerialCommands.end())
	{
		return true;
	}
	return false;
}

bool CommandGroup::m_PreStartSerial()
{
	if (m_IsDoneSerial())
	{
		return true;
	}

	iCommand* pCurrentCommand = *m_itNextSerialCommand;

	bool isValid = pCurrentCommand->PreStart();

	return isValid;
}

bool CommandGroup::m_UpdateSerial(double deltaTime)
{
	// Are there any more commands? 
	if (m_itNextSerialCommand == m_vecSerialCommands.end())
	{
		return true;
	}

	// get pointer to this command by dereferncing the iterator
	iCommand* pCurrentCommand = *m_itNextSerialCommand;

	// Yes, there's a current command 
	pCurrentCommand->Update(deltaTime);

	if (!pCurrentCommand->IsDone())
	{
		// This command is not done yet
		return false;
	}

	pCurrentCommand->PostEnd();

	// Remove command and move to next
	delete pCurrentCommand;
	m_itNextSerialCommand = m_vecSerialCommands.erase(m_itNextSerialCommand);

	// All commands are done?
	if (m_itNextSerialCommand == m_vecSerialCommands.end())
	{
		return true;
	}

	// Pre start next command
	iCommand* pNextCommand = *m_itNextSerialCommand;
	pNextCommand->PreStart();

	// We're not done. there are more commands to do
	return false;
}

bool CommandGroup::m_PostEndSerial()
{
	iCommand* pCurrentCommand = *m_itNextSerialCommand;

	bool isValid = pCurrentCommand->PostEnd();

	return isValid;
}