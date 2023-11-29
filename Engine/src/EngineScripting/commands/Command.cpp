#include "EngineScripting/commands/Command.h"
#include "common/utils.h"

bool Command::Initialize()
{
    m_UUID = myutils::GenerateUUID();

    return true;
}

uint64_t Command::GetUUID()
{
    return m_UUID;
}

std::string Command::GetName()
{
    return m_name;
}
