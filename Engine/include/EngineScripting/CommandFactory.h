#pragma once
#include "iCommand.h"

class CommandFactory
{
public:
    /**
     * Creates a command based on the name and parameters passed.
     *
     * @param nParams. Number of parameters been passed.
     * @param commandName. Name of the command to be created.
     * @return iCommand interface to the specified command.
     */
    static iCommand* CreateCommand(std::string commandName, int nParams, ...);
};
