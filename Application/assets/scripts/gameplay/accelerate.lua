local commands = require("assets.scripts.commands")

-- Accelerates the object to the given direction
-- Returns the command UUID
function Accelerate(entity, direction, acceleration, maxSpeed)
    local mainCommandGroup = CommandGroup:new("accelerate")

    local moveCommand = commands.AccelerateTowards(entity, direction, acceleration, maxSpeed)

    mainCommandGroup:addCommand(moveCommand, true) 

    return commands.DispatchCommands(mainCommandGroup, true)
end

return {
    Accelerate = Accelerate
}
