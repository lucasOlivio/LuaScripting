#pragma once

#include <string>

class iCommand
{
public:
	virtual ~iCommand() {};

	virtual void Initialize(std::string initParams) = 0;
	// Called every frame/step:
	// Returns true when command is done
	virtual bool Update(double deltaTime) = 0;

	virtual bool isDone(void) = 0;

	// Optional: Called before the 1st Update
	virtual bool PreStart(void) = 0;
	// Optional: Called after isDone() returns true
	virtual bool PostEnd(void) = 0;
};

