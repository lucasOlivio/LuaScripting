local json = require("assets/scripts/json")
local CommandGroup = require("assets/scripts/commands/commandgroup")
local MoveCommand = require("assets/scripts/commands/commands")

local entity = -1

function onstart(thisEntity)
    entity = thisEntity
    print("startn entity: " .. entity)
    
    -- Example usage:
    local moveCommand1 = MoveCommand(entity, {200.0, 0.0, 0.0}, 3)

    -- Create the main command group
    local mainCommandGroup = CommandGroup:new("movegroup")
    mainCommandGroup:addCommand(moveCommand1, false)  -- Add moveCommand1 to serial

    -- Convert the serialized result to a JSON string
    print("serializing...\n")
    local jsonString = json.stringify(mainCommandGroup, false)

    print("sending...")
    SendCommands(jsonString)
end

function update(deltatime)
end
