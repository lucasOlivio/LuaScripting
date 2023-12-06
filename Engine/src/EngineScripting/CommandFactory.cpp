#include "EngineScripting/CommandFactory.h"
#include "EngineScripting/commands.h"
#include "common/ParserJSON.h"
#include <cstdarg>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

CommandFactory::CommandFactory()
{
}

iCommand* CommandFactory::CreateCommand(const char* json)
{
    using namespace rapidjson;

    // Parse json string into object
    Document document;
    document.Parse(json);

    if (document.HasParseError()) {
        m_errorMsg = "Failed to parse command json\n";
        return nullptr;
    }

    // Reset error msg
    m_errorMsg = "";

    // Desserialize into CommandGroup nested structure
    iCommand* pCommand = m_DeserializeCommand(document, 0);

    if (m_errorMsg != "")
    {
        // If there is an error we should not return this value 
        // and just delete the command to avoid memory leak
        delete pCommand;
        return nullptr;
    }

    return pCommand;
}

std::string CommandFactory::GetError()
{
    return m_errorMsg;
}

iCommand* CommandFactory::m_DeserializeCommand(rapidjson::Value& document, uint16_t rootUUID)
{
    using namespace rapidjson;

    if (!document.IsObject())
    {
        m_errorMsg += "Invalid json structure\n";
        return nullptr;
    }
    
    // If have a command member then its a final command (the leaf of the structure)
    if (document.HasMember("command"))
    {
        Value& command = document["command"];
        iCommand* pCommand = m_CreateFinalCommand(command);

        if (pCommand)
        {
            if (rootUUID > 0)
            {
                pCommand->SetUUID(rootUUID);
            }

            return pCommand;
        }

        // Stringfy the invalid command
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);

        m_errorMsg = m_errorMsg + "Command or args not valid: " + buffer.GetString() + "\n";

        return nullptr;
    }

    // Should be a command group then
    CommandGroup* pCommandGroup = new CommandGroup();

    pCommandGroup->Initialize(document);

    if (rootUUID > 0)
    {
        pCommandGroup->SetUUID(rootUUID);
    }

    // Check if it's a serial or parallel group
    // (Empty vectors come as objects so we must check)
    if (document.HasMember("serial") && document["serial"].IsArray())
    {
        // Array of serial commands to be executed
        Value& serialArray = document["serial"].GetArray();
        for (SizeType i = 0; i < serialArray.Size(); ++i)
        {
            iCommand* pSerial = m_DeserializeCommand(serialArray[i], pCommandGroup->GetUUID());

            // Propagates the root UUID to all children

            pCommandGroup->AddSerialCommand(pSerial);
        }
    }
    
    if (document.HasMember("parallel") && document["parallel"].IsArray())
    {
        // Array of parallel commands to be executed
        Value& parallelArray = document["parallel"].GetArray();
        for (SizeType i = 0; i < parallelArray.Size(); ++i)
        {
            iCommand* pParallel = m_DeserializeCommand(parallelArray[i], pCommandGroup->GetUUID());

            pCommandGroup->AddParallelCommand(pParallel);
        }
    }

    return pCommandGroup;
}

iCommand* CommandFactory::m_CreateFinalCommand(rapidjson::Value& command)
{
    using namespace rapidjson;

    iCommand* pCommand = nullptr;
    std::string name = command["name"].GetString();
    Value& args = command["args"].GetObject();

    if (name == "MoveTo")
    {
        pCommand = new MoveTo();
    }
    else if (name == "OrientTo")
    {
        pCommand = new OrientTo();
    }
    else if (name == "FollowCurve")
    {
        pCommand = new FollowCurve();
    }
    else if (name == "FollowObject")
    {
        pCommand = new FollowObject();
    }
    else if (name == "DestroyEntity")
    {
        pCommand = new DestroyEntity();
    }
    else if (name == "CreateEntity")
    {
        pCommand = new CreateEntity();
    }
    else if (name == "AccelerateTowards")
    {
        pCommand = new AccelerateTowards();
    }
    else if (name == "FollowMouse")
    {
        pCommand = new FollowMouse();
    }

    if (!pCommand)
    {
        return nullptr;
    }

    bool isValid = pCommand->Initialize(args);

    if (!isValid)
    {
        // Value not valid, command should not go through
        delete pCommand;
        return nullptr;
    }

    return pCommand;
}
