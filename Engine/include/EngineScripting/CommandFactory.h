#pragma once
#include "iCommand.h"
#include "scene/SceneView.h"

class CommandFactory
{
public:
    CommandFactory(SceneView* pScene);

    /**
     * Creates a command based on the name and parameters passed.
     *
     * @param command. Name of the command to be created.
     * @param args. parameters to the command as json string.
     */
    iCommand* CreateCommand(const char* command, const char* args);

    std::string GetError();
private:
    std::string m_errorMsg;

    SceneView* m_pScene;
};
