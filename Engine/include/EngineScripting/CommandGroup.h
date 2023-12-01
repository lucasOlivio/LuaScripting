#pragma once

#include <vector>
#include <string>

#include "commands/Command.h"

class CommandGroup : public Command
{
public:
	CommandGroup();
	virtual ~CommandGroup();

	void AddSerialCommand(iCommand* pTheCommand);
	void AddParallelCommand(iCommand* pTheCommand);

	// iCommand
	virtual bool Initialize(rapidjson::Value& document);
	virtual bool Initialize(std::string name);
	virtual bool PreStart(void);
	virtual bool PostEnd(void);
	virtual bool Update(double deltaTime);
	virtual bool IsDone(void);

	void ClearCommands();

private:
	// Serial commands
	std::vector< iCommand* > m_vecSerialCommands;
	// Points to the next command we're going to run
	std::vector< iCommand* >::iterator m_itNextSerialCommand;
	bool m_PreStartSerial();
	bool m_UpdateSerial(double deltaTime);
	bool m_IsDoneSerial(void);
	bool m_PostEndSerial();

	// Parallel commands
	std::vector< iCommand* > m_vecParallelCommands;
	bool m_PreStartParallel();
	bool m_UpdateParallel(double deltaTime);
	bool m_IsDoneParallel(void);
	bool m_PostEndParallel();
};
