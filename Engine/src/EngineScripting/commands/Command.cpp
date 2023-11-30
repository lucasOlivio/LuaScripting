#include "EngineScripting/commands/Command.h"
#include "common/utils.h"

bool Command::Initialize()
{
    m_UUID = myutils::GenerateUUID();

    return true;
}

uint16_t Command::GetUUID()
{
    return m_UUID;
}

void Command::SetUUID(uint16_t value)
{
    m_UUID = value;
}

std::string Command::GetName()
{
    return m_name;
}
