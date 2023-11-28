#include "EngineScripting/CommandFactory.h"
#include "EngineScripting/commands.h"
#include <cstdarg>

iCommand* CommandFactory::CreateCommand(std::string commandName, int nParams, ...)
{
    using namespace std;

    iCommand* pCommand = nullptr;

    va_list args;
    va_start(args, nParams);

    if (commandName == "MoveTo" && nParams == 5)
    {
        pCommand = new MoveTo(va_arg(args, EntityID), va_arg(args, float), // entity, x
                              va_arg(args, float), va_arg(args, float),    // y, z
                              va_arg(args, float));                        // seconds
    }

    va_end(args);

    return pCommand;
}
