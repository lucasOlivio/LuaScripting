local commands = require("assets.scripts.commands")

local entity = -1

function onstart(thisEntity)
    entity = thisEntity
    print("startn entity: " .. entity)
    
    -- Example usage:
    local followobjCommand = commands.FollowObjectCommand(entity, "ship2", 0.1, 500, false)

    -- Create the main command group
    local followGroup = CommandGroup:new("followobjCommand")
    followGroup:addCommand(followobjCommand, false)

    local mainCommandGroup = CommandGroup:new("followGroup")
    mainCommandGroup:addCommand(followGroup, false) 

    commands.DispatchCommands(mainCommandGroup)
end

function update(deltatime)
end
