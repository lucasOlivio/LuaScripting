local commands = require("assets.scripts.commands")

-- Returns the command UUID
function Follow(entity, targetTag, timeStep, maxSpeed, maxRotationSpeed, followDistance, offset)
    local followobjCommand = commands.FollowObjectCommand(entity, targetTag, timeStep, maxSpeed,
                                                          maxRotationSpeed, followDistance, offset)

    local mainCommandGroup = CommandGroup:new("followGroup")
    mainCommandGroup:addCommand(followobjCommand, true) 

    return commands.DispatchCommands(mainCommandGroup)
end

return {
    Follow = Follow
}