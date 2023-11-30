local CommandGroup = require("assets/scripts/commands/commandgroup")
local MoveCommand = require("assets/scripts/commands/commands")
local DisptachCommands = require("assets/scripts/commands/utils")

local entity = -1

function onstart(thisEntity)
    entity = thisEntity
    print("startn entity: " .. entity)
    
    -- Example usage:
    local moveCommand1 = MoveCommand(entity, {1600.0, 0.0, 0.0}, 6, 0.2, 0.0, false)
    local moveCommand2 = MoveCommand(entity, {1600.0, 1600.0, 0.0}, 6, 0.2, 0.0, false)
    local moveCommand3 = MoveCommand(entity, {0.0, 1600.0, 0.0}, 6, 0.2, 0.2, true)

    -- Create the main command group
    local mainCommandGroup = CommandGroup:new("movegroup")
    mainCommandGroup:addCommand(moveCommand1, false)  -- Add moveCommand1 to serial
    mainCommandGroup:addCommand(moveCommand2, false)  -- Add moveCommand2 to serial
    mainCommandGroup:addCommand(moveCommand3, false)  -- Add moveCommand3 to serial

    DisptachCommands(mainCommandGroup)
end

function update(deltatime)
end
