#include "EngineScripting/CommandFactory.h"
#include "EngineScripting/commands.h"
#include "common/ParserJSON.h"
#include <cstdarg>
#include <rapidjson/document.h>

CommandFactory::CommandFactory(SceneView* pScene) :
    m_pScene(pScene)
{
}

iCommand* CommandFactory::CreateCommand(const char* command, const char* args)
{
    using namespace rapidjson;
    using namespace glm;
    using namespace std;

    ParserJSON parser = ParserJSON();
    iCommand* pCommand = nullptr;
    Document document;
    document.Parse(args);

    if (command == "MoveTo")
    {
        // Check if json have all the argument needed
        if (document.HasMember("entity")   &&
            document.HasMember("location") &&
            document.HasMember("time"))
        {
            int entity;
            vec3 location;
            float time;

            // Get variables values from json string
            parser.GetInt(document["entity"], entity);
            parser.GetVec3(document["location"], location);
            parser.GetFloat(document["time"], time);

            pCommand = new MoveTo(m_pScene, entity, location, time);
        }
    }

    // On error we save all parameters in case the caller needs to deal with it
    if (pCommand == nullptr)
    {
        m_errorMsg = string("No command '") + command + "' found with args: " + args;
    }
    else
    {
        m_errorMsg = "";
    }

    return pCommand;
}

std::string CommandFactory::GetError()
{
    return m_errorMsg;
}
