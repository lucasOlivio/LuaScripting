local gameplay = require("assets.scripts.gameplay")
local commands = require("assets.scripts.commands")
local directions = require("assets.scripts.common.directions")

function onstart()
    local mainCommandGroup = CommandGroup:new("followGroup")
    local curveCommand = commands.FollowCurveCommand(entity, {
        {300, 0, 600},
        {600, 0, 1200},
        {600, 0, 600}
    }, 7.0, 50.0, 0.0, 0.0, true)
    mainCommandGroup:addCommand(curveCommand, false)

    commands.DispatchCommands(mainCommandGroup)
end

function oncollision(tagCollided)
    gameplay.DestroyShip(entity)
end
