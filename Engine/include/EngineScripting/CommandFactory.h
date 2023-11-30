#pragma once
#include "commands/iCommand.h"
#include "scene/SceneView.h"

class CommandFactory
{
public:
    CommandFactory(SceneView* pScene);

    /**
     * Creates a command batch based on the json structure
     *
     * @param json. Json structure with batch of commands (See docs for example).
     */
    iCommand* CreateCommand(const char* json);

    std::string GetError();
private:
    std::string m_errorMsg;

    SceneView* m_pScene;

    // Deserialize command json into a nested CommanGroup
    iCommand* m_DeserializeCommand(rapidjson::Value& document, uint16_t rootUUID);

    // Creates a final command that will be executed
    iCommand* m_CreateFinalCommand(rapidjson::Value& command);
};
