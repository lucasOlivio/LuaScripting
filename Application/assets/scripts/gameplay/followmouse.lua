local commands = require("assets.scripts.commands")

-- Capute and follows the mouse indefinitely
-- Returns the command UUID
function FollowMouse(entity, sensitivity)
    local mainCommandGroup = CommandGroup:new("followmouse")

    local moveCommand = commands.FollowMouse(entity, sensitivity)

    mainCommandGroup:addCommand(moveCommand, true) 

    return commands.DispatchCommands(mainCommandGroup, true)
end

return {
    FollowMouse = FollowMouse
}
