#pragma once

#include <vector>
#include <string>

#include "iCommand.h"

class CommandGroup : public iCommand
{
public:
	CommandGroup();
	virtual ~CommandGroup();

	std::string friendlyName;
	unsigned int UniqueID;

	void AddSerialCommand(iCommand* pTheCommand);
	void AddParallelCommand(iCommand* pTheCommand);

	// iCommand
	virtual bool PreStart(void);
	virtual bool PostEnd(void);
	virtual bool Update(double deltaTime);
	virtual bool IsDone(void);

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
